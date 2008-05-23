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
#include "ConsoleCommand.h"
#include "Debug.h"

namespace orxonox
{
    CommandEvaluation::CommandEvaluation()
    {
        this->initialize("");
        this->state_ = CS_Uninitialized;
    }

    void CommandEvaluation::initialize(const std::string& command)
    {
        this->bNewCommand_ = true;

        this->originalCommand_ = command;
        this->command_ = command;
        this->originalCommandTokens_.split(command, " ", SubString::WhiteSpaces, false, '\\', false, '"', false, '(', ')', false, '\0');
        this->commandTokens_.split(command, " ", SubString::WhiteSpaces, false, '\\', false, '"', false, '(', ')', false, '\0');

        this->additionalParameter_ = "";

        this->bEvaluatedParams_ = false;

        this->listOfPossibleIdentifiers_.clear();
        this->listOfPossibleFunctions_.clear();

        this->functionclass_ = 0;
        this->function_ = 0;

        this->errorMessage_ = "";
        this->state_ = CS_Empty;
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
            COUT(4) << "CE_execute (evaluation): " << this->function_->getName() << " " << this->param_[0] << " " << this->param_[1] << " " << this->param_[2] << " " << this->param_[3] << " " << this->param_[4] << std::endl;
            (*this->function_)(this->param_[0], this->param_[1], this->param_[2], this->param_[3], this->param_[4]);
            return true;
        }

        COUT(4) << "CE_execute: " << this->originalCommand_ << "\n";

