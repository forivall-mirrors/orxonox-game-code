/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include <iostream>
#include <string>

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#include <OgreTimer.h>

#include "CoreIncludes.h"
#include "ConsoleCommand.h"
#include "CommandExecutor.h"
#include "Debug.h"
#include "TclBind.h"
#include "TclThreadManager.h"
#include "util/Convert.h"

#define TCLTHREAD_MAX_QUEUE_LENGTH 1024
#define TCLTHREAD_MAX_CPU_USAGE 0.50

namespace orxonox
{
    ConsoleCommand(TclThreadManager, tclthread, AccessLevel::None, true);
    ConsoleCommand(TclThreadManager, create,    AccessLevel::None, false);
    ConsoleCommand(TclThreadManager, destroy,   AccessLevel::None, false);
    ConsoleCommand(TclThreadManager, execute,   AccessLevel::None, false);
    ConsoleCommand(TclThreadManager, query,     AccessLevel::None, false);

    TclThreadManager* instance = &TclThreadManager::getInstance();

    TclThreadManager::TclThreadManager()
    {
        RegisterRootObject(TclThreadManager);

        this->IDcount_ = 0;
        this->isReady_ = false;
    }

    TclThreadManager& TclThreadManager::getInstance()
    {
        static TclThreadManager instance;
        return instance;
    }

    void TclThreadManager::tclthread(unsigned int threadID, const std::string& command)
    {
        TclThreadManager::execute(threadID, command);
    }

    unsigned int TclThreadManager::create()
    {
        if (TclThreadManager::getInstance().createTclThread())
        {
            COUT(0) << "Created new Tcl-thread with ID " << TclThreadManager::getInstance().IDcount_ << "." << std::endl;
            return TclThreadManager::getInstance().IDcount_;
        }
        return 0;
    }

    void TclThreadManager::destroy(unsigned int threadID)
    {
        if (TclThreadManager::getInstance().destroyTclThread(threadID))
        {
            COUT(0) << "Destroyed Tcl-thread with ID " << threadID << "." << std::endl;
        }
    }

    void TclThreadManager::execute(unsigned int threadID, const std::string& command)
    {
        TclThreadManager::getInstance().pushCommandBack(threadID, command);
    }

    std::string TclThreadManager::query(unsigned int threadID, const std::string& command)
    {
        return TclThreadManager::getInstance().eval(threadID, command);
    }

    void TclThreadManager::tcl_execute(Tcl::object const &args)
    {
std::cout << "3_1\n";
std::cout << "Tcl-thread_execute: args: " << args.get() << std::endl;
        std::string command = args.get();

        if (command.size() >= 2 && command[0] == '{' && command[command.size() - 1] == '}')
            command = command.substr(1, command.size() - 2);

        TclThreadManager::getInstance().pushCommandBack(command);
    }

    std::string TclThreadManager::tcl_orxonox(Tcl::object const &args)
    {
std::cout << "4_1\n";
std::cout << "Tcl-thread_orxonox: args: " << args.get() << std::endl;
        std::string command = args.get();

        if (command.size() >= 2 && command[0] == '{' && command[command.size() - 1] == '}')
            command = command.substr(1, command.size() - 2);

        return TclThreadManager::getInstance().eval(command);
    }

    Tcl::interpreter* TclThreadManager::createTclInterpreter(unsigned int threadID) const
    {
        Tcl::interpreter* i = new Tcl::interpreter(TclBind::getInstance().getTclLibPath());

        i->def("orxonox", TclThreadManager::tcl_orxonox, Tcl::variadic());
        i->def("execute", TclThreadManager::tcl_execute, Tcl::variadic());

        std::string id = getConvertedValue<unsigned int, std::string>(threadID);
        i->eval("rename exit tclexit; proc exit {} { orxonox TclThreadManager destroy " + id + " }");
        i->eval("set threadid " + id);
        i->eval("redef_puts");

        return i;
    }

