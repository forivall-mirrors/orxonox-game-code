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
#include "TclBind.h"

namespace orxonox
{
    ConsoleCommandShortcutExtern(tcl, AccessLevel::None);

    void Tcl_puts(Tcl::object const &args)
    {
        COUT(0) << args.get() << std::endl;
    }

    std::string Tcl_execute(Tcl::object const &args)
    {
std::cout << "1\n";
std::cout << "args: " << args.get() << std::endl;
        std::string command = args.get();
        if (command.size() >= 2 && command[0] == '{' && command[command.size() - 1] == '}')
            command = command.substr(1, command.size() - 2);
        CommandEvaluation evaluation = CommandExecutor::evaluate(command);
std::cout << "2\n";
        if (!CommandExecutor::execute(evaluation))
            COUT(1) << "Error: Can't execute command \"" << command << "\"!" << std::endl;
std::cout << "3\n";
        if (evaluation.hasReturnvalue())
        {
std::cout << "4\n";
            return evaluation.getReturnvalue().toString();
        }
std::cout << "5\n";
        return "";
    }

    std::string tcl(const std::string& tclcode)
    {
        try
        {
            static Tcl::interpreter i;
            i.def("puts", Tcl_puts, Tcl::variadic());
            i.def("execute", Tcl_execute, Tcl::variadic());
            i.eval("proc unknown {args} { return [execute $args] }");
            std::string output = i.eval(tclcode);
            if (output != "")
                COUT(0) << "tcl> " << output << std::endl;
            return output;
        }
        catch (Tcl::tcl_error const &e)
        {
            COUT(1) << "tcl> Error: " << e.what() << std::endl;
        }
        catch (std::exception const &e)
        {
            COUT(1) << "Error while executing tcl: " << e.what() << std::endl;
        }

        return "";
    }
}
