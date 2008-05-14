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

#include "ConsoleCommand.h"
#include "CommandExecutor.h"
#include "Debug.h"
#include "TclThreadManager.h"
#include "TclBind.h"
#include "util/String.h"

namespace orxonox
{
    ConsoleCommandShortcutGeneric(tcl, createExecutor(createFunctor(&TclBind::tcl), "tcl", AccessLevel::None));
    ConsoleCommandShortcutGeneric(bgerror, createExecutor(createFunctor(&TclBind::bgerror), "bgerror", AccessLevel::None));

    TclBind::TclBind()
    {
        this->interpreter_ = 0;
        this->bSetTclLibPath_ = false;
    }

    TclBind::~TclBind()
    {
        if (this->interpreter_)
            delete this->interpreter_;
    }

    TclBind& TclBind::getInstance()
    {
        static TclBind instance;
        return instance;
    }

    void TclBind::setDataPath(const std::string& datapath)
    {
        this->tclLibPath_ = datapath + "/tcl";
        this->bSetTclLibPath_ = true;

        this->createTclInterpreter();
    }

    void TclBind::createTclInterpreter()
    {
        if (this->bSetTclLibPath_ && !this->interpreter_)
        {
            this->interpreter_ = new Tcl::interpreter(this->tclLibPath_);
            this->interpreter_->def("orxonox::query", TclBind::tcl_query, Tcl::variadic());
            this->interpreter_->def("orxonox::crossquery", TclThreadManager::tcl_crossquery, Tcl::variadic());
            this->interpreter_->def("execute", TclBind::tcl_execute, Tcl::variadic());

            try
            {
                this->interpreter_->eval("proc query args { orxonox::query $args }");
                this->interpreter_->eval("proc crossquery {id args} { orxonox::crossquery 0 $id $args }");
                this->interpreter_->eval("set id 0");
                this->interpreter_->eval("rename exit tcl::exit; proc exit {} { execute exit }");
                this->interpreter_->eval("redef_puts");
            }
            catch (Tcl::tcl_error const &e)
            {   COUT(1) << "Tcl error while creating Tcl-interpreter: " << e.what() << std::endl;   }
            catch (std::exception const &e)
            {   COUT(1) << "Error while creating Tcl-interpreter: " << e.what() << std::endl;   }
        }
    }

    void TclBind::createNewTclInterpreter()
    {
        if (this->interpreter_)
        {
            delete this->interpreter_;
            this->interpreter_ = 0;
        }

        this->createTclInterpreter();
    }

    std::string TclBind::tcl_query(Tcl::object const &args)
    {
        COUT(4) << "Tcl_query: " << args.get() << std::endl;

        std::string command = stripEnclosingBraces(args.get());

        if (!CommandExecutor::execute(command, false))
            COUT(1) << "Error: Can't execute command \"" << command << "\"!" << std::endl;

        if (CommandExecutor::getLastEvaluation().hasReturnvalue())
            return CommandExecutor::getLastEvaluation().getReturnvalue().toString();

        return "";
    }

    void TclBind::tcl_execute(Tcl::object const &args)
    {
        COUT(4) << "Tcl_execute: " << args.get() << std::endl;
        std::string command = stripEnclosingBraces(args.get());

        if (!CommandExecutor::execute(command, false))
            COUT(1) << "Error: Can't execute command \"" << command << "\"!" << std::endl;
    }

    std::string TclBind::tcl(const std::string& tclcode)
    {
        try
        {
            std::string output = TclBind::getInstance().interpreter_->eval(tclcode);
            if (output != "")
                COUT(0) << "tcl> " << output << std::endl;
            return output;
        }
        catch (Tcl::tcl_error const &e)
        {   COUT(1) << "tcl> Error: " << e.what() << std::endl;   }
        catch (std::exception const &e)
        {   COUT(1) << "Error while executing Tcl: " << e.what() << std::endl;   }

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
