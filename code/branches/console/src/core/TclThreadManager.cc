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
    ConsoleCommand(TclThreadManager, status,    AccessLevel::None, false);
    ConsoleCommand(TclThreadManager, dump,      AccessLevel::None, false);

    TclThreadManager* instance = &TclThreadManager::getInstance();

    TclThreadManager::TclThreadManager()
    {
        RegisterRootObject(TclThreadManager);

        this->IDcount_ = 0;
        this->isReady_ = false;
        this->isQuerying_ = false;
        this->queryID_ = 0;
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

    void TclThreadManager::status()
    {
        COUT(0) << "Thread ID" << '\t' << "Queue size" << '\t' << "State" << std::endl;

        std::string output = "Orxonox";
        output += "\t\t";
        {
            // mutex orxqueue
            boost::mutex::scoped_lock lock(TclThreadManager::getInstance().orxonoxQueueMutex_);
            output += getConvertedValue<unsigned int, std::string>(TclThreadManager::getInstance().orxonoxQueue_.size());
        }
        output += "\t\t";
        {
            // mutex orxstate
            boost::mutex::scoped_lock lock(TclThreadManager::getInstance().orxonoxStateMutex_);
            if (TclThreadManager::getInstance().isReady_)
                output += "ready";
            else
                output += "busy";
        }
        COUT(0) << output << std::endl;

        // mutex threads
        boost::mutex::scoped_lock lock(TclThreadManager::getInstance().threadsMutex_);
        for (std::map<unsigned int, TclThread*>::const_iterator it = TclThreadManager::getInstance().threads_.begin(); it != TclThreadManager::getInstance().threads_.end(); ++it)
        {
            std::string output = getConvertedValue<unsigned int, std::string>((*it).first);
            output += "\t\t";
            {
                boost::mutex::scoped_lock lock(TclThreadManager::getInstance().threadQueuesMutex_);
                std::map<unsigned int, std::pair<std::queue<std::string>, boost::condition*> >::const_iterator it2 = TclThreadManager::getInstance().threadQueues_.find((*it).first);
                if (it2 != TclThreadManager::getInstance().threadQueues_.end())
                    output += getConvertedValue<unsigned int, std::string>((*it2).second.first.size());
                else
                    output += "-";
            }
            output += "\t\t";
            if (TclThreadManager::getInstance().getState((*it).second) == TclThread::Ready)
                output += "ready";
            else
                output += "busy";

            COUT(0) << output << std::endl;
        }
    }

    void TclThreadManager::dump(unsigned int threadID)
    {
        if (threadID == 0)
        {
            // mutex orxqueue
            boost::mutex::scoped_lock lock(TclThreadManager::getInstance().orxonoxQueueMutex_);

            COUT(0) << "Queue dump of Orxonox:" << std::endl;
            for (unsigned int index = 0; index < TclThreadManager::getInstance().orxonoxQueue_.size(); index++)
            {
                std::string command = TclThreadManager::getInstance().orxonoxQueue_.front();
                COUT(0) << index << ": " << command << std::endl;
                TclThreadManager::getInstance().orxonoxQueue_.pop();
                TclThreadManager::getInstance().orxonoxQueue_.push(command);
            }
        }
        else
        {
            // mutex threadqueues
            boost::mutex::scoped_lock lock(TclThreadManager::getInstance().threadQueuesMutex_);

            std::map<unsigned int, std::pair<std::queue<std::string>, boost::condition*> >::iterator it = TclThreadManager::getInstance().threadQueues_.find(threadID);
            if (it != TclThreadManager::getInstance().threadQueues_.end())
            {
                COUT(0) << "Queue dump of Tcl-thread " << threadID << ":" << std::endl;
                for (unsigned int index = 0; index < (*it).second.first.size(); index++)
                {
                    std::string command = (*it).second.first.front();
                    COUT(0) << index << ": " << command << std::endl;
                    (*it).second.first.pop();
                    (*it).second.first.push(command);
                }
            }
            else
            {
                COUT(1) << "Error: (" << __LINE__ << ") No Tcl-thread with ID " << threadID << "." << std::endl;
            }
        }
    }

    void TclThreadManager::tcl_execute(Tcl::object const &args)
    {
std::cout << "Tcl-thread_execute: args: " << args.get() << std::endl;
        std::string command = args.get();

        if (command.size() >= 2 && command[0] == '{' && command[command.size() - 1] == '}')
            command = command.substr(1, command.size() - 2);

        TclThreadManager::getInstance().pushCommandBack(command);
    }

    std::string TclThreadManager::tcl_query(int id, Tcl::object const &args)
    {
std::cout << "Tcl-thread_query: id: " << id << " args: " << args.get()  << std::endl;
        {
            // mutex query
            boost::mutex::scoped_lock lock(TclThreadManager::getInstance().orxonoxQueryMutex_);
            if (TclThreadManager::getInstance().isQuerying_ && (id == (int)TclThreadManager::getInstance().queryID_))
            {
                COUT(1) << "Error: Orxonox Tcl-interpreter can't be queried right now." << std::endl;
                return "";
            }
        }

        std::string command = args.get();

        while (command.size() >= 2 && command[0] == '{' && command[command.size() - 1] == '}')
            command = command.substr(1, command.size() - 2);

        return TclThreadManager::getInstance().eval(command);
    }

    Tcl::interpreter* TclThreadManager::createTclInterpreter(unsigned int threadID) const
    {
        Tcl::interpreter* i = 0;
        i = new Tcl::interpreter(TclBind::getInstance().getTclLibPath());

        i->def("query", TclThreadManager::tcl_query, Tcl::variadic());
        i->def("execute", TclThreadManager::tcl_execute, Tcl::variadic());

        std::string id = getConvertedValue<unsigned int, std::string>(threadID);
        try
        {
            i->eval("rename exit tclexit; proc exit {} { orxonox TclThreadManager destroy " + id + " }");
            i->eval("proc orxonox args { query " + id + " $args }");
            i->eval("set threadid " + id);
            i->eval("redef_puts");
        }
        catch (Tcl::tcl_error const &e)
        {
            COUT(1) << "Tcl error: " << e.what() << std::endl;
        }
        catch (std::exception const &e)
        {
            COUT(1) << "Error while creating Tcl-thread (" << id << "): " << e.what() << std::endl;
        }

        return i;
    }

    bool TclThreadManager::createTclThread()
    {
        this->IDcount_++;
        TclThread* newthread = new TclThread;
        newthread->threadID_ = this->IDcount_;
        newthread->interpreter_ = this->createTclInterpreter(this->IDcount_);
        newthread->evalMutex_ = new boost::mutex;
        newthread->stateMutex_ = new boost::mutex;
        newthread->state_ = new TclThread::State(TclThread::Ready);
        newthread->thread_ = new boost::thread(boost::bind(&tclThreadLoop, newthread));
        {
            // mutex threads
            boost::mutex::scoped_lock lock(this->threadsMutex_);
            this->threads_[this->IDcount_] = newthread;
        }
        {
            // mutes threadqueues
            boost::mutex::scoped_lock lock(this->threadQueuesMutex_);
            this->threadQueues_[this->IDcount_] = std::pair<std::queue<std::string>, boost::condition*>(std::queue<std::string>(), new boost::condition());
        }
        return true;
    }

    bool TclThreadManager::destroyTclThread(unsigned int threadID)
    {
        // mutex threads
        boost::mutex::scoped_lock lock(this->threadsMutex_);

        std::map<unsigned int, TclThread*>::iterator it = this->threads_.find(threadID);
        if (it != this->threads_.end())
        {
            this->setState((*it).second, TclThread::Finished);
            (*it).second->thread_->timed_join(boost::posix_time::time_duration(0, 0, 0, 10));

            delete (*it).second->interpreter_;
            delete (*it).second->thread_;
            delete (*it).second->evalMutex_;
            delete (*it).second->stateMutex_;
            delete (*it).second->state_;
            delete (*it).second;

            this->threads_.erase(it);
            {
                // mutex threadqueues
                boost::mutex::scoped_lock lock(this->threadQueuesMutex_);
                std::map<unsigned int, std::pair<std::queue<std::string>, boost::condition*> >::iterator it = this->threadQueues_.find(threadID);
                if (it != this->threadQueues_.end())
                {
                    delete (*it).second.second;
                    this->threadQueues_.erase(threadID);
                }
                else
                {
                    return false;
                }
            }
            return true;
        }
        else
        {
            COUT(1) << "Error: (" << __LINE__ << ") No Tcl-thread with ID " << threadID << "." << std::endl;
            return false;
        }
    }

    void TclThreadManager::setState(TclThread* tclThread, TclThread::State state)
    {
        // mutex state
        boost::mutex::scoped_lock lock(*tclThread->stateMutex_);
        *tclThread->state_ = state;
    }

    TclThread::State TclThreadManager::getState(TclThread* tclThread)
    {
        // mutex state
        boost::mutex::scoped_lock lock(*tclThread->stateMutex_);
        TclThread::State state = *tclThread->state_;
        return state;
    }

    void TclThreadManager::pushCommandBack(const std::string& command)
    {
        // mutex orxqueue
        boost::mutex::scoped_lock lock(this->orxonoxQueueMutex_);

        while (this->orxonoxQueue_.size() >= TCLTHREAD_MAX_QUEUE_LENGTH)
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

        std::map<unsigned int, std::pair<std::queue<std::string>, boost::condition*> >::iterator it = this->threadQueues_.find(threadID);
        if (it != this->threadQueues_.end())
        {
            while ((*it).second.first.size() >= TCLTHREAD_MAX_QUEUE_LENGTH)
                (*it).second.second->wait(lock);

            (*it).second.first.push(command);
        }
        else
        {
            COUT(1) << "Error: (" << __LINE__ << ") No Tcl-thread with ID " << threadID << "." << std::endl;
        }
    }

    std::string TclThreadManager::popCommandFront(unsigned int threadID)
    {
        // mutex threadqueues
        boost::mutex::scoped_lock lock(this->threadQueuesMutex_);

        std::map<unsigned int, std::pair<std::queue<std::string>, boost::condition*> >::iterator it = this->threadQueues_.find(threadID);
        if (it != this->threadQueues_.end())
        {
            std::string command = (*it).second.first.front();
            (*it).second.first.pop();
            return command;
        }
        else
        {
            COUT(1) << "Error: (" << __LINE__ << ") No Tcl-thread with ID " << threadID << "." << std::endl;
            return "";
        }
    }

    bool TclThreadManager::isEmpty(unsigned int threadID)
    {
        // mutex threadqueues
        boost::mutex::scoped_lock lock(this->threadQueuesMutex_);

        std::map<unsigned int, std::pair<std::queue<std::string>, boost::condition*> >::const_iterator it = this->threadQueues_.find(threadID);
        if (it != this->threadQueues_.end())
        {
            return (*it).second.first.empty();
        }
        else
        {
            COUT(1) << "Error: (" << __LINE__ << ") No Tcl-thread with ID " << threadID << "." << std::endl;
            return true;
        }
    }

    std::string TclThreadManager::eval(const std::string& command)
    {
        // mutex orxstate
        boost::mutex::scoped_lock lock(this->orxonoxStateMutex_);

        while (!this->isReady_)
            this->orxonoxEvalCondition_.wait(lock);

        CommandExecutor::execute(command, false);

        if (CommandExecutor::getLastEvaluation().hasReturnvalue())
            return CommandExecutor::getLastEvaluation().getReturnvalue().toString();

        return "";
    }

    std::string TclThreadManager::eval(unsigned int threadID, const std::string& command)
    {
        {
            // mutex query
            boost::mutex::scoped_lock lock(this->orxonoxQueryMutex_);
            this->isQuerying_ = true;
            this->queryID_ = threadID;
std::cout << "2_0\n";
        }

        {
            // mutex threads
            boost::mutex::scoped_lock lock(this->threadsMutex_);

            std::map<unsigned int, TclThread*>::iterator it = this->threads_.find(threadID);
            if (it != this->threads_.end())
            {
std::cout << "2_1\n";
                if (this->getState((*it).second) == TclThread::Ready)
                {
std::cout << "2_2\n";
                    // mutex threadeval
                    boost::mutex::scoped_lock lock(*(*it).second->evalMutex_);
                    try
                    {
std::cout << "2_3\n";
                        return (*it).second->interpreter_->eval(command);
                    }
                    catch (Tcl::tcl_error const &e)
                    {
                        COUT(1) << "Tcl error: " << e.what() << std::endl;
                    }
                    catch (std::exception const &e)
                    {
                        COUT(1) << "Error while executing Tcl: " << e.what() << std::endl;
                    }
    std::cout << "2_4\n";
                }
                else
                {
                    COUT(1) << "Error: Tcl-thread with ID " << threadID << " is not ready. Wait until it's finished or start a new thread." << std::endl;
                }
            }
            else
            {
                COUT(1) << "Error: (" << __LINE__ << ") No Tcl-thread with ID " << threadID << "." << std::endl;
            }
        }

        {
            // mutex query
            boost::mutex::scoped_lock lock(this->orxonoxQueryMutex_);
            this->isQuerying_ = false;
            this->queryID_ = 0;
std::cout << "2_5\n";
        }

        return "";
    }

    void TclThreadManager::tick(float dt)
    {
        {
            // mutex orxstate
            boost::mutex::scoped_lock lock(this->orxonoxStateMutex_);
            this->isReady_ = true;
        }

        this->orxonoxEvalCondition_.notify_one();
        boost::this_thread::yield();

        {
            // mutex orxstate
            boost::mutex::scoped_lock lock(this->orxonoxStateMutex_);
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

    void tclThreadLoop(TclThread* tclThread)
    {
        while (true)
        {
            if (!TclThreadManager::getInstance().isEmpty(tclThread->threadID_))
            {
                std::cout << "a\n";
                TclThreadManager::getInstance().setState(tclThread, TclThread::Busy);
//                if (!TclThreadManager::getInstance().isEmpty(tclThread->threadID_))
                {
                    try
                    {
                        std::cout << "c\n";
                        throw std::exception();
                        std::cout << "d\n";
                    }
                    catch (...)
                    {
                        std::cout << "e\n";
                    }
                }
                TclThreadManager::getInstance().setState(tclThread, TclThread::Ready);
                std::cout << "f\n";

//                boost::this_thread::yield();
//                if (TclThreadManager::getInstance().getState(tclThread) == TclThread::Finished)
//                    return;
            }
        }

/*        while (true)
        {
            TclThreadManager::getInstance().setState(tclThread, TclThread::Busy);
            while (!TclThreadManager::getInstance().isEmpty(tclThread->threadID_))
            {
                try
                {
std::cout << "1_1\n";
                    // mutex threadeval
                    boost::mutex::scoped_lock lock(*tclThread->evalMutex_);
                    tclThread->interpreter_->eval(TclThreadManager::getInstance().popCommandFront(tclThread->threadID_));
std::cout << "1_2\n";
                }
                catch (Tcl::tcl_error const &e)
                {
                    TclThreadManager::getInstance().pushCommandBack("error Tcl error (thread " + getConvertedValue<unsigned int, std::string>(tclThread->threadID_) + "): " + e.what());
                }
                catch (std::exception const &e)
                {
                    TclThreadManager::getInstance().pushCommandBack("error Error while executing Tcl (thread " + getConvertedValue<unsigned int, std::string>(tclThread->threadID_) + "): " + e.what());
                }
std::cout << "1_4\n";
            }
            TclThreadManager::getInstance().setState(tclThread, TclThread::Ready);

            while (TclThreadManager::getInstance().isEmpty(tclThread->threadID_))
            {
                boost::this_thread::yield();
                if (TclThreadManager::getInstance().getState(tclThread) == TclThread::Finished)
                    return;
            }
        }*/
    }
}
