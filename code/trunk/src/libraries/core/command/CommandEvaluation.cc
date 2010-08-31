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

#include "CommandEvaluation.h"

#include "util/StringUtils.h"
#include "CommandExecutor.h"
#include "ConsoleCommand.h"

namespace orxonox
{
    CommandEvaluation::CommandEvaluation()
    {
        this->initialize("");
    }

    void CommandEvaluation::initialize(const std::string& command)
    {
        this->execCommand_ = 0;
        this->hintCommand_ = 0;
        this->string_ = command;
        this->execArgumentsOffset_ = 0;
        this->hintArgumentsOffset_ = 0;
        this->bPossibleArgumentsRetrieved_ = false;
        this->possibleArguments_.clear();
        this->bEvaluatedParams_ = false;
        this->bTriedToEvaluatedParams_ = false;
        this->numberOfEvaluatedParams_ = 0;

        this->tokens_.split(command, " ", SubString::WhiteSpaces, false, '\\', true, '"', true, '{', '}', true, '\0');
    }

    unsigned int CommandEvaluation::getNumberOfArguments() const
    {
        unsigned int count = this->tokens_.size();
        if (count > 0 && this->string_[this->string_.size() - 1] != ' ')
            return count;
        else
            return count + 1;
    }

    const std::string& CommandEvaluation::getLastArgument() const
    {
        if (this->tokens_.size() > 0 && this->string_[this->string_.size() - 1] != ' ')
            return this->tokens_.back();
        else
            return BLANKSTRING;
    }

    const std::string& CommandEvaluation::getToken(unsigned int i) const
    {
        if (i < this->tokens_.size())
            return this->tokens_[i];
        else
            return BLANKSTRING;
    }

    int CommandEvaluation::execute()
    {
        int error;
        this->query(&error);
        return error;
    }

    MultiType CommandEvaluation::query(int* error)
    {
        if (error)
        {
            *error = CommandExecutor::Success;

            if (!this->execCommand_)
                *error = CommandExecutor::Error;
            else if (!this->execCommand_->isActive())
                *error = CommandExecutor::Deactivated;
            else if (!this->execCommand_->hasAccess())
                *error = CommandExecutor::Denied;

            if (*error != CommandExecutor::Success)
                return MT_Type::Null;
        }

        if (this->execCommand_ && this->execCommand_->isActive() && this->execCommand_->hasAccess())
        {
            if (!this->bTriedToEvaluatedParams_)
                this->evaluateParams(false);

            if (this->bEvaluatedParams_)
            {
                COUT(6) << "CE_execute (evaluation): " << this->execCommand_->getName() << " with " << this->numberOfEvaluatedParams_ << " params: " << this->param_[0] << ' ' << this->param_[1] << ' ' << this->param_[2] << ' ' << this->param_[3] << ' ' << this->param_[4] << std::endl;
                switch (this->numberOfEvaluatedParams_)
                {
                    case 0:  return (*this->execCommand_->getExecutor())();
                    case 1:  return (*this->execCommand_->getExecutor())(this->param_[0]);
                    case 2:  return (*this->execCommand_->getExecutor())(this->param_[0], this->param_[1]);
                    case 3:  return (*this->execCommand_->getExecutor())(this->param_[0], this->param_[1], this->param_[2]);
                    case 4:  return (*this->execCommand_->getExecutor())(this->param_[0], this->param_[1], this->param_[2], this->param_[3]);
                    case 5:
                    default: return (*this->execCommand_->getExecutor())(this->param_[0], this->param_[1], this->param_[2], this->param_[3], this->param_[4]);
                }
            }
            else if (error)
                *error = CommandExecutor::Incomplete;
        }

        return MT_Type::Null;
    }

    int CommandEvaluation::evaluateParams(bool bPrintError)
    {
        this->bTriedToEvaluatedParams_ = true;

        if (!this->execCommand_)
        {
            if (bPrintError)
                COUT(1) << "Error: Can't evaluate params, no console command assigned." << std::endl;
            return CommandExecutor::Error;
        }

        int error;
        this->numberOfEvaluatedParams_ = this->execCommand_->getExecutor()->evaluateParams(this->tokens_.subSet(this->execArgumentsOffset_), this->param_, &error, " ");
        if (!error)
            this->bEvaluatedParams_ = true;
        else if (bPrintError)
            COUT(1) << "Error: Can't evaluate params, not enough arguments given." << std::endl;

        return error;
    }

    void CommandEvaluation::setEvaluatedParameter(unsigned int index, const MultiType& param)
    {
        if (index < MAX_FUNCTOR_ARGUMENTS)
            this->param_[index] = param;
    }

    MultiType CommandEvaluation::getEvaluatedParameter(unsigned int index) const
    {
        if (index < MAX_FUNCTOR_ARGUMENTS)
            return this->param_[index];

        return MT_Type::Null;
    }

    std::string CommandEvaluation::complete()
    {
        if (!this->hintCommand_ || !this->hintCommand_->isActive())
            return this->string_;

        if (!this->bPossibleArgumentsRetrieved_)
            this->retrievePossibleArguments();

        if (CommandEvaluation::getSize(this->possibleArguments_) == 0)
        {
            return this->string_;
        }
        else
        {
            std::string output = this->string_.substr(0, this->string_.find_last_of(' ') + 1);
            output += CommandEvaluation::getCommonBegin(this->possibleArguments_);
            return output;
        }
    }

