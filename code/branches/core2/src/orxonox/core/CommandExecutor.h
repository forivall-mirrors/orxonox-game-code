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
#include "util/MultiTypeMath.h"
#include "CorePrereqs.h"

#define COMMAND_EXECUTOR_CURSOR "$"

namespace orxonox
{
    enum CommandState
    {
        CS_Uninitialized,
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

    void exec(const std::string& filename);

    enum KeybindMode {}; // temporary

    ///////////////////////
    // CommandEvaluation //
    ///////////////////////
    class _CoreExport CommandEvaluation
    {
        friend class CommandExecutor;

        public:
            CommandEvaluation();

            KeybindMode getKeybindMode();
            bool isValid() const;

            inline void setAdditionalParameter(const std::string& param)
                { this->additionalParameter_ = param; this->bEvaluatedParams_ = false; }
            inline std::string getAdditionalParameter() const
                { return (this->additionalParameter_ != "") ? (" " + this->additionalParameter_) : ""; }

            void setEvaluatedParameter(unsigned int index, MultiTypeMath param);
            MultiTypeMath getEvaluatedParameter(unsigned int index) const;

            void evaluateParams();

        private:
            std::string processedCommand_;
            SubString tokens_;
            std::string additionalParameter_;

            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleFunctionClasses_;
            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleShortcuts_;
            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleFunctions_;
            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleConfigValueClasses_;
            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleConfigValues_;
            std::list<std::pair<const std::string*, const std::string*> > listOfPossibleKeys_;

            Identifier* functionclass_;
            Identifier* configvalueclass_;
            ExecutorStatic* shortcut_;
            ExecutorStatic* function_;
            ConfigValueContainer* configvalue_;
            ConfigValueContainer* key_;

            std::string errorMessage_;
            CommandState state_;

            bool bEvaluatedParams_;
            MultiTypeMath param_[5];
            ExecutorStatic* evaluatedExecutor_;
    };

    /////////////////////
    // CommandExecutor //
    /////////////////////
    class _CoreExport CommandExecutor
    {
        public:
            static bool execute(const std::string& command);
            static bool execute(const CommandEvaluation& evaluation);

            static std::string complete(const std::string& command);
            static std::string complete(const CommandEvaluation& evaluation);

            static std::string hint(const std::string& command);
            static std::string hint(const CommandEvaluation& evaluation);

            static CommandEvaluation evaluate(const std::string& command);

            static bool addConsoleCommandShortcut(ExecutorStatic* executor);
            static ExecutorStatic* getConsoleCommandShortcut(const std::string& name);
            static ExecutorStatic* getLowercaseConsoleCommandShortcut(const std::string& name);

            /** @brief Returns the map that stores all console commands. @return The const_iterator */
            static inline const std::map<std::string, ExecutorStatic*>& getConsoleCommandShortcutMap() { return CommandExecutor::getInstance().consoleCommandShortcuts_; }
            /** @brief Returns a const_iterator to the beginning of the map that stores all console commands. @return The const_iterator */
            static inline std::map<std::string, ExecutorStatic*>::const_iterator getConsoleCommandShortcutMapBegin() { return CommandExecutor::getInstance().consoleCommandShortcuts_.begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all console commands. @return The const_iterator */
            static inline std::map<std::string, ExecutorStatic*>::const_iterator getConsoleCommandShortcutMapEnd() { return CommandExecutor::getInstance().consoleCommandShortcuts_.end(); }

            /** @brief Returns the map that stores all console commands with their names in lowercase. @return The const_iterator */
            static inline const std::map<std::string, ExecutorStatic*>& getLowercaseConsoleCommandShortcutMap() { return CommandExecutor::getInstance().consoleCommandShortcuts_LC_; }
            /** @brief Returns a const_iterator to the beginning of the map that stores all console commands with their names in lowercase. @return The const_iterator */
            static inline std::map<std::string, ExecutorStatic*>::const_iterator getLowercaseConsoleCommandShortcutMapBegin() { return CommandExecutor::getInstance().consoleCommandShortcuts_LC_.begin(); }
            /** @brief Returns a const_iterator to the end of the map that stores all console commands with their names in lowercase. @return The const_iterator */
            static inline std::map<std::string, ExecutorStatic*>::const_iterator getLowercaseConsoleCommandShortcutMapEnd() { return CommandExecutor::getInstance().consoleCommandShortcuts_LC_.end(); }

        private:
            CommandExecutor() {}
            CommandExecutor(const CommandExecutor& other) {}
            ~CommandExecutor() {}

            static CommandExecutor& getInstance();
            static CommandEvaluation& getEvaluation();

            static void parse(const std::string& command, bool bInitialize = true);
            static void initialize(const std::string& command);

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

            static bool compareStringsInList(const std::pair<const std::string*, const std::string*>& first, const std::pair<const std::string*, const std::string*>& second);

            static std::string dump(const std::list<std::pair<const std::string*, const std::string*> >& list);
            static std::string dump(const ExecutorStatic* executor);
            static std::string dump(const ConfigValueContainer* container);

            static std::string getCommonBegin(const std::list<std::pair<const std::string*, const std::string*> >& list);

            static Identifier* getIdentifierOfPossibleFunctionClass(const std::string& name);
            static ExecutorStatic* getExecutorOfPossibleShortcut(const std::string& name);
            static ExecutorStatic* getExecutorOfPossibleFunction(const std::string& name, Identifier* identifier);
            static Identifier* getIdentifierOfPossibleConfigValueClass(const std::string& name);
            static ConfigValueContainer* getContainerOfPossibleConfigValue(const std::string& name, Identifier* identifier);
            static ConfigValueContainer* getContainerOfPossibleKey(const std::string& name);

            CommandEvaluation evaluation_;

            std::map<std::string, ExecutorStatic*> consoleCommandShortcuts_;
            std::map<std::string, ExecutorStatic*> consoleCommandShortcuts_LC_;
    };
}

#endif /* _CommandExecutor_H__ */
