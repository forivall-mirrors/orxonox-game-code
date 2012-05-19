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
    @brief Implementation of CommandEvaluation
*/

#include "CommandEvaluation.h"

#include "util/StringUtils.h"
#include "CommandExecutor.h"
#include "ConsoleCommand.h"

namespace orxonox
{
    /**
        @brief Constructor: Initializes the command evaluation with an empty command.
    */
    CommandEvaluation::CommandEvaluation()
    {
        this->initialize("");
    }

    /**
        @brief Initializes all values.
    */
    void CommandEvaluation::initialize(const std::string& command)
    {
        this->execCommand_ = 0;
        this->hintCommand_ = 0;
        this->string_ = command;
        this->execArgumentsOffset_ = 0;
        this->hintArgumentsOffset_ = 0;
        this->bPossibleArgumentsRetrieved_ = false;
        this->possibleArguments_.clear();
        this->bEvaluatedArguments_ = false;
        this->bTriedToEvaluatedArguments_ = false;
        this->numberOfEvaluatedArguments_ = 0;

        // split the command into tokens
        this->tokens_.split(command, " ", SubString::WhiteSpaces, false, '\\', true, '"', true, '{', '}', true, '\0');
    }

    /**
        @brief Returns the number of tokens according to the definition of CommandExecutor (which counts also an empty argument at the end of the string).
    */
    unsigned int CommandEvaluation::getNumberOfArguments() const
    {
        unsigned int count = this->tokens_.size();

        // If the last char in the string is a space character (or the string is empty), add +1 to the number of tokens, because this counts as an additional (but empty) argument
        if (count == 0 || this->string_[this->string_.size() - 1] == ' ')
            return count + 1;
        else
            return count;
    }

    /**
        @brief Returns the last argument (which is the one the user currently enters into the shell).
    */
    const std::string& CommandEvaluation::getLastArgument() const
    {
        // the string is empty or ends with a space character, the user is just about to enter a new argument (but its still empty). return a blank string in this case.
        if (this->tokens_.size() == 0 || this->string_[this->string_.size() - 1] == ' ')
            return BLANKSTRING;
        else
            return this->tokens_.back();
    }

    /**
        @brief Returns the token with the given index (or a blank string if it doesn't exist).
    */
    const std::string& CommandEvaluation::getToken(unsigned int i) const
    {
        if (i < this->tokens_.size())
            return this->tokens_[i];
        else
            return BLANKSTRING;
    }

    /**
        @brief Executes the command which was evaluated by this object.
        @return Returns the error code (see @ref CommandExecutorErrorCodes "CommandExecutor error codes")
    */
    int CommandEvaluation::execute()
    {
        int error;
        this->query(&error);
        return error;
    }

    /**
        @brief Executes the command which was evaluated by this object and returns its return-value.
        @param error A pointer to an integer (or NULL) which will be used to write error codes to (see @ref CommandExecutorErrorCodes "CommandExecutor error codes")
        @return Returns the result of the command (or MultiType::Null if there is no return value)
    */
    MultiType CommandEvaluation::query(int* error)
    {
        // check if an error value was passed by reference
        if (error)
        {
            // Determine the error-code and return if it is not Success

            *error = CommandExecutor::Success;

            if (!this->execCommand_)
                *error = CommandExecutor::Inexistent;
            else if (!this->execCommand_->isActive())
                *error = CommandExecutor::Deactivated;
            else if (!this->execCommand_->hasAccess())
                *error = CommandExecutor::Denied;

            if (*error != CommandExecutor::Success)
                return MultiType::Null;
        }

        // check if it's possible to execute the command
        if (this->execCommand_ && this->execCommand_->isActive() && this->execCommand_->hasAccess())
        {
            // if the arguments weren't evaluated yet, do it now.
            if (!this->bTriedToEvaluatedArguments_)
                this->evaluateArguments(false);

            // check if the argument evaluation succeded
            if (this->bEvaluatedArguments_)
            {
                orxout(verbose, context::commands) << "CE_execute (evaluation): " << this->execCommand_->getName() << " with " << this->numberOfEvaluatedArguments_ << " arguments: " << this->arguments_[0] << ' ' << this->arguments_[1] << ' ' << this->arguments_[2] << ' ' << this->arguments_[3] << ' ' << this->arguments_[4] << endl;

                // pass as many arguments to the executor as were evaluated (thus the executor can still use additional default values)
                switch (this->numberOfEvaluatedArguments_)
                {
                    case 0:  return (*this->execCommand_->getExecutor())();
                    case 1:  return (*this->execCommand_->getExecutor())(this->arguments_[0]);
                    case 2:  return (*this->execCommand_->getExecutor())(this->arguments_[0], this->arguments_[1]);
                    case 3:  return (*this->execCommand_->getExecutor())(this->arguments_[0], this->arguments_[1], this->arguments_[2]);
                    case 4:  return (*this->execCommand_->getExecutor())(this->arguments_[0], this->arguments_[1], this->arguments_[2], this->arguments_[3]);
                    case 5:
                    default: return (*this->execCommand_->getExecutor())(this->arguments_[0], this->arguments_[1], this->arguments_[2], this->arguments_[3], this->arguments_[4]);
                }
            }
            else if (error)
                *error = CommandExecutor::Incomplete;
        }

        // return a null value in case of an error
        return MultiType::Null;
    }

