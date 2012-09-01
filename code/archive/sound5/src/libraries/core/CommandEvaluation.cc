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

#include "util/Debug.h"
#include "util/StringUtils.h"
#include "ConsoleCommand.h"
#include "Identifier.h"

namespace orxonox
{
    CommandEvaluation::CommandEvaluation()
    {
        this->initialize("");
        this->state_ = CommandState::Uninitialized;
    }

    void CommandEvaluation::initialize(const std::string& command)
    {
        this->bNewCommand_ = true;
        this->bCommandChanged_ = false;
        this->originalCommand_ = command;
        this->command_ = command;
        this->commandTokens_.split(command, " ", SubString::WhiteSpaces, false, '\\', false, '"', false, '(', ')', false, '\0');

        this->additionalParameter_.clear();

        this->bEvaluatedParams_ = false;

        this->listOfPossibleIdentifiers_.clear();
        this->listOfPossibleFunctions_.clear();
        this->listOfPossibleArguments_.clear();

        this->functionclass_ = 0;
        this->function_ = 0;
        this->possibleArgument_.clear();
        this->argument_.clear();

        this->errorMessage_.clear();
        this->state_ = CommandState::Empty;
    }

    bool CommandEvaluation::isValid() const
    {
        return (this->function_);
    }

    bool CommandEvaluation::execute() const
    {
        if (!this->isValid())
            return false;

        if (this->bEvaluatedParams_ && this->function_)
        {
            COUT(6) << "CE_execute (evaluation): " << this->function_->getName() << ' ' << this->param_[0] << ' ' << this->param_[1] << ' ' << this->param_[2] << ' ' << this->param_[3] << ' ' << this->param_[4] << std::endl;
            (*this->function_)(this->param_[0], this->param_[1], this->param_[2], this->param_[3], this->param_[4]);
            return true;
        }

        if (!this->bCommandChanged_ || nocaseCmp(removeTrailingWhitespaces(this->command_), removeTrailingWhitespaces(this->originalCommand_)) == 0)
        {
            COUT(4) << "CE_execute: " << this->command_ << "\n";

            unsigned int startindex = this->getStartindex();
            if (this->commandTokens_.size() > startindex)
                return this->function_->parse(removeSlashes(this->commandTokens_.subSet(startindex).join() + this->getAdditionalParameter()));
            else
                return this->function_->parse(removeSlashes(this->additionalParameter_));
        }

        return false;
    }

    const std::string& CommandEvaluation::complete()
    {
        if (!this->bNewCommand_)
        {
            switch (this->state_)
            {
                case CommandState::Uninitialized:
                    break;
                case CommandState::Empty:
                    break;
                case CommandState::ShortcutOrIdentifier:
                    if (this->function_)
                    {
                        if (this->function_->getParamCount() == 0)
                            return (this->command_ = this->function_->getName());
                        else
                            return (this->command_ = this->function_->getName() + ' ');
                    }
                    else if (this->functionclass_)
                        return (this->command_ = this->functionclass_->getName() + ' ');
                    break;
                case CommandState::Function:
                    if (this->function_)
                    {
                        if (this->function_->getParamCount() == 0)
                            return (this->command_ = this->functionclass_->getName() + ' ' + this->function_->getName());
                        else
                            return (this->command_ = this->functionclass_->getName() + ' ' + this->function_->getName() + ' ');
                    }
                    break;
                case CommandState::ParamPreparation:
                case CommandState::Params:
                {
                    if (this->argument_.empty() && this->possibleArgument_.empty())
                        break;

                    unsigned int maxIndex = this->commandTokens_.size();
                    if (this->command_[this->command_.size() - 1] != ' ')
                        maxIndex -= 1;
                    std::string whitespace;

                    if (!this->possibleArgument_.empty())
                    {
                        this->argument_ = this->possibleArgument_;
                        if (this->function_->getParamCount() > (maxIndex + 1 - this->getStartindex()))
                            whitespace = " ";
                    }

                    return (this->command_ = this->commandTokens_.subSet(0, maxIndex).join() + ' ' + this->argument_ + whitespace);
                    break;
                }
                case CommandState::Finished:
                    break;
                case CommandState::Error:
                    break;
            }
        }
        this->bNewCommand_ = false;
        return this->command_;
    }

