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
#include <boost/thread/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <OgreTimer.h>
#include <cpptcl/cpptcl.h>

#include "util/Clock.h"
#include "util/Convert.h"
#include "util/Exception.h"
#include "util/StringUtils.h"
#include "CommandExecutor.h"
#include "ConsoleCommand.h"
#include "CoreIncludes.h"
#include "TclBind.h"
#include "TclThreadList.h"

namespace orxonox
{
    const float TCLTHREADMANAGER_MAX_CPU_USAGE = 0.50f;

    SetConsoleCommandShortcutAlias(TclThreadManager, execute, "tclexecute").argumentCompleter(0, autocompletion::tclthreads());
    SetConsoleCommandShortcutAlias(TclThreadManager, query,   "tclquery"  ).argumentCompleter(0, autocompletion::tclthreads());
    SetConsoleCommand(TclThreadManager, create,  false);
    SetConsoleCommand(TclThreadManager, destroy, false).argumentCompleter(0, autocompletion::tclthreads());
    SetConsoleCommand(TclThreadManager, execute, false).argumentCompleter(0, autocompletion::tclthreads());
    SetConsoleCommand(TclThreadManager, query,   false).argumentCompleter(0, autocompletion::tclthreads());
    SetConsoleCommand(TclThreadManager, source,  false).argumentCompleter(0, autocompletion::tclthreads());

    /**
        @brief A struct containing all information about a Tcl-interpreter
    */
    struct TclInterpreterBundle
    {
        TclInterpreterBundle()
        {
            this->lock_ = new boost::unique_lock<boost::mutex>(this->mutex_, boost::defer_lock);
            this->bRunning_ = true;
        }

        ~TclInterpreterBundle()
        {
            delete this->lock_;
        }

        unsigned int                      id_;             ///< The id of the interpreter
        Tcl::interpreter*                 interpreter_;    ///< The Tcl-interpreter
        boost::mutex                      mutex_;          ///< A mutex to lock the interpreter while it's being used
        boost::unique_lock<boost::mutex>* lock_;           ///< The corresponding lock for the mutex
        TclThreadList<std::string>        queue_;          ///< The command queue for commands passed by execute(command)
        TclThreadList<unsigned int>       queriers_;       ///< A list containing the id's of all other threads sending a query to this interpreter (to avoid circular queries and deadlocks)
        bool                              bRunning_;       ///< This variable stays true until destroy() gets called
    };

    TclThreadManager* TclThreadManager::singletonPtr_s = 0;

    /**
        @brief Constructor
        @param interpreter A pointer to the standard Tcl-interpreter (see @ref TclBind)
    */
    TclThreadManager::TclThreadManager(Tcl::interpreter* interpreter)
    {
        RegisterRootObject(TclThreadManager);

        this->numInterpreterBundles_ = 0;

        this->interpreterBundlesMutex_ = new boost::shared_mutex();
        this->mainInterpreterMutex_ = new boost::mutex();
        this->messageQueue_ = new TclThreadList<std::string>();

        TclInterpreterBundle* newbundle = new TclInterpreterBundle();
        newbundle->id_ = 0;
        newbundle->interpreter_ = interpreter;
        newbundle->lock_->lock();

        {
            boost::unique_lock<boost::shared_mutex> lock(*this->interpreterBundlesMutex_);
            this->interpreterBundles_[0] = newbundle;
        }
    }

    /**
        @brief Destructor
    */
    TclThreadManager::~TclThreadManager()
    {
        delete this->interpreterBundlesMutex_;
//        delete this->mainInterpreterMutex_; // <-- temporary disabled to avoid crash if a thread is still actively queriyng
        delete this->messageQueue_;
    }

