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

#include "TclThreadManager.h"

#include <boost/bind.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <OgreTimer.h>
#include <cpptcl/cpptcl.h>

#include "util/Convert.h"
#include "util/Debug.h"
#include "Clock.h"
#include "CommandExecutor.h"
#include "ConsoleCommand.h"
#include "CoreIncludes.h"
#include "TclBind.h"

namespace orxonox
{
    const unsigned int TCLTHREADMANAGER_MAX_QUEUE_LENGTH = 100;
    const float TCLTHREADMANAGER_MAX_CPU_USAGE = 0.50f;

    SetConsoleCommandShortcutAlias(TclThreadManager, execute, "tclexecute").argumentCompleter(0, autocompletion::tclthreads());
    SetConsoleCommandShortcutAlias(TclThreadManager, query,   "tclquery"  ).argumentCompleter(0, autocompletion::tclthreads());
    SetConsoleCommand(TclThreadManager, create,  false);
    SetConsoleCommand(TclThreadManager, destroy, false).argumentCompleter(0, autocompletion::tclthreads());
    SetConsoleCommand(TclThreadManager, execute, false).argumentCompleter(0, autocompletion::tclthreads());
    SetConsoleCommand(TclThreadManager, query,   false).argumentCompleter(0, autocompletion::tclthreads());
    SetConsoleCommand(TclThreadManager, status,  false);
    SetConsoleCommand(TclThreadManager, dump,    false).argumentCompleter(0, autocompletion::tclthreads());
    SetConsoleCommand(TclThreadManager, flush,   false).argumentCompleter(0, autocompletion::tclthreads());

    struct TclInterpreterBundle
    {
        unsigned int id_;

        std::list<std::string> queue_;
        boost::mutex queueMutex_;

        Tcl::interpreter* interpreter_;
        std::string interpreterName_;
        boost::try_mutex interpreterMutex_;

        std::list<unsigned int> queriers_;
        boost::mutex queriersMutex_;

        bool running_;
        boost::mutex runningMutex_;

        bool finished_;
        boost::mutex finishedMutex_;
        boost::condition finishedCondition_;
    };


    static boost::thread::id threadID_g;
    static boost::mutex bundlesMutex_g;
    static boost::condition fullQueueCondition_g;
    static boost::condition orxonoxEvalCondition_g;

    TclThreadManager* TclThreadManager::singletonRef_s = 0;

    TclThreadManager::TclThreadManager(Tcl::interpreter* interpreter)
        : orxonoxInterpreterBundle_(new TclInterpreterBundle())
    {
        RegisterRootObject(TclThreadManager);

        assert(singletonRef_s == 0);
        singletonRef_s = this;

        this->threadCounter_ = 0;
        this->orxonoxInterpreterBundle_->id_ = 0;
        this->orxonoxInterpreterBundle_->interpreter_ = interpreter;
        threadID_g = boost::this_thread::get_id();
    }

    TclThreadManager::~TclThreadManager()
    {
        unsigned int threadID;
        {
            boost::mutex::scoped_lock bundles_lock(bundlesMutex_g);
            if (this->interpreterBundles_.begin() == this->interpreterBundles_.end())
                return;
            else
                threadID = this->interpreterBundles_.begin()->first;
        }
        this->destroy(threadID);

        delete this->orxonoxInterpreterBundle_;

        singletonRef_s = 0;
    }

    unsigned int TclThreadManager::create()
    {
        boost::mutex::scoped_lock bundles_lock(bundlesMutex_g);
        TclThreadManager::getInstance().threadCounter_++;
        std::string name = multi_cast<std::string>(TclThreadManager::getInstance().threadCounter_);

        TclInterpreterBundle* bundle = new TclInterpreterBundle;
        bundle->id_ = TclThreadManager::getInstance().threadCounter_;
        bundle->interpreter_ = TclThreadManager::getInstance().createNewTclInterpreter(name);
        bundle->interpreterName_ = name;
        bundle->running_ = true;
        bundle->finished_ = true;

        TclThreadManager::getInstance().interpreterBundles_[TclThreadManager::getInstance().threadCounter_] = bundle;
        COUT(0) << "Created new Tcl-interpreter with ID " << TclThreadManager::getInstance().threadCounter_ << std::endl;
        return TclThreadManager::getInstance().threadCounter_;
    }

