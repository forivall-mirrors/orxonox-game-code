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

        if ((CommandExecutor::getEvaluation().getCommand() != command) || (CommandExecutor::getEvaluation().getState() == CS_Uninitialized))
            CommandExecutor::parse(command);

        return CommandExecutor::getEvaluation().execute();
    }

    std::string CommandExecutor::complete(const std::string& command)
    {
        if ((CommandExecutor::getEvaluation().getCommand() != command) || (CommandExecutor::getEvaluation().getState() == CS_Uninitialized))
            CommandExecutor::parse(command);

        return CommandExecutor::getEvaluation().complete();
    }

    std::string CommandExecutor::hint(const std::string& command)
    {
        if ((CommandExecutor::getEvaluation().getCommand() != command) || (CommandExecutor::getEvaluation().getState() == CS_Uninitialized))
            CommandExecutor::parse(command);

        return CommandExecutor::getEvaluation().hint();
    }

    CommandEvaluation CommandExecutor::evaluate(const std::string& command)
    {
        CommandExecutor::parse(command, true);
        CommandExecutor::getEvaluation().evaluateParams();
        return CommandExecutor::getEvaluation();
    }

    void CommandExecutor::parse(const std::string& command, bool bInitialize)
    {
        if (bInitialize)
            CommandExecutor::getEvaluation().initialize(command);
        else
            CommandExecutor::getEvaluation().setNewCommand(false);

        CommandExecutor::getEvaluation().setTokens(command + COMMAND_EXECUTOR_CURSOR);
        CommandExecutor::getEvaluation().setCommand(command);

        switch (CommandExecutor::getEvaluation().getState())
        {
            case CS_Uninitialized:
                // Impossible
                break;
            case CS_Empty:
                CommandExecutor::createListOfPossibleIdentifiers(CommandExecutor::getArgument(0));
                CommandExecutor::createListOfPossibleFunctions(CommandExecutor::getArgument(0));

                if (CommandExecutor::argumentsGiven() > 0)
                {
                    CommandExecutor::getEvaluation().setState(CS_ShortcutOrIdentifier);
                    CommandExecutor::parse(command, false);
                    return;
                }
                break;
            case CS_ShortcutOrIdentifier:
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

                if (CommandExecutor::argumentsFinished() == 0)
                {
                    // There is no finished first argument
                    unsigned int numIdentifiers = CommandExecutor::getEvaluation().getListOfPossibleIdentifiers().size();
                    unsigned int numCommands = CommandExecutor::getEvaluation().getListOfPossibleFunctions().size();

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

                // It's not a shortcut nor a classname
                CommandExecutor::getEvaluation().setState(CS_Error);
                AddLanguageEntry("commandexecutorunknownfirstargument", "is not a shortcut nor a classname");
                CommandExecutor::getEvaluation().setError("Error: " + CommandExecutor::getArgument(0) + " " + GetLocalisation("commandexecutorunknownfirstargument") + ".");
                break;
            case CS_Function:
                if (CommandExecutor::getEvaluation().getIdentifier() && CommandExecutor::argumentsGiven() > 1)
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
                    }

                    if (CommandExecutor::argumentsFinished() <= 1)
                    {
                        // There is no finished second argument
                        CommandExecutor::createListOfPossibleFunctions(CommandExecutor::getArgument(0), CommandExecutor::getEvaluation().getIdentifier());
                        unsigned int numFunctions = CommandExecutor::getEvaluation().getListOfPossibleFunctions().size();

                        if (numFunctions == 1)
                        {
                            // It must be this command
                            const std::string* possibleCommand = (*CommandExecutor::getEvaluation().getListOfPossibleFunctions().begin()).second;
                            CommandExecutor::getEvaluation().setState(CS_Function_Params);
                            CommandExecutor::getEvaluation().setFunction(CommandExecutor::getPossibleCommand(*possibleCommand));
                            CommandExecutor::parse(*possibleCommand + " ", false);
                            return;
                        }
                    }

                    // It's not a function
                    AddLanguageEntry("commandexecutorunknowncommand", "is not a valid commandname");
                    CommandExecutor::getEvaluation().setError("Error: " + CommandExecutor::getArgument(1) + " " + GetLocalisation("commandexecutorunknowncommand") + ".");
                }

                // Bad state
                CommandExecutor::getEvaluation().setState(CS_Error);
                break;
            case CS_Shortcut_Params:
            case CS_Function_Params:
                if (CommandExecutor::getEvaluation().getFunction())
                {
                    unsigned int startindex = 0;
                    if (CommandExecutor::getEvaluation().getState() == CS_Shortcut_Params)
                        startindex = 1;
                    else if (CommandExecutor::getEvaluation().getState() == CS_Function_Params)
                        startindex = 2;

                    if (CommandExecutor::argumentsGiven() >= startindex)
                    {
                        if (CommandExecutor::enoughArgumentsGiven(CommandExecutor::getEvaluation().getFunction()))
                        {
                            if (CommandExecutor::getEvaluation().getState() == CS_Shortcut_Params)
                                CommandExecutor::getEvaluation().setState(CS_Shortcut_Finished);
                            else if (CommandExecutor::getEvaluation().getState() == CS_Function_Params)
                                CommandExecutor::getEvaluation().setState(CS_Function_Finished);
                            return;
                        }
                        else
                        {
                            CommandExecutor::createListOfPossibleArguments(CommandExecutor::getLastArgument(), CommandExecutor::getEvaluation().getFunction(), CommandExecutor::getEvaluation().getOriginalTokens().size() - startindex);
                            unsigned int numArguments = CommandExecutor::getEvaluation().getListOfPossibleArguments().size();

                            if (numArguments == 1)
                            {
                                // There is exactly one possible argument
                                const std::string* possibleArgument = (*CommandExecutor::getEvaluation().getListOfPossibleArguments().begin()).second;
                                CommandExecutor::parse(CommandExecutor::getEvaluation().getTokens().subSet(0, CommandExecutor::getEvaluation().getTokens().size() - 1 - startindex).join() + " " + (*possibleArgument) + " ");
                                return;
                            }

                            if (!CommandExecutor::getEvaluation().isNewCommand())
                            {
                                // There is more than one argument, but the user wants to use this - check if there is a perfect match
                                const std::string* possibleArgument = CommandExecutor::getPossibleArgument(CommandExecutor::getLastArgument(), CommandExecutor::getEvaluation().getFunction(), CommandExecutor::getEvaluation().getOriginalTokens().size() - startindex);
                                if (possibleArgument)
                                {
                                    // There is such an argument - use it
                                    CommandExecutor::parse(command + " ", false);
                                    return;
                                }
                            }
                        }
                    }
                }

                // Bad state
                CommandExecutor::getEvaluation().setState(CS_Error);
                break;
            case CS_Shortcut_Finished:
            case CS_Function_Finished:
            case CS_Error:
                break;
        }
    }

    unsigned int CommandExecutor::argumentsFinished()
    {
        // Because we added a cursor we have +1 arguments
        if (CommandExecutor::getEvaluation().getTokens().size() >= 1)
            return (CommandExecutor::getEvaluation().getTokens().size() - 1);
        else
            return 0;
    }

    unsigned int CommandExecutor::argumentsGiven()
    {
        return CommandExecutor::getEvaluation().getOriginalTokens().size();
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
        if ((index >= 0) && index < (CommandExecutor::getEvaluation().getOriginalTokens().size()))
            return CommandExecutor::getEvaluation().getOriginalTokens()[index];
        else
            return "";
    }

    std::string CommandExecutor::getLastArgument()
    {
        if (CommandExecutor::getEvaluation().getOriginalTokens().size() > 0)
            return CommandExecutor::getEvaluation().getOriginalTokens()[0];
        else
            return "";
    }

    void CommandExecutor::createListOfPossibleIdentifiers(const std::string& fragment)
    {
        for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMapBegin(); it != Identifier::getLowercaseIdentifierMapEnd(); ++it)
            if ((*it).second->hasConsoleCommands())
                if ((*it).first.find(getLowercase(fragment)) == 0 || fragment == "")
                    CommandExecutor::getEvaluation().getListOfPossibleIdentifiers().push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second->getName()));

        CommandExecutor::getEvaluation().getListOfPossibleIdentifiers().sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleFunctions(const std::string& fragment, Identifier* identifier)
    {
        if (!identifier)
        {
            for (std::map<std::string, ConsoleCommand*>::const_iterator it = CommandExecutor::getLowercaseConsoleCommandShortcutMapBegin(); it != CommandExecutor::getLowercaseConsoleCommandShortcutMapEnd(); ++it)
                if ((*it).first.find(getLowercase(fragment)) == 0 || fragment == "")
                    CommandExecutor::getEvaluation().getListOfPossibleFunctions().push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second->getName()));
        }
        else
        {
            for (std::map<std::string, ConsoleCommand*>::const_iterator it = identifier->getLowercaseConsoleCommandMapBegin(); it != identifier->getLowercaseConsoleCommandMapEnd(); ++it)
                if ((*it).first.find(getLowercase(fragment)) == 0 || fragment == "")
                    CommandExecutor::getEvaluation().getListOfPossibleFunctions().push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second->getName()));
        }
        CommandExecutor::getEvaluation().getListOfPossibleFunctions().sort(CommandExecutor::compareStringsInList);
    }

    void CommandExecutor::createListOfPossibleArguments(const std::string& fragment, ConsoleCommand* command, unsigned int param)
    {
        for (std::list<std::pair<std::string, std::string> >::const_iterator it = command->getArgumentCompletionListBegin(param); it != command->getArgumentCompletionListEnd(param); ++it)
            if ((*it).first.find(getLowercase(fragment)) == 0 || fragment == "")
                CommandExecutor::getEvaluation().getListOfPossibleArguments().push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second));

        CommandExecutor::getEvaluation().getListOfPossibleIdentifiers().sort(CommandExecutor::compareStringsInList);

    }

    Identifier* CommandExecutor::getPossibleIdentifier(const std::string& name)
    {
        std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMap().find(getLowercase(name));
        if ((it != Identifier::getLowercaseIdentifierMapEnd()) && (*it).second->hasConsoleCommands())
            return (*it).second;

        return 0;
    }

    ConsoleCommand* CommandExecutor::getPossibleCommand(const std::string& name, Identifier* identifier)
    {
        if (!identifier)
        {
            std::map<std::string, ConsoleCommand*>::const_iterator it = CommandExecutor::getLowercaseConsoleCommandShortcutMap().find(getLowercase(name));
            if (it != CommandExecutor::getLowercaseConsoleCommandShortcutMapEnd())
                return (*it).second;
        }
        else
        {
            std::map<std::string, ConsoleCommand*>::const_iterator it = identifier->getLowercaseConsoleCommandMap().find(getLowercase(name));
            if (it != identifier->getLowercaseConsoleCommandMapEnd())
                return (*it).second;
        }
        return 0;
    }

    const std::string* CommandExecutor::getPossibleArgument(const std::string& name, ConsoleCommand* command, unsigned int param)
    {
        std::string lowercasename = getLowercase(name);
        for (std::list<std::pair<std::string, std::string> >::const_iterator it = command->getArgumentCompletionListBegin(param); it != command->getArgumentCompletionListEnd(param); ++it)
            if ((*it).first == lowercasename)
                return &(*it).second;

        return 0;
    }

    bool CommandExecutor::compareStringsInList(const std::pair<const std::string*, const std::string*>& first, const std::pair<const std::string*, const std::string*>& second)
    {
        return ((*first.first) < (*second.first));
    }
}
