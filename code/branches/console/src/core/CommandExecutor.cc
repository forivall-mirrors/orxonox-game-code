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
#include "ConsoleCommand.h"
#include "util/String.h"
#include "util/Convert.h"
#include "Identifier.h"
#include "Language.h"
#include "Debug.h"
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

    ConsoleCommand& CommandExecutor::addConsoleCommandShortcut(ConsoleCommand* command)
    {
        std::map<std::string, ConsoleCommand*>::const_iterator it = CommandExecutor::getInstance().consoleCommandShortcuts_.find(command->getName());
        if (it != CommandExecutor::getInstance().consoleCommandShortcuts_.end())
        {
            COUT(2) << "Warning: Overwriting console-command shortcut with name " << command->getName() << "." << std::endl;
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
            return (*it).second;
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
            return (*it).second;
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
        CommandExecutor::parse(command, true);
        CommandExecutor::getEvaluation().evaluateParams();
        return CommandExecutor::getEvaluation();
    }

    void CommandExecutor::parseIfNeeded(const std::string& command)
    {
        if ((CommandExecutor::getEvaluation().getCommand() != command) || (CommandExecutor::getEvaluation().getState() == CS_Uninitialized))
            CommandExecutor::parse(command);
        else if (!CommandExecutor::getEvaluation().isValid())
        {
            CommandExecutor::getEvaluation().setNewCommand(false);
            CommandExecutor::parse(command, false);
        }
    }

    void CommandExecutor::parse(const std::string& command, bool bInitialize)
    {
std::cout << "parse: command: >" << command << "<" << std::endl;
        if (bInitialize)
            CommandExecutor::getEvaluation().initialize(command);

        CommandExecutor::getEvaluation().setTokens(command);
        CommandExecutor::getEvaluation().setCommand(command);

        switch (CommandExecutor::getEvaluation().getState())
        {
            case CS_Uninitialized:
            {
std::cout << "parse: state: CS_Uninitialized" << std::endl;
                // Impossible
                break;
            }
            case CS_Empty:
            {
std::cout << "parse: state: CS_Empty" << std::endl;
                CommandExecutor::createListOfPossibleIdentifiers(CommandExecutor::getArgument(0));
                CommandExecutor::createListOfPossibleFunctions(CommandExecutor::getArgument(0));

                if (CommandExecutor::argumentsGiven() > 0)
                {
                    CommandExecutor::getEvaluation().setState(CS_ShortcutOrIdentifier);
                    CommandExecutor::parse(command, false);
                    return;
                }
                break;
            }
            case CS_ShortcutOrIdentifier:
            {
std::cout << "parse: state: CS_ShortcutOrIdentifier" << std::endl;
                if (CommandExecutor::argumentsFinished() > 0 || !CommandExecutor::getEvaluation().isNewCommand())
                {
                    // There's already a finished first argument - check if it's function or a classname
                    CommandExecutor::getEvaluation().setIdentifier(CommandExecutor::getPossibleIdentifier(CommandExecutor::getArgument(0)));
                    CommandExecutor::getEvaluation().setFunction(CommandExecutor::getPossibleCommand(CommandExecutor::getArgument(0)));

                    if (CommandExecutor::getEvaluation().getFunction())
                    {
                        // It's a shortcut - continue parsing
                        CommandExecutor::getEvaluation().setState(CS_Shortcut_Params);
                        if (CommandExecutor::argumentsFinished() > 0 )
                            CommandExecutor::parse(command, false);
                        else
                            CommandExecutor::parse(command + " ", false);
                        return;
                    }
                    else if (CommandExecutor::getEvaluation().getIdentifier())
                    {
                        // It's a classname - continue parsing
                        CommandExecutor::getEvaluation().setState(CS_Function);
                        if (CommandExecutor::argumentsFinished() > 0 )
                            CommandExecutor::parse(command, false);
                        else
                            CommandExecutor::parse(command + " ", false);
                        return;
                    }
                }

                unsigned int numIdentifiers = CommandExecutor::getEvaluation().getListOfPossibleIdentifiers().size();
                unsigned int numCommands = CommandExecutor::getEvaluation().getListOfPossibleFunctions().size();

                if (CommandExecutor::argumentsFinished() == 0)
                {
                    // There is no finished first argument
                    if (numCommands == 1 && numIdentifiers == 0)
                    {
                        // It must be this command
                        const std::string* possibleCommand = (*CommandExecutor::getEvaluation().getListOfPossibleFunctions().begin()).second;
                        CommandExecutor::getEvaluation().setState(CS_Shortcut_Params);
                        CommandExecutor::getEvaluation().setFunction(CommandExecutor::getPossibleCommand(*possibleCommand));
                        CommandExecutor::parse(*possibleCommand + " ", false);
                        return;
                    }
                    else if (numIdentifiers == 1 && numCommands == 0)
                    {
                        // It must be this classname
                        const std::string* possibleIdentifier = (*CommandExecutor::getEvaluation().getListOfPossibleIdentifiers().begin()).second;
                        CommandExecutor::getEvaluation().setState(CS_Function);
                        CommandExecutor::getEvaluation().setIdentifier(CommandExecutor::getPossibleIdentifier(*possibleIdentifier));
                        CommandExecutor::parse(*possibleIdentifier + " ", false);
                        return;
                    }
                }

                if (numCommands == 0 && numIdentifiers == 0)
                {
                    // It's not a shortcut nor a classname
                    CommandExecutor::getEvaluation().setState(CS_Error);
                    AddLanguageEntry("commandexecutorunknownfirstargument", "is not a shortcut nor a classname");
                    CommandExecutor::getEvaluation().setError("Error: " + CommandExecutor::getArgument(0) + " " + GetLocalisation("commandexecutorunknownfirstargument") + ".");
                }
                break;
            }
            case CS_Function:
std::cout << "parse: state: CS_Function" << std::endl;
            {
                if (CommandExecutor::getEvaluation().getIdentifier() && CommandExecutor::argumentsGiven() > 0)
                {
                    if (CommandExecutor::argumentsFinished() > 1 || !CommandExecutor::getEvaluation().isNewCommand())
                    {
                        // There is already a second argument - check if it's a valid function
                        CommandExecutor::getEvaluation().setFunction(CommandExecutor::getPossibleCommand(CommandExecutor::getArgument(1), CommandExecutor::getEvaluation().getIdentifier()));

                        if (CommandExecutor::getEvaluation().getFunction())
                        {
                            // It's a shortcut - continue parsing
                            CommandExecutor::getEvaluation().setState(CS_Function_Params);
                            if (CommandExecutor::argumentsFinished() > 1 )
                                CommandExecutor::parse(command, false);
                            else
                                CommandExecutor::parse(command + " ", false);
                            return;
                        }
                        else if (CommandExecutor::argumentsFinished() > 1)
                        {
                            // It's not a function
                            AddLanguageEntry("commandexecutorunknowncommand", "is not a valid commandname");
                            CommandExecutor::getEvaluation().setError("Error: " + CommandExecutor::getArgument(1) + " " + GetLocalisation("commandexecutorunknowncommand") + ".");
                            CommandExecutor::getEvaluation().setState(CS_Error);
                        }
                    }

                    CommandExecutor::createListOfPossibleFunctions(CommandExecutor::getArgument(1), CommandExecutor::getEvaluation().getIdentifier());

                    if (CommandExecutor::argumentsFinished() <= 1)
                    {
                        // There is no finished second argument
                        unsigned int numFunctions = CommandExecutor::getEvaluation().getListOfPossibleFunctions().size();

                        if (numFunctions == 1)
                        {
                            // It must be this command
                            const std::string* possibleCommand = (*CommandExecutor::getEvaluation().getListOfPossibleFunctions().begin()).second;
                            CommandExecutor::getEvaluation().setState(CS_Function_Params);
                            CommandExecutor::getEvaluation().setFunction(CommandExecutor::getPossibleCommand(*possibleCommand, CommandExecutor::getEvaluation().getIdentifier()));
                            CommandExecutor::parse(CommandExecutor::getArgument(0) + " " + *possibleCommand + " ", false);
                            return;
                        }
                        else if (numFunctions == 0)
                        {
                            // It's not a function
                            AddLanguageEntry("commandexecutorunknowncommand", "is not a valid commandname");
                            CommandExecutor::getEvaluation().setError("Error: " + CommandExecutor::getArgument(1) + " " + GetLocalisation("commandexecutorunknowncommand") + ".");
                            CommandExecutor::getEvaluation().setState(CS_Error);
                        }
                    }

                    // It's ambiguous
                    return;
                }

                // Bad state
                CommandExecutor::getEvaluation().setState(CS_Error);
                break;
            }
            case CS_Shortcut_Params:
std::cout << "parse: state: CS_Shortcut_Params" << std::endl;
            case CS_Function_Params:
            {
std::cout << "parse: state: CS_Function_Params" << std::endl;
                if (CommandExecutor::getEvaluation().getFunction())
                {
std::cout << "1\n";
                    unsigned int startindex = 0;
                    if (CommandExecutor::getEvaluation().getState() == CS_Shortcut_Params)
                        startindex = 1;
                    else if (CommandExecutor::getEvaluation().getState() == CS_Function_Params)
                        startindex = 2;
std::cout << "2\n";

                    if (CommandExecutor::argumentsGiven() >= startindex)
                    {
std::cout << "3\n";
                        if (CommandExecutor::enoughArgumentsGiven(CommandExecutor::getEvaluation().getFunction()))
                        {
std::cout << "4\n";
                            if (CommandExecutor::getEvaluation().getState() == CS_Shortcut_Params)
                                CommandExecutor::getEvaluation().setState(CS_Shortcut_Finished);
                            else if (CommandExecutor::getEvaluation().getState() == CS_Function_Params)
                                CommandExecutor::getEvaluation().setState(CS_Function_Finished);

                            return;
                        }
                        else
                        {
std::cout << "5\n";
std::cout << "last argument: " << CommandExecutor::getLastArgument() << std::endl;
std::cout << "function: " << CommandExecutor::getEvaluation().getFunction() << std::endl;
std::cout << "functionname: " << CommandExecutor::getEvaluation().getFunction()->getName() << std::endl;
std::cout << "param nr: " << CommandExecutor::getEvaluation().getTokens().size() - startindex << std::endl;
                            CommandExecutor::createListOfPossibleArguments(CommandExecutor::getLastArgument(), CommandExecutor::getEvaluation().getFunction(), CommandExecutor::getEvaluation().getTokens().size() - startindex);
std::cout << "6\n";
                            unsigned int numArguments = CommandExecutor::getEvaluation().getListOfPossibleArguments().size();

                            if (numArguments == 1)
                            {
std::cout << "7\n";
                                // There is exactly one possible argument
                                const std::string* possibleArgument = (*CommandExecutor::getEvaluation().getListOfPossibleArguments().begin()).second;
                                CommandExecutor::parse(CommandExecutor::getEvaluation().getTokens().subSet(0, CommandExecutor::getEvaluation().getTokens().size() - 1 - startindex).join() + " " + (*possibleArgument) + " ");
                                return;
                            }

std::cout << "8\n";
                            if ((CommandExecutor::argumentsGiven() > CommandExecutor::argumentsFinished()) && (!CommandExecutor::getEvaluation().isNewCommand()))
                            {
std::cout << "9\n";
                                // There is more than one argument, but the user wants to use this - check if there is a perfect match
                                const std::string* possibleArgument = CommandExecutor::getPossibleArgument(CommandExecutor::getLastArgument(), CommandExecutor::getEvaluation().getFunction(), CommandExecutor::getEvaluation().getTokens().size() - startindex);
                                if (possibleArgument)
                                {
std::cout << "10\n";
                                    // There is such an argument - use it
                                    CommandExecutor::parse(command + " ", false);
                                    return;
                                }
std::cout << "11\n";
                            }
std::cout << "12\n";
                        }
std::cout << "13\n";

                        // Nothing to do
                        return;
                    }
                }

                // Bad state
                CommandExecutor::getEvaluation().setState(CS_Error);
                break;
            }
            case CS_Shortcut_Finished:
std::cout << "parse: state: CS_Shortcut_Finished" << std::endl;
                break;
            case CS_Function_Finished:
std::cout << "parse: state: CS_Function_Finished" << std::endl;
                break;
            case CS_Error:
std::cout << "parse: state: CS_Error" << std::endl;
                break;
        }
    }

    unsigned int CommandExecutor::argumentsFinished()
    {
        if (CommandExecutor::getEvaluation().getCommand().size() > 0)
        {
            if (CommandExecutor::getEvaluation().getCommand()[CommandExecutor::getEvaluation().getCommand().size() - 1] == ' ')
                return CommandExecutor::getEvaluation().getTokens().size();
            else if (CommandExecutor::getEvaluation().getTokens().size() > 0)
                return CommandExecutor::getEvaluation().getTokens().size() - 1;
        }
        return 0;
    }

    unsigned int CommandExecutor::argumentsGiven()
    {
        return CommandExecutor::getEvaluation().getTokens().size();
    }

    bool CommandExecutor::enoughArgumentsGiven(ConsoleCommand* command)
    {
        if (CommandExecutor::getEvaluation().getIdentifier())
            return (CommandExecutor::argumentsGiven() >= (2 + command->getParamCount()));
        else
            return (CommandExecutor::argumentsGiven() >= (1 + command->getParamCount()));
    }

    std::string CommandExecutor::getArgument(unsigned int index)
    {
        if ((index >= 0) && index < (CommandExecutor::getEvaluation().getTokens().size()))
            return CommandExecutor::getEvaluation().getTokens()[index];
        else
            return "";
    }

    std::string CommandExecutor::getLastArgument()
    {
        if (CommandExecutor::getEvaluation().getTokens().size() > 0)
            if (CommandExecutor::getEvaluation().getCommand().size() > 0 && CommandExecutor::getEvaluation().getCommand()[CommandExecutor::getEvaluation().getCommand().size() - 1] != ' ')
                return CommandExecutor::getEvaluation().getTokens()[CommandExecutor::getEvaluation().getTokens().size() - 1];

        return "";
    }

    void CommandExecutor::createListOfPossibleIdentifiers(const std::string& fragment)
    {
        std::string lowercase = getLowercase(fragment);
        for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMapBegin(); it != Identifier::getLowercaseIdentifierMapEnd(); ++it)
            if ((*it).second->hasConsoleCommands())
                if ((*it).first.find(lowercase) == 0 || fragment == "")
                    CommandExecutor::getEvaluation().getListOfPossibleIdentifiers().push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second->getName()));

        CommandExecutor::getEvaluation().getListOfPossibleIdentifiers().sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleFunctions(const std::string& fragment, Identifier* identifier)
    {
        std::string lowercase = getLowercase(fragment);
        if (!identifier)
        {
            for (std::map<std::string, ConsoleCommand*>::const_iterator it = CommandExecutor::getLowercaseConsoleCommandShortcutMapBegin(); it != CommandExecutor::getLowercaseConsoleCommandShortcutMapEnd(); ++it)
                if ((*it).first.find(lowercase) == 0 || fragment == "")
                    CommandExecutor::getEvaluation().getListOfPossibleFunctions().push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second->getName()));
        }
        else
        {
            for (std::map<std::string, ConsoleCommand*>::const_iterator it = identifier->getLowercaseConsoleCommandMapBegin(); it != identifier->getLowercaseConsoleCommandMapEnd(); ++it)
                if ((*it).first.find(lowercase) == 0 || fragment == "")
                    CommandExecutor::getEvaluation().getListOfPossibleFunctions().push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second->getName()));
        }

        CommandExecutor::getEvaluation().getListOfPossibleFunctions().sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleArguments(const std::string& fragment, ConsoleCommand* command, unsigned int param)
    {
std::cout << "2_1\n";
        std::string lowercase = getLowercase(fragment);
std::cout << "2_2\n";
        for (std::list<std::pair<std::string, std::string> >::const_iterator it = command->getArgumentCompletionListBegin(param); it != command->getArgumentCompletionListEnd(param); ++it)
        {
std::cout << "2_3\n";
            if ((*it).first.find(lowercase) == 0 || fragment == "")
            {
std::cout << "2_4\n";
                CommandExecutor::getEvaluation().getListOfPossibleArguments().push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second));
std::cout << "2_5\n";
            }
        }

