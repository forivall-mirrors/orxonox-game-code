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

#ifndef _CommandExecutor_H__
#define _CommandExecutor_H__

#include "core/CorePrereqs.h"

#include <map>
#include <set>
#include <string>

#include "util/MultiType.h"
#include "CommandEvaluation.h"

// tolua_begin
namespace orxonox
{
    class _CoreExport CommandExecutor
    {
// tolua_end
        public:
            static bool execute(const std::string& command, bool useTcl = true); // tolua_export

            static MultiType queryMT(const std::string& command, bool* success = 0, bool useTcl = true);
            static std::string query(const std::string& command, bool* success = 0, bool useTcl = true); // tolua_export

            static std::string complete(const std::string& command);
            static std::string hint(const std::string& command);

            static CommandEvaluation evaluate(const std::string& command);
            static const CommandEvaluation& getLastEvaluation();

        private:
            CommandExecutor() {}
            CommandExecutor(const CommandExecutor& other);
            ~CommandExecutor() {}

            static CommandExecutor& getInstance();
            static CommandEvaluation& getEvaluation();

            static void parseIfNeeded(const std::string& command);
            static void parse(const std::string& command, bool bInitialize = true);

            static unsigned int argumentsFinished();
            static unsigned int argumentsGiven();
            static bool enoughArgumentsGiven(ConsoleCommand* command);
            static const std::string& getArgument(unsigned int index);
            static const std::string& getLastArgument();

            static void createListOfPossibleIdentifiers(const std::string& fragment);
            static void createListOfPossibleFunctions(const std::string& fragment, Identifier* identifier = 0);
            static void createListOfPossibleArguments(const std::string& fragment, ConsoleCommand* command, unsigned int param);

            static Identifier* getPossibleIdentifier(const std::string& name);
            static ConsoleCommand* getPossibleCommand(const std::string& name, Identifier* identifier = 0);
            static const std::string& getPossibleArgument(const std::string& name, ConsoleCommand* command, unsigned int param);

            static void createArgumentCompletionList(ConsoleCommand* command, unsigned int param);
            static std::string getCommonBegin(const std::list<std::pair<const std::string*, const std::string*> >& list);
            static std::string getCommonBegin(const ArgumentCompletionList& list);

            CommandEvaluation evaluation_;
    }; // tolua_export
} // tolua_export

#endif /* _CommandExecutor_H__ */