    unsigned int TclThreadManager::createID(unsigned int threadID)
    {
        unsigned int temp = TclThreadManager::getInstance().threadCounter_;
        TclThreadManager::getInstance().threadCounter_ = threadID - 1;
        TclThreadManager::create();
        TclThreadManager::getInstance().threadCounter_ = temp;
        return threadID;
    }

    void TclThreadManager::destroy(unsigned int threadID)
    {
        TclInterpreterBundle* bundle = TclThreadManager::getInstance().getInterpreterBundle(threadID);
        if (bundle)
        {
            {
                boost::mutex::scoped_lock running_lock(bundle->runningMutex_);
                bundle->running_ = false;
            }
            while (true)
            {
                {
                    boost::mutex::scoped_lock finished_lock(bundle->finishedMutex_);
                    if (bundle->finished_)
                    {
                        boost::mutex::scoped_lock bundles_lock(bundlesMutex_g);
                        boost::mutex::scoped_try_lock interpreter_lock(bundle->interpreterMutex_);
                        try
                        {
                            while (!interpreter_lock.try_lock())
                            {
                                orxonoxEvalCondition_g.notify_one();
                                boost::this_thread::yield();
                            }
                        } catch (...) {}
                        delete bundle->interpreter_;
                        delete bundle;
                        TclThreadManager::getInstance().interpreterBundles_.erase(threadID);
                        break;
                    }
                }

                orxonoxEvalCondition_g.notify_one();
                boost::this_thread::yield();
            }

            COUT(0) << "Destroyed Tcl-interpreter with ID " << threadID << std::endl;
        }
    }

    void TclThreadManager::execute(unsigned int threadID, const std::string& _command)
    {
        std::string command = stripEnclosingBraces(_command);

        if (threadID == 0)
            TclThreadManager::getInstance().pushCommandToQueue(command);
        else
            TclThreadManager::getInstance().pushCommandToQueue(threadID, command);
    }

    std::string TclThreadManager::query(unsigned int threadID, const std::string& command)
    {
        return TclThreadManager::getInstance().evalQuery(TclThreadManager::getInstance().orxonoxInterpreterBundle_->id_, threadID, command);
    }

    void TclThreadManager::status()
    {
        COUT(0) << "Thread ID" << '\t' << "Queue size" << '\t' << "State" << std::endl;

        std::string output = "Orxonox";
        output += "\t\t";
        {
            boost::mutex::scoped_lock queue_lock(TclThreadManager::getInstance().orxonoxInterpreterBundle_->queueMutex_);
            output += multi_cast<std::string>(TclThreadManager::getInstance().orxonoxInterpreterBundle_->queue_.size());
        }
        output += "\t\t";
        output += "busy";
        COUT(0) << output << std::endl;

        boost::mutex::scoped_lock bundles_lock(bundlesMutex_g);
        for (std::map<unsigned int, TclInterpreterBundle*>::const_iterator it = TclThreadManager::getInstance().interpreterBundles_.begin(); it != TclThreadManager::getInstance().interpreterBundles_.end(); ++it)
        {
            std::string output = multi_cast<std::string>((*it).first);
            output += "\t\t";
            {
                boost::mutex::scoped_lock queue_lock((*it).second->queueMutex_);
                output += multi_cast<std::string>((*it).second->queue_.size());
            }
            output += "\t\t";
            {
                boost::mutex::scoped_try_lock interpreter_lock((*it).second->interpreterMutex_);
                if (interpreter_lock.try_lock())
                    output += "ready";
                else
                    output += "busy";
            }
            COUT(0) << output << std::endl;
        }
    }

    void TclThreadManager::dump(unsigned int threadID)
    {
        TclInterpreterBundle* bundle = 0;
        if (threadID == 0)
        {
            bundle = TclThreadManager::getInstance().orxonoxInterpreterBundle_;
            COUT(0) << "Queue dump of Orxonox:" << std::endl;
        }
        else
        {
            if ((bundle = TclThreadManager::getInstance().getInterpreterBundle(threadID)))
            {
                COUT(0) << "Queue dump of Tcl-thread " << threadID << ":" << std::endl;
            }
            else
                return;
        }

        boost::mutex::scoped_lock queue_lock(bundle->queueMutex_);
        unsigned int index = 0;
        for (std::list<std::string>::const_iterator it = bundle->queue_.begin(); it != bundle->queue_.end(); ++it)
        {
            index++;
            COUT(0) << index << ": " << (*it) << std::endl;
        }
    }

