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

#define TCLTHREADMANAGER_MAX_QUEUE_LENGTH 1024
#define TCLTHREADMANAGER_MAX_CPU_USAGE 0.50

namespace orxonox
{
//    ConsoleCommand(TclThreadManager, tclthread, AccessLevel::None, true);
    ConsoleCommand(TclThreadManager, create,    AccessLevel::None, false);
    ConsoleCommand(TclThreadManager, destroy,   AccessLevel::None, false);
    ConsoleCommand(TclThreadManager, execute,   AccessLevel::None, false);
    ConsoleCommand(TclThreadManager, query,     AccessLevel::None, false);
//    ConsoleCommand(TclThreadManager, status,    AccessLevel::None, false);
//    ConsoleCommand(TclThreadManager, dump,      AccessLevel::None, false);

    TclThreadManager* instance = &TclThreadManager::getInstance();

    TclThreadManager::TclThreadManager()
    {
        RegisterRootObject(TclThreadManager);

        this->threadCounter_ = 0;
        this->orxonoxInterpreterBundle_.id_ = 0;
        this->orxonoxInterpreterBundle_.interpreter_ = TclBind::getInstance().getTclInterpreter();
    }

    TclThreadManager& TclThreadManager::getInstance()
    {
        static TclThreadManager instance;
        return instance;
    }

    unsigned int TclThreadManager::create()
    {
        boost::mutex::scoped_lock lock(TclThreadManager::getInstance().bundlesMutex_);
        TclThreadManager::getInstance().threadCounter_++;
        std::string name = getConvertedValue<unsigned int, std::string>(TclThreadManager::getInstance().threadCounter_);

        TclInterpreterBundle* bundle = new TclInterpreterBundle;
        bundle->id_ = TclThreadManager::getInstance().threadCounter_;
        bundle->interpreter_ = TclThreadManager::getInstance().createNewTclInterpreter(name);
        bundle->interpreterName_ = name;
        bundle->running_ = true;

        TclThreadManager::getInstance().interpreterBundles_[TclThreadManager::getInstance().threadCounter_] = bundle;
        COUT(0) << "Created new Tcl-interpreter with ID " << TclThreadManager::getInstance().threadCounter_ << std::endl;
        return TclThreadManager::getInstance().threadCounter_;
    }

    void TclThreadManager::destroy(unsigned int threadID)
    {
        TclInterpreterBundle* bundle = TclThreadManager::getInstance().getInterpreterBundle(threadID);
        if (bundle)
        {
            {
                boost::mutex::scoped_lock lock(bundle->runningMutex_);
                bundle->running_ = false;
            }
            {
                boost::mutex::scoped_lock lock(bundle->finishedMutex_);
                while (!bundle->finished_)
                    bundle->finishedCondition_.wait(lock);
            }
            {
                boost::mutex::scoped_lock lock(bundle->interpreterMutex_);
                delete bundle->interpreter_;
            }
            delete bundle;
            {
                boost::mutex::scoped_lock lock(TclThreadManager::getInstance().bundlesMutex_);
                TclThreadManager::getInstance().interpreterBundles_.erase(threadID);
            }
        }
    }

    void TclThreadManager::execute(unsigned int threadID, const std::string& command)
    {
        TclThreadManager::getInstance().pushCommandToQueue(threadID, command);
    }

    std::string TclThreadManager::query(unsigned int threadID, const std::string& command)
    {
        return TclThreadManager::getInstance().evalQuery(TclThreadManager::getInstance().orxonoxInterpreterBundle_.id_, threadID, command);
    }

    void TclThreadManager::tcl_execute(Tcl::object const &args)
    {
        std::string command = args.get();
        while (command.size() >= 2 && command[0] == '{' && command[command.size() - 1] == '}')
            command = command.substr(1, command.size() - 2);

        TclThreadManager::getInstance().pushCommandToQueue(command);
    }

    std::string TclThreadManager::tcl_query(int querierID, Tcl::object const &args)
    {
        std::string command = args.get();
        while (command.size() >= 2 && command[0] == '{' && command[command.size() - 1] == '}')
            command = command.substr(1, command.size() - 2);

        return TclThreadManager::getInstance().evalQuery((unsigned int)querierID, command);
    }