std::cout << "2_6\n";
        CommandExecutor::getEvaluation().getListOfPossibleArguments().sort(CommandExecutor::compareStringsInList);
std::cout << "2_7\n";
    }

    Identifier* CommandExecutor::getPossibleIdentifier(const std::string& name)
    {
        std::string lowercase = getLowercase(name);
        std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMap().find(lowercase);
        if ((it != Identifier::getLowercaseIdentifierMapEnd()) && (*it).second->hasConsoleCommands())
            return (*it).second;

        return 0;
    }

    ConsoleCommand* CommandExecutor::getPossibleCommand(const std::string& name, Identifier* identifier)
    {
        std::string lowercase = getLowercase(name);
        if (!identifier)
        {
            std::map<std::string, ConsoleCommand*>::const_iterator it = CommandExecutor::getLowercaseConsoleCommandShortcutMap().find(lowercase);
            if (it != CommandExecutor::getLowercaseConsoleCommandShortcutMapEnd())
                return (*it).second;
        }
        else
        {
            std::map<std::string, ConsoleCommand*>::const_iterator it = identifier->getLowercaseConsoleCommandMap().find(lowercase);
            if (it != identifier->getLowercaseConsoleCommandMapEnd())
                return (*it).second;
        }
        return 0;
    }

    const std::string* CommandExecutor::getPossibleArgument(const std::string& name, ConsoleCommand* command, unsigned int param)
    {
        std::string lowercase = getLowercase(name);
        for (std::list<std::pair<std::string, std::string> >::const_iterator it = command->getArgumentCompletionListBegin(param); it != command->getArgumentCompletionListEnd(param); ++it)
            if ((*it).first == lowercase)
                return &(*it).second;

        return 0;
    }

    bool CommandExecutor::compareStringsInList(const std::pair<const std::string*, const std::string*>& first, const std::pair<const std::string*, const std::string*>& second)
    {
        return ((*first.first) < (*second.first));
    }
}