    bool TclThreadManager::createTclThread()
    {
        this->IDcount_++;
        Tcl::interpreter* i = this->createTclInterpreter(this->IDcount_);
        boost::try_mutex* m = new boost::try_mutex;
        boost::thread*    t = new boost::thread(boost::bind(&tclThreadLoop, i, m, this->IDcount_));
        TclThread newthread = {i, t, m};
        {
            // mutex threads
            boost::mutex::scoped_lock lock(this->threadsMutex_);
            this->threads_[this->IDcount_] = newthread;
        }
        {
            // mutes threadqueues
            boost::mutex::scoped_lock lock(this->threadQueuesMutex_);
            this->threadQueues_[this->IDcount_];
        }
        return true;
    }

    bool TclThreadManager::destroyTclThread(unsigned int threadID)
    {
        // mutex threads
        boost::mutex::scoped_lock lock(this->threadsMutex_);

        std::map<unsigned int, TclThread>::iterator it = this->threads_.find(threadID);
        if (it != this->threads_.end())
        {
            delete (*it).second.interpreter_;
            delete (*it).second.thread_;
            delete (*it).second.mutex_;

            this->threads_.erase(it);
            {
                // mutex threadqueues
                boost::mutex::scoped_lock lock(this->threadQueuesMutex_);
                this->threadQueues_.erase(threadID);
            }
            return true;
        }
        else
        {
            COUT(1) << "Error: No Tcl-thread with ID " << threadID << "." << std::endl;
            return false;
        }
    }

    void TclThreadManager::pushCommandBack(const std::string& command)
    {
        // mutex orxqueue
        boost::mutex::scoped_lock lock(this->orxonoxQueueMutex_);

        while (this->orxonoxQueue_.size() == TCLTHREAD_MAX_QUEUE_LENGTH)
            this->orxonoxQueueCondition_.wait(lock);

        this->orxonoxQueue_.push(command);
    }

    std::string TclThreadManager::popCommandFront()
    {
        // mutex orxqueue
        boost::mutex::scoped_lock lock(this->orxonoxQueueMutex_);

        std::string command = this->orxonoxQueue_.front();
        this->orxonoxQueue_.pop();

        this->orxonoxQueueCondition_.notify_one();

        return command;
    }

    bool TclThreadManager::isEmpty()
    {
        // mutex orxqueue
        boost::mutex::scoped_lock lock(this->orxonoxQueueMutex_);

        return this->orxonoxQueue_.empty();
    }

    void TclThreadManager::pushCommandBack(unsigned int threadID, const std::string& command)
    {
        // mutex threadqueues
        boost::mutex::scoped_lock lock(this->threadQueuesMutex_);

        std::map<unsigned int, std::queue<std::string> >::iterator it = this->threadQueues_.find(threadID);
        if (it != this->threadQueues_.end())
        {
            (*it).second.push(command);
        }
        else
        {
            COUT(1) << "Error: No Tcl-thread with ID " << threadID << "." << std::endl;
        }
    }

    std::string TclThreadManager::popCommandFront(unsigned int threadID)
    {
        // mutex threadqueues
        boost::mutex::scoped_lock lock(this->threadQueuesMutex_);

        std::map<unsigned int, std::queue<std::string> >::iterator it = this->threadQueues_.find(threadID);
        if (it != this->threadQueues_.end())
        {
            std::string command = (*it).second.front();
            (*it).second.pop();
            return command;
        }
        else
        {
            COUT(1) << "Error: No Tcl-thread with ID " << threadID << "." << std::endl;
            return "";
        }
    }

    bool TclThreadManager::isEmpty(unsigned int threadID)
    {
        // mutex threadqueues
        boost::mutex::scoped_lock lock(this->threadQueuesMutex_);

        std::map<unsigned int, std::queue<std::string> >::iterator it = this->threadQueues_.find(threadID);
        if (it != this->threadQueues_.end())
        {
            return (*it).second.empty();
        }
        else
        {
            COUT(1) << "Error: No Tcl-thread with ID " << threadID << "." << std::endl;
            return true;
        }
    }

    std::string TclThreadManager::eval(const std::string& command)
    {
        boost::mutex::scoped_lock lock(this->orxonoxEvalMutex_);

        while (!this->isReady_)
            this->orxonoxEvalCondition_.wait(lock);

        CommandExecutor::execute(command, false);

        if (CommandExecutor::getLastEvaluation().hasReturnvalue())
            return CommandExecutor::getLastEvaluation().getReturnvalue().toString();

        return "";
    }