    std::string CommandEvaluation::hint()
    {
        if (!this->hintCommand_ || !this->hintCommand_->isActive())
            return "";

        if (!this->bPossibleArgumentsRetrieved_)
            this->retrievePossibleArguments();

        if (CommandEvaluation::getSize(this->possibleArguments_) > 0 || (!this->possibleArguments_.empty() && this->isValid()))
            return CommandEvaluation::dump(this->possibleArguments_);

        if (this->isValid())
        {
            return CommandEvaluation::dump(this->hintCommand_);
        }
        else
        {
            if (this->getNumberOfArguments() > 2)
            {
                return std::string("Error: There is no command with name \"") + this->getToken(0) + " " + this->getToken(1) + "\".";
            }
            else
            {
                std::string groupLC = getLowercase(this->getToken(0));
                for (std::map<std::string, std::map<std::string, ConsoleCommand*> >::const_iterator it_group = ConsoleCommand::getCommandsLC().begin(); it_group != ConsoleCommand::getCommandsLC().end(); ++it_group)
                    if (it_group->first == groupLC)
                        return std::string("Error: There is no command in group \"") + this->getToken(0) + "\" starting with \"" + this->getToken(1) + "\".";

                return std::string("Error: There is no command starting with \"") + this->getToken(0) + "\".";
            }
        }
    }

    std::string CommandEvaluation::getCommandSuggestion() const
    {
        std::string token0_LC = getLowercase(this->getToken(0));
        std::string token1_LC = getLowercase(this->getToken(1));

        std::string nearestCommand;
        unsigned int nearestDistance = (unsigned int)-1;

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

        return nearestCommand;
    }

    void CommandEvaluation::retrievePossibleArguments()
    {
        this->bPossibleArgumentsRetrieved_ = true;
        unsigned int argumentID = std::min(this->getNumberOfArguments() - this->hintArgumentsOffset_, this->hintCommand_->getExecutor()->getParamCount());
        ArgumentCompleter* ac = this->hintCommand_->getArgumentCompleter(argumentID - 1);

        if (ac)
        {
            MultiType param[MAX_FUNCTOR_ARGUMENTS];

            size_t max = this->hintArgumentsOffset_ + this->hintCommand_->getExecutor()->getParamCount();

            for (size_t i = 0; i < argumentID; ++i)
                param[i] = this->getToken(std::min(this->getNumberOfArguments(), max) - i - 1);

            if (this->getNumberOfArguments() > max)
            {
                if (ac->useMultipleWords())
                {
                    std::string surplusArguments = this->tokens_.subSet(max - 1).join();
                    if (this->string_[this->string_.size() - 1] == ' ')
                        surplusArguments += ' ';

                    this->possibleArguments_ = (*ac)(surplusArguments, param[1], param[2], param[3], param[4]);
                    CommandEvaluation::strip(this->possibleArguments_, this->getToken(this->getNumberOfArguments() - 1));
                }
            }
            else
            {
                this->possibleArguments_ = (*ac)(param[0], param[1], param[2], param[3], param[4]);
                CommandEvaluation::strip(this->possibleArguments_, param[0]);
            }
        }
    }

    /* static */ size_t CommandEvaluation::getSize(const ArgumentCompletionList& list)
    {
        size_t count = 0;
        for (ArgumentCompletionList::const_iterator it = list.begin(); it != list.end(); ++it)
            if (it->getComparable() != "")
                ++count;
        return count;
    }

    /* static */ void CommandEvaluation::strip(ArgumentCompletionList& list, const std::string& fragment)
    {
        std::string fragmentLC = getLowercase(fragment);

        for (ArgumentCompletionList::iterator it = list.begin(); it != list.end(); )
        {
            const std::string& entry = it->getComparable();

            if (entry == "")
            {
                ++it;
                continue;
            }

            if (entry.size() < fragmentLC.size())
            {
                list.erase(it++);
            }
            else
            {
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

    /* static */ std::string CommandEvaluation::getCommonBegin(const ArgumentCompletionList& list)
    {
        if (CommandEvaluation::getSize(list) == 0)
        {
            return "";
        }
        else if (CommandEvaluation::getSize(list) == 1)
        {
            for (ArgumentCompletionList::const_iterator it = list.begin(); it != list.end(); ++it)
            {
                if (it->getComparable() != "")
                {
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
            std::string output;
            for (unsigned int i = 0; true; i++)
            {
                char tempComparable = 0;
                char temp = 0;
                for (ArgumentCompletionList::const_iterator it = list.begin(); it != list.end(); ++it)
                {
                    const std::string& argumentComparable = it->getComparable();
                    const std::string& argument = it->getString();

                    if (argumentComparable == "")
                        continue;

                    if (argument.size() > i)
                    {
                        if (tempComparable == 0)
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

    /* static */ std::string CommandEvaluation::dump(const ArgumentCompletionList& list)
    {
        std::string output;
        for (ArgumentCompletionList::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            output += it->getDisplay();

            if (it->getComparable() != "")
                output += ' ';
        }
        return output;
    }

    /* static */ std::string CommandEvaluation::dump(const ConsoleCommand* command)
    {
        std::string output = command->getName();
        if (command->getExecutor()->getParamCount() > 0)
            output += ": ";

        for (unsigned int i = 0; i < command->getExecutor()->getParamCount(); i++)
        {
            if (i != 0)
                output += ' ';

            if (command->getExecutor()->defaultValueSet(i))
                output += '[';
            else
                output += '{';

            output += command->getExecutor()->getTypenameParam(i);

            if (command->getExecutor()->defaultValueSet(i))
                output += '=' + command->getExecutor()->getDefaultValue(i).getString() + ']';
            else
                output += '}';
        }
        return output;
    }
}
