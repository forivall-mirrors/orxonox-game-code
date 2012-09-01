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

/**
    @file
    @ingroup Command CommandExecEval
    @brief Declaration of the orxonox::CommandExecutor class which is used to execute and evaluate @ref orxonox::ConsoleCommand "console commands".

    @anchor CommandExecutorExample

    orxonox::CommandExecutor can be used to execute console commands (see orxonox::ConsoleCommand).
    Commands are strings that can be entered by the user in the shell or they can be part of a
    script.

    A command can be passed to orxonox::CommandExecutor::execute() which will execute them - eiter
    directly, or - if requested - passes it to Tcl. See orxonox::TclBind for more information.
    Appart from execute() the command can also be passed to orxonox::CommandExecutor::query() which
    behaves the same except for that it returns the return value of the command.

    If you don't want to execute the command, but rather gather more information about it, pass it
    to orxonox::CommandExecutor::evaluate(). This function returns an instance of
    orxonox::CommandEvaluation. This class provides more information about the command, for example
    the evaluated instance of orxonox::ConsoleCommand. Its also possible to gather hints about the
    command or to complete the command-string by using argument completion functions. More than that
    the command evaluation can also evaluate the arguments, which allows faster execution of the
    command.

    Example:
    @code
    CommandExecutor::execute("log test");               // writes "test" to the console
    CommandExecutor::execute("log [expr 1+1]");         // writes "2" to the console - expr is a Tcl command

    CommandExecutor::query("expr 1+1");                 // returns "2"
    CommandExecutor::queryMT("expr 1+1");               // returns 2
    @endcode

    And another example about how to use evaluate():
    @code
    CommandEvaluation evaluation;
    evaluation = CommandExecutor::evaluate("log test"); // returns an evaluation of "log test"

    evaluation.execute();                               // writes "test" to the console
    evaluation.hint();                                  // returns "log: {string}"
    @endcode

    @anchor CommandExecutorErrorCodes

    @b Error @b codes:

    orxonox::CommandExecutor defines a number of error codes that are returned
    by different functions that operate with commands:

     - CommandExecutor::Success: No error
     - CommandExecutor::Error: The command doesn't exist
     - CommandExecutor::Incomplete: The command needs more arguments
     - CommandExecutor::Deactivated: The command is not active
     - CommandExecutor::Denied: The command needs a different @ref orxonox::AccessLevel "access level"
*/

#ifndef _CommandExecutor_H__
#define _CommandExecutor_H__

#include "core/CorePrereqs.h"

#include <map>
#include <list>
#include <string>

#include "util/MultiType.h"
#include "CommandEvaluation.h"

// tolua_begin
namespace orxonox
{
    /**
        @brief This class is used to execute and evaluate command-strings.

        CommandExecutor executes command-strings and returns evaluated commands. It's
        also possible to execute Tcl commands if the corresponding argument of execute()
        is true.

        @see See @ref CommandExecutorExample "this description" for more information and some examples.
    */
    class _CoreExport CommandExecutor
    {
// tolua_end
        public:
            static int execute(const std::string& command, bool useTcl = true, bool printErrors = true); // tolua_export

            static MultiType queryMT(const std::string& command, int* error = 0, bool useTcl = true);
            static std::string query(const std::string& command, int* error = 0, bool useTcl = true); // tolua_export

            static CommandEvaluation evaluate(const std::string& command);

            static const int Success = 0;       ///< Error code for "success" (or no error)
            static const int Inexistent = 1;    ///< Error code if the command doesn't exist
            static const int Incomplete = 2;    ///< Error code if the command needs more arguments
            static const int Deactivated = 3;   ///< Error code if the command is not active
            static const int Denied = 4;        ///< Error code if the command needs a different access level
            static const int Error = 5;         ///< Error code if the command returned an error

            static std::string getErrorDescription(int error);

            static MultiType unhide(const std::string& command);
            static void alias(const std::string& alias, const std::string& command);
            static void _autocomplete(const std::string& group, const std::string& name) {} ///< Pseudo console command used whenever no real command is available. In these cases this command provides auto-completion for console commands and groups.

        private:
            CommandExecutor() {}                            ///< Empty constructor
            CommandExecutor(const CommandExecutor& other);  ///< Not implemented copy-constructor
            ~CommandExecutor() {}                           ///< Empty destructor

            static CommandExecutor& getInstance();

            bool getCached(const std::string& command, CommandEvaluation& evaluation);
            void cache(const std::string& command, const CommandEvaluation& evaluation);

            /// Helper struct, used to store cached entries
            struct CacheEntry
            {
                CommandEvaluation evaluation_;                  ///< The command evaluation which is stored in the cache
                std::list<std::string>::iterator iterator_;     ///< The iterator of the corresponding element in cachelist_, used for faster access
            };

            std::map<std::string, CacheEntry> cache_;   ///< A map that connects command strings and evaluated commands in the cache
            std::list<std::string> cachelist_;          ///< A list used to sort the elements in the cache by their age
    }; // tolua_export
} // tolua_export

#endif /* _CommandExecutor_H__ */
