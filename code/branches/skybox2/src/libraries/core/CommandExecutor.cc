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

#include "CommandExecutor.h"

#include "util/Debug.h"
#include "util/StringUtils.h"
#include "ConsoleCommand.h"
#include "Identifier.h"
#include "Language.h"
#include "TclBind.h"

namespace orxonox
{
    CommandExecutor& CommandExecutor::getInstance()
    {
        static CommandExecutor instance;
        return instance;
    }

    CommandEvaluation& CommandExecutor::getEvaluation()
    {
        return CommandExecutor::getInstance().evaluation_;
    }

    const CommandEvaluation& CommandExecutor::getLastEvaluation()
    {
        return CommandExecutor::getInstance().evaluation_;
    }

    ConsoleCommand& CommandExecutor::addConsoleCommandShortcut(ConsoleCommand* command, bool bDeleteAtExit)
    {
        std::map<std::string, ConsoleCommand*>::const_iterator it = CommandExecutor::getInstance().consoleCommandShortcuts_.find(command->getName());
        if (it != CommandExecutor::getInstance().consoleCommandShortcuts_.end())
        {
            COUT(2) << "Warning: Overwriting console-command shortcut with name " << command->getName() << '.' << std::endl;
        }

        // Make sure we can also delete the external ConsoleCommands that don't belong to an Identifier
        if (command && bDeleteAtExit)
        {
            CommandExecutor::getInstance().consoleCommandExternals_.insert(command);
        }

        CommandExecutor::getInstance().consoleCommandShortcuts_[command->getName()] = command;
        CommandExecutor::getInstance().consoleCommandShortcuts_LC_[getLowercase(command->getName())] = command;
        return (*command);
    }