        unsigned int startindex = this->getStartindex();
        if (this->originalCommandTokens_.size() > startindex)
            return this->function_->parse(removeSlashes(this->originalCommandTokens_.subSet(startindex).join() + this->getAdditionalParameter()));
        else
            return this->function_->parse(removeSlashes(this->additionalParameter_));
    }

    std::string CommandEvaluation::complete() const
    {
        switch (this->state_)
        {
            case CS_Uninitialized:
            case CS_Empty:
            case CS_ShortcutOrIdentifier:
                {
                    std::list<std::pair<const std::string*, const std::string*> > temp;
                    if (this->state_ == CS_Empty)
                    {
                        temp.insert(temp.end(), this->listOfPossibleFunctions_.begin(), this->listOfPossibleFunctions_.end());
                        temp.insert(temp.end(), this->listOfPossibleIdentifiers_.begin(), this->listOfPossibleIdentifiers_.end());
                    }
                    return (CommandEvaluation::getCommonBegin(temp));
                }
                break;
            case CS_Shortcut_Params:
                if (this->function_)
                {
                    if (this->commandTokens_.size() > 1)
                        return (this->function_->getName() + " " + this->commandTokens_.subSet(1, this->commandTokens_.size() - 1).join() + " " + CommandEvaluation::getCommonBegin(this->listOfPossibleArguments_));
                    else
                        return (this->function_->getName() + " ");
                }
                break;
            case CS_Shortcut_Finished:
                if (this->function_)
                {
                    if (this->commandTokens_.size() > 1)
                        return (this->function_->getName() + " " + this->originalCommandTokens_.subSet(1, this->originalCommandTokens_.size() - 1).join());
                    else
                        return (this->function_->getName());
                }
                break;
            case CS_Function:
                if (this->functionclass_)
                    return (this->functionclass_->getName() + " " + CommandEvaluation::getCommonBegin(this->listOfPossibleFunctions_));
                break;
            case CS_Function_Params:
                if (this->functionclass_ && this->function_)
                {
                    if (this->commandTokens_.size() > 2)
                        return (this->functionclass_->getName() + " " + this->function_->getName() + " " + this->commandTokens_.subSet(2, this->commandTokens_.size() - 1).join() + " " + CommandEvaluation::getCommonBegin(this->listOfPossibleArguments_));
                    else
                        return (this->functionclass_->getName() + " " + this->function_->getName() + " ");
                }
                break;
            case CS_Function_Finished:
                if (this->functionclass_ && this->function_)
                {
                    if (this->commandTokens_.size() > 2)
                        return (this->functionclass_->getName() + " " + this->function_->getName() + " " + this->originalCommandTokens_.subSet(2, this->originalCommandTokens_.size() - 1).join());
                    else
                        return (this->functionclass_->getName() + " " + this->function_->getName());
                }
               break;
            case CS_Error:
                break;
        }

        return this->originalCommand_;
    }

    std::string CommandEvaluation::hint() const
    {
        switch (this->state_)
        {
            case CS_Uninitialized:
                break;
            case CS_Empty:
            case CS_ShortcutOrIdentifier:
                return (CommandEvaluation::dump(this->listOfPossibleFunctions_) + "\n" + CommandEvaluation::dump(this->listOfPossibleIdentifiers_));
                break;
            case CS_Function:
                return CommandEvaluation::dump(this->listOfPossibleFunctions_);
                break;
            case CS_Shortcut_Params:
            case CS_Function_Params:
                return CommandEvaluation::dump(this->listOfPossibleArguments_);
            case CS_Shortcut_Finished:
            case CS_Function_Finished:
                if (this->function_)
                    return CommandEvaluation::dump(this->function_);
                break;
            case CS_Error:
                return this->errorMessage_;
                break;
        }

        return "";
    }

    void CommandEvaluation::evaluateParams()
    {
        this->bEvaluatedParams_ = false;

        for (unsigned int i = 0; i < MAX_FUNCTOR_ARGUMENTS; i++)
            this->param_[i] = MT_null;

        if (!this->isValid())
            return;

        unsigned int startindex = this->getStartindex();

        if (this->originalCommandTokens_.size() <= startindex)
        {
            if (this->function_->evaluate(this->getAdditionalParameter(), this->param_, " "))
                this->bEvaluatedParams_ = true;
        }
        else if (this->originalCommandTokens_.size() > startindex)
        {
            if (this->function_->evaluate(this->originalCommandTokens_.subSet(startindex).join() + this->getAdditionalParameter(), this->param_, " "))
                this->bEvaluatedParams_ = true;
        }
    }

    void CommandEvaluation::setEvaluatedParameter(unsigned int index, MultiTypeMath param)
    {
        if (index >= 0 && index < MAX_FUNCTOR_ARGUMENTS)
            this->param_[index] = param;
    }

    MultiTypeMath CommandEvaluation::getEvaluatedParameter(unsigned int index) const
    {
        if (index >= 0 && index < MAX_FUNCTOR_ARGUMENTS)
            return this->param_[index];

        return MT_null;
    }

    bool CommandEvaluation::hasReturnvalue() const
    {
        if (this->function_)
            return this->function_->hasReturnvalue();

        return MT_null;
    }

    MultiTypeMath CommandEvaluation::getReturnvalue() const
    {
        if (this->function_)
            return this->function_->getReturnvalue();

        return MultiTypeMath();
    }


    unsigned int CommandEvaluation::getStartindex() const
    {
        if (this->state_ == CS_Shortcut_Params || this->state_ == CS_Shortcut_Finished)
            return 1;
        else if (this->state_ == CS_Function || this->state_ == CS_Function_Params || this->state_ == CS_Function_Finished)
            return 2;
        else
            return 0;
    }

    std::string CommandEvaluation::getCommonBegin(const std::list<std::pair<const std::string*, const std::string*> >& list)
    {
        if (list.size() == 0)
        {
            return "";
        }
        else if (list.size() == 1)
        {
            return ((*(*list.begin()).first) + " ");
        }
        else
        {
            std::string output = "";
            for (unsigned int i = 0; true; i++)
            {
                char temp = 0;
                for (std::list<std::pair<const std::string*, const std::string*> >::const_iterator it = list.begin(); it != list.end(); ++it)
                {
                    if ((*(*it).first).size() > i)
                    {
                        if (it == list.begin())
                        {
                            temp = (*(*it).first)[i];
                        }
                        else
                        {
                            if (temp != (*(*it).first)[i])
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

    std::string CommandEvaluation::dump(const std::list<std::pair<const std::string*, const std::string*> >& list)
    {
        std::string output = "";
        for (std::list<std::pair<const std::string*, const std::string*> >::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            if (it != list.begin())
                output += " ";

            output += *(*it).second;
        }
        return output;
    }

    std::string CommandEvaluation::dump(const ConsoleCommand* command)
    {
        std::string output = "";
        for (unsigned int i = 0; i < command->getParamCount(); i++)
        {
            if (i != 0)
                output += " ";

            if (command->defaultValueSet(i))
                output += "[";
            else
                output += "{";

            output += command->getTypenameParam(i);

            if (command->defaultValueSet(i))
                output += "=" + command->getDefaultValue(i).toString() + "]";
            else
                output += "}";
        }
        return output;
    }
}