    /**
        @brief The "main loop" of the TclThreadManager. Creates new threads if needed and handles queries and queued commands for the main interpreter.
    */
    void TclThreadManager::preUpdate(const Clock& time)
    {
        // Get the bundle of the main interpreter (0)
        TclInterpreterBundle* bundle = this->getInterpreterBundle(0);
        if (bundle)
        {
            // Unlock the mutex to allow other threads accessing the main interpreter
            bundle->lock_->unlock();

            // Lock the main interpreter mutex once to synchronize with threads that want to query the main interpreter
            {
                boost::unique_lock<boost::mutex> lock(*this->mainInterpreterMutex_);
            }

            // Lock the mutex again to gain exclusive access to the interpreter for the rest of the mainloop
            bundle->lock_->lock();

            // Execute commands in the queues of the threaded interpreters
            {
                boost::shared_lock<boost::shared_mutex> lock(*this->interpreterBundlesMutex_);
                for (std::map<unsigned int, TclInterpreterBundle*>::const_iterator it = this->interpreterBundles_.begin(); it != this->interpreterBundles_.end(); ++it)
                {
                    if (it->first == 0)
                        continue; // We'll handle the default interpreter later (and without threads of course)

                    TclInterpreterBundle* bundle = it->second;
                    if (!bundle->queue_.empty())
                    {
                        // There are commands in the queue
                        try
                        {
                            if (!bundle->lock_->owns_lock() && bundle->lock_->try_lock())
                            {
                                // We sucessfully obtained a lock for the interpreter
                                std::string command;
                                if (bundle->queue_.try_pop_front(&command))
                                {
                                    // Start a thread to execute the command
                                    boost::thread(boost::bind(&tclThread, bundle, command));
                                }
                                else
                                {
                                    // Somehow the queue become empty (maybe multiple consumers) - unlock the mutex
                                    bundle->lock_->unlock();
                                }
                            }
                        }
                        catch (...)
                        {
                            // A lock error occurred - this is possible if the lock gets locked between !bundle->lock_->owns_lock() and bundle->lock_->try_lock()
                            // This isn't too bad, just continue
                        }
                    }
                }
            }

            // Execute commands in the message queue
            if (!this->messageQueue_->empty())
            {
                std::string command;
                while (true)
                {
                    // Pop the front value from the list (break the loop if there are no elements in the list)
                    if (!this->messageQueue_->try_pop_front(&command))
                        break;

                    // Execute the command
                    CommandExecutor::execute(command, false);
                }
            }

            // Execute commands in the queue of the main interpreter
            if (!bundle->queue_.empty())
            {
                // Calculate the time we have until we reach the maximal cpu usage
                unsigned long maxtime = (unsigned long)(time.getDeltaTime() * 1000000 * TCLTHREADMANAGER_MAX_CPU_USAGE);

                Ogre::Timer timer;
                std::string command;

                while (timer.getMicroseconds() < maxtime)
                {
                    // Pop the front value from the list (break the loop if there are no elements in the list)
                    if (!bundle->queue_.try_pop_front(&command))
                        break;

                    // Execute the command
                    CommandExecutor::execute(command, false);
                }
            }
        }
    }

    /**
        @brief Creates a new Tcl-interpreter.
    */
    unsigned int TclThreadManager::create()
    {
        TclThreadManager::getInstance().numInterpreterBundles_++;
        TclThreadManager::createWithId(TclThreadManager::getInstance().numInterpreterBundles_);
        COUT(0) << "Created new Tcl-interpreter with ID " << TclThreadManager::getInstance().numInterpreterBundles_ << std::endl;
        return TclThreadManager::getInstance().numInterpreterBundles_;
    }

    /**
        @brief Creates a new Tcl-interpreter with a given id.

        Use with caution - if the id collides with an already existing interpreter, this call will fail.
        This will also be a problem, if the auto-numbered interpreters (by using create()) reach an id
        which was previously used in this function. Use high numbers to be safe.
    */
    Tcl::interpreter* TclThreadManager::createWithId(unsigned int id)
    {
        TclInterpreterBundle* newbundle = new TclInterpreterBundle();
        newbundle->id_ = id;
        newbundle->interpreter_ = TclBind::createTclInterpreter();

        TclThreadManager::initialize(newbundle);

        {
            // Add the new bundle to the map
            boost::unique_lock<boost::shared_mutex> lock(*TclThreadManager::getInstance().interpreterBundlesMutex_);
            TclThreadManager::getInstance().interpreterBundles_[id] = newbundle;
        }

        return newbundle->interpreter_;
    }

