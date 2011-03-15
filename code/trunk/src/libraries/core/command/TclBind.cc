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

#include "TclBind.h"

#include <exception>
#include <string>
#include <cpptcl/cpptcl.h>

#include "SpecialConfig.h"
#include "util/Debug.h"
#include "util/Exception.h"
#include "util/StringUtils.h"
#include "core/PathConfig.h"
#include "CommandExecutor.h"
#include "ConsoleCommand.h"
#include "TclThreadManager.h"

namespace orxonox
{
    SetConsoleCommand("tcl", &TclBind::tcl);
    SetConsoleCommand("bgerror", &TclBind::bgerror).hide();

    TclBind* TclBind::singletonPtr_s = 0;

    /**
        @brief Constructor: Initializes the Tcl-interpreter with a given data path.
        @param datapath Path to the directory that contains the Orxonox-specific Tcl-files
    */
    TclBind::TclBind(const std::string& datapath)
    {
        this->interpreter_ = 0;
        this->bSetTclDataPath_ = false;
        this->setDataPath(datapath);
    }

    /**
        @brief Destructor: Deletes the Tcl-interpreter.
    */
    TclBind::~TclBind()
    {
        if (this->interpreter_)
            delete this->interpreter_;
    }

    /**
        @brief Defines the path to the directory that contains the Orxonox-specific Tcl-files and initializes the Tcl-interpreter accordingly.
    */
    void TclBind::setDataPath(const std::string& datapath)
    {
        // String has POSIX slashes
        this->tclDataPath_ = datapath + "tcl" + '/';
        this->bSetTclDataPath_ = true;

        this->initializeTclInterpreter();
    }

    /**
        @brief Creates and initializes the Tcl-interpreter by registering all callbacks and defining some useful functions.
    */
    void TclBind::initializeTclInterpreter()
    {
        if (this->bSetTclDataPath_ && !this->interpreter_)
        {
            this->interpreter_ = this->createTclInterpreter();

            this->interpreter_->def("::orxonox::query", TclBind::tcl_query, Tcl::variadic());
            this->interpreter_->def("::orxonox::execute", TclBind::tcl_execute, Tcl::variadic());
            this->interpreter_->def("::orxonox::crossquery", TclThreadManager::tcl_crossquery, Tcl::variadic());
            this->interpreter_->def("::orxonox::crossexecute", TclThreadManager::tcl_crossexecute, Tcl::variadic());

            try
            {
                this->interpreter_->def("query", TclBind::tcl_query, Tcl::variadic());
                this->interpreter_->def("execute", TclBind::tcl_execute, Tcl::variadic());
                this->interpreter_->eval("proc crossquery   {id args} { ::orxonox::crossquery 0 $id $args }");
                this->interpreter_->eval("proc crossexecute {id args} { ::orxonox::crossexecute 0 $id $args }");
                this->interpreter_->eval("proc running      {}        { return 1 }");
                this->interpreter_->eval("set id 0");
                this->interpreter_->eval("rename exit ::tcl::exit; proc exit {} { execute exit }");
            }
            catch (Tcl::tcl_error const &e)
            {   COUT(1) << "Tcl error while creating Tcl-interpreter: " << e.what() << std::endl;   }
        }
    }

    /**
        @brief Creates and initializes a new Tcl-interpreter and calls the Orxonox-specific
        init.tcl script that defines some special functions which are required by Orxonox.
    */
    Tcl::interpreter* TclBind::createTclInterpreter()
    {
        Tcl::interpreter* interpreter = new Tcl::interpreter();
        const std::string& libpath = TclBind::getTclLibraryPath();

        try
        {
            if (!libpath.empty())
                interpreter->eval("set tcl_library \"" + libpath + '"');

            Tcl_Init(interpreter->get());

            interpreter->eval("source \"" + TclBind::getInstance().tclDataPath_ + "/init.tcl\"");
        }
        catch (Tcl::tcl_error const &e)
        {   COUT(1) << "Tcl error while creating Tcl-interpreter: " << e.what() << std::endl; COUT(1) << "Error: Tcl isn't properly initialized. Orxonox might possibly not work like that." << std::endl;   }

        return interpreter;
    }