    void TclThreadManager::flush(unsigned int threadID)
    {
        TclInterpreterBundle* bundle = 0;
        if (threadID == 0)
            bundle = TclThreadManager::getInstance().orxonoxInterpreterBundle_;
        else
            if (!(bundle = TclThreadManager::getInstance().getInterpreterBundle(threadID)))
                return;

        boost::mutex::scoped_lock queue_lock(bundle->queueMutex_);
        bundle->queue_.clear();
        if (threadID == 0)
        {
            COUT(0) << "Flushed queue of Orxonox Tcl-interpreter." << std::endl;
        }
        else
        {
            COUT(0) << "Flushed queue of Tcl-interpreter " << threadID << "." << std::endl;
        }
    }

    void TclThreadManager::tcl_execute(Tcl::object const &args)
    {
        TclThreadManager::getInstance().pushCommandToQueue(stripEnclosingBraces(args.get()));
    }

    std::string TclThreadManager::tcl_query(int querierID, Tcl::object const &args)
    {
        return TclThreadManager::getInstance().evalQuery(static_cast<unsigned int>(querierID), stripEnclosingBraces(args.get()));
    }

    std::string TclThreadManager::tcl_crossquery(int querierID, int threadID, Tcl::object const &args)
    {
        return TclThreadManager::getInstance().evalQuery(static_cast<unsigned int>(querierID), static_cast<unsigned int>(threadID), stripEnclosingBraces(args.get()));
    }

