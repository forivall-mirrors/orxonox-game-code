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

#include "CommandExecutor.h"
#include "ConsoleCommand.h"
#include "util/String.h"
#include "Identifier.h"
#include "Language.h"
#include "Debug.h"
#include "Executor.h"
#include "ConfigValueContainer.h"

#define COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE "set"
#define COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY "tset"
#define COMMAND_EXECUTOR_KEYWORD_SET_KEYBIND "bind"

namespace orxonox
{
    ConsoleCommandShortcutGeneric(keyword1, createExecutor((FunctorStatic*)0, "set", AccessLevel::User));
    ConsoleCommandShortcutGeneric(keyword2, createExecutor((FunctorStatic*)0, "tset", AccessLevel::User));
    ConsoleCommandShortcutGeneric(keyword3, createExecutor((FunctorStatic*)0, "bind", AccessLevel::User));

    CommandExecutor& CommandExecutor::getInstance()
    {
        static CommandExecutor instance;
        return instance;
    }

    bool CommandExecutor::addConsoleCommandShortcut(ExecutorStatic* executor)
    {
        CommandExecutor::getInstance().consoleCommandShortcuts_s[executor->getName()] = executor;
        CommandExecutor::getInstance().consoleCommandShortcuts_LC_s[getLowercase(executor->getName())] = executor;
        return true;
    }

    /**
        @brief Returns the executor of a console command shortcut with given name.
        @brief name The name of the requested console command shortcut
        @return The executor of the requested console command shortcut
    */
    ExecutorStatic* CommandExecutor::getConsoleCommandShortcut(const std::string& name)
    {
        std::map<std::string, ExecutorStatic*>::const_iterator it = CommandExecutor::getInstance().consoleCommandShortcuts_s.find(name);
        if (it != CommandExecutor::getInstance().consoleCommandShortcuts_s.end())
            return (*it).second;
        else
            return 0;
    }

    /**
        @brief Returns the executor of a console command shortcut with given name in lowercase.
        @brief name The name of the requested console command shortcut in lowercase
        @return The executor of the requested console command shortcut
    */
    ExecutorStatic* CommandExecutor::getLowercaseConsoleCommandShortcut(const std::string& name)
    {
        std::map<std::string, ExecutorStatic*>::const_iterator it = CommandExecutor::getInstance().consoleCommandShortcuts_LC_s.find(name);
        if (it != CommandExecutor::getInstance().consoleCommandShortcuts_LC_s.end())
            return (*it).second;
        else
            return 0;
    }

    bool CommandExecutor::execute(const std::string& command)
    {
        if (CommandExecutor::getInstance().lastProcessedCommand_s != command)
            CommandExecutor::parse(command);

        CommandExecutor::getInstance().tokens_s.split(command, " ", SubString::WhiteSpaces, false, '\\', '"', '(', ')', '\0');

        switch (CommandExecutor::getInstance().state_s)
        {
            case CS_Empty:
                break;
            case CS_FunctionClass_Or_Shortcut_Or_Keyword:
                break;
            case CS_Shortcut_Params:
                // not enough parameters
                break;
            case CS_Shortcut_Finished:
                // call the shortcut
                if (CommandExecutor::getInstance().shortcut_s != 0)
                    return CommandExecutor::getInstance().shortcut_s->parse(CommandExecutor::getInstance().tokens_s.subSet(1).join());
                break;
            case CS_Function:
                break;
            case CS_Function_Params:
                // not enough parameters
                break;
            case CS_Function_Finished:
                // call the shortcut
                if (CommandExecutor::getInstance().function_s != 0)
                    return CommandExecutor::getInstance().function_s->parse(CommandExecutor::getInstance().tokens_s.subSet(2).join());
                break;
            case CS_ConfigValueClass:
                break;
            case CS_ConfigValue:
                break;
            case CS_ConfigValueType:
                // not enough parameters
                break;
            case CS_ConfigValueFinished:
                // set the config value
                if (CommandExecutor::getInstance().configvalue_s != 0)
                    return CommandExecutor::getInstance().configvalue_s->parseString(CommandExecutor::getInstance().tokens_s.subSet(3).join());
                break;
            case CS_KeybindKey:
                break;
            case CS_KeybindCommand:
                // not enough parameters
                break;
            case CS_KeybindFinished:
                // set the keybind
                // ...todo
                break;
            case CS_Error:
                break;
        }

        return false;
    }

