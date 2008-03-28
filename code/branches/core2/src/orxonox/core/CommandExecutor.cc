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
#include "util/SubString.h"
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

    SubString CommandExecutor::tokens_s;
    std::string CommandExecutor::lastProcessedCommand_s;

    std::list<const std::string*> CommandExecutor::listOfPossibleFunctionClasses_s;
    std::list<const std::string*> CommandExecutor::listOfPossibleShortcuts_s;
    std::list<const std::string*> CommandExecutor::listOfPossibleFunctions_s;
    std::list<const std::string*> CommandExecutor::listOfPossibleConfigValueClasses_s;
    std::list<const std::string*> CommandExecutor::listOfPossibleConfigValues_s;
    std::list<const std::string*> CommandExecutor::listOfPossibleKeys_s;

    Identifier* CommandExecutor::functionclass_s;
    Identifier* CommandExecutor::configvalueclass_s;
    ExecutorStatic* CommandExecutor::shortcut_s;
    ExecutorStatic* CommandExecutor::function_s;
    ConfigValueContainer* CommandExecutor::configvalue_s;
    ConfigValueContainer* CommandExecutor::key_s;

    std::string CommandExecutor::errorMessage_s;
    CommandExecutor::CommandState CommandExecutor::state_s;

    std::map<std::string, ExecutorStatic*> CommandExecutor::consoleCommandShortcuts_s;
    std::map<std::string, ExecutorStatic*> CommandExecutor::consoleCommandShortcuts_LC_s;

    bool CommandExecutor::addConsoleCommandShortcut(ExecutorStatic* executor)
    {
        CommandExecutor::consoleCommandShortcuts_s[executor->getName()] = executor;
        CommandExecutor::consoleCommandShortcuts_LC_s[getLowercase(executor->getName())] = executor;
        return true;
    }

    /**
        @brief Returns the executor of a console command shortcut with given name.
        @brief name The name of the requested console command shortcut
        @return The executor of the requested console command shortcut
    */
    ExecutorStatic* CommandExecutor::getConsoleCommandShortcut(const std::string& name)
    {
        std::map<std::string, ExecutorStatic*>::const_iterator it = CommandExecutor::consoleCommandShortcuts_s.find(name);
        if (it != CommandExecutor::consoleCommandShortcuts_s.end())
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
        std::map<std::string, ExecutorStatic*>::const_iterator it = CommandExecutor::consoleCommandShortcuts_LC_s.find(name);
        if (it != CommandExecutor::consoleCommandShortcuts_LC_s.end())
            return (*it).second;
        else
            return 0;
    }

    bool CommandExecutor::execute(const std::string& command)
    {
        if (CommandExecutor::lastProcessedCommand_s != command)
            CommandExecutor::parse(command);

        switch (CommandExecutor::state_s)
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
                if (CommandExecutor::shortcut_s != 0)
                    return CommandExecutor::shortcut_s->parse(CommandExecutor::tokens_s.subSet(1).join());
                break;
            case CS_Function:
                break;
            case CS_Function_Params:
                // not enough parameters
                break;
            case CS_Function_Finished:
                // call the shortcut
                if (CommandExecutor::function_s != 0)
                    return CommandExecutor::function_s->parse(CommandExecutor::tokens_s.subSet(2).join());
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
                if (CommandExecutor::configvalue_s != 0)
                    return CommandExecutor::configvalue_s->parseString(CommandExecutor::tokens_s.subSet(3).join());
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
        if (CommandExecutor::lastProcessedCommand_s != command)
            CommandExecutor::parse(command);

        switch (CommandExecutor::state_s)
        {
            case CS_Empty:
                break;
            case CS_FunctionClass_Or_Shortcut_Or_Keyword:
                break;
            case CS_Shortcut_Params:
                break;
            case CS_Shortcut_Finished:
                break;
            case CS_Function:
                break;
            case CS_Function_Params:
                break;
            case CS_Function_Finished:
                break;
            case CS_ConfigValueClass:
                break;
            case CS_ConfigValue:
                break;
            case CS_ConfigValueType:
                break;
            case CS_ConfigValueFinished:
                break;
            case CS_KeybindKey:
                break;
            case CS_KeybindCommand:
                break;
            case CS_KeybindFinished:
                break;
            case CS_Error:
                break;
        }

        return CommandExecutor::lastProcessedCommand_s;
    }

    std::string CommandExecutor::hint(const std::string& command)
    {
        if (CommandExecutor::lastProcessedCommand_s != command)
            CommandExecutor::parse(command);

        switch (CommandExecutor::state_s)
        {
            case CS_Empty:
                return (CommandExecutor::dump(CommandExecutor::listOfPossibleShortcuts_s) + "\n" + CommandExecutor::dump(CommandExecutor::listOfPossibleFunctionClasses_s));
                break;
            case CS_FunctionClass_Or_Shortcut_Or_Keyword:
                break;
            case CS_Shortcut_Params:
                if (CommandExecutor::shortcut_s != 0)
                    return CommandExecutor::dump(CommandExecutor::shortcut_s);
                break;
            case CS_Shortcut_Finished:
                break;
            case CS_Function:
                return CommandExecutor::dump(CommandExecutor::listOfPossibleFunctions_s);
                break;
            case CS_Function_Params:
                if (CommandExecutor::function_s != 0)
                    return CommandExecutor::dump(CommandExecutor::function_s);
                break;
            case CS_Function_Finished:
                break;
            case CS_ConfigValueClass:
                return CommandExecutor::dump(CommandExecutor::listOfPossibleConfigValueClasses_s);
                break;
            case CS_ConfigValue:
                return CommandExecutor::dump(CommandExecutor::listOfPossibleConfigValues_s);
                break;
            case CS_ConfigValueType:
                if (CommandExecutor::configvalue_s != 0)
                    CommandExecutor::dump(CommandExecutor::configvalue_s);
                break;
            case CS_ConfigValueFinished:
                break;
            case CS_KeybindKey:
                return CommandExecutor::dump(CommandExecutor::listOfPossibleKeys_s);
                break;
            case CS_KeybindCommand:
                if (CommandExecutor::key_s != 0)
                    CommandExecutor::dump(CommandExecutor::key_s);
                break;
            case CS_KeybindFinished:
                break;
            case CS_Error:
                return "Error";
                break;
        }

        return "";
    }

    void CommandExecutor::parse(const std::string& command, bool bInitialize)
    {
        CommandExecutor::tokens_s.split((command + CommandExecutor::cursor_s), " ", SubString::WhiteSpaces, false, '\\', '"', '(', ')', '\0');
        CommandExecutor::lastProcessedCommand_s = command;

        if (bInitialize)
            CommandExecutor::initialize();

        switch (CommandExecutor::state_s)
        {
            case CS_Empty:
                if (CommandExecutor::argumentsGiven() == 0)
                {
                    // We want a hint for the first token
                    // Check if there is already a perfect match
                    CommandExecutor::functionclass_s = CommandExecutor::getIdentifierOfPossibleFunctionClass(CommandExecutor::getToken(0));
                    CommandExecutor::shortcut_s = CommandExecutor::getExecutorOfPossibleShortcut(CommandExecutor::getToken(0));

                    if ((CommandExecutor::functionclass_s != 0) || (CommandExecutor::shortcut_s != 0))
                    {
                        // Yes, there is a class or a shortcut with the searched name
                        // Add a whitespace and continue parsing
                        CommandExecutor::state_s = CS_FunctionClass_Or_Shortcut_Or_Keyword;
                        CommandExecutor::parse(command + " ", false);
                        return;
                    }

                    // No perfect match: Create the lists of all possible classes and shortcuts and return
                    CommandExecutor::createListOfPossibleFunctionClasses(CommandExecutor::getToken(0));
                    CommandExecutor::createListOfPossibleShortcuts(CommandExecutor::getToken(0));
                    CommandExecutor::state_s = CS_Empty;
                    return;
                }
                else
                {
                    // There is at least one argument: Check if it's a shortcut, a classname or a special keyword
                    CommandExecutor::state_s = CS_FunctionClass_Or_Shortcut_Or_Keyword;
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
                        CommandExecutor::state_s = CS_ConfigValueClass;
                        CommandExecutor::parse(command, false);
                        return;
                    }
                    else if (CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_KEYBIND)
                    {
                        // We want to set a keybinding
                        CommandExecutor::state_s = CS_KeybindKey;
                        CommandExecutor::parse(command, false);
                        return;
                    }

                    if (CommandExecutor::functionclass_s == 0)
                        CommandExecutor::functionclass_s = CommandExecutor::getIdentifierOfPossibleFunctionClass(CommandExecutor::getToken(0));
                    if (CommandExecutor::shortcut_s == 0)
                        CommandExecutor::shortcut_s = CommandExecutor::getExecutorOfPossibleShortcut(CommandExecutor::getToken(0));

                    if ((CommandExecutor::functionclass_s == 0) && (CommandExecutor::shortcut_s == 0))
                    {
                        // Argument 1 seems to be wrong
                        AddLanguageEntry("CommandExecutor::NoSuchCommandOrClassName", "No such command or classname");
                        CommandExecutor::errorMessage_s = (CommandExecutor::getToken(0) + ": " + GetLocalisation("CommandExecutor::NoSuchCommandOrClassName"));
                        CommandExecutor::state_s = CS_Error;
                        return;
                    }
                    else if (CommandExecutor::shortcut_s != 0)
                    {
                        // Argument 1 is a shortcut: Return the needed parameter types
                        CommandExecutor::state_s = CS_Shortcut_Params;
                        CommandExecutor::parse(command, false);
                        return;
                    }
                    else
                    {
                        // Argument 1 is a classname: Return the possible functions
                        CommandExecutor::state_s = CS_Function;
                        CommandExecutor::parse(command, false);
                        return;
                    }
                }
                else
                {
                    CommandExecutor::state_s = CS_Error;
                    return;
                }
                break;
            case CS_Shortcut_Params:
                if (CommandExecutor::shortcut_s != 0)
                {
                    // Valid command
                    // Check if there are enough parameters
                    if (CommandExecutor::enoughParametersGiven(1, CommandExecutor::shortcut_s))
                    {
                        CommandExecutor::state_s = CS_Shortcut_Finished;
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::state_s = CS_Error;
                    return;
                }
                break;
            case CS_Function:
                if (CommandExecutor::functionclass_s != 0)
                {
                    // We have a valid classname
                    // Check if there is a second argument
                    if (CommandExecutor::argumentsGiven() >= 2)
                    {
                        // There is a second argument: Check if it's a valid functionname
                        CommandExecutor::function_s = CommandExecutor::getExecutorOfPossibleFunction(CommandExecutor::getToken(1), CommandExecutor::functionclass_s);
                        if (CommandExecutor::function_s == 0)
                        {
                            // Argument 2 seems to be wrong
                            AddLanguageEntry("CommandExecutor::NoSuchFunctionnameIn", "No such functionname in");
                            CommandExecutor::errorMessage_s = (CommandExecutor::getToken(1) + ": " + GetLocalisation("CommandExecutor::NoSuchFunctionnameIn") + " " + CommandExecutor::functionclass_s->getName());
                            CommandExecutor::state_s = CS_Error;
                            return;
                        }
                        else
                        {
                            // Argument 2 seems to be a valid functionname: Get the parameters
                            CommandExecutor::state_s = CS_Function_Params;
                            CommandExecutor::parse(command, false);
                            return;
                        }
                    }
                    else
                    {
                        // There is no finished second argument
                        // Check if there's already a perfect match
                        if (CommandExecutor::tokens_s.size() >= 2)
                        {
                            CommandExecutor::function_s = CommandExecutor::getExecutorOfPossibleFunction(CommandExecutor::getToken(1), CommandExecutor::functionclass_s);
                            if (CommandExecutor::function_s != 0)
                            {
                                // There is a perfect match: Add a whitespace and continue parsing
                                CommandExecutor::state_s = CS_Function_Params;
                                CommandExecutor::parse(command + " ", false);
                                return;
                            }
                        }

                        // No perfect match: Create the list of all possible functions and return
                        CommandExecutor::createListOfPossibleFunctions(CommandExecutor::getToken(1), CommandExecutor::functionclass_s);
                        CommandExecutor::state_s = CS_Function;
                        return;
                    }
                }
                else
                {
                    CommandExecutor::state_s = CS_Error;
                    return;
                }
                break;
            case CS_Function_Params:
                if ((CommandExecutor::functionclass_s != 0) && (CommandExecutor::function_s != 0))
                {
                    // Valid command
                    // Check if there are enough parameters
                    if (CommandExecutor::enoughParametersGiven(2, CommandExecutor::function_s))
                    {
                        CommandExecutor::state_s = CS_Function_Finished;
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::state_s = CS_Error;
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
                        CommandExecutor::configvalueclass_s = CommandExecutor::getIdentifierOfPossibleConfigValueClass(CommandExecutor::getToken(1));
                        if (CommandExecutor::configvalueclass_s == 0)
                        {
                            // Argument 2 seems to be wrong
                            AddLanguageEntry("CommandExecutor::NoSuchClassWithConfigValues", "No such class with config values");
                            CommandExecutor::errorMessage_s = (CommandExecutor::getToken(1) + ": " + GetLocalisation("CommandExecutor::NoSuchClassWithConfigValues"));
                            CommandExecutor::state_s = CS_Error;
                            return;
                        }
                        else
                        {
                            // Argument 2 seems to be a valid classname: Search for possible config values
                            CommandExecutor::state_s = CS_ConfigValue;
                            CommandExecutor::parse(command, false);
                            return;
                        }
                    }
                    else
                    {
                        // There's no finished second argument
                        // Check if there's already a perfect match
                        if (CommandExecutor::tokens_s.size() >= 2)
                        {
                            CommandExecutor::configvalueclass_s = CommandExecutor::getIdentifierOfPossibleConfigValueClass(CommandExecutor::getToken(1));
                            if (CommandExecutor::configvalueclass_s != 0)
                            {
                                // There is a perfect match: Add a whitespace and continue parsing
                                CommandExecutor::state_s = CS_ConfigValue;
                                CommandExecutor::parse(command + " ", false);
                                return;
                            }
                        }

                        // No perfect match: Create the list of all possible classnames and return
                        CommandExecutor::createListOfPossibleConfigValueClasses(CommandExecutor::getToken(1));
                        CommandExecutor::state_s = CS_ConfigValueClass;
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::state_s = CS_Error;
                    return;
                }
                break;
            case CS_ConfigValue:
                if (((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE) || (CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY)) && (CommandExecutor::configvalueclass_s != 0))
                {
                    // Check if there is a third argument
                    if (CommandExecutor::argumentsGiven() >= 3)
                    {
                        // There is a third argument: Check if it's a valid config value
                        CommandExecutor::configvalue_s = CommandExecutor::getContainerOfPossibleConfigValue(CommandExecutor::getToken(2), CommandExecutor::configvalueclass_s);
                        if (CommandExecutor::configvalue_s == 0)
                        {
                            // Argument 3 seems to be wrong
                            AddLanguageEntry("CommandExecutor::NoSuchConfigValueIn", "No such config value in");
                            CommandExecutor::errorMessage_s = (CommandExecutor::getToken(2) + ": " + GetLocalisation("CommandExecutor::NoSuchConfigValueIn") + " " + CommandExecutor::configvalueclass_s->getName());
                            CommandExecutor::state_s = CS_Error;
                            return;
                        }
                        else
                        {
                            // Argument 3 seems to be a valid config value: Get the type
                            CommandExecutor::state_s = CS_ConfigValueType;
                            CommandExecutor::parse(command, false);
                            return;
                        }
                    }
                    else
                    {
                        // There is no finished third argument
                        // Check if there's already a perfect match
                        if (CommandExecutor::tokens_s.size() >= 3)
                        {
                            CommandExecutor::configvalue_s = CommandExecutor::getContainerOfPossibleConfigValue(CommandExecutor::getToken(2), CommandExecutor::configvalueclass_s);
                            if (CommandExecutor::configvalueclass_s != 0)
                            {
                                // There is a perfect match: Add a whitespace and continue parsing
                                CommandExecutor::state_s = CS_ConfigValueType;
                                CommandExecutor::parse(command + " ", false);
                                return;
                            }
                        }

                        // No perfect match: Create the list of all possible config values
                        CommandExecutor::createListOfPossibleConfigValues(CommandExecutor::getToken(2), CommandExecutor::configvalueclass_s);
                        CommandExecutor::state_s = CS_ConfigValueType;
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::state_s = CS_Error;
                    return;
                }
                break;
            case CS_ConfigValueType:
                if (((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE) || (CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_CONFIG_VALUE_TEMPORARY)) && (CommandExecutor::configvalueclass_s != 0) && (CommandExecutor::configvalue_s != 0))
                {
                    // Valid command
                    // Check if there are enough parameters
                    if (CommandExecutor::tokens_s.size() >= 4)
                    {
                        CommandExecutor::state_s = CS_ConfigValueFinished;
                        return;
                    }
                }
                else
                {
                    // Something is wrong
                    CommandExecutor::state_s = CS_Error;
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
                    CommandExecutor::state_s = CS_Error;
                    return;
                }
                break;
            case CS_KeybindCommand:
                if ((CommandExecutor::getToken(0) == COMMAND_EXECUTOR_KEYWORD_SET_KEYBIND) && (false)) // todo
                {
                    // Valid command
                    // Check if there are enough parameters
                    if (CommandExecutor::tokens_s.size() >= 3)
                    {
                        CommandExecutor::state_s = CS_KeybindFinished;
                        return;
                    }

                }
                else
                {
                    // Something is wrong
                    CommandExecutor::state_s = CS_Error;
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
        CommandExecutor::listOfPossibleFunctionClasses_s.clear();
        CommandExecutor::listOfPossibleShortcuts_s.clear();
        CommandExecutor::listOfPossibleFunctions_s.clear();
        CommandExecutor::listOfPossibleConfigValueClasses_s.clear();
        CommandExecutor::listOfPossibleConfigValues_s.clear();
        CommandExecutor::listOfPossibleKeys_s.clear();

        CommandExecutor::functionclass_s = 0;
        CommandExecutor::configvalueclass_s = 0;
        CommandExecutor::shortcut_s = 0;
        CommandExecutor::function_s = 0;
        CommandExecutor::configvalue_s = 0;
        CommandExecutor::key_s = 0;

        CommandExecutor::errorMessage_s = "";
        CommandExecutor::state_s = CS_Empty;
    }

    bool CommandExecutor::argumentsGiven(unsigned int num)
    {
        // Because we added a cursor we have +1 arguments
        // There are num arguments given if there are at least num arguments + one cursor
        return (CommandExecutor::tokens_s.size() >= (num + 1));
    }

    unsigned int CommandExecutor::argumentsGiven()
    {
        // Because we added a cursor we have +1 arguments
        if (CommandExecutor::tokens_s.size() >= 1)
            return (CommandExecutor::tokens_s.size() - 1);
        else
            return 0;
    }

    std::string CommandExecutor::getToken(unsigned int index)
    {
        if ((index >= 0) && (index < (CommandExecutor::tokens_s.size() - 1)))
            return CommandExecutor::tokens_s[index];
        else if (index == (CommandExecutor::tokens_s.size() - 1))
            return CommandExecutor::tokens_s[index].substr(0, CommandExecutor::tokens_s[index].size() - 1);
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
        return (CommandExecutor::tokens_s.size() >= neededParams);
    }

    void CommandExecutor::createListOfPossibleFunctionClasses(const std::string& fragment)
    {
        for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMapBegin(); it != Identifier::getLowercaseIdentifierMapEnd(); ++it)
        {
            if ((*it).second->hasConsoleCommands())
            {
                if ((*it).first.find(getLowercase(fragment)) == 0)
                {
                    CommandExecutor::listOfPossibleFunctionClasses_s.push_back(&(*it).first);
                }
            }
        }

        CommandExecutor::listOfPossibleFunctionClasses_s.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleShortcuts(const std::string& fragment)
    {
        for (std::map<std::string, ExecutorStatic*>::const_iterator it = CommandExecutor::getLowercaseConsoleCommandShortcutMapBegin(); it != CommandExecutor::getLowercaseConsoleCommandShortcutMapEnd(); ++it)
        {
            if ((*it).first.find(getLowercase(fragment)) == 0)
            {
                CommandExecutor::listOfPossibleShortcuts_s.push_back(&(*it).first);
            }
        }

        CommandExecutor::listOfPossibleShortcuts_s.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleFunctions(const std::string& fragment, Identifier* identifier)
    {
        for (std::map<std::string, ExecutorStatic*>::const_iterator it = identifier->getLowercaseConsoleCommandMapBegin(); it != identifier->getLowercaseConsoleCommandMapEnd(); ++it)
        {
            if ((*it).first.find(getLowercase(fragment)) == 0)
            {
                CommandExecutor::listOfPossibleFunctions_s.push_back(&(*it).first);
            }
        }

        CommandExecutor::listOfPossibleFunctions_s.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleConfigValueClasses(const std::string& fragment)
    {
        for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMapBegin(); it != Identifier::getLowercaseIdentifierMapEnd(); ++it)
        {
            if ((*it).second->hasConfigValues())
            {
                if ((*it).first.find(getLowercase(fragment)) == 0)
                {
                    CommandExecutor::listOfPossibleConfigValueClasses_s.push_back(&(*it).first);
                }
            }
        }

        CommandExecutor::listOfPossibleConfigValueClasses_s.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleConfigValues(const std::string& fragment, Identifier* identifier)
    {
        for (std::map<std::string, ConfigValueContainer*>::const_iterator it = identifier->getLowercaseConfigValueMapBegin(); it != identifier->getLowercaseConfigValueMapEnd(); ++it)
        {
            if ((*it).first.find(getLowercase(fragment)) == 0)
            {
                CommandExecutor::listOfPossibleConfigValues_s.push_back(&(*it).first);
            }
        }

        CommandExecutor::listOfPossibleConfigValues_s.sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleKeys(const std::string& fragment)
    {
        // todo

        CommandExecutor::listOfPossibleKeys_s.sort(CommandExecutor::compareStringsInList);
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
}
