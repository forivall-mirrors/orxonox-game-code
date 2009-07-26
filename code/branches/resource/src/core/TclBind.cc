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
#include "util/StringUtils.h"
#include "CommandExecutor.h"
#include "ConsoleCommand.h"
#include "Core.h"
#include "TclThreadManager.h"

namespace orxonox
{
    SetConsoleCommandShortcut(TclBind, tcl);
    SetConsoleCommandShortcut(TclBind, bgerror);

    TclBind* TclBind::singletonRef_s = 0;

    TclBind::TclBind(const std::string& datapath)
    {
        assert(singletonRef_s == 0);
        singletonRef_s = this;
        this->interpreter_ = 0;
        this->bSetTclDataPath_ = false;
        this->setDataPath(datapath);
    }

    TclBind::~TclBind()
    {
        if (this->interpreter_)
            delete this->interpreter_;
        singletonRef_s = 0;
    }

    void TclBind::setDataPath(const std::string& datapath)
    {
        // String has POSIX slashes
        this->tclDataPath_ = datapath + "tcl" + '/';
        this->bSetTclDataPath_ = true;

        this->initializeTclInterpreter();
    }

    void TclBind::initializeTclInterpreter()
    {
        if (this->bSetTclDataPath_ && !this->interpreter_)
        {
            this->interpreter_ = this->createTclInterpreter();

            this->interpreter_->def("::orxonox::query", TclBind::tcl_query, Tcl::variadic());
            this->interpreter_->def("::orxonox::crossquery", TclThreadManager::tcl_crossquery, Tcl::variadic());
            this->interpreter_->def("execute", TclBind::tcl_execute, Tcl::variadic());
            this->interpreter_->def("::orxonox::crossexecute", TclThreadManager::tcl_crossexecute, Tcl::variadic());

            try
            {
                this->interpreter_->eval("proc query        {args}    { ::orxonox::query $args }");
                this->interpreter_->eval("proc crossquery   {id args} { ::orxonox::crossquery 0 $id $args }");
                this->interpreter_->eval("proc crossexecute {id args} { ::orxonox::crossquery 0 $id $args }");
                this->interpreter_->eval("proc running      {}        { return 1 }");
                this->interpreter_->eval("set id 0");
                this->interpreter_->eval("rename exit ::tcl::exit; proc exit {} { execute exit }");
            }
            catch (Tcl::tcl_error const &e)
            {   COUT(1) << "Tcl error while creating Tcl-interpreter: " << e.what() << std::endl;   }
            catch (std::exception const &e)
            {   COUT(1) << "Error while creating Tcl-interpreter: " << e.what() << std::endl;   }
            catch (...)
            {   COUT(1) << "Error while creating Tcl-interpreter." << std::endl;   }
        }
    }

    Tcl::interpreter* TclBind::createTclInterpreter()
    {
        Tcl::interpreter* interpreter;
#ifdef DEPENDENCY_PACKAGE_ENABLE
        if (Core::isDevelopmentRun())
            interpreter = new Tcl::interpreter(std::string(ORXONOX_DEP_LIB_PATH) + "/tcl");
        else
            interpreter = new Tcl::interpreter(Core::getRootPathString() + "lib/tcl");
#else
        interpreter = new Tcl::interpreter();
#endif
        try
        {
            interpreter->eval("source \"" + TclBind::getInstance().tclDataPath_ + "/init.tcl\"");
        }
        catch (Tcl::tcl_error const &e)
        {   COUT(1) << "Tcl error while creating Tcl-interpreter: " << e.what() << std::endl;   }
        catch (std::exception const &e)
        {   COUT(1) << "Error while creating Tcl-interpreter: " << e.what() << std::endl;   }
        catch (...)
        {   COUT(1) << "Error while creating Tcl-interpreter." << std::endl;   }

        return interpreter;
    }

    std::string TclBind::tcl_query(Tcl::object const &args)
    {
        COUT(4) << "Tcl_query: " << args.get() << std::endl;

        std::string command = stripEnclosingBraces(args.get());

        if (!CommandExecutor::execute(command, false))
        {
            COUT(1) << "Error: Can't execute command \"" << command << "\"!" << std::endl;
        }

        if (CommandExecutor::getLastEvaluation().hasReturnvalue())
            return CommandExecutor::getLastEvaluation().getReturnvalue().getString();

        return "";
    }

    void TclBind::tcl_execute(Tcl::object const &args)
    {
        COUT(4) << "Tcl_execute: " << args.get() << std::endl;
        std::string command = stripEnclosingBraces(args.get());

        if (!CommandExecutor::execute(command, false))
        {
            COUT(1) << "Error: Can't execute command \"" << command << "\"!" << std::endl;
        }
    }

    std::string TclBind::tcl(const std::string& tclcode)
    {
        if (TclBind::getInstance().interpreter_)
        {
            try
            {
                std::string output = TclBind::getInstance().interpreter_->eval("uplevel #0 " + tclcode);
                if (output != "")
                {
                    COUT(0) << "tcl> " << output << std::endl;
                }
                return output;
            }
            catch (Tcl::tcl_error const &e)
            {   COUT(1) << "tcl> Error: " << e.what() << std::endl;   }
            catch (std::exception const &e)
            {   COUT(1) << "Error while executing Tcl: " << e.what() << std::endl;   }
        }

        return "";
    }

    void TclBind::bgerror(std::string error)
    {
        COUT(1) << "Tcl background error: " << stripEnclosingBraces(error) << std::endl;
    }

    bool TclBind::eval(const std::string& tclcode)
    {
        try
        {
            TclBind::getInstance().interpreter_->eval(tclcode);
            return true;
        }
        catch (Tcl::tcl_error const &e)
        {   COUT(1) << "Tcl error: " << e.what() << std::endl;   }
        catch (std::exception const &e)
        {   COUT(1) << "Error while executing Tcl: " << e.what() << std::endl;   }

        return false;
    }
}