    void TclThreadManager::initialize(TclInterpreterBundle* bundle)
    {
        const std::string& id_string = getConvertedValue<unsigned int, std::string>(bundle->id_);

        // Initialize the new interpreter
        try
        {
            // Define the functions which are implemented in C++
            bundle->interpreter_->def("::orxonox::execute",      TclThreadManager::tcl_execute,      Tcl::variadic());
            bundle->interpreter_->def("::orxonox::crossexecute", TclThreadManager::tcl_crossexecute, Tcl::variadic());
            bundle->interpreter_->def("::orxonox::query",        TclThreadManager::tcl_query,        Tcl::variadic());
            bundle->interpreter_->def("::orxonox::crossquery",   TclThreadManager::tcl_crossquery,   Tcl::variadic());
            bundle->interpreter_->def("::orxonox::running",      TclThreadManager::tcl_running);

            // Create threadspecific shortcuts for the functions above
            bundle->interpreter_->def("execute",      TclThreadManager::tcl_execute,      Tcl::variadic());
            bundle->interpreter_->def("crossexecute", TclThreadManager::tcl_crossexecute, Tcl::variadic());
            bundle->interpreter_->eval("proc query      {args}    { ::orxonox::query " + id_string + " $args }");
            bundle->interpreter_->eval("proc crossquery {id args} { ::orxonox::crossquery " + id_string + " $id $args }");
            bundle->interpreter_->eval("proc running    {}        { return [::orxonox::running " + id_string + "] }");

            // Define a variable containing the thread id
            bundle->interpreter_->eval("set id " + id_string);

            // Use our own exit function to avoid shutting down the whole program instead of just the interpreter
            bundle->interpreter_->eval("rename exit ::tcl::exit");
            bundle->interpreter_->eval("proc exit {} { execute TclThreadManager destroy " + id_string + " }");

            // Redefine some native functions
            bundle->interpreter_->eval("rename while ::tcl::while");
            bundle->interpreter_->eval("rename ::orxonox::while while");
            bundle->interpreter_->eval("rename for ::tcl::for");
            bundle->interpreter_->eval("rename ::orxonox::for for");
        }
        catch (const Tcl::tcl_error& e)
        {   bundle->interpreter_ = 0; COUT(1) << "Tcl error while creating Tcl-interpreter (" << id_string << "): " << e.what() << std::endl;   }
        catch (...)
        {   bundle->interpreter_ = 0; COUT(1) << "Error while creating Tcl-interpreter (" << id_string << "): " << Exception::handleMessage() << std::endl;   }
    }

    /**
        @brief Stops and destroys a given Tcl-interpreter
    */
    void TclThreadManager::destroy(unsigned int id)
    {
        // TODO
        // Not yet implemented
        TclInterpreterBundle* bundle = TclThreadManager::getInstance().getInterpreterBundle(id);
        if (bundle)
        {
            bundle->bRunning_ = false;
        }
    }

    /**
        @brief Sends a command to the queue of a given Tcl-interpreter
        @param id The id of the target interpreter
        @param command The command to be sent
    */
    void TclThreadManager::execute(unsigned int target_id, const std::string& command)
    {
        TclThreadManager::getInstance()._execute(target_id, command);
    }

    /**
        @brief This function can be called from Tcl to execute a console command.

        Commands which shall be executed are put into a queue and processed as soon as the
        main thread feels ready to do so. The queue may also be full which results in a temporary
        suspension of the calling thread until the queue gets ready again.
    */
    void TclThreadManager::tcl_execute(const Tcl::object& args)
    {
        TclThreadManager::getInstance()._execute(0, stripEnclosingBraces(args.get()));
    }

    /**
        @brief This function can be called from Tcl to send a command to the queue of any interpreter.
        @param target_id The id of the target thread
    */
    void TclThreadManager::tcl_crossexecute(int target_id, const Tcl::object& args)
    {
        TclThreadManager::getInstance()._execute(static_cast<unsigned int>(target_id), stripEnclosingBraces(args.get()));
    }

    /**
        @brief Sends a command to the queue of a given Tcl-interpreter
        @param id The id of the target interpreter
        @param command The command to be sent
    */
    void TclThreadManager::_execute(unsigned int target_id, const std::string& command)
    {
        TclInterpreterBundle* bundle = this->getInterpreterBundle(target_id);
        if (bundle)
            bundle->queue_.push_back(command);
    }