    /**
        @brief Returns the executor of a console command shortcut with given name.
        @brief name The name of the requested console command shortcut
        @return The executor of the requested console command shortcut
    */
    ConsoleCommand* CommandExecutor::getConsoleCommandShortcut(const std::string& name)
    {
        std::map<std::string, ConsoleCommand*>::const_iterator it = CommandExecutor::getInstance().consoleCommandShortcuts_.find(name);
        if (it != CommandExecutor::getInstance().consoleCommandShortcuts_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Returns the executor of a console command shortcut with given name in lowercase.
        @brief name The name of the requested console command shortcut in lowercase
        @return The executor of the requested console command shortcut
    */
    ConsoleCommand* CommandExecutor::getLowercaseConsoleCommandShortcut(const std::string& name)
    {
        std::map<std::string, ConsoleCommand*>::const_iterator it = CommandExecutor::getInstance().consoleCommandShortcuts_LC_.find(name);
        if (it != CommandExecutor::getInstance().consoleCommandShortcuts_LC_.end())
            return it->second;
        else
            return 0;
    }

    bool CommandExecutor::execute(const std::string& command, bool useTcl)
    {
        if (useTcl)
            return TclBind::eval(command);

        CommandExecutor::parseIfNeeded(command);
        return CommandExecutor::getEvaluation().execute();
    }

    MultiType CommandExecutor::getReturnValue()
    {
        return CommandExecutor::getEvaluation().getReturnvalue();
    }

    std::string CommandExecutor::getReturnValueString()
    {
        return CommandExecutor::getEvaluation().getReturnvalue().getString();
    }

    std::string CommandExecutor::complete(const std::string& command)
    {
        CommandExecutor::parseIfNeeded(command);
        return CommandExecutor::getEvaluation().complete();
    }

    std::string CommandExecutor::hint(const std::string& command)
    {
        CommandExecutor::parseIfNeeded(command);
        return CommandExecutor::getEvaluation().hint();
    }

    CommandEvaluation CommandExecutor::evaluate(const std::string& command)
    {
        CommandExecutor::parse(command);
        CommandExecutor::getEvaluation().evaluateParams();
        return CommandExecutor::getEvaluation();
    }

    void CommandExecutor::parseIfNeeded(const std::string& command)
    {
        if (CommandExecutor::getEvaluation().state_ == CommandState::Uninitialized)
        {
            CommandExecutor::parse(command);
        }
        else if (CommandExecutor::getEvaluation().originalCommand_ != command)
        {
            if (CommandExecutor::getEvaluation().command_ == command)
            {
                CommandExecutor::parse(command);
                CommandExecutor::getEvaluation().bNewCommand_ = false;
            }
            else
            {
                CommandExecutor::parse(command);
            }
        }
    }

    void CommandExecutor::parse(const std::string& command, bool bInitialize)
    {
        if (bInitialize)
            CommandExecutor::getEvaluation().initialize(command);

        CommandExecutor::getEvaluation().commandTokens_.split(command, " ", SubString::WhiteSpaces, false, '\\', false, '"', false, '(', ')', false, '\0');
        CommandExecutor::getEvaluation().command_ = command;

        switch (CommandExecutor::getEvaluation().state_)
        {
            case CommandState::Uninitialized:
            {
                // Impossible
                break;
            }
            case CommandState::Empty:
            {
                if (CommandExecutor::argumentsGiven() == 0)
                {
                    CommandExecutor::createListOfPossibleFunctions("");
                    CommandExecutor::createListOfPossibleIdentifiers("");
                    break;
                }
                else
                {
                    CommandExecutor::getEvaluation().state_ = CommandState::ShortcutOrIdentifier;
                    // Move on to next case
                }
            }
            case CommandState::ShortcutOrIdentifier:
            {
                if (CommandExecutor::argumentsGiven() > 1)
                {
                    // There's a finished first argument - check if it's a shortcut or a classname
                    CommandExecutor::getEvaluation().function_ = CommandExecutor::getPossibleCommand(CommandExecutor::getArgument(0));
                    CommandExecutor::getEvaluation().functionclass_ = CommandExecutor::getPossibleIdentifier(CommandExecutor::getArgument(0));

                    if (CommandExecutor::getEvaluation().function_)
                    {
                        // It's a shortcut
                        CommandExecutor::getEvaluation().state_ = CommandState::ParamPreparation;
                        CommandExecutor::getEvaluation().functionclass_ = 0;
                        // Move on to next case
                    }
                    else if (CommandExecutor::getEvaluation().functionclass_)
                    {
                        // It's a functionname
                        CommandExecutor::getEvaluation().state_ = CommandState::Function;
                        CommandExecutor::getEvaluation().function_ = 0;
                        // Move on to next case
                    }
                    else
                    {
                        // The first argument is bad
                        CommandExecutor::getEvaluation().state_ = CommandState::Error;
                        AddLanguageEntry("commandexecutorunknownfirstargument", "is not a shortcut nor a classname");
                        CommandExecutor::getEvaluation().errorMessage_ = "Error: " + CommandExecutor::getArgument(0) + ' ' + GetLocalisation("commandexecutorunknownfirstargument") + '.';
                        return;
                    }
                }
                else
                {
                    // There's no finished first argument - search possible shortcuts or classnames
                    CommandExecutor::createListOfPossibleFunctions(CommandExecutor::getArgument(0));
                    CommandExecutor::createListOfPossibleIdentifiers(CommandExecutor::getArgument(0));

                    unsigned int num_functions = CommandExecutor::getEvaluation().listOfPossibleFunctions_.size();
                    unsigned int num_identifiers = CommandExecutor::getEvaluation().listOfPossibleIdentifiers_.size();

                    if (num_functions == 1 && num_identifiers == 0)
                    {
                        // It's a shortcut
                        const std::string& functionname = *CommandExecutor::getEvaluation().listOfPossibleFunctions_.begin()->first;
                        CommandExecutor::getEvaluation().function_ = CommandExecutor::getPossibleCommand(functionname);
                        if (getLowercase(functionname) != getLowercase(CommandExecutor::getArgument(0)))
                        {
                            // Unfinished shortcut
                            CommandExecutor::getEvaluation().bCommandChanged_ = true;
                        }
                        CommandExecutor::getEvaluation().state_ = CommandState::ParamPreparation;
                        CommandExecutor::getEvaluation().functionclass_ = 0;
                        CommandExecutor::getEvaluation().command_ = CommandExecutor::getEvaluation().function_->getName();
                        if (CommandExecutor::getEvaluation().function_->getParamCount() > 0)
                        {
                            CommandExecutor::getEvaluation().command_ += ' ';
                            CommandExecutor::getEvaluation().bCommandChanged_ = true;
                        }
                        // Move on to next case
                    }
                    else if (num_identifiers == 1 && num_functions == 0)
                    {
                        // It's a classname
                        const std::string& classname = *CommandExecutor::getEvaluation().listOfPossibleIdentifiers_.begin()->first;
                        CommandExecutor::getEvaluation().functionclass_ = CommandExecutor::getPossibleIdentifier(classname);
                        if (getLowercase(classname) != getLowercase(CommandExecutor::getArgument(0)))
                        {
                            // Unfinished classname
                            CommandExecutor::getEvaluation().bCommandChanged_ = true;
                        }
                        CommandExecutor::getEvaluation().state_ = CommandState::Function;
                        CommandExecutor::getEvaluation().function_ = 0;
                        CommandExecutor::getEvaluation().command_ = CommandExecutor::getEvaluation().functionclass_->getName() + ' ';
                        // Move on to next case
                    }
                    else if (num_identifiers == 0 && num_functions == 0)
                    {
                        // No possibilities
                        CommandExecutor::getEvaluation().state_ = CommandState::Error;
                        AddLanguageEntry("commandexecutorunknownfirstargumentstart", "There is no command or classname starting with");
                        CommandExecutor::getEvaluation().errorMessage_ = "Error: " + GetLocalisation("commandexecutorunknownfirstargumentstart") + ' ' + CommandExecutor::getArgument(0) + '.';
                        return;
                    }
                    else
                    {
                        // There are several possiblilities
                        std::list<std::pair<const std::string*, const std::string*> > temp;
                        temp.insert(temp.end(), CommandExecutor::getEvaluation().listOfPossibleFunctions_.begin(), CommandExecutor::getEvaluation().listOfPossibleFunctions_.end());
                        temp.insert(temp.end(), CommandExecutor::getEvaluation().listOfPossibleIdentifiers_.begin(), CommandExecutor::getEvaluation().listOfPossibleIdentifiers_.end());
                        CommandExecutor::getEvaluation().command_ = CommandExecutor::getCommonBegin(temp);
                        CommandExecutor::getEvaluation().function_ = CommandExecutor::getPossibleCommand(CommandExecutor::getArgument(0));
                        CommandExecutor::getEvaluation().functionclass_ = CommandExecutor::getPossibleIdentifier(CommandExecutor::getArgument(0));
                        CommandExecutor::getEvaluation().bCommandChanged_ = true;
                        return;
                    }
                }
            }
            case CommandState::Function:
            {
                if (CommandExecutor::getEvaluation().functionclass_)
                {
                    // There is a classname - search for the commandname
                    if (CommandExecutor::argumentsGiven() > 2)
                    {
                        // There is a finished second argument - check if it's a commandname
                        CommandExecutor::getEvaluation().function_ = CommandExecutor::getPossibleCommand(CommandExecutor::getArgument(1), CommandExecutor::getEvaluation().functionclass_);

                        if (CommandExecutor::getEvaluation().function_)
                        {
                            // It's a function
                            CommandExecutor::getEvaluation().state_ = CommandState::ParamPreparation;
                            // Move on to next case
                        }
                        else
                        {
                            // The second argument is bad
                            CommandExecutor::getEvaluation().state_ = CommandState::Error;
                            AddLanguageEntry("commandexecutorunknownsecondargument", "is not a function of");
                            CommandExecutor::getEvaluation().errorMessage_ = "Error: " + CommandExecutor::getArgument(1) + " " + GetLocalisation("commandexecutorunknownsecondargument") + " " + CommandExecutor::getEvaluation().functionclass_->getName() + ".";
                            return;
                        }
                    }
                    else
                    {
                        // There is no finished second argument - search for possibilities
                        CommandExecutor::createListOfPossibleFunctions(CommandExecutor::getArgument(1), CommandExecutor::getEvaluation().functionclass_);
                        unsigned int num_functions = CommandExecutor::getEvaluation().listOfPossibleFunctions_.size();

                        if (num_functions == 1)
                        {
                            // It's a function
                            const std::string& functionname = *CommandExecutor::getEvaluation().listOfPossibleFunctions_.begin()->first;
                            CommandExecutor::getEvaluation().function_ = CommandExecutor::getPossibleCommand(functionname, CommandExecutor::getEvaluation().functionclass_);
                            if (getLowercase(functionname) != getLowercase(CommandExecutor::getArgument(1)))
                            {
                                // Unfinished function
                                CommandExecutor::getEvaluation().bCommandChanged_ = true;
                            }
                            CommandExecutor::getEvaluation().state_ = CommandState::ParamPreparation;
                            CommandExecutor::getEvaluation().command_ = CommandExecutor::getEvaluation().functionclass_->getName() + ' ' + CommandExecutor::getEvaluation().function_->getName();
                            if (CommandExecutor::getEvaluation().function_->getParamCount() > 0)
                            {
                                CommandExecutor::getEvaluation().command_ += ' ';
                                CommandExecutor::getEvaluation().bCommandChanged_ = true;
                            }
                            // Move on to next case
                        }
                        else if (num_functions == 0)
                        {
                            // No possibilities
                            CommandExecutor::getEvaluation().state_ = CommandState::Error;
                            AddLanguageEntry("commandexecutorunknownsecondargumentstart", "has no function starting with");
                            CommandExecutor::getEvaluation().errorMessage_ = "Error: " + CommandExecutor::getEvaluation().functionclass_->getName() + ' ' + GetLocalisation("commandexecutorunknownsecondargumentstart") + ' ' + CommandExecutor::getArgument(1) + '.';
                            return;
                        }
                        else
                        {
                            // There are several possibilities
                            CommandExecutor::getEvaluation().command_ = CommandExecutor::getEvaluation().functionclass_->getName() + ' ' + CommandExecutor::getCommonBegin(CommandExecutor::getEvaluation().listOfPossibleFunctions_);
                            CommandExecutor::getEvaluation().function_ = CommandExecutor::getPossibleCommand(CommandExecutor::getArgument(1), CommandExecutor::getEvaluation().functionclass_);
                            CommandExecutor::getEvaluation().bCommandChanged_ = true;
                            return;
                        }
                    }
                }
                else
                {
                    // There is no classname - move on to CommandState::ParamPreparation
                }
            }
            case CommandState::ParamPreparation:
            {
                if (CommandExecutor::getEvaluation().function_->getParamCount() == 0 || CommandExecutor::enoughArgumentsGiven(CommandExecutor::getEvaluation().function_))
                {
                    CommandExecutor::getEvaluation().state_ = CommandState::Finished;
                    return;
                }
                else
                {
                    unsigned int argumentNumber = CommandExecutor::argumentsGiven() - 2;
                    if (CommandExecutor::getEvaluation().functionclass_)
                        argumentNumber -= 1;

                    CommandExecutor::createListOfPossibleArguments(CommandExecutor::getLastArgument(), CommandExecutor::getEvaluation().function_, argumentNumber);
                    CommandExecutor::getEvaluation().state_ = CommandState::Params;

                    if (CommandExecutor::getEvaluation().bCommandChanged_)
                    {
                        // Don't do more than one change
                        return;
                    }
                }
            }
            case CommandState::Params:
            {
                if (CommandExecutor::getEvaluation().listOfPossibleArguments_.size() == 1)
                {
                    // There is exactly one possible argument
                    CommandExecutor::getEvaluation().argument_ = CommandExecutor::getEvaluation().listOfPossibleArguments_.begin()->getString();
                    CommandExecutor::getEvaluation().possibleArgument_ = CommandExecutor::getEvaluation().listOfPossibleArguments_.begin()->getString();
                    CommandExecutor::getEvaluation().state_ = CommandState::ParamPreparation;
                    return;
                }
                else if (CommandExecutor::getEvaluation().listOfPossibleArguments_.size() == 0)
                {
                    // The user tries something new - we let him do
                    CommandExecutor::getEvaluation().state_ = CommandState::ParamPreparation;
                    CommandExecutor::getEvaluation().argument_ = CommandExecutor::getLastArgument();
                    return;
                }
                else
                {
                    // There are several possibilities
                    unsigned int argumentNumber = CommandExecutor::argumentsGiven();
                    if (argumentNumber > 0)
                        --argumentNumber;
                    if (CommandExecutor::getEvaluation().functionclass_ && argumentNumber > 0)
                        --argumentNumber;

                    CommandExecutor::getEvaluation().argument_ = CommandExecutor::getCommonBegin(CommandExecutor::getEvaluation().listOfPossibleArguments_);
                    CommandExecutor::getEvaluation().possibleArgument_ = CommandExecutor::getPossibleArgument(CommandExecutor::getLastArgument(), CommandExecutor::getEvaluation().function_, argumentNumber);
                    CommandExecutor::getEvaluation().state_ = CommandState::ParamPreparation;
                    return;
                }
            }
            case CommandState::Finished:
            {
                // Nothing more to do
                break;
            }
            case CommandState::Error:
            {
                // Bad, very bad
                break;
            }
        }
    }

    unsigned int CommandExecutor::argumentsFinished()
    {
        unsigned int argumentsGiven = CommandExecutor::argumentsGiven();
        if (argumentsGiven > 0)
            return argumentsGiven - 1;
        else
            return 0;
    }

    unsigned int CommandExecutor::argumentsGiven()
    {
        if (CommandExecutor::getEvaluation().command_.size() > 0 && CommandExecutor::getEvaluation().command_[CommandExecutor::getEvaluation().command_.size() - 1] == ' ')
            return CommandExecutor::getEvaluation().commandTokens_.size() + 1;
        else
            return CommandExecutor::getEvaluation().commandTokens_.size();
    }

    bool CommandExecutor::enoughArgumentsGiven(ConsoleCommand* command)
    {
        if (CommandExecutor::getEvaluation().functionclass_)
            return (CommandExecutor::argumentsGiven() > (2 + command->getParamCount()));
        else
            return (CommandExecutor::argumentsGiven() > (1 + command->getParamCount()));
    }

    const std::string& CommandExecutor::getArgument(unsigned int index)
    {
        if (index < (CommandExecutor::getEvaluation().commandTokens_.size()))
            return CommandExecutor::getEvaluation().commandTokens_[index];
        else
            return BLANKSTRING;
    }

    const std::string& CommandExecutor::getLastArgument()
    {
        return CommandExecutor::getArgument(CommandExecutor::argumentsGiven() - 1);
    }

    void CommandExecutor::createListOfPossibleIdentifiers(const std::string& fragment)
    {
        CommandExecutor::getEvaluation().listOfPossibleIdentifiers_.clear();
        const std::string& lowercase = getLowercase(fragment);
        for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseStringIdentifierMapBegin(); it != Identifier::getLowercaseStringIdentifierMapEnd(); ++it)
            if (it->second->hasConsoleCommands())
                if (it->first.find(lowercase) == 0 || fragment.empty())
                    CommandExecutor::getEvaluation().listOfPossibleIdentifiers_.push_back(std::pair<const std::string*, const std::string*>(&it->first, &it->second->getName()));
    }

    void CommandExecutor::createListOfPossibleFunctions(const std::string& fragment, Identifier* identifier)
    {
        CommandExecutor::getEvaluation().listOfPossibleFunctions_.clear();
        const std::string& lowercase = getLowercase(fragment);
        if (!identifier)
        {
            for (std::map<std::string, ConsoleCommand*>::const_iterator it = CommandExecutor::getLowercaseConsoleCommandShortcutMapBegin(); it != CommandExecutor::getLowercaseConsoleCommandShortcutMapEnd(); ++it)
                if (it->first.find(lowercase) == 0 || fragment.empty())
                    CommandExecutor::getEvaluation().listOfPossibleFunctions_.push_back(std::pair<const std::string*, const std::string*>(&it->first, &it->second->getName()));
        }
        else
        {
            for (std::map<std::string, ConsoleCommand*>::const_iterator it = identifier->getLowercaseConsoleCommandMapBegin(); it != identifier->getLowercaseConsoleCommandMapEnd(); ++it)
                if (it->first.find(lowercase) == 0 || fragment.empty())
                    CommandExecutor::getEvaluation().listOfPossibleFunctions_.push_back(std::pair<const std::string*, const std::string*>(&it->first, &it->second->getName()));
        }
    }

    void CommandExecutor::createListOfPossibleArguments(const std::string& fragment, ConsoleCommand* command, unsigned int param)
    {
        CommandExecutor::createArgumentCompletionList(command, param);

        CommandExecutor::getEvaluation().listOfPossibleArguments_.clear();
        const std::string& lowercase = getLowercase(fragment);
        for (ArgumentCompletionList::const_iterator it = command->getArgumentCompletionListBegin(); it != command->getArgumentCompletionListEnd(); ++it)
        {
            if (it->lowercaseComparison())
            {
                if (it->getComparable().find(lowercase) == 0 || fragment.empty())
                    CommandExecutor::getEvaluation().listOfPossibleArguments_.push_back(*it);
            }
            else
            {
                if (it->getComparable().find(fragment) == 0 || fragment.empty())
                    CommandExecutor::getEvaluation().listOfPossibleArguments_.push_back(*it);
            }
        }
    }

    Identifier* CommandExecutor::getPossibleIdentifier(const std::string& name)
    {
        const std::string& lowercase = getLowercase(name);
        std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseStringIdentifierMap().find(lowercase);
        if ((it != Identifier::getLowercaseStringIdentifierMapEnd()) && it->second->hasConsoleCommands())
            return it->second;

        return 0;
    }

    ConsoleCommand* CommandExecutor::getPossibleCommand(const std::string& name, Identifier* identifier)
    {
        const std::string& lowercase = getLowercase(name);
        if (!identifier)
        {
            std::map<std::string, ConsoleCommand*>::const_iterator it = CommandExecutor::getLowercaseConsoleCommandShortcutMap().find(lowercase);
            if (it != CommandExecutor::getLowercaseConsoleCommandShortcutMapEnd())
                return it->second;
        }
        else
        {
            std::map<std::string, ConsoleCommand*>::const_iterator it = identifier->getLowercaseConsoleCommandMap().find(lowercase);
            if (it != identifier->getLowercaseConsoleCommandMapEnd())
                return it->second;
        }
        return 0;
    }

    const std::string& CommandExecutor::getPossibleArgument(const std::string& name, ConsoleCommand* command, unsigned int param)
    {
        CommandExecutor::createArgumentCompletionList(command, param);

        const std::string& lowercase = getLowercase(name);
        for (ArgumentCompletionList::const_iterator it = command->getArgumentCompletionListBegin(); it != command->getArgumentCompletionListEnd(); ++it)
        {
            if (it->lowercaseComparison())
            {
                if (it->getComparable() == lowercase)
                    return it->getString();
            }
            else
            {
                if (it->getComparable() == name)
                    return it->getString();
            }
        }

        return BLANKSTRING;
    }

    void CommandExecutor::createArgumentCompletionList(ConsoleCommand* command, unsigned int param)
    {
        std::string params[5];

        unsigned int index = 0;
        unsigned int lowestIndex = 1 + (CommandExecutor::getEvaluation().functionclass_ != 0);

        for (unsigned int i = CommandExecutor::argumentsGiven() - 1; i >= lowestIndex; --i)
        {
            params[index] = CommandExecutor::getArgument(i);
            ++index;
            if (index >= 5)
                break;
        }

        command->createArgumentCompletionList(param, params[0], params[1], params[2], params[3], params[4]);
    }

    std::string CommandExecutor::getCommonBegin(const std::list<std::pair<const std::string*, const std::string*> >& list)
    {
        if (list.size() == 0)
        {
            return "";
        }
        else if (list.size() == 1)
        {
            return ((*list.begin()->first) + ' ');
        }
        else
        {
            std::string output;
            for (unsigned int i = 0; true; i++)
            {
                char temp = 0;
                for (std::list<std::pair<const std::string*, const std::string*> >::const_iterator it = list.begin(); it != list.end(); ++it)
                {
                    if (it->first->size() > i)
                    {
                        if (it == list.begin())
                        {
                            temp = (*it->first)[i];
                        }
                        else
                        {
                            if (temp != (*it->first)[i])
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

    std::string CommandExecutor::getCommonBegin(const ArgumentCompletionList& list)
    {
        if (list.size() == 0)
        {
            return "";
        }
        else if (list.size() == 1)
        {
            return (list.begin()->getComparable() + ' ');
        }
        else
        {
            std::string output;
            for (unsigned int i = 0; true; i++)
            {
                char tempComparable = 0;
                char temp = 0;
                for (ArgumentCompletionList::const_iterator it = list.begin(); it != list.end(); ++it)
                {
                    const std::string& argumentComparable = it->getComparable();
                    const std::string& argument = it->getString();
                    if (argument.size() > i)
                    {
                        if (it == list.begin())
                        {
                            tempComparable = argumentComparable[i];
                            temp = argument[i];
                        }
                        else
                        {
                            if (tempComparable != argumentComparable[i])
                                return output;
                            else if (temp != argument[i])
                                temp = tempComparable;
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

    void CommandExecutor::destroyExternalCommands()
    {
        for (std::set<ConsoleCommand*>::const_iterator it = CommandExecutor::getInstance().consoleCommandExternals_.begin();
            it != CommandExecutor::getInstance().consoleCommandExternals_.end(); ++it)
            delete *it;
    }
}
