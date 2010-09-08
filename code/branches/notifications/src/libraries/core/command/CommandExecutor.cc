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
#include "TclBind.h"
#include "Shell.h"

namespace orxonox
{
    static const std::string __CC_CommandExecutor_name = "CommandExecutor";
    static const std::string __CC_autocomplete_name = "autocomplete";

    SetConsoleCommand(__CC_CommandExecutor_name, __CC_autocomplete_name, &CommandExecutor::_autocomplete)
        .hide()
        .argumentCompleter(0, autocompletion::groupsandcommands())
        .argumentCompleter(1, autocompletion::subcommands());

    SetConsoleCommand("unhide", &CommandExecutor::unhide)
        .argumentCompleter(0, autocompletion::hiddencommand());

    SetConsoleCommand("alias", &CommandExecutor::alias)
        .argumentCompleter(1, autocompletion::command());

    /* static */ CommandExecutor& CommandExecutor::getInstance()
    {
        static CommandExecutor instance;
        return instance;
    }

    /* static */ int CommandExecutor::execute(const std::string& command, bool useTcl)
    {
        int error;
        CommandExecutor::queryMT(command, &error, useTcl);
        return error;
    }

    /* static */ MultiType CommandExecutor::queryMT(const std::string& command, int* error, bool useTcl)
    {
        if (useTcl)
            return TclBind::eval(command, error);
        else
        {
            CommandEvaluation evaluation;
            if (!CommandExecutor::getInstance().getCached(command, evaluation))
            {
                evaluation = CommandExecutor::evaluate(command);
                evaluation.evaluateParams();
                CommandExecutor::getInstance().cache(command, evaluation);
            }

            return evaluation.query(error);
        }
    }

    /* static */ std::string CommandExecutor::query(const std::string& command, int* error, bool useTcl)
    {
        return CommandExecutor::queryMT(command, error, useTcl).getString();
    }

    /* static */ CommandEvaluation CommandExecutor::evaluate(const std::string& command)
    {
        CommandEvaluation evaluation;
        evaluation.initialize(command);

        evaluation.hintCommand_ = ConsoleCommand::getCommand(__CC_CommandExecutor_name, __CC_autocomplete_name);

        if (evaluation.getNumberOfArguments() >= 1)
        {
            evaluation.execCommand_ = ConsoleCommand::getCommandLC(evaluation.getToken(0));
            if (evaluation.execCommand_)
                evaluation.execArgumentsOffset_ = 1;
            else if (evaluation.getNumberOfArguments() >= 2)
            {
                evaluation.execCommand_ = ConsoleCommand::getCommandLC(evaluation.getToken(0), evaluation.getToken(1));
                if (evaluation.execCommand_)
                    evaluation.execArgumentsOffset_ = 2;
            }
        }

        if (evaluation.execCommand_ && evaluation.getNumberOfArguments() > evaluation.execArgumentsOffset_)
        {
            evaluation.hintCommand_ = evaluation.execCommand_;
            evaluation.hintArgumentsOffset_ = evaluation.execArgumentsOffset_;
        }

        return evaluation;
    }

    bool CommandExecutor::getCached(const std::string& command, CommandEvaluation& evaluation)
    {
        if (Shell::getCacheSize() == 0)
            return false;

        std::map<std::string, CacheEntry>::iterator it = this->cache_.find(command);
        if (it != this->cache_.end())
        {
            // update ordered list of cached commands (move it to the front)
            this->cachelist_.erase(it->second.iterator_);
            this->cachelist_.push_front(command);
            it->second.iterator_ = this->cachelist_.begin();

            // assign the cached evaluation
            evaluation = it->second.evaluation_;
            return true;
        }
        return false;
    }

    void CommandExecutor::cache(const std::string& command, const CommandEvaluation& evaluation)
    {
        if (Shell::getCacheSize() == 0)
            return;

        // push command to the front of the ordered list
        this->cachelist_.push_front(command);

        // create a cache entry and store it in the cache
        CacheEntry entry;
        entry.evaluation_ = evaluation;
        entry.iterator_ = this->cachelist_.begin();
        this->cache_[command] = entry;

        // remove the last command in the ordered list from the cache if it exceeds the maximum size of the cache
        if (this->cachelist_.size() > Shell::getCacheSize())
        {
            this->cache_.erase(this->cachelist_.back());
            this->cachelist_.pop_back();
        }
    }

    /* static */ MultiType CommandExecutor::unhide(const std::string& command)
    {
        return CommandExecutor::queryMT(command);
    }

    /* static */ void CommandExecutor::alias(const std::string& alias, const std::string& command)
    {
        CommandEvaluation evaluation = CommandExecutor::evaluate(command);
        if (evaluation.isValid())
        {
            ExecutorPtr executor = new Executor(*evaluation.getConsoleCommand()->getExecutor().get());

            if (!evaluation.evaluateParams())
            {
                for (size_t i = 0; i < MAX_FUNCTOR_ARGUMENTS; ++i)
                    executor->setDefaultValue(i, evaluation.getEvaluatedParameter(i));
            }

            SubString tokens(alias, " ");

            if ((tokens.size() == 1 && ConsoleCommand::getCommand(tokens[0])) || (tokens.size() == 2 && ConsoleCommand::getCommand(tokens[0], tokens[1])))
            {
                COUT(1) << "Error: A command with name \"" << alias << "\" already exists." << std::endl;
                return;
            }

            if (tokens.size() == 1)
                createConsoleCommand(tokens[0], executor);
            else if (tokens.size() == 2)
                createConsoleCommand(tokens[0], tokens[1], executor);
            else
                COUT(1) << "Error: \"" << alias << "\" is not a valid alias name (must have one or two words)." << std::endl;
        }
        else
            COUT(1) << "Error: \"" << command << "\" is not a valid command (did you mean \"" << evaluation.getCommandSuggestion() << "\"?)." << std::endl;
    }
}