    /**
        @brief Evaluates the arguments of the command.
        @param bPrintError If true, the function prints an error message if it doesn't succeed
        @return Returns the error code (see @ref CommandExecutorErrorCodes "CommandExecutor error codes")
    */
    int CommandEvaluation::evaluateArguments(bool bPrintError)
    {
        this->bTriedToEvaluatedArguments_ = true;

        // check if there's a command to execute
        if (!this->execCommand_)
        {
            if (bPrintError)
                orxout(internal_error, context::commands) << "Can't evaluate arguments, no console command assigned." << endl;
            return CommandExecutor::Inexistent;
        }

        int error;

        // try to evaluate the arguments using the executor of the evaluated command.
        // the arguments are currently stored as strings in token_, but afterwards they will be converted to the right type and stored in arguments_
        this->numberOfEvaluatedArguments_ = this->execCommand_->getExecutor()->evaluateArguments(this->tokens_.subSet(this->execArgumentsOffset_), this->arguments_, &error, " ");

        // check if an error occurred
        if (!error)
            this->bEvaluatedArguments_ = true;
        else if (bPrintError)
            orxout(internal_error, context::commands) << "Can't evaluate arguments, not enough arguments given." << endl;

        return error;
    }

    /**
        @brief Replaces an evaluated argument with a new value.
        @param index The index of the parameter (the first argument has index 0)
        @param arg The new value of the parameter
    */
    void CommandEvaluation::setEvaluatedArgument(unsigned int index, const MultiType& arg)
    {
        if (index < MAX_FUNCTOR_ARGUMENTS)
            this->arguments_[index] = arg;
    }

    /**
        @brief Returns the evaluated argument with given index.
        @param index The index of the argument (the first argument has index 0)
    */
    MultiType CommandEvaluation::getEvaluatedArgument(unsigned int index) const
    {
        if (index < MAX_FUNCTOR_ARGUMENTS)
            return this->arguments_[index];

        return MultiType::Null;
    }

    /**
        @brief Completes the given command string using the list of possible arguments.
        @return Returns the completed command string

        This is called by the shell if the user presses the @a tab key. The currently entered
        argument will be completed as good as possible by using the argument completion list
        of the evaluated command.
    */
    std::string CommandEvaluation::complete()
    {
        // check if it's possible to complete the command
        if (!this->hintCommand_ || !this->hintCommand_->isActive())
            return this->string_;

        // get the list of possible arguments
        if (!this->bPossibleArgumentsRetrieved_)
            this->retrievePossibleArguments();

        // if the list is empty, return the current command string
        if (CommandEvaluation::getSize(this->possibleArguments_) == 0)
        {
            return this->string_;
        }
        else
        {
            // get the first part of the command string from the beginning up to the last space character
            std::string output = this->string_.substr(0, this->string_.find_last_of(' ') + 1);

            // add the common begin of all possible arguments
            output += CommandEvaluation::getCommonBegin(this->possibleArguments_);

            // return the resulting string
            return output;
        }
    }

    /**
        @brief Returns a string containing hints or possible arguments for the evaluated command.

        This is called by the shell if the user presses the @a tab key. It prints a list of
        possible arguments or other hints, returned by the argument completion list of the
        evaluated command. If there's no such list, the syntax of the command is returned.
    */
    std::string CommandEvaluation::hint()
    {
        // check if it's possible to get hints for this command
        if (!this->hintCommand_ || !this->hintCommand_->isActive())
            return "";

        // get the list of possible arguments
        if (!this->bPossibleArgumentsRetrieved_)
            this->retrievePossibleArguments();

        // return the list of possible arguments if:
        //   a) it contains at least one non-empty argument
        //   b) it contains an entry that may be empty (not an actual argument, just a helping text) AND the command is valid
        if (CommandEvaluation::getSize(this->possibleArguments_) > 0 || (!this->possibleArguments_.empty() && this->isValid()))
            return CommandEvaluation::dump(this->possibleArguments_);

        // at this point there's no valid argument in the list, so check if the command is actually valid
        if (this->isValid())
        {
            // yes it is - return the syntax of the command
            return CommandEvaluation::dump(this->hintCommand_);
        }
        else
        {
            // no the command is not valid
            if (this->getNumberOfArguments() > 2)
            {
                // the user typed 2+ arguments, but they don't name a command - print an error
                return std::string("Error: There is no command with name \"") + this->getToken(0) + " " + this->getToken(1) + "\".";
            }
            else
            {
                // the user typed 1-2 arguments, check what he tried to type and print a suitable error
                std::string groupLC = getLowercase(this->getToken(0));
                for (std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = ConsoleCommand::getCommandsLC().begin(); it_group != ConsoleCommand::getCommandsLC().end(); ++it_group)
                    if (it_group->first == groupLC)
                        return std::string("Error: There is no command in group \"") + this->getToken(0) + "\" starting with \"" + this->getToken(1) + "\".";

                return std::string("Error: There is no command starting with \"") + this->getToken(0) + "\".";
            }
        }
    }