    bool TclThreadManager::tcl_running(int threadID)
    {
        TclInterpreterBundle* bundle = TclThreadManager::getInstance().getInterpreterBundle(static_cast<unsigned int>(threadID));
        if (bundle)
        {
            boost::mutex::scoped_lock running_lock(bundle->runningMutex_);
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

//            i->eval("rename while tcl::while");
//            i->eval("proc while {test command} { tcl::while {[uplevel 1 expr $test]} {uplevel 1 $command} }"); // (\"$test\" && [orxonox::running " + threadID + "]])
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
        boost::mutex::scoped_lock bundles_lock(bundlesMutex_g);
        std::map<unsigned int, TclInterpreterBundle*>::iterator it = this->interpreterBundles_.find(threadID);
        if (it != this->interpreterBundles_.end())
        {
            return (*it).second;
        }
        else
        {
            this->error("Error: No Tcl-interpreter with ID " + multi_cast<std::string>(threadID) + " existing.");
            return 0;
        }
    }

    Tcl::interpreter* TclThreadManager::getTclInterpreter(unsigned int threadID)
    {
        return this->getInterpreterBundle(threadID)->interpreter_;
    }

    std::string TclThreadManager::dumpList(const std::list<unsigned int>& list)
    {
        std::string output = "";
        for (std::list<unsigned int>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            if (it != list.begin())
                output += " ";

            output += multi_cast<std::string>(*it);
        }
        return output;
    }

    void TclThreadManager::error(const std::string& error)
    {
        if (boost::this_thread::get_id() != threadID_g)
        {
            boost::mutex::scoped_lock queue_lock(this->orxonoxInterpreterBundle_->queueMutex_);
            if (this->orxonoxInterpreterBundle_->queue_.size() >= TCLTHREADMANAGER_MAX_QUEUE_LENGTH)
            {
                boost::this_thread::yield();
                return;
            }
        }

        this->forceCommandToFrontOfQueue("error " + error);
    }

    void TclThreadManager::debug(const std::string& error)
    {
        if (boost::this_thread::get_id() != threadID_g)
        {
            boost::mutex::scoped_lock queue_lock(this->orxonoxInterpreterBundle_->queueMutex_);
            if (this->orxonoxInterpreterBundle_->queue_.size() >= TCLTHREADMANAGER_MAX_QUEUE_LENGTH)
            {
                boost::this_thread::yield();
                return;
            }
        }

        this->forceCommandToFrontOfQueue("debug " + error);
    }

    void TclThreadManager::pushCommandToQueue(const std::string& command)
    {
        boost::mutex::scoped_lock queue_lock(this->orxonoxInterpreterBundle_->queueMutex_);
        while (this->orxonoxInterpreterBundle_->queue_.size() >= TCLTHREADMANAGER_MAX_QUEUE_LENGTH)
            fullQueueCondition_g.wait(queue_lock);

        this->orxonoxInterpreterBundle_->queue_.push_back(command);
    }

    void TclThreadManager::forceCommandToFrontOfQueue(const std::string& command)
    {
        boost::mutex::scoped_lock queue_lock(this->orxonoxInterpreterBundle_->queueMutex_);
        this->orxonoxInterpreterBundle_->queue_.push_front(command);
    }

    std::string TclThreadManager::popCommandFromQueue()
    {
        boost::mutex::scoped_lock queue_lock(this->orxonoxInterpreterBundle_->queueMutex_);
        std::string temp = this->orxonoxInterpreterBundle_->queue_.front();
        this->orxonoxInterpreterBundle_->queue_.pop_front();
        fullQueueCondition_g.notify_one();
        return temp;
    }

    bool TclThreadManager::queueIsEmpty()
    {
        boost::mutex::scoped_lock queue_lock(this->orxonoxInterpreterBundle_->queueMutex_);
        return this->orxonoxInterpreterBundle_->queue_.empty();
    }

    void TclThreadManager::pushCommandToQueue(unsigned int threadID, const std::string& command)
    {
        TclInterpreterBundle* bundle = this->getInterpreterBundle(threadID);
        if (bundle)
        {
            boost::mutex::scoped_lock queue_lock(bundle->queueMutex_);
            if (bundle->queue_.size() >= TCLTHREADMANAGER_MAX_QUEUE_LENGTH)
            {
                this->error("Error: Queue of Tcl-interpreter " + multi_cast<std::string>(threadID) + " is full, couldn't add command.");
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
            boost::mutex::scoped_lock queue_lock(bundle->queueMutex_);
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
            boost::mutex::scoped_lock queue_lock(bundle->queueMutex_);
            return bundle->queue_.empty();
        }
        return true;
    }

    bool TclThreadManager::updateQueriersList(TclInterpreterBundle* querier, TclInterpreterBundle* target)
    {
        if (querier == target)
            return false;

        boost::mutex::scoped_lock queriers_lock(target->queriersMutex_);

        {
            boost::mutex::scoped_lock queriers_lock(querier->queriersMutex_);
            target->queriers_.insert(target->queriers_.end(), querier->queriers_.begin(), querier->queriers_.end());
        }

        target->queriers_.insert(target->queriers_.end(), querier->id_);

        if (std::find(target->queriers_.begin(), target->queriers_.end(), target->id_) != target->queriers_.end())
        {
            this->error("Error: Circular query (" + this->dumpList(target->queriers_) + " -> " + multi_cast<std::string>(target->id_) + "), couldn't query Tcl-interpreter with ID " + multi_cast<std::string>(target->id_) + " from other interpreter with ID " + multi_cast<std::string>(querier->id_) + ".");
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
            if (this->updateQueriersList(querier, this->orxonoxInterpreterBundle_))
            {
                boost::mutex::scoped_lock interpreter_lock(this->orxonoxInterpreterBundle_->interpreterMutex_);
                orxonoxEvalCondition_g.wait(interpreter_lock);

                if (!CommandExecutor::execute(command, false))
                    this->error("Error: Can't execute command \"" + command + "\"!");

                if (CommandExecutor::getLastEvaluation().hasReturnvalue())
                    output = CommandExecutor::getLastEvaluation().getReturnvalue().getString();
            }

            boost::mutex::scoped_lock queriers_lock(this->orxonoxInterpreterBundle_->queriersMutex_);
            this->orxonoxInterpreterBundle_->queriers_.clear();
        }
        return output;
    }

    std::string TclThreadManager::evalQuery(unsigned int querierID, unsigned int threadID, const std::string& command)
    {
        TclInterpreterBundle* target = 0;
        if (threadID)
            target = this->getInterpreterBundle(threadID);
        else
            target = this->orxonoxInterpreterBundle_;

        std::string output = "";
        if (target)
        {
            TclInterpreterBundle* querier = 0;
            if (querierID)
                querier = this->getInterpreterBundle(querierID);
            else
                querier = this->orxonoxInterpreterBundle_;

            if (querier)
            {
                if (this->updateQueriersList(querier, target))
                {
                    boost::mutex::scoped_try_lock interpreter_lock(target->interpreterMutex_);
                    bool successfullyLocked = false;
                    try
                    {
                        if (querierID == 0 || std::find(querier->queriers_.begin(), querier->queriers_.end(), 0U) != querier->queriers_.end())
                            successfullyLocked = interpreter_lock.try_lock();
                        else
                        {
                            while (!interpreter_lock.try_lock())
                            {
                                boost::this_thread::yield();
                            }

                            successfullyLocked = true;
                        }
                    } catch (...) {}

                    if (successfullyLocked)
                    {
                        this->debug("TclThread_query: " + command);
                        try
                        {   output = static_cast<std::string>(target->interpreter_->eval(command));   }
                        catch (Tcl::tcl_error const &e)
                        {   this->error("Tcl error: " + static_cast<std::string>(e.what()));   }
                        catch (std::exception const &e)
                        {   this->error("Error while executing Tcl: " + static_cast<std::string>(e.what()));   }
                    }
                    else
                    {
                        this->error("Error: Couldn't query Tcl-interpreter with ID " + multi_cast<std::string>(threadID) + ", interpreter is busy right now.");
                    }
                }

                boost::mutex::scoped_lock queriers_lock(target->queriersMutex_);
                target->queriers_.clear();
            }
        }
        return output;
    }

    void TclThreadManager::update(const Clock& time)
    {
        {
            orxonoxEvalCondition_g.notify_one();
            boost::this_thread::yield();
        }

        {
            boost::mutex::scoped_lock bundles_lock(bundlesMutex_g);
            for (std::map<unsigned int, TclInterpreterBundle*>::iterator it = this->interpreterBundles_.begin(); it != this->interpreterBundles_.end(); ++it)
            {
                boost::mutex::scoped_lock queue_lock((*it).second->queueMutex_);
                if (!(*it).second->queue_.empty())
                {
                    std::string command = (*it).second->queue_.front();
                    (*it).second->queue_.pop_front();
                    {
                        boost::mutex::scoped_lock finished_lock((*it).second->finishedMutex_);
                        (*it).second->finished_ = false;
                    }
                    boost::thread(boost::bind(&tclThread, (*it).second, command));
                }
            }
        }

        {
            boost::mutex::scoped_lock interpreter_lock(this->orxonoxInterpreterBundle_->interpreterMutex_);
            unsigned long maxtime = static_cast<unsigned long>(time.getDeltaTime() * 1000000 * TCLTHREADMANAGER_MAX_CPU_USAGE);
            Ogre::Timer timer;
            while (!this->queueIsEmpty())
            {
                CommandExecutor::execute(this->popCommandFromQueue(), false);
                if (timer.getMicroseconds() > maxtime)
                    break;
            }
        }
    }

    std::list<unsigned int> TclThreadManager::getThreadList() const
    {
        boost::mutex::scoped_lock bundles_lock(bundlesMutex_g);
        std::list<unsigned int> threads;
        for (std::map<unsigned int, TclInterpreterBundle*>::const_iterator it = this->interpreterBundles_.begin(); it != this->interpreterBundles_.end(); ++it)
            threads.push_back((*it).first);
        return threads;
    }

    void tclThread(TclInterpreterBundle* interpreterBundle, std::string command)
    {
        TclThreadManager::getInstance().debug("TclThread_execute: " + command);
        boost::mutex::scoped_lock interpreter_lock(interpreterBundle->interpreterMutex_);
        try
        {
            interpreterBundle->interpreter_->eval(command);
        }
        catch (Tcl::tcl_error const &e)
        {
            TclThreadManager::getInstance().error("Tcl (ID " + multi_cast<std::string>(interpreterBundle->id_) + ") error: " + e.what());
        }
        catch (std::exception const &e)
        {
            TclThreadManager::getInstance().error("Error while executing Tcl (ID " + multi_cast<std::string>(interpreterBundle->id_) + "): " + e.what());
        }

        boost::mutex::scoped_lock finished_lock(interpreterBundle->finishedMutex_);
        interpreterBundle->finished_ = true;
        interpreterBundle->finishedCondition_.notify_all();
    }
}