    std::string TclThreadManager::tcl_crossquery(int querierID, int threadID, Tcl::object const &args)
    {
        std::string command = args.get();
        while (command.size() >= 2 && command[0] == '{' && command[command.size() - 1] == '}')
            command = command.substr(1, command.size() - 2);

        return TclThreadManager::getInstance().evalQuery((unsigned int)querierID, (unsigned int)threadID, command);
    }

    bool TclThreadManager::tcl_running(int threadID)
    {
        TclInterpreterBundle* bundle = TclThreadManager::getInstance().getInterpreterBundle((unsigned int)threadID);
        if (bundle)
        {
            boost::mutex::scoped_lock lock(bundle->runningMutex_);
            return bundle->running_;
        }
        return false;
    }

    Tcl::interpreter* TclThreadManager::createNewTclInterpreter(const std::string& threadID)
    {
        Tcl::interpreter* i = 0;
        i = new Tcl::interpreter(TclBind::getInstance().getTclLibPath());

        try
        {
            i->def("orxonox::query", TclThreadManager::tcl_query, Tcl::variadic());
            i->def("orxonox::crossquery", TclThreadManager::tcl_crossquery, Tcl::variadic());
            i->def("orxonox::execute", TclThreadManager::tcl_execute, Tcl::variadic());
            i->def("orxonox::running", TclThreadManager::tcl_running);

            i->def("execute", TclThreadManager::tcl_execute, Tcl::variadic());
            i->eval("proc query args { orxonox::query " + threadID + " $args }");
            i->eval("proc crossquery {id args} { orxonox::crossquery " + threadID + " $id $args }");
            i->eval("set id " + threadID);

            i->eval("rename exit tcl::exit");
            i->eval("proc exit {} { orxonox TclThreadManager destroy " + threadID + " }");

            i->eval("redef_puts");

            i->eval("rename while tcl::while");
            i->eval("proc while {test command} { tcl::while {$test && [orxonox::running " + threadID + "]} \"$command\" }");
//            i->eval("rename for tcl::for");
//            i->eval("proc for {start test next command} { uplevel tcl::for \"$start\" \"$test\" \"$next\" \"$command\" }");
        }
        catch (Tcl::tcl_error const &e)
        {   COUT(1) << "Tcl error while creating Tcl-interpreter (" << threadID << "): " << e.what() << std::endl;   }
        catch (std::exception const &e)
        {   COUT(1) << "Error while creating Tcl-interpreter (" << threadID << "): " << e.what() << std::endl;   }

        return i;
    }

    TclInterpreterBundle* TclThreadManager::getInterpreterBundle(unsigned int threadID)
    {
        if (threadID == 0)
            return &this->orxonoxInterpreterBundle_;

        boost::mutex::scoped_lock lock(this->bundlesMutex_);
        std::map<unsigned int, TclInterpreterBundle*>::iterator it = this->interpreterBundles_.find(threadID);
        if (it != this->interpreterBundles_.end())
        {
            return (*it).second;
        }
        else
        {
            this->forceCommandToFrontOfQueue("error Error: No Tcl-interpreter with ID " + getConvertedValue<unsigned int, std::string>(threadID) + " existing.");
            return 0;
        }
    }

    std::string TclThreadManager::dumpList(const std::list<unsigned int>& list)
    {
        std::string output = "";
        for (std::list<unsigned int>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            if (it != list.begin())
                output += " ";

            output += getConvertedValue<unsigned int, std::string>(*it);
        }
        return output;
    }

    void TclThreadManager::pushCommandToQueue(const std::string& command)
    {
        boost::mutex::scoped_lock lock(this->orxonoxInterpreterBundle_.queueMutex_);
        while (this->orxonoxInterpreterBundle_.queue_.size() >= TCLTHREADMANAGER_MAX_QUEUE_LENGTH)
            this->fullQueueCondition_.wait(lock);

        this->orxonoxInterpreterBundle_.queue_.push_back(command);
    }

    void TclThreadManager::forceCommandToFrontOfQueue(const std::string& command)
    {
        boost::mutex::scoped_lock lock(this->orxonoxInterpreterBundle_.queueMutex_);
        this->orxonoxInterpreterBundle_.queue_.push_front(command);
    }