    /**
        @brief Sends a query to a given Tcl-interpreter and waits for the result
        @param id The id of the target interpreter
        @param command The command to be sent
        @return The result of the command
    */
    std::string TclThreadManager::query(unsigned int target_id, const std::string& command)
    {
        return TclThreadManager::getInstance()._query(0, target_id, command);
    }

    /**
        @brief This function can be called from Tcl to send a query to the main thread.
        @param source_id The id of the calling thread

        A query waits for the result of the command. This means, the calling thread will be blocked until
        the main thread answers the query. In return, the main thread sends the result of the console
        command back to Tcl.
    */
    std::string TclThreadManager::tcl_query(int source_id, const Tcl::object& args)
    {
        return TclThreadManager::getInstance()._query(static_cast<unsigned int>(source_id), 0, stripEnclosingBraces(args.get()), true);
    }

    /**
        @brief This function can be called from Tcl to send a query to another thread.
        @param source_id The id of the calling thread
        @param target_id The id of the target thread
    */
    std::string TclThreadManager::tcl_crossquery(int source_id, int target_id, const Tcl::object& args)
    {
        return TclThreadManager::getInstance()._query(static_cast<unsigned int>(source_id), static_cast<unsigned int>(target_id), stripEnclosingBraces(args.get()));
    }

    /**
        @brief This function performs a query to any Tcl interpreter
        @param source_id The id of the calling thread
        @param target_id The id of the target thread
        @param command The command to send as a query
        @param bUseCommandExecutor Only used if the target_id is 0 (which references the main interpreter). In this case it means if the command should be passed to the CommandExecutor (true) or to the main Tcl interpreter (false). This is true when called by tcl_query and false when called by tcl_crossquery.
    */
    std::string TclThreadManager::_query(unsigned int source_id, unsigned int target_id, const std::string& command, bool bUseCommandExecutor)
    {
        TclInterpreterBundle* source_bundle = this->getInterpreterBundle(source_id);
        TclInterpreterBundle* target_bundle = this->getInterpreterBundle(target_id);
        std::string output;

        if (source_bundle && target_bundle)
        {
            // At this point we assume the mutex of source_bundle to be locked (because it's executing this query right now an waits for the return value)
            // We can safely use it's querier list (because there's no other place in the code using the list except this query - and the interpreter can't start more than one query)

            if ((source_bundle->id_ == target_bundle->id_) || source_bundle->queriers_.is_in(target_bundle->id_))
            {
                // This query would lead to a deadlock - return with an error
                TclThreadManager::error("Error: Circular query (" + this->dumpList(source_bundle->queriers_.getList()) + ' ' + getConvertedValue<unsigned int, std::string>(source_bundle->id_) \
                            + " -> " + getConvertedValue<unsigned int, std::string>(target_bundle->id_) \
                            + "), couldn't query Tcl-interpreter with ID " + getConvertedValue<unsigned int, std::string>(target_bundle->id_) \
                            + " from other interpreter with ID " + getConvertedValue<unsigned int, std::string>(source_bundle->id_) + '.');
            }
            else
            {
                boost::unique_lock<boost::mutex> lock(target_bundle->mutex_, boost::try_to_lock);
                boost::unique_lock<boost::mutex> mainlock(*this->mainInterpreterMutex_, boost::defer_lock);

                if (!lock.owns_lock() && source_bundle->id_ != 0)
                {
                    // We couldn't obtain the try_lock immediately and we're not the main interpreter - wait until the lock becomes possible (note: the main interpreter won't wait and instead returns an error - see below)
                    if (target_bundle->id_ == 0)
                    {
                        // We're querying the main interpreter - use the main interpreter mutex to synchronize
                        mainlock.lock();
                        lock.lock();
                    }
                    else
                    {
                        // We're querying a threaded interpreter - no synchronization needed
                        lock.lock();
                    }
                }

                if (lock.owns_lock())
                {
                    // Now the mutex of target_bundle is also locked an we can update the querier list
                    target_bundle->queriers_.insert(target_bundle->queriers_.getList().begin(), source_bundle->queriers_.getList().begin(), source_bundle->queriers_.getList().end());
                    target_bundle->queriers_.push_back(source_bundle->id_);

                    // Perform the query (note: this happens in the main thread because we need the returnvalue)
                    if (target_bundle->id_ == 0 && bUseCommandExecutor)
                    {
                        // It's a query to the CommandExecutor
                        TclThreadManager::debug("TclThread_query -> CE: " + command);
                        if (!CommandExecutor::execute(command, false))
                            TclThreadManager::error("Error: Can't execute command \"" + command + "\"!");

                        if (CommandExecutor::getLastEvaluation().hasReturnvalue())
                            output = CommandExecutor::getLastEvaluation().getReturnvalue().getString();
                    }
                    else
                    {
                        // It's a query to a Tcl interpreter
                        TclThreadManager::debug("TclThread_query: " + command);

                        output = TclThreadManager::eval(target_bundle, command, "query");
                    }

                    // Clear the queriers list of the target
                    target_bundle->queriers_.clear();

                    // Unlock the mutex of the target_bundle
                    lock.unlock();

                    // Finally unlock the main interpreter lock if necessary
                    if (mainlock.owns_lock())
                        mainlock.unlock();
                }
                else
                {
                    // This happens if the main thread tries to query a busy interpreter
                    // To avoid a lock of the main thread, we simply don't proceed with the query in this case
                    TclThreadManager::error("Error: Couldn't query Tcl-interpreter with ID " + getConvertedValue<unsigned int, std::string>(target_bundle->id_) + ", interpreter is busy right now.");
                }
            }

        }

        return output;
    }