    std::string CommandExecutor::complete(const std::string& command)
    {
        if (CommandExecutor::getInstance().lastProcessedCommand_s != command)
            CommandExecutor::parse(command);

        CommandExecutor::getInstance().tokens_s.split(command, " ", SubString::WhiteSpaces, false, '\\', '"', '(', ')', '\0');

        std::list<const std::string*> temp;
        if (CommandExecutor::getInstance().state_s == CS_Empty)
        {
            temp.insert(temp.end(), CommandExecutor::getInstance().listOfPossibleShortcuts_s.begin(), CommandExecutor::getInstance().listOfPossibleShortcuts_s.end());
            temp.insert(temp.end(), CommandExecutor::getInstance().listOfPossibleFunctionClasses_s.begin(), CommandExecutor::getInstance().listOfPossibleFunctionClasses_s.end());
        }

        switch (CommandExecutor::getInstance().state_s)
        {
            case CS_Empty:
                return (CommandExecutor::getInstance().tokens_s.subSet(0, CommandExecutor::getInstance().tokens_s.size() - 1).join() + " " + CommandExecutor::getCommonBegin(temp));
                break;
            case CS_FunctionClass_Or_Shortcut_Or_Keyword:
                break;
            case CS_Shortcut_Params:
                if (command[command.size() - 1] != ' ')
                    return (command + " ");
                break;
            case CS_Shortcut_Finished:
                break;
            case CS_Function:
                return (CommandExecutor::getInstance().tokens_s.subSet(0, CommandExecutor::getInstance().tokens_s.size() - 1).join() + " " + CommandExecutor::getCommonBegin(CommandExecutor::getInstance().listOfPossibleFunctions_s));
                break;
            case CS_Function_Params:
                if (command[command.size() - 1] != ' ')
                    return (command + " ");
                break;
            case CS_Function_Finished:
                break;
            case CS_ConfigValueClass:
                return (CommandExecutor::getInstance().tokens_s.subSet(0, CommandExecutor::getInstance().tokens_s.size() - 1).join() + " " + CommandExecutor::getCommonBegin(CommandExecutor::getInstance().listOfPossibleConfigValueClasses_s));
                break;
            case CS_ConfigValue:
                return (CommandExecutor::getInstance().tokens_s.subSet(0, CommandExecutor::getInstance().tokens_s.size() - 1).join() + " " + CommandExecutor::getCommonBegin(CommandExecutor::getInstance().listOfPossibleConfigValues_s));
                break;
            case CS_ConfigValueType:
                if (command[command.size() - 1] != ' ')
                    return (command + " ");
                break;
            case CS_ConfigValueFinished:
                break;
            case CS_KeybindKey:
                return (CommandExecutor::getInstance().tokens_s.subSet(0, CommandExecutor::getInstance().tokens_s.size() - 1).join() + " " + CommandExecutor::getCommonBegin(CommandExecutor::getInstance().listOfPossibleKeys_s));
                break;
            case CS_KeybindCommand:
                if (command[command.size() - 1] != ' ')
                    return (command + " ");
                break;
            case CS_KeybindFinished:
                break;
            case CS_Error:
                break;
        }

        return CommandExecutor::getInstance().lastProcessedCommand_s;
    }