    std::string TclThreadManager::popCommandFromQueue()
    {
        boost::mutex::scoped_lock lock(this->orxonoxInterpreterBundle_.queueMutex_);
        std::string temp = this->orxonoxInterpreterBundle_.queue_.front();
        this->orxonoxInterpreterBundle_.queue_.pop_front();
        this->fullQueueCondition_.notify_one();
        return temp;
    }

    bool TclThreadManager::queueIsEmpty()
    {
        boost::mutex::scoped_lock lock(this->orxonoxInterpreterBundle_.queueMutex_);
        return this->orxonoxInterpreterBundle_.queue_.empty();
    }

    void TclThreadManager::pushCommandToQueue(unsigned int threadID, const std::string& command)
    {
        TclInterpreterBundle* bundle = this->getInterpreterBundle(threadID);
        if (bundle)
        {
            boost::mutex::scoped_lock lock(bundle->queueMutex_);
            if (bundle->queue_.size() >= TCLTHREADMANAGER_MAX_QUEUE_LENGTH)
            {
                this->forceCommandToFrontOfQueue("error Error: Queue of Tcl-interpreter " + getConvertedValue<unsigned int, std::string>(threadID) + " is full, couldn't add command.");
                return;
            }

            bundle->queue_.push_back(command);
        }
    }

    std::string TclThreadManager::popCommandFromQueue(unsigned int threadID)
    {
        TclInterpreterBundle* bundle = this->getInterpreterBundle(threadID);
        if (bundle)
        {
            boost::mutex::scoped_lock lock(bundle->queueMutex_);
            std::string temp = bundle->queue_.front();
            bundle->queue_.pop_front();
            return temp;
        }
        return "";
    }

    bool TclThreadManager::queueIsEmpty(unsigned int threadID)
    {
        TclInterpreterBundle* bundle = this->getInterpreterBundle(threadID);
        if (bundle)
        {
            boost::mutex::scoped_lock lock(bundle->queueMutex_);
            return bundle->queue_.empty();
        }
        return true;
    }

    bool TclThreadManager::updateQueriersList(TclInterpreterBundle* querier, TclInterpreterBundle* target)
    {
        if (querier == target)
            return false;

        boost::mutex::scoped_lock lock(target->queriersMutex_);

        {
            boost::mutex::scoped_lock lock(querier->queriersMutex_);
            target->queriers_.insert(target->queriers_.end(), querier->queriers_.begin(), querier->queriers_.end());
        }

        target->queriers_.insert(target->queriers_.end(), querier->id_);

        if (std::find(target->queriers_.begin(), target->queriers_.end(), target->id_) != target->queriers_.end())
        {
            this->forceCommandToFrontOfQueue("error Error: Circular query (" + this->dumpList(target->queriers_) + " -> " + getConvertedValue<unsigned int, std::string>(target->id_) + "), couldn't query Tcl-interpreter with ID " + getConvertedValue<unsigned int, std::string>(target->id_) + " from other interpreter with ID " + getConvertedValue<unsigned int, std::string>(querier->id_) + ".");
            return false;
        }

        return true;
    }

    std::string TclThreadManager::evalQuery(unsigned int querierID, const std::string& command)
    {
        TclInterpreterBundle* querier = this->getInterpreterBundle(querierID);
        std::string output = "";
        if (querier)
        {
            if (this->updateQueriersList(querier, &this->orxonoxInterpreterBundle_))
            {
                boost::mutex::scoped_lock lock(this->orxonoxInterpreterBundle_.interpreterMutex_);
                this->orxonoxEvalCondition_.wait(lock);

                if (!CommandExecutor::execute(command, false))
                    this->forceCommandToFrontOfQueue("error Error: Can't execute command \"" + command + "\"!");

                if (CommandExecutor::getLastEvaluation().hasReturnvalue())
                    output = CommandExecutor::getLastEvaluation().getReturnvalue().toString();
            }

            boost::mutex::scoped_lock lock(this->orxonoxInterpreterBundle_.queriersMutex_);
            this->orxonoxInterpreterBundle_.queriers_.clear();
        }
        return output;
    }