    std::string TclThreadManager::eval(unsigned int threadID, const std::string& command)
    {
        // mutex threads
        boost::mutex::scoped_lock lock(this->threadsMutex_);

        std::map<unsigned int, TclThread>::iterator it = this->threads_.find(threadID);
        if (it != this->threads_.end())
        {
            boost::try_mutex::scoped_try_lock lock(*(*it).second.mutex_, boost::defer_lock_t());
            bool threadIsReady = false;
            try
            {
                threadIsReady = lock.try_lock();
            }
            catch(boost::lock_error& e){ std::cout << "lockerror thread" << std::endl; }

            if (threadIsReady)
            {
                try
                {
std::cout << "2_1\n";
                    return (*it).second.interpreter_->eval(command);
                }
                catch (Tcl::tcl_error const &e)
                {
std::cout << "2_2\n";
                    COUT(1) << "Tcl error: " << e.what() << std::endl;
                }
                catch (std::exception const &e)
                {
std::cout << "2_3\n";
                    COUT(1) << "Error while executing tcl: " << e.what() << std::endl;
                }
            }
            else
            {
                COUT(1) << "Error: Tcl-thread with ID " << threadID << " is not ready. Wait until it's finished or start a new thread." << std::endl;
            }
        }
        else
        {
            COUT(1) << "Error: No Tcl-thread with ID " << threadID << "." << std::endl;
        }
        return "";
    }

    void TclThreadManager::tick(float dt)
    {
        {
            boost::mutex::scoped_lock lock(this->orxonoxEvalMutex_);
            this->isReady_ = true;
        }

        this->orxonoxEvalCondition_.notify_one();
        boost::this_thread::yield();

        {
            boost::mutex::scoped_lock lock(this->orxonoxEvalMutex_);
            this->isReady_ = false;
        }

        unsigned long maxtime = (unsigned long)(dt * 1000000 * TCLTHREAD_MAX_CPU_USAGE);
        Ogre::Timer timer;
        while (!TclThreadManager::getInstance().isEmpty())
        {
            CommandExecutor::execute(TclThreadManager::getInstance().popCommandFront(), false);
            if (timer.getMicroseconds() > maxtime)
                break;
        }
    }

    void tclThreadLoop(Tcl::interpreter* interpreter, boost::try_mutex* mutex, unsigned int threadID)
    {
        while (true)
        {
            {
                boost::try_mutex::scoped_try_lock lock(*mutex, boost::defer_lock_t());
                bool threadIsReady = false;
                try
                {
                    threadIsReady = lock.try_lock();
                }
                catch(boost::lock_error& e){ std::cout << "lockerror thread" << std::endl; }

                if (threadIsReady)
                {
                    while (!TclThreadManager::getInstance().isEmpty(threadID))
                    {
                        try
                        {
std::cout << "1\n";
std::cout << threadID << std::endl;
std::string temp = TclThreadManager::getInstance().popCommandFront(threadID);
std::cout << temp << std::endl;
                            interpreter->eval(temp);
std::cout << "2\n";
                        }
                        catch (Tcl::tcl_error const &e)
                        {
std::cout << "3\n";
                            TclThreadManager::getInstance().pushCommandBack("error Tcl error (thread " + getConvertedValue<unsigned int, std::string>(threadID) + "): " + e.what());
                        }
                        catch (std::exception const &e)
                        {
std::cout << "4\n";
                            TclThreadManager::getInstance().pushCommandBack("error Error while executing tcl (thread " + getConvertedValue<unsigned int, std::string>(threadID) + "): " + e.what());
                        }
                        catch (...)
                        {
std::cout << "5\n";
                            TclThreadManager::getInstance().pushCommandBack("error Error while executing tcl (thread " + getConvertedValue<unsigned int, std::string>(threadID) + ").");
                        }
                    }
                }
            }

            while (TclThreadManager::getInstance().isEmpty(threadID))
                boost::this_thread::yield();
        }
    }
}