    /**
        @brief If the command couln't be evaluated because it doesn't exist, print a suggestion for
        a command that looks close to the entered command (useful if the user mistyped the command).
    */
    std::string CommandEvaluation::getCommandSuggestion() const
    {
        std::string token0_LC = getLowercase(this->getToken(0));
        std::string token1_LC = getLowercase(this->getToken(1));

        std::string nearestCommand;
        unsigned int nearestDistance = (unsigned int)-1;

        // iterate through all groups and their commands and calculate the distance to the current command. keep the best.
        for (std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = ConsoleCommand::getCommandsLC().begin(); it_group != ConsoleCommand::getCommandsLC().end(); ++it_group)
        {
            if (it_group->first != "")
            {
                for (std::map<std::string, ConsoleCommand*>::const_iterator it_name = it_group->second.begin(); it_name != it_group->second.end(); ++it_name)
                {
                    std::string command = it_group->first + " " + it_name->first;
                    unsigned int distance = getLevenshteinDistance(command, token0_LC + " " + token1_LC);
                    if (distance < nearestDistance)
                    {
                        nearestCommand = command;
                        nearestDistance = distance;
                    }
                }
            }
        }

        // now also iterate through all shortcuts and keep the best if it's better than the one found above.
        std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = ConsoleCommand::getCommandsLC().find("");
        if (it_group !=  ConsoleCommand::getCommandsLC().end())
        {
            for (std::map<std::string, ConsoleCommand*>::const_iterator it_name = it_group->second.begin(); it_name != it_group->second.end(); ++it_name)
            {
                std::string command = it_name->first;
                unsigned int distance = getLevenshteinDistance(command, token0_LC);
                if (distance < nearestDistance)
                {
                    nearestCommand = command;
                    nearestDistance = distance;
                }
            }
        }

        // return the command that's closest to the current one.
        return nearestCommand;
    }

    /**
        @brief Gets the possible arguments for the command in its current state.
    */
    void CommandEvaluation::retrievePossibleArguments()
    {
        this->bPossibleArgumentsRetrieved_ = true;

        // we use the hintCommand_ to get possible arguments. get the index of the last argument. limit the index if its greater than the number of arguments supported by the command.
        unsigned int argumentID = std::min(this->getNumberOfArguments() - this->hintArgumentsOffset_, this->hintCommand_->getExecutor()->getParamCount());

        // get the argument completer for the given argument index
        ArgumentCompleter* ac = this->hintCommand_->getArgumentCompleter(argumentID - 1);

        // check if an argument completer exists
        if (ac)
        {
            MultiType arg[MAX_FUNCTOR_ARGUMENTS];

            // the index of the last argument in the command string that is supported by this argument completer
            size_t max = this->hintArgumentsOffset_ + this->hintCommand_->getExecutor()->getParamCount();

            // write the argument strings to the argument array (in reversed order, as required by the argument completion function)
            for (size_t i = 0; i < argumentID; ++i)
                arg[i] = this->getToken(std::min(this->getNumberOfArguments(), (unsigned int)max) - i - 1);

            // check if there are more arguments given by the user than supported
            if (this->getNumberOfArguments() > max)
            {
                // yes - now check if multiple words are supported by the argument completer
                if (ac->useMultipleWords())
                {
                    // yes - join the surplus arguments
                    std::string surplusArguments = this->tokens_.subSet(max - 1).join();
                    if (this->string_[this->string_.size() - 1] == ' ')
                        surplusArguments += ' ';

                    // pass all surplus arguments as the first argument to the argument completer
                    this->possibleArguments_ = (*ac)(surplusArguments, arg[1], arg[2], arg[3], arg[4]);

                    // strip the list using the last argument
                    CommandEvaluation::strip(this->possibleArguments_, this->getToken(this->getNumberOfArguments() - 1));
                }
                else
                {
                    // no - the user typed more arguments than supported, no action
                }
            }
            else
            {
                // no - so simply call the argument completer and get the list of arguments
                this->possibleArguments_ = (*ac)(arg[0], arg[1], arg[2], arg[3], arg[4]);

                // strip the list using the last argument (stored arg[0])
                CommandEvaluation::strip(this->possibleArguments_, arg[0]);
            }
        }
    }