    std::string TclThreadManager::evalQuery(unsigned int querierID, unsigned int threadID, const std::string& command)
    {
        TclInterpreterBundle* target = this->getInterpreterBundle(threadID);
        std::string output = "";
        if (target)
        {
            TclInterpreterBundle* querier = 0;
            if (querierID)
                querier = this->getInterpreterBundle(querierID);
            else
                querier = &this->orxonoxInterpreterBundle_;

            if (querier)
            {
                if (this->updateQueriersList(querier, target))
                {
                    boost::mutex::scoped_lock lock(target->interpreterMutex_, boost::defer_lock_t());
                    bool successfullyLocked = false;
                    try
                    {
                        if (querierID == 0 || std::find(querier->queriers_.begin(), querier->queriers_.end(), (unsigned int)0) != querier->queriers_.end())
                            successfullyLocked = lock.try_lock();
                        else
                        {
                            while (!lock.try_lock())
                                boost::this_thread::yield();

                            successfullyLocked = true;
                        }
                    } catch (...) {}

                    if (successfullyLocked)
                    {
                        try
                        {   output = (std::string)target->interpreter_->eval(command);   }
                        catch (Tcl::tcl_error const &e)
                        {   this->forceCommandToFrontOfQueue("error Tcl error: " + (std::string)e.what());   }
                        catch (std::exception const &e)
                        {   this->forceCommandToFrontOfQueue("error Error while executing Tcl: " + (std::string)e.what());   }
                    }
                    else
                    {
                        this->forceCommandToFrontOfQueue("error Error: Couldn't query Tcl-interpreter with ID " + getConvertedValue<unsigned int, std::string>(threadID) + ", interpreter is busy right now.");
                    }
                }

                boost::mutex::scoped_lock lock(target->queriersMutex_);
                target->queriers_.clear();
            }
        }
        return output;
    }

    void TclThreadManager::tick(float dt)
    {
        {
            this->orxonoxEvalCondition_.notify_one();
            boost::this_thread::yield();
        }

        {
            boost::mutex::scoped_lock lock(this->bundlesMutex_);
            for (std::map<unsigned int, TclInterpreterBundle*>::iterator it = this->interpreterBundles_.begin(); it != this->interpreterBundles_.end(); ++it)
            {
                boost::mutex::scoped_lock lock((*it).second->queueMutex_);
                if (!(*it).second->queue_.empty())
                {
                    std::string command = (*it).second->queue_.front();
                    (*it).second->queue_.pop_front();

                    {
                        boost::mutex::scoped_lock lock((*it).second->finishedMutex_);
                        (*it).second->finished_ = false;
                    }

                    boost::thread(boost::bind(&tclThread, (*it).second, command));
                }
            }
        }

        {
            boost::mutex::scoped_lock lock(this->orxonoxInterpreterBundle_.interpreterMutex_);
            unsigned long maxtime = (unsigned long)(dt * 1000000 * TCLTHREADMANAGER_MAX_CPU_USAGE);
            Ogre::Timer timer;
            while (!this->queueIsEmpty())
            {
                CommandExecutor::execute(this->popCommandFromQueue(), false);
                if (timer.getMicroseconds() > maxtime)
                    break;
            }
        }
    }

    void tclThread(TclInterpreterBundle* interpreterBundle, std::string command)
    {
        try
        {
            boost::mutex::scoped_lock lock(interpreterBundle->interpreterMutex_);
            interpreterBundle->interpreter_->eval(command);
        }
        catch (Tcl::tcl_error const &e)
        {
            TclThreadManager::getInstance().forceCommandToFrontOfQueue("error Tcl (ID " + getConvertedValue<unsigned int, std::string>(interpreterBundle->id_) + ") error: " + e.what());
        }
        catch (std::exception const &e)
        {
            TclThreadManager::getInstance().forceCommandToFrontOfQueue("error Error while executing Tcl (ID " + getConvertedValue<unsigned int, std::string>(interpreterBundle->id_) + "): " + e.what());
        }

        boost::mutex::scoped_lock lock(interpreterBundle->finishedMutex_);
        interpreterBundle->finished_ = true;
        interpreterBundle->finishedCondition_.notify_all();
    }
}