    std::string CommandEvaluation::hint() const
    {
        switch (this->state_)
        {
            case CommandState::Uninitialized:
                break;
            case CommandState::Empty:
            case CommandState::ShortcutOrIdentifier:
                if (this->listOfPossibleFunctions_.size() == 0)
                    return CommandEvaluation::dump(this->listOfPossibleIdentifiers_);
                else if (this->listOfPossibleIdentifiers_.size() == 0)
                    return CommandEvaluation::dump(this->listOfPossibleFunctions_);
                else
                    return (CommandEvaluation::dump(this->listOfPossibleFunctions_) + "\n" + CommandEvaluation::dump(this->listOfPossibleIdentifiers_));
                break;
            case CommandState::Function:
                return CommandEvaluation::dump(this->listOfPossibleFunctions_);
                break;
            case CommandState::ParamPreparation:
            case CommandState::Params:
                if (this->listOfPossibleArguments_.size() > 0)
                    return CommandEvaluation::dump(this->listOfPossibleArguments_);
                else
                    return CommandEvaluation::dump(this->function_);
            case CommandState::Finished:
                if (this->function_)
                    return CommandEvaluation::dump(this->function_);
                break;
            case CommandState::Error:
                return this->errorMessage_;
                break;
        }

        return "";
    }

    void CommandEvaluation::evaluateParams()
    {
        this->bEvaluatedParams_ = false;

        for (unsigned int i = 0; i < MAX_FUNCTOR_ARGUMENTS; i++)
            this->param_[i] = MT_Type::Null;

        if (!this->isValid())
            return;

        unsigned int startindex = this->getStartindex();

        if (this->commandTokens_.size() <= startindex)
        {
            if (this->function_->evaluate(this->getAdditionalParameter(), this->param_, " "))
                this->bEvaluatedParams_ = true;
        }
        else if (this->commandTokens_.size() > startindex)
        {
            if (this->function_->evaluate(this->commandTokens_.subSet(startindex).join() + this->getAdditionalParameter(), this->param_, " "))
                this->bEvaluatedParams_ = true;
        }
    }

    void CommandEvaluation::setEvaluatedParameter(unsigned int index, MultiType param)
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

    bool CommandEvaluation::hasReturnvalue() const
    {
        if (this->function_)
            return this->function_->hasReturnvalue();

        return MT_Type::Null;
    }

    MultiType CommandEvaluation::getReturnvalue() const
    {
        if (this->function_)
            return this->function_->getReturnvalue();

        return MultiType();
    }


    unsigned int CommandEvaluation::getStartindex() const
    {
        if (this->functionclass_ && this->function_)
            return 2;
        else if (this->function_)
            return 1;
        else
            return 0;
    }

    std::string CommandEvaluation::dump(const std::list<std::pair<const std::string*, const std::string*> >& list)
    {
        std::string output;
        for (std::list<std::pair<const std::string*, const std::string*> >::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            if (it != list.begin())
                output += ' ';

            output += *(it->second);
        }
        return output;
    }

    std::string CommandEvaluation::dump(const ArgumentCompletionList& list)
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

    std::string CommandEvaluation::dump(const ConsoleCommand* command)
    {
        std::string output = command->getName();
        if (command->getParamCount() > 0)
            output += ": ";

        for (unsigned int i = 0; i < command->getParamCount(); i++)
        {
            if (i != 0)
                output += ' ';

            if (command->defaultValueSet(i))
                output += '[';
            else
                output += '{';

            output += command->getTypenameParam(i);

            if (command->defaultValueSet(i))
                output += '=' + command->getDefaultValue(i).getString() + ']';
            else
                output += '}';
        }
        return output;
    }
}
