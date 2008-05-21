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
        }
    }

    bool CommandExecutor::argumentsFinished(unsigned int num)
    {
        return (CommandExecutor::argumentsFinished() >= num);
    }

    unsigned int CommandExecutor::argumentsFinished()
    {
        // Because we added a cursor we have +1 arguments
        if (CommandExecutor::getEvaluation().getTokens().size() >= 1)
            return (CommandExecutor::getEvaluation().getTokens().size() - 1);
        else
            return 0;
    }

    std::string CommandExecutor::getToken(unsigned int index)
    {
        if ((index >= 0) && index < (CommandExecutor::getEvaluation().getOriginalTokens().size()))
            return CommandExecutor::getEvaluation().getOriginalTokens()[index];
        else
            return "";
    }

    unsigned int CommandExecutor::argumentsGiven()
    {
        return CommandExecutor::getEvaluation().getOriginalTokens().size();
    }

    bool CommandExecutor::enoughArgumentsGiven(ConsoleCommand* command, unsigned int head)
    {
        return (CommandExecutor::argumentsGiven() >= (head + command->getParamCount()));
    }

    void CommandExecutor::createListOfPossibleFunctionClasses(const std::string& fragment)
    {
        for (std::map<std::string, Identifier*>::const_iterator it = Identifier::getLowercaseIdentifierMapBegin(); it != Identifier::getLowercaseIdentifierMapEnd(); ++it)
            if ((*it).second->hasConsoleCommands())
                if ((*it).first.find(getLowercase(fragment)) == 0 || fragment == "")
                    CommandExecutor::getEvaluation().getListOfPossibleFunctionClasses().push_back(std::pair<const std::string*, const std::string*>(&(*it).first, &(*it).second->getName()));

        CommandExecutor::getEvaluation().getListOfPossibleFunctionClasses().sort(CommandExecutor::compareStringsInList);
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

    bool CommandExecutor::compareStringsInList(const std::pair<const std::string*, const std::string*>& first, const std::pair<const std::string*, const std::string*>& second)
    {
        return ((*first.first) < (*second.first));
    }
}