    std::string CommandExecutor::hint(const std::string& command)
    {
        if (CommandExecutor::getInstance().lastProcessedCommand_s != command)
            CommandExecutor::parse(command);

        CommandExecutor::getInstance().tokens_s.split(command, " ", SubString::WhiteSpaces, false, '\\', '"', '(', ')', '\0');

        switch (CommandExecutor::getInstance().state_s)
        {
            case CS_Empty:
                return (CommandExecutor::dump(CommandExecutor::getInstance().listOfPossibleShortcuts_s) + "\n" + CommandExecutor::dump(CommandExecutor::getInstance().listOfPossibleFunctionClasses_s));
                break;
            case CS_FunctionClass_Or_Shortcut_Or_Keyword:
                break;
            case CS_Shortcut_Params:
                if (CommandExecutor::getInstance().shortcut_s != 0)
                    return CommandExecutor::dump(CommandExecutor::getInstance().shortcut_s);
                break;
            case CS_Shortcut_Finished:
                if (CommandExecutor::getInstance().shortcut_s != 0)
                    return CommandExecutor::dump(CommandExecutor::getInstance().shortcut_s);
                break;
            case CS_Function:
                return CommandExecutor::dump(CommandExecutor::getInstance().listOfPossibleFunctions_s);
                break;
            case CS_Function_Params:
                if (CommandExecutor::getInstance().function_s != 0)
                    return CommandExecutor::dump(CommandExecutor::getInstance().function_s);
                break;
            case CS_Function_Finished:
                if (CommandExecutor::getInstance().function_s != 0)
                    return CommandExecutor::dump(CommandExecutor::getInstance().function_s);
                break;
            case CS_ConfigValueClass:
                return CommandExecutor::dump(CommandExecutor::getInstance().listOfPossibleConfigValueClasses_s);
                break;
            case CS_ConfigValue:
                return CommandExecutor::dump(CommandExecutor::getInstance().listOfPossibleConfigValues_s);
                break;
            case CS_ConfigValueType:
                if (CommandExecutor::getInstance().configvalue_s != 0)
                    return CommandExecutor::dump(CommandExecutor::getInstance().configvalue_s);
                break;
            case CS_ConfigValueFinished:
                if (CommandExecutor::getInstance().configvalue_s != 0)
                    return CommandExecutor::dump(CommandExecutor::getInstance().configvalue_s);
                break;
            case CS_KeybindKey:
                return CommandExecutor::dump(CommandExecutor::getInstance().listOfPossibleKeys_s);
                break;
            case CS_KeybindCommand:
                if (CommandExecutor::getInstance().key_s != 0)
                    return CommandExecutor::dump(CommandExecutor::getInstance().key_s);
                break;
            case CS_KeybindFinished:
                if (CommandExecutor::getInstance().key_s != 0)
                    return CommandExecutor::dump(CommandExecutor::getInstance().key_s);
                break;
            case CS_Error:
                return "Error";
                break;
        }

        return "";
    }

