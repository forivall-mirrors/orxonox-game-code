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

/**
    @file
    @brief Implementation of CommandExecutor
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

    /**
        @brief Returns a reference to the only instance of CommandExecutor.
    */
    /* static */ CommandExecutor& CommandExecutor::getInstance()
    {
        static CommandExecutor instance;
        return instance;
    }

    /**
        @brief Executes a command.
        @param command A string containing the command
        @param useTcl If true, the command is passed to tcl (see TclBind)
        @return Returns the error-code (see @ref CommandExecutorErrorCodes "error codes")
    */
    /* static */ int CommandExecutor::execute(const std::string& command, bool useTcl, bool printErrors)
    {
        int error;
        CommandExecutor::queryMT(command, &error, useTcl);
        if (error && printErrors)
            orxout(user_error) << "Can't execute \"" << command << "\", " << CommandExecutor::getErrorDescription(error) << ". (execute)" << endl;
        return error;
    }

    /**
        @brief Executes a command and returns its return-value.
        @param command A string containing the command
        @param error A pointer to a value (or NULL) where the error-code should be written to (see @ref CommandExecutorErrorCodes "error codes")
        @param useTcl If true, the command is passed to tcl (see TclBind)
        @return Returns the return-value of the command (if any - MT_Type::Null otherwise)
    */
    /* static */ MultiType CommandExecutor::queryMT(const std::string& command, int* error, bool useTcl)
    {
        MultiType result;
        int error_internal;

        if (useTcl)
        {
            // pass the command to tcl
            result = TclBind::eval(command, &error_internal);
        }
        else
        {
            CommandEvaluation evaluation;

            // try to get the command evaluation from the cache
            if (!CommandExecutor::getInstance().getCached(command, evaluation))
            {
                // it wasn't in the cache - evaluate the command
                evaluation = CommandExecutor::evaluate(command);

                // evaluate its arguments
                evaluation.evaluateArguments();

                // write the evaluation to the cache
                CommandExecutor::getInstance().cache(command, evaluation);
            }

            // query the command and return its return-value
            result = evaluation.query(&error_internal);
        }

        if (error)
            *error = error_internal;
        else if (error_internal)
            orxout(user_error) << "Can't execute \"" << command << "\", " << CommandExecutor::getErrorDescription(error_internal) << ". (query)" << endl;

        return result;
    }

    /**
        @brief Executes a command and returns its return-value as string.
        @param command A string containing the command
        @param error A pointer to a value (or NULL) where the error-code should be written to (see @ref CommandExecutorErrorCodes "error codes")
        @param useTcl If true, the command is passed to tcl (see TclBind)
        @return Returns the return-value of the command converted to a string (or "" if there's no return value)
    */
    /* static */ std::string CommandExecutor::query(const std::string& command, int* error, bool useTcl)
    {
        return CommandExecutor::queryMT(command, error, useTcl).getString();
    }

    /**
        @brief Evaluates the given command.
        @param command A string containing the command
        @return Returns an instance of CommandEvaluation, which contains the evaluated ConsoleCommand, if the command is valid.

        Evaluates the given command string and returns an instance of CommandEvaluation.
        If the command is valid, this contains the evaluated ConsoleCommand. Otherwise it
        can still be used to print hints or complete the command.

        @note Tcl commands can not be evaluated. You have to pass them to execute() or to
        TclBind directly.
    */
    /* static */ CommandEvaluation CommandExecutor::evaluate(const std::string& command)
    {
        // initialize the command evaluation
        CommandEvaluation evaluation;
        evaluation.initialize(command);

        // assign the fallback-command to get hints about the possible commands and groups
        evaluation.hintCommand_ = ConsoleCommand::getCommand(__CC_CommandExecutor_name, __CC_autocomplete_name);

        // check if there's at least one argument
        if (evaluation.getNumberOfArguments() >= 1)
        {
            // try to get a command from the first token
            evaluation.execCommand_ = ConsoleCommand::getCommandLC(evaluation.getToken(0));
            if (evaluation.execCommand_)
                evaluation.execArgumentsOffset_ = 1;
            else if (evaluation.getNumberOfArguments() >= 2)
            {
                // try to get a command from the first two tokens
                evaluation.execCommand_ = ConsoleCommand::getCommandLC(evaluation.getToken(0), evaluation.getToken(1));
                if (evaluation.execCommand_)
                    evaluation.execArgumentsOffset_ = 2;
            }
        }

        // if a valid command was found and the user is already entering arguments, overwrite hintCommand_ with execCommand_
        if (evaluation.execCommand_ && evaluation.getNumberOfArguments() > evaluation.execArgumentsOffset_)
        {
            evaluation.hintCommand_ = evaluation.execCommand_;
            evaluation.hintArgumentsOffset_ = evaluation.execArgumentsOffset_;
        }

        return evaluation;
    }

    /**
        @brief Returns a description of the error code.
        @param error The error code
    */
    /* static */ std::string CommandExecutor::getErrorDescription(int error)
    {
        switch (error)
        {
            case CommandExecutor::Inexistent:  return "command doesn't exist";
            case CommandExecutor::Incomplete:  return "not enough arguments given";
            case CommandExecutor::Deactivated: return "command is not active";
            case CommandExecutor::Denied:      return "access denied";
            case CommandExecutor::Error:       return "an error occurred";
            default: return "";
        }
    }

    /**
        @brief Gets an evaluated command from the cache.
        @param command The command that should be looked up in the cache
        @param evaluation Reference to a CommandEvaluation that will be used to return the cached evaluation.
        @return Returns true if the command was found in the cache
    */
    bool CommandExecutor::getCached(const std::string& command, CommandEvaluation& evaluation)
    {
        if (Shell::getCacheSize() == 0)
            return false;

        // check if the command is in the cache
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

    /**
        @brief Writes a command evaluation for a given command to the cache.
    */
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

    /**
        @brief This function is used as console command which executes commands that are usually hidden.

        The argument completion function of this console commands is used to find
        and enter hidden commands and their arguments.
    */
    /* static */ MultiType CommandExecutor::unhide(const std::string& command)
    {
        return CommandExecutor::queryMT(command);
    }

    /**
        @brief This function is used as console command which saves a command and optionally also it's arguments as a new console command with a new name.
        @param alias The name of the new command alias
        @param command The existing command and (optionally) its arguments
    */
    /* static */ void CommandExecutor::alias(const std::string& alias, const std::string& command)
    {
        // first check if the given command is valid, else print an error
        CommandEvaluation evaluation = CommandExecutor::evaluate(command);
        if (evaluation.isValid())
        {
            // it is valid - copy the executor of this command
            ExecutorPtr executor = new Executor(*evaluation.getConsoleCommand()->getExecutor().get());

            // evaluate the arguments and if this returns no error, store them as default values
            if (!evaluation.evaluateArguments())
            {
                for (size_t i = 0; i < MAX_FUNCTOR_ARGUMENTS; ++i)
                    executor->setDefaultValue(i, evaluation.getEvaluatedArgument(i));
            }

            // split the alias in tokens
            SubString tokens(alias, " ");

            // check if the alias already exists - print an error and return if it does
            if ((tokens.size() == 1 && ConsoleCommand::getCommand(tokens[0])) || (tokens.size() == 2 && ConsoleCommand::getCommand(tokens[0], tokens[1])))
            {
                orxout(user_error) << "A command with name \"" << alias << "\" already exists." << endl;
                return;
            }

            // create a new console command with the given alias as its name
            if (tokens.size() == 1)
                createConsoleCommand(tokens[0], executor);
            else if (tokens.size() == 2)
                createConsoleCommand(tokens[0], tokens[1], executor);
            else
                orxout(user_error) << "\"" << alias << "\" is not a valid alias name (must have one or two words)." << endl;
        }
        else
            orxout(user_error) << "\"" << command << "\" is not a valid command (did you mean \"" << evaluation.getCommandSuggestion() << "\"?)." << endl;
    }
}