    /**
        @brief Returns the path to the Tcl-library (not the Orxonox-specific Tcl-files).
    */
    std::string TclBind::getTclLibraryPath()
    {
#ifdef DEPENDENCY_PACKAGE_ENABLE
        if (PathConfig::isDevelopmentRun())
            return (std::string(specialConfig::dependencyLibraryDirectory) + "/tcl");
        else
            return (PathConfig::getRootPathString() + "lib/tcl");
#else
        return "";
#endif
    }

    /**
        @brief Callback: Used to send an Orxonox-command from Tcl to the CommandExecutor and to send its result back to Tcl.
    */
    std::string TclBind::tcl_query(Tcl::object const &args)
    {
        COUT(4) << "Tcl_query: " << args.get() << std::endl;
        return TclBind::tcl_helper(args, true);
    }

    /**
        @brief Callback: Used to send an Orxonox-command from Tcl to the CommandExecutor.
    */
    void TclBind::tcl_execute(Tcl::object const &args)
    {
        COUT(4) << "Tcl_execute: " << args.get() << std::endl;
        TclBind::tcl_helper(args, false);
    }

    /**
        @brief Helper function, used by tcl_query() and tcl_execute().
    */
    std::string TclBind::tcl_helper(Tcl::object const &args, bool bQuery)
    {
        const std::string& command = stripEnclosingBraces(args.get());

        int error;
        std::string result;

        CommandEvaluation evaluation = CommandExecutor::evaluate(command);

        if (bQuery)
            result = evaluation.query(&error).getString();
        else
            error = evaluation.execute();

        switch (error)
        {
            case CommandExecutor::Error:       COUT(1) << "Error: Can't execute command \"" << command << "\", command doesn't exist. (B)" << std::endl; break;
            case CommandExecutor::Incomplete:  COUT(1) << "Error: Can't execute command \"" << command << "\", not enough arguments given. (B)" << std::endl; break;
            case CommandExecutor::Deactivated: COUT(1) << "Error: Can't execute command \"" << command << "\", command is not active. (B)" << std::endl; break;
            case CommandExecutor::Denied:      COUT(1) << "Error: Can't execute command \"" << command << "\", access denied. (B)" << std::endl; break;
        }

        if (error == CommandExecutor::Error)
            COUT(3) << "Did you mean \"" << evaluation.getCommandSuggestion() << "\"?" << std::endl;

        return result;
    }

    /**
        @brief Console command, executes Tcl code. Can be used to bind Tcl-commands to a key, because native
        Tcl-commands can not be evaluated and are thus not supported by the key-binder.
    */
    std::string TclBind::tcl(const std::string& tclcode)
    {
        if (TclBind::getInstance().interpreter_)
        {
            try
            {
                return TclBind::getInstance().interpreter_->eval("uplevel #0 " + tclcode);
            }
            catch (Tcl::tcl_error const &e)
            {   COUT(1) << "Tcl error: " << e.what() << std::endl;   }
        }

        return "";
    }

    /**
        @brief Console command and implementation of the Tcl-feature "bgerror" which is called if an error
        occurred in the background of a Tcl-script.
    */
    void TclBind::bgerror(const std::string& error)
    {
        COUT(1) << "Tcl background error: " << stripEnclosingBraces(error) << std::endl;
    }

    /**
        @brief Executes Tcl-code and returns the return-value.
        @param tclcode A string that contains Tcl-code
        @param error A pointer to an integer (or NULL) that is used to write an error-code (see @ref CommandExecutorErrorCodes "CommandExecutor error codes")
        @return Returns the return-value of the executed code (or an empty string if there's no return-value)
    */
    std::string TclBind::eval(const std::string& tclcode, int* error)
    {
        if (error)
            *error = CommandExecutor::Success;

        try
        {
            // execute the code
            return TclBind::getInstance().interpreter_->eval(tclcode);
        }
        catch (Tcl::tcl_error const &e)
        {   COUT(1) << "Tcl error: " << e.what() << std::endl;   }

        if (error)
            *error = CommandExecutor::Error;
        return "";
    }
}