    /**
        @brief Creates a non-interactive Tcl-interpreter which executes a file.
    */
    void TclThreadManager::source(const std::string& file)
    {
        boost::thread(boost::bind(&sourceThread, file));
    }

    /**
        @brief This function can be called from Tcl to ask if the thread is still suposed to be running.
        @param id The id of the thread in question
    */
    bool TclThreadManager::tcl_running(int id)
    {
        TclInterpreterBundle* bundle = TclThreadManager::getInstance().getInterpreterBundle(static_cast<unsigned int>(id));
        if (bundle)
            return bundle->bRunning_;
        else
            return false;
    }

    /**
        @brief Returns the interpreter bundle with the given id.
        @param id The id of the interpreter
        @return The interpreter or 0 if the id doesn't exist
    */
    TclInterpreterBundle* TclThreadManager::getInterpreterBundle(unsigned int id)
    {
        boost::shared_lock<boost::shared_mutex> lock(*this->interpreterBundlesMutex_);

        std::map<unsigned int, TclInterpreterBundle*>::const_iterator it = this->interpreterBundles_.find(id);
        if (it != this->interpreterBundles_.end())
        {
            return it->second;
        }
        else
        {
            TclThreadManager::error("Error: No Tcl-interpreter with ID " + getConvertedValue<unsigned int, std::string>(id) + " existing.");
            return 0;
        }
    }

    /**
        @brief Returns a string containing all elements of a unsigned-integer-list separated by spaces.
    */
    std::string TclThreadManager::dumpList(const std::list<unsigned int>& list)
    {
        std::string output;
        for (std::list<unsigned int>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            if (it != list.begin())
                output += ' ';

            output += getConvertedValue<unsigned int, std::string>(*it);
        }
        return output;
    }

    /**
        @brief Returns a list with the numbers of all existing Tcl-interpreters.

        This function is used by the auto completion function.
    */
    std::list<unsigned int> TclThreadManager::getThreadList() const
    {
        boost::shared_lock<boost::shared_mutex> lock(*this->interpreterBundlesMutex_);

        std::list<unsigned int> threads;
        for (std::map<unsigned int, TclInterpreterBundle*>::const_iterator it = this->interpreterBundles_.begin(); it != this->interpreterBundles_.end(); ++it)
            if (it->first > 0 && it->first <= this->numInterpreterBundles_) // only list autonumbered interpreters (created with create()) - exclude the default interpreter 0 and all manually numbered interpreters)
                threads.push_back(it->first);
        return threads;
    }

