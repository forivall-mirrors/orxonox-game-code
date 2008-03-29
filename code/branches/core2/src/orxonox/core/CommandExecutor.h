/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

#include <string>
#include <map>
#include <list>

#include "util/SubString.h"
#include "CorePrereqs.h"

#define COMMAND_EXECUTOR_CURSOR '$'

namespace orxonox
{
    class _CoreExport CommandExecutor
    {
        enum CommandState
        {
            CS_Empty,
            CS_FunctionClass_Or_Shortcut_Or_Keyword,
            CS_Shortcut_Params,
            CS_Shortcut_Finished,
            CS_Function,
            CS_Function_Params,
            CS_Function_Finished,
            CS_ConfigValueClass,
            CS_ConfigValue,
            CS_ConfigValueType,
            CS_ConfigValueFinished,
            CS_KeybindKey,
            CS_KeybindCommand,
            CS_KeybindFinished,
            CS_Error
        };

        public:
            static bool execute(const std::string& command);
            static std::string complete(const std::string& command);
            static std::string hint(const std::string& command);

            static bool addConsoleCommandShortcut(ExecutorStatic* executor);
            static ExecutorStatic* getConsoleCommandShortcut(const std::string& name);
            static ExecutorStatic* getLowercaseConsoleCommandShortcut(const std::string& name);

            /** @brief Returns the map that stores all console commands. @return The const_iterator */
            static inline const std::map<std::string, ExecutorStatic*>& getConsoleCommandShortcutMap() { return CommandExecutor::getInstance().consoleCommandShortcuts_s; }
            /** @brief Returns a const_iterator to the beginning of the map that stores all console commands. @return The const_iterator */
            static inline std::map<std::string, ExecutorStatic*>::const_iterator getConsoleCommandShortcutMapBegin() { return CommandExecutor::getInstance().consoleCommandShortcuts_s.begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all console commands. @return The const_iterator */
            static inline std::map<std::string, ExecutorStatic*>::const_iterator getConsoleCommandShortcutMapEnd() { return CommandExecutor::getInstance().consoleCommandShortcuts_s.end(); }

            /** @brief Returns the map that stores all console commands with their names in lowercase. @return The const_iterator */
            static inline const std::map<std::string, ExecutorStatic*>& getLowercaseConsoleCommandShortcutMap() { return CommandExecutor::getInstance().consoleCommandShortcuts_LC_s; }
            /** @brief Returns a const_iterator to the beginning of the map that stores all console commands with their names in lowercase. @return The const_iterator */
            static inline std::map<std::string, ExecutorStatic*>::const_iterator getLowercaseConsoleCommandShortcutMapBegin() { return CommandExecutor::getInstance().consoleCommandShortcuts_LC_s.begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all console commands with their names in lowercase. @return The const_iterator */
            static inline std::map<std::string, ExecutorStatic*>::const_iterator getLowercaseConsoleCommandShortcutMapEnd() { return CommandExecutor::getInstance().consoleCommandShortcuts_LC_s.end(); }

        private:
            CommandExecutor() {}
            CommandExecutor(const CommandExecutor& other) {}
            ~CommandExecutor() {}

            static CommandExecutor& getInstance();

            static void parse(const std::string& command, bool bInitialize = true);
            static void initialize();

            static bool argumentsGiven(unsigned int num);
            static unsigned int argumentsGiven();

            static std::string getToken(unsigned int index);

            static bool enoughParametersGiven(unsigned int head, Executor* executor);

            static void createListOfPossibleShortcuts(const std::string& fragment);
            static void createListOfPossibleFunctionClasses(const std::string& fragment);
            static void createListOfPossibleFunctions(const std::string& fragment, Identifier* identifier);
            static void createListOfPossibleConfigValueClasses(const std::string& fragment);
            static void createListOfPossibleConfigValues(const std::string& fragment, Identifier* identifier);
            static void createListOfPossibleKeys(const std::string& fragment);

            static bool compareStringsInList(const std::string* first, const std::string* second);

            static std::string dump(const std::list<const std::string*>& list);
            static std::string dump(const ExecutorStatic* executor);
            static std::string dump(const ConfigValueContainer* container);

            static std::string getCommonBegin(const std::list<const std::string*>& list);

            static Identifier* getIdentifierOfPossibleFunctionClass(const std::string& name);
            static ExecutorStatic* getExecutorOfPossibleShortcut(const std::string& name);
            static ExecutorStatic* getExecutorOfPossibleFunction(const std::string& name, Identifier* identifier);
            static Identifier* getIdentifierOfPossibleConfigValueClass(const std::string& name);
            static ConfigValueContainer* getContainerOfPossibleConfigValue(const std::string& name, Identifier* identifier);
            static ConfigValueContainer* getContainerOfPossibleKey(const std::string& name);

            std::string lastProcessedCommand_s;
            SubString tokens_s;
            std::list<const std::string*> listOfPossibleFunctionClasses_s;
            std::list<const std::string*> listOfPossibleShortcuts_s;
            std::list<const std::string*> listOfPossibleFunctions_s;
            std::list<const std::string*> listOfPossibleConfigValueClasses_s;
            std::list<const std::string*> listOfPossibleConfigValues_s;
            std::list<const std::string*> listOfPossibleKeys_s;

            Identifier* functionclass_s;
            Identifier* configvalueclass_s;
            ExecutorStatic* shortcut_s;
            ExecutorStatic* function_s;
            ConfigValueContainer* configvalue_s;
            ConfigValueContainer* key_s;

            std::string errorMessage_s;
            CommandState state_s;

            std::map<std::string, ExecutorStatic*> consoleCommandShortcuts_s;
            std::map<std::string, ExecutorStatic*> consoleCommandShortcuts_LC_s;
    };
}

#endif /* _CommandExecutor_H__ */
