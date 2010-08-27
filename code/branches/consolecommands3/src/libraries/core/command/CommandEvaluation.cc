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

        this->tokens_.split(command, " ", SubString::WhiteSpaces, false, '\\', false, '"', false, '(', ')', false, '\0');
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

    int CommandEvaluation::execute() const
    {
        int error;
        this->query(&error);
        return error;
    }

    MultiType CommandEvaluation::query(int* error) const
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
            return this->execCommand_->getExecutor()->parse(this->tokens_.subSet(this->execArgumentsOffset_).join(), error, " ", false);
        else
            return MT_Type::Null;
    }

    std::string CommandEvaluation::complete() const
    {
        if (!this->hintCommand_ || !this->hintCommand_->isActive())
            return this->string_;

        if (!this->bPossibleArgumentsRetrieved_)
            this->retrievePossibleArguments();

        if (this->possibleArguments_.empty())
        {
            return this->string_;
        }
        else
        {
            std::string output;
            for (unsigned int i = 0; i < this->getNumberOfArguments() - 1; ++i)
                output += this->getToken(i) + ' ';

            output += CommandEvaluation::getCommonBegin(this->possibleArguments_);
            return output;
        }
    }

    std::string CommandEvaluation::hint() const
    {
        if (!this->hintCommand_ || !this->hintCommand_->isActive())
            return "";

        if (!this->bPossibleArgumentsRetrieved_)
            this->retrievePossibleArguments();

        if (!this->possibleArguments_.empty())
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
                std::map<std::string, std::map<std::string, _ConsoleCommand*> >::const_iterator it_group = _ConsoleCommand::getCommands().begin();
                for ( ; it_group != _ConsoleCommand::getCommands().end(); ++it_group)
                    if (getLowercase(it_group->first) == groupLC)
                        return std::string("Error: There is no command in group \"") + this->getToken(0) + "\" starting with \"" + this->getToken(1) + "\".";

                return std::string("Error: There is no command starting with \"") + this->getToken(0) + "\".";
            }
        }
    }

    void CommandEvaluation::retrievePossibleArguments() const
    {
        this->bPossibleArgumentsRetrieved_ = true;
        unsigned int argumentID = std::min(this->getNumberOfArguments() - this->hintArgumentsOffset_, this->hintCommand_->getExecutor()->getParamCount());
        ArgumentCompleter* ac = this->hintCommand_->getArgumentCompleter(argumentID - 1);

COUT(0) << "hint: args: " << this->getNumberOfArguments() << ", aID: " << argumentID << ", offset: " << this->hintArgumentsOffset_ << ", ac: " << ac << std::endl;
        if (ac)
        {
            MultiType param[MAX_FUNCTOR_ARGUMENTS];

            for (size_t i = 0; i < argumentID; ++i)
            {
                param[i] = this->getToken(this->getNumberOfArguments() - i - 1);
COUT(0) << i << ": " << (this->getNumberOfArguments() - i - 1) << " -> " << this->getToken(this->getNumberOfArguments() - i - 1) << " / " << param[i] << std::endl;
            }

COUT(0) << "hint: 1: " << param[0] << ", 2: " << param[1] << ", 3: " << param[2] << ", 4: " << param[3] << ", 5: " << param[4] << std::endl;
            this->possibleArguments_ = (*ac)(param[0], param[1], param[2], param[3], param[4]);

            CommandEvaluation::strip(this->possibleArguments_, param[0]);
        }
    }

    /* static */ void CommandEvaluation::strip(ArgumentCompletionList& list, const std::string& fragment)
    {
        std::string fragmentLC = getLowercase(fragment);

        for (ArgumentCompletionList::iterator it = list.begin(); it != list.end(); )
        {
            const std::string& entry = it->getComparable();

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

    /* static */ std::string CommandEvaluation::dump(const ArgumentCompletionList& list)
    {
        std::string output;
        for (ArgumentCompletionList::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            if (it != list.begin())
                output += ' ';

            output += it->getDisplay();
        }
        return output;
    }

    /* static */ std::string CommandEvaluation::dump(const _ConsoleCommand* command)
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

    /* static */ std::string CommandEvaluation::getCommonBegin(const ArgumentCompletionList& list)
    {
        if (list.size() == 0)
        {
            return "";
        }
        else if (list.size() == 1)
        {
            if (list.begin()->hasDisplay())
                return (list.begin()->getString());
            else
                return (list.begin()->getString() + ' ');
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
}