    /**
        @brief Returns the size of an argument completion list - empty ("") arguments are not counted.
    */
    /* static */ size_t CommandEvaluation::getSize(const ArgumentCompletionList& list)
    {
        size_t count = 0;
        for (ArgumentCompletionList::const_iterator it = list.begin(); it != list.end(); ++it)
            if (it->getComparable() != "")
                ++count;
        return count;
    }

    /**
        @brief Removes all elements from the list that don't start with @a fragment.
        @param list The argument completion list
        @param fragment The argument that is currently entered by the user and that needs to be completed
    */
    /* static */ void CommandEvaluation::strip(ArgumentCompletionList& list, const std::string& fragment)
    {
        std::string fragmentLC = getLowercase(fragment);

        // iterate through the list
        for (ArgumentCompletionList::iterator it = list.begin(); it != list.end(); )
        {
            const std::string& entry = it->getComparable();

            // check if the argument is empty - if yes, keep it always in the list
            if (entry == "")
            {
                ++it;
                continue;
            }

            // check the length of the argument - arguments smaller than 'fragment' are always erased
            if (entry.size() < fragmentLC.size())
            {
                list.erase(it++);
            }
            else
            {
                // compare the argument char by char with 'fragment'
                bool bErase = false;
                for (size_t i = 0; i < fragmentLC.size(); ++i)
                {
                    if (fragmentLC[i] != entry[i])
                    {
                        bErase = true;
                        break;
                    }
                }

                if (bErase)
                    list.erase(it++);
                else
                    ++it;
            }
        }
    }

    /**
        @brief Returns the commond begin of all arguments in the list.
    */
    /* static */ std::string CommandEvaluation::getCommonBegin(const ArgumentCompletionList& list)
    {
        if (CommandEvaluation::getSize(list) == 0)
        {
            // no (non-empty) values in the list, return an empty string
            return "";
        }
        else if (CommandEvaluation::getSize(list) == 1)
        {
            // only one (non-empty) value in the list - search it and return it
            for (ArgumentCompletionList::const_iterator it = list.begin(); it != list.end(); ++it)
            {
                if (it->getComparable() != "")
                {
                    // arguments that have a separate string to be displayed need a little more care - just return them without modification. add a space character to the others.
                    if (it->hasDisplay())
                        return (it->getString());
                    else
                        return (it->getString() + ' ');
                }
            }

            return "";
        }
        else
        {
            // multiple arguments in the list - iterate through it and find the common begin of all arguments
            std::string output;
            for (unsigned int i = 0; true; i++)
            {
                char tempComparable = '\0';
                char temp = '\0';
                for (ArgumentCompletionList::const_iterator it = list.begin(); it != list.end(); ++it)
                {
                    const std::string& argumentComparable = it->getComparable();
                    const std::string& argument = it->getString();

                    // ignore empty arguments
                    if (argumentComparable == "")
                        continue;

                    if (argument.size() > i)
                    {
                        if (tempComparable == '\0')
                        {
                            // the first entry is always taken
                            tempComparable = argumentComparable[i];
                            temp = argument[i];
                        }
                        else
                        {
                            // all other entries need comparison to the first entry
                            if (tempComparable != argumentComparable[i])
                                return output;
                            else if (temp != argument[i]) // the comparables match, but the normal chars don't - switch to comparable only
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

    /**
        @brief Joins the elements of the given list to a string.
    */
    /* static */ std::string CommandEvaluation::dump(const ArgumentCompletionList& list)
    {
        std::string output;
        for (ArgumentCompletionList::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            output += it->getDisplay();

            // add a space character between two elements for all non-empty arguments
            if (it->getComparable() != "")
                output += ' ';
        }
        return output;
    }

    /**
        @brief Returns a string that explains the syntax of the given command.
    */
    /* static */ std::string CommandEvaluation::dump(const ConsoleCommand* command)
    {
        // get the name of the command
        std::string output = command->getName();

        // check if there are parameters
        if (command->getExecutor()->getParamCount() > 0)
            output += ": ";

        // iterate through the parameters
        for (unsigned int i = 0; i < command->getExecutor()->getParamCount(); i++)
        {
            // separate the parameters with a space character
            if (i != 0)
                output += ' ';

            // print default values in [], others in {} braces
            if (command->getExecutor()->defaultValueSet(i))
                output += '[';
            else
                output += '{';

            // add the type-name of the parameter
            output += command->getExecutor()->getTypenameParam(i);

            // print the default value if available
            if (command->getExecutor()->defaultValueSet(i))
                output += '=' + command->getExecutor()->getDefaultValue(i).getString() + ']';
            else
                output += '}';
        }
        return output;
    }
}