    /**
        @brief A helper function to print errors in a thread safe manner.
    */
    void TclThreadManager::error(const std::string& error)
    {
        TclThreadManager::getInstance().messageQueue_->push_back("error " + error);
    }

    /**
        @brief A helper function to print debug information in a thread safe manner.
    */
    void TclThreadManager::debug(const std::string& error)
    {
        TclThreadManager::getInstance().messageQueue_->push_back("debug " + error);
    }

    /**
        @brief Evaluates a Tcl command without throwing exceptions (which may rise problems on certain machines).
        @return The Tcl return value

        Errors are reported through the @ref error function.
    */
    std::string TclThreadManager::eval(TclInterpreterBundle* bundle, const std::string& command, const std::string& action)
    {
        Tcl_Interp* interpreter = bundle->interpreter_->get();
        int cc = Tcl_Eval(interpreter, command.c_str());

        Tcl::details::result result(interpreter);

        if (cc != TCL_OK)
        {
            TclThreadManager::error("Tcl error (" + action + ", ID " + getConvertedValue<unsigned int, std::string>(bundle->id_) + "): " + static_cast<std::string>(result));
            return "";
        }
        else
        {
            return result;
        }
    }

    ////////////////
    // The Thread //
    ////////////////

    /**
        @brief The main function of the thread. Executes a Tcl command.
        @param bundle The interpreter bundle containing all necessary variables
        @param command the Command to execute
    */
    void tclThread(TclInterpreterBundle* bundle, const std::string& command)
    {
        TclThreadManager::debug("TclThread_execute: " + command);

        TclThreadManager::eval(bundle, command, "execute");

        bundle->lock_->unlock();
    }

    /**
        @brief The main function of a non-interactive source thread. Executes the file.
        @param file The name of the file that should be executed by the non-interactive interpreter.
    */
    void sourceThread(const std::string& file)
    {
        TclThreadManager::debug("TclThread_source: " + file);

        // Prepare the command-line arguments
        const int argc = 2;
        char* argv[argc];
        argv[0] = "tclthread";
        argv[1] = const_cast<char*>(file.c_str());

        // Start the Tcl-command Tcl_Main with the Tcl_OrxonoxAppInit hook
        Tcl_Main(argc, argv, Tcl_OrxonoxAppInit);

//        Tcl::object object(file);
//        int cc = Tcl_FSEvalFile(bundle->interpreter_->get(), object.get_object());
//        Tcl::details::result result(bundle->interpreter_->get());
//        if (cc != TCL_OK)
//            TclThreadManager::error("Tcl error (source, ID " + getConvertedValue<unsigned int, std::string>(bundle->id_) + "): " + static_cast<std::string>(result));
//
//        // Unlock the mutex
//        bundle->lock_->unlock();
    }

    /**
        @brief A tcl-init hook to inject the non-interactive Tcl-interpreter into the TclThreadManager.
    */
    int Tcl_OrxonoxAppInit(Tcl_Interp* interp)
    {
        // Create a new interpreter bundle
        unsigned int id = TclThreadManager::create();
        TclInterpreterBundle* bundle = TclThreadManager::getInstance().getInterpreterBundle(id);

        // Replace the default interpreter in the bundle with the non-interactive one (passed as an argument to this function)
        if (bundle->interpreter_)
            delete bundle->interpreter_;
        bundle->interpreter_ = new Tcl::interpreter(interp, true);

        // Initialize the non-interactive interpreter (like in @ref TclBind::createTclInterpreter but exception safe)
        const std::string& libpath = TclBind::getTclLibraryPath();
        if (!libpath.empty())
            TclThreadManager::eval(bundle, "set tcl_library \"" + libpath + '"', "source");
        int cc = Tcl_Init(interp);
        TclThreadManager::eval(bundle, "source \"" + TclBind::getInstance().getTclDataPath() + "/init.tcl\"", "source");

        // Initialize the non-interactive interpreter also with the thread-specific stuff
        TclThreadManager::initialize(bundle);

        // Lock the mutex (this will be locked until the thread finishes - no chance to interact with the interpreter)
        bundle->lock_->lock();

        // Return to Tcl_Main
        if (!bundle->interpreter_)
            return TCL_ERROR;
        else
            return cc;
    }
}