    void CommandExecutor::parse(const std::string& command, bool bInitialize)
    {
        CommandExecutor::getInstance().tokens_s.split((command + COMMAND_EXECUTOR_CURSOR), " ", SubString::WhiteSpaces, false, '\\', '"', '(', ')', '\0');
        CommandExecutor::getInstance().lastProcessedCommand_s = command;

        if (bInitialize)
            CommandExecutor::initialize();

        switch (CommandExecutor::getInstance().state_s)
        {
            case CS_Empty:
                if (CommandExecutor::argumentsGiven() == 0)
                {
                    // We want a hint for the first token
                    // Check if there is already a perfect match
                    CommandExecutor::getInstance().functionclass_s = CommandExecutor::getIdentifierOfPossibleFunctionClass(CommandExecutor::getToken(0));
                    CommandExecutor::getInstance().shortcut_s = CommandExecutor::getExecutorOfPossibleShortcut(CommandExecutor::getToken(0));

                    if ((CommandExecutor::getInstance().functionclass_s != 0) || (CommandExecutor::getInstance().shortcut_s != 0))
                    {
                        // Yes, there is a class or a shortcut with the searched name
                        // Add a whitespace and continue parsing
                        CommandExecutor::getInstance().state_s = CS_FunctionClass_Or_Shortcut_Or_Keyword;
                        CommandExecutor::parse(command + " ", false);
                        return;
                    }

                    // No perfect match: Create the lists of all possible classes and shortcuts and return
                    CommandExecutor::createListOfPossibleFunctionClasses(CommandExecutor::getToken(0));
                    CommandExecutor::createListOfPossibleShortcuts(CommandExecutor::getToken(0));
                    return;
                }
                else
                {
                    // There is at least one argument: Check if it's a shortcut, a classname or a special keyword
                    CommandExecutor::getInstance().state_s = CS_FunctionClass_Or_Shortcut_Or_Keyword;
                    CommandExecutor::parse(command, false);
                    return;
                }
                break;
            case CS_FunctionClass_Or_Shortcut_Or_Keyword:
                if (CommandExecutor::argumentsGiven() >= 1)
                {
                    if ((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE) || (CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY))
                    {
                        // We want to set a config value
                        CommandExecutor::getInstance().state_s = CS_ConfigValueClass;
                        CommandExecutor::parse(command, false);
                        return;
                    }
                    else if (CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_KEYBIND)
                    {
                        // We want to set a keybinding
                        CommandExecutor::getInstance().state_s = CS_KeybindKey;
                        CommandExecutor::parse(command, false);
                        return;
                    }

                    if (CommandExecutor::getInstance().functionclass_s == 0)
                        CommandExecutor::getInstance().functionclass_s = CommandExecutor::getIdentifierOfPossibleFunctionClass(CommandExecutor::getToken(0));
                    if (CommandExecutor::getInstance().shortcut_s == 0)
                        CommandExecutor::getInstance().shortcut_s = CommandExecutor::getExecutorOfPossibleShortcut(CommandExecutor::getToken(0));

                    if ((CommandExecutor::getInstance().functionclass_s == 0) && (CommandExecutor::getInstance().shortcut_s == 0))
                    {
                        // Argument 1 seems to be wrong
                        AddLanguageEntry("CommandExecutor::NoSuchCommandOrClassName", "No such command or classname");
                        CommandExecutor::getInstance().errorMessage_s = (CommandExecutor::getToken(0) + ": " + GetLocalisation("CommandExecutor::NoSuchCommandOrClassName"));
                        CommandExecutor::getInstance().state_s = CS_Error;
                        return;
                    }
                    else if (CommandExecutor::getInstance().shortcut_s != 0)
                    {
                        // Argument 1 is a shortcut: Return the needed parameter types
                        CommandExecutor::getInstance().state_s = CS_Shortcut_Params;
                        CommandExecutor::parse(command, false);
                        return;
                    }
                    else
                    {
                        // Argument 1 is a classname: Return the possible functions
                        CommandExecutor::getInstance().state_s = CS_Function;
                        CommandExecutor::parse(command, false);
                        return;
                    }
                }
                else
                {
                    CommandExecutor::getInstance().state_s = CS_Error;
                    return;
                }
                break;
            case CS_Shortcut_Params:
                if (CommandExecutor::getInstance().shortcut_s != 0)
                {
                    // Valid command
                    // Check if there are enough parameters
                    if (CommandExecutor::enoughParametersGiven(1, CommandExecutor::getInstance().shortcut_s))
                    {
                        CommandExecutor::getInstance().state_s = CS_Shortcut_Finished;
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getInstance().state_s = CS_Error;
                    return;
                }
                break;
            case CS_Function:
                if (CommandExecutor::getInstance().functionclass_s != 0)
                {
                    // We have a valid classname
                    // Check if there is a second argument
                    if (CommandExecutor::argumentsGiven() >= 2)
                    {
                        // There is a second argument: Check if it's a valid functionname
                        CommandExecutor::getInstance().function_s = CommandExecutor::getExecutorOfPossibleFunction(CommandExecutor::getToken(1), CommandExecutor::getInstance().functionclass_s);
                        if (CommandExecutor::getInstance().function_s == 0)
                        {
                            // Argument 2 seems to be wrong
                            AddLanguageEntry("CommandExecutor::NoSuchFunctionnameIn", "No such functionname in");
                            CommandExecutor::getInstance().errorMessage_s = (CommandExecutor::getToken(1) + ": " + GetLocalisation("CommandExecutor::NoSuchFunctionnameIn") + " " + CommandExecutor::getInstance().functionclass_s->getName());
                            CommandExecutor::getInstance().state_s = CS_Error;
                            return;
                        }
                        else
                        {
                            // Argument 2 seems to be a valid functionname: Get the parameters
                            CommandExecutor::getInstance().state_s = CS_Function_Params;
                            CommandExecutor::parse(command, false);
                            return;
                        }
                    }
                    else
                    {
                        // There is no finished second argument
                        // Check if there's already a perfect match
                        if (CommandExecutor::getInstance().tokens_s.size() >= 2)
                        {
                            CommandExecutor::getInstance().function_s = CommandExecutor::getExecutorOfPossibleFunction(CommandExecutor::getToken(1), CommandExecutor::getInstance().functionclass_s);
                            if (CommandExecutor::getInstance().function_s != 0)
                            {
                                // There is a perfect match: Add a whitespace and continue parsing
                                CommandExecutor::getInstance().state_s = CS_Function_Params;
                                CommandExecutor::parse(command + " ", false);
                                return;
                            }
                        }

                        // No perfect match: Create the list of all possible functions and return
                        CommandExecutor::createListOfPossibleFunctions(CommandExecutor::getToken(1), CommandExecutor::getInstance().functionclass_s);
                        return;
                    }
                }
                else
                {
                    CommandExecutor::getInstance().state_s = CS_Error;
                    return;
                }
                break;
            case CS_Function_Params:
                if ((CommandExecutor::getInstance().functionclass_s != 0) && (CommandExecutor::getInstance().function_s != 0))
                {
                    // Valid command
                    // Check if there are enough parameters
                    if (CommandExecutor::enoughParametersGiven(2, CommandExecutor::getInstance().function_s))
                    {
                        CommandExecutor::getInstance().state_s = CS_Function_Finished;
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getInstance().state_s = CS_Error;
                    return;
                }
                break;
            case CS_ConfigValueClass:
                if (((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE) || (CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY)))
                {
                    // We want to set a config value
                    // Check if there is a second argument
                    if (CommandExecutor::argumentsGiven() >= 2)
                    {
                        // There is a second argument: Check if it's a valid classname
                        CommandExecutor::getInstance().configvalueclass_s = CommandExecutor::getIdentifierOfPossibleConfigValueClass(CommandExecutor::getToken(1));
                        if (CommandExecutor::getInstance().configvalueclass_s == 0)
                        {
                            // Argument 2 seems to be wrong
                            AddLanguageEntry("CommandExecutor::NoSuchClassWithConfigValues", "No such class with config values");
                            CommandExecutor::getInstance().errorMessage_s = (CommandExecutor::getToken(1) + ": " + GetLocalisation("CommandExecutor::NoSuchClassWithConfigValues"));
                            CommandExecutor::getInstance().state_s = CS_Error;
                            return;
                        }
                        else
                        {
                            // Argument 2 seems to be a valid classname: Search for possible config values
                            CommandExecutor::getInstance().state_s = CS_ConfigValue;
                            CommandExecutor::parse(command, false);
                            return;
                        }
                    }
                    else
                    {
                        // There's no finished second argument
                        // Check if there's already a perfect match
                        if (CommandExecutor::getInstance().tokens_s.size() >= 2)
                        {
                            CommandExecutor::getInstance().configvalueclass_s = CommandExecutor::getIdentifierOfPossibleConfigValueClass(CommandExecutor::getToken(1));
                            if (CommandExecutor::getInstance().configvalueclass_s != 0)
                            {
                                // There is a perfect match: Add a whitespace and continue parsing
                                CommandExecutor::getInstance().state_s = CS_ConfigValue;
                                CommandExecutor::parse(command + " ", false);
                                return;
                            }
                        }

                        // No perfect match: Create the list of all possible classnames and return
                        CommandExecutor::createListOfPossibleConfigValueClasses(CommandExecutor::getToken(1));
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getInstance().state_s = CS_Error;
                    return;
                }
                break;
            case CS_ConfigValue:
                if (((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE) || (CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY)) && (CommandExecutor::getInstance().configvalueclass_s != 0))
                {
                    // Check if there is a third argument
                    if (CommandExecutor::argumentsGiven() >= 3)
                    {
                        // There is a third argument: Check if it's a valid config value
                        CommandExecutor::getInstance().configvalue_s = CommandExecutor::getContainerOfPossibleConfigValue(CommandExecutor::getToken(2), CommandExecutor::getInstance().configvalueclass_s);
                        if (CommandExecutor::getInstance().configvalue_s == 0)
                        {
                            // Argument 3 seems to be wrong
                            AddLanguageEntry("CommandExecutor::NoSuchConfigValueIn", "No such config value in");
                            CommandExecutor::getInstance().errorMessage_s = (CommandExecutor::getToken(2) + ": " + GetLocalisation("CommandExecutor::NoSuchConfigValueIn") + " " + CommandExecutor::getInstance().configvalueclass_s->getName());
                            CommandExecutor::getInstance().state_s = CS_Error;
                            return;
                        }
                        else
                        {
                            // Argument 3 seems to be a valid config value: Get the type
                            CommandExecutor::getInstance().state_s = CS_ConfigValueType;
                            CommandExecutor::parse(command, false);
                            return;
                        }
                    }
                    else
                    {
                        // There is no finished third argument
                        // Check if there's already a perfect match
                        if (CommandExecutor::getInstance().tokens_s.size() >= 3)
                        {
                            CommandExecutor::getInstance().configvalue_s = CommandExecutor::getContainerOfPossibleConfigValue(CommandExecutor::getToken(2), CommandExecutor::getInstance().configvalueclass_s);
                            if (CommandExecutor::getInstance().configvalueclass_s != 0)
                            {
                                // There is a perfect match: Add a whitespace and continue parsing
                                CommandExecutor::getInstance().state_s = CS_ConfigValueType;
                                CommandExecutor::parse(command + " ", false);
                                return;
                            }
                        }

                        // No perfect match: Create the list of all possible config values
                        CommandExecutor::createListOfPossibleConfigValues(CommandExecutor::getToken(2), CommandExecutor::getInstance().configvalueclass_s);
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getInstance().state_s = CS_Error;
                    return;
                }
                break;
            case CS_ConfigValueType:
                if (((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE) || (CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY)) && (CommandExecutor::getInstance().configvalueclass_s != 0) && (CommandExecutor::getInstance().configvalue_s != 0))
                {
                    // Valid command
                    // Check if there are enough parameters
                    if (CommandExecutor::getInstance().tokens_s.size() >= 4)
                    {
                        CommandExecutor::getInstance().state_s = CS_ConfigValueFinished;
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getInstance().state_s = CS_Error;
                    return;
                }
                break;
            case CS_KeybindKey:
                if ((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_KEYBIND))
                {
                    // todo
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getInstance().state_s = CS_Error;
                    return;
                }
                break;
            case CS_KeybindCommand:
                if ((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_KEYBIND) && (false)) // todo
                {
                    // Valid command
                    // Check if there are enough parameters
                    if (CommandExecutor::getInstance().tokens_s.size() >= 3)
                    {
                        CommandExecutor::getInstance().state_s = CS_KeybindFinished;
                        return;
                    }

                }
                else
                {
                    // Something is wrong
                    CommandExecutor::getInstance().state_s = CS_Error;
                    return;
                }
                break;
            case CS_Shortcut_Finished:
                // Nothing to do
                break;
            case CS_Function_Finished:
                // Nothing to do
                break;
            case CS_ConfigValueFinished:
                // Nothing to do
                break;
            case CS_KeybindFinished:
                // Nothing to do
                break;
            case CS_Error:
                // This is bad
                break;
        }
    }

    void CommandExecutor::initialize()
    {
        CommandExecutor::getInstance().listOfPossibleFunctionClasses_s.clear();
        CommandExecutor::getInstance().listOfPossibleShortcuts_s.clear();
        CommandExecutor::getInstance().listOfPossibleFunctions_s.clear();
        CommandExecutor::getInstance().listOfPossibleConfigValueClasses_s.clear();
        CommandExecutor::getInstance().listOfPossibleConfigValues_s.clear();
        CommandExecutor::getInstance().listOfPossibleKeys_s.clear();

        CommandExecutor::getInstance().functionclass_s = 0;
        CommandExecutor::getInstance().configvalueclass_s = 0;
        CommandExecutor::getInstance().shortcut_s = 0;
        CommandExecutor::getInstance().function_s = 0;
        CommandExecutor::getInstance().configvalue_s = 0;
        CommandExecutor::getInstance().key_s = 0;

        CommandExecutor::getInstance().errorMessage_s = "";
        CommandExecutor::getInstance().state_s = CS_Empty;
    }

    bool CommandExecutor::argumentsGiven(unsigned int num)
    {
        // Because we added a cursor we have +1 arguments
        // There are num arguments given if there are at least num arguments + one cursor
        return (CommandExecutor::getInstance().tokens_s.size() >= (num + 1));
    }

    unsigned int CommandExecutor::argumentsGiven()
    {
        // Because we added a cursor we have +1 arguments
        if (CommandExecutor::getInstance().tokens_s.size() >= 1)
            return (CommandExecutor::getInstance().tokens_s.size() - 1);
        else
            return 0;
    }

    std::string CommandExecutor::getToken(unsigned int index)
    {
        if ((index >= 0) && (index < (CommandExecutor::getInstance().tokens_s.size() - 1)))
            return CommandExecutor::getInstance().tokens_s[index];
        else if (index == (CommandExecutor::getInstance().tokens_s.size() - 1))
            return CommandExecutor::getInstance().tokens_s[index].substr(0, CommandExecutor::getInstance().tokens_s[index].size() - 1);
        else
            return "";
    }

    bool CommandExecutor::enoughParametersGiven(unsigned int head, Executor* executor)
    {
        unsigned int neededParams = head + executor->getParamCount();
        for (unsigned int i = executor->getParamCount() - 1; i >= 0; i--)
        {
            if (executor->defaultValueSet(i))
                neededParams--;
            else
                break;
        }
        return (CommandExecutor::getInstance().tokens_s.size() >= neededParams);
    }

    void CommandExecutor::createListOfPossibleFunctionClasses(const std::string& fragment)
    {
        for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMapBegin(); it != Identifier::getLowercaseIdentifierMapEnd(); ++it)
        {
            if ((*it).second->hasConsoleCommands())
            {
                if ((*it).first.find(getLowercase(fragment)) == 0)
                {
                    CommandExecutor::getInstance().listOfPossibleFunctionClasses_s.push_back(&(*it).first);
                }
            }
        }

        CommandExecutor::getInstance().listOfPossibleFunctionClasses_s.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleShortcuts(const std::string& fragment)
    {
        for (std::map<std::string, ExecutorStatic*>::const_iterator it = CommandExecutor::getLowercaseConsoleCommandShortcutMapBegin(); it != CommandExecutor::getLowercaseConsoleCommandShortcutMapEnd(); ++it)
        {
            if ((*it).first.find(getLowercase(fragment)) == 0)
            {
                CommandExecutor::getInstance().listOfPossibleShortcuts_s.push_back(&(*it).first);
            }
        }

        CommandExecutor::getInstance().listOfPossibleShortcuts_s.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleFunctions(const std::string& fragment, Identifier* identifier)
    {
        for (std::map<std::string, ExecutorStatic*>::const_iterator it = identifier->getLowercaseConsoleCommandMapBegin(); it != identifier->getLowercaseConsoleCommandMapEnd(); ++it)
        {
            if ((*it).first.find(getLowercase(fragment)) == 0)
            {
                CommandExecutor::getInstance().listOfPossibleFunctions_s.push_back(&(*it).first);
            }
        }

        CommandExecutor::getInstance().listOfPossibleFunctions_s.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleConfigValueClasses(const std::string& fragment)
    {
        for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMapBegin(); it != Identifier::getLowercaseIdentifierMapEnd(); ++it)
        {
            if ((*it).second->hasConfigValues())
            {
                if ((*it).first.find(getLowercase(fragment)) == 0)
                {
                    CommandExecutor::getInstance().listOfPossibleConfigValueClasses_s.push_back(&(*it).first);
                }
            }
        }

        CommandExecutor::getInstance().listOfPossibleConfigValueClasses_s.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleConfigValues(const std::string& fragment, Identifier* identifier)
    {
        for (std::map<std::string, ConfigValueContainer*>::const_iterator it = identifier->getLowercaseConfigValueMapBegin(); it != identifier->getLowercaseConfigValueMapEnd(); ++it)
        {
            if ((*it).first.find(getLowercase(fragment)) == 0)
            {
                CommandExecutor::getInstance().listOfPossibleConfigValues_s.push_back(&(*it).first);
            }
        }

        CommandExecutor::getInstance().listOfPossibleConfigValues_s.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleKeys(const std::string& fragment)
    {
        // todo

        CommandExecutor::getInstance().listOfPossibleKeys_s.sort(CommandExecutor::compareStringsInList);
    }

    bool CommandExecutor::compareStringsInList(const std::string* first, const std::string* second)
    {
        return ((*first) < (*second));
    }

    Identifier* CommandExecutor::getIdentifierOfPossibleFunctionClass(const std::string& name)
    {
        std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMap().find(getLowercase(name));
        if ((it != Identifier::getLowercaseIdentifierMapEnd()) && (*it).second->hasConsoleCommands())
            return (*it).second;

        return 0;
    }

    ExecutorStatic* CommandExecutor::getExecutorOfPossibleShortcut(const std::string& name)
    {
        std::map<std::string, ExecutorStatic*>::const_iterator it = CommandExecutor::getLowercaseConsoleCommandShortcutMap().find(getLowercase(name));
        if (it != CommandExecutor::getLowercaseConsoleCommandShortcutMapEnd())
            return (*it).second;

        return 0;
    }

    ExecutorStatic* CommandExecutor::getExecutorOfPossibleFunction(const std::string& name, Identifier* identifier)
    {
        std::map<std::string, ExecutorStatic*>::const_iterator it = identifier->getLowercaseConsoleCommandMap().find(getLowercase(name));
        if (it != identifier->getLowercaseConsoleCommandMapEnd())
            return (*it).second;

        return 0;
    }

    Identifier* CommandExecutor::getIdentifierOfPossibleConfigValueClass(const std::string& name)
    {
        std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMap().find(getLowercase(name));
        if ((it != Identifier::getLowercaseIdentifierMapEnd()) && (*it).second->hasConfigValues())
            return (*it).second;

        return 0;
    }

    ConfigValueContainer* CommandExecutor::getContainerOfPossibleConfigValue(const std::string& name, Identifier* identifier)
    {
        std::map<std::string, ConfigValueContainer*>::const_iterator it = identifier->getLowercaseConfigValueMap().find(getLowercase(name));
        if (it != identifier->getLowercaseConfigValueMapEnd())
            return (*it).second;

        return 0;
    }

    ConfigValueContainer* CommandExecutor::getContainerOfPossibleKey(const std::string& name)
    {
        // todo

        return 0;
    }

    std::string CommandExecutor::dump(const std::list<const std::string*>& list)
    {
        std::string output = "";
        for (std::list<const std::string*>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            if (it != list.begin())
                output += " ";

            output += (**it);
        }
        return output;
    }

    std::string CommandExecutor::dump(const ExecutorStatic* executor)
    {
        std::string output = "";
        for (unsigned int i = 0; i < executor->getParamCount(); i++)
        {
            if (i != 0)
                output += " ";

            if (executor->defaultValueSet(i))
                output += "[";
            else
                output += "{";

            output += executor->getTypenameParam(i);

            if (executor->defaultValueSet(i))
                output += "]";
            else
                output += "}";
        }
        return output;
    }

    std::string CommandExecutor::dump(const ConfigValueContainer* container)
    {
        return container->getTypename();
    }

    std::string CommandExecutor::getCommonBegin(const std::list<const std::string*>& list)
    {
        if (list.size() == 0)
        {
            return "";
        }
        else if (list.size() == 1)
        {
            return ((**list.begin()) + " ");
        }
        else
        {
            std::string output = "";
            for (unsigned int i = 0; true; i++)
            {
                char temp = 0;
                for (std::list<const std::string*>::const_iterator it = list.begin(); it != list.end(); ++it)
                {
                    if ((**it).size() > i)
                    {
                        if (it == list.begin())
                        {
                            temp = (**it)[i];
                        }
                        else
                        {
                            if (temp != (**it)[i])
                                return output;
                        }
                    }
                    else
                    {
                        return output;
                    }
                }
                output += temp;
            }
            return output;
        }
    }
}
