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

namespace orxonox
{
    CommandEvaluation::CommandEvaluation()
    {
        this->processedCommand_ = "";
        this->additionalParameter_ = "";

        this->functionclass_ = 0;
        this->configvalueclass_ = 0;
        this->shortcut_ = 0;
        this->function_ = 0;
        this->configvalue_ = 0;
        this->key_ = 0;

        this->errorMessage_ = "";
        this->state_ = CS_Uninitialized;

        this->bEvaluatedParams_ = false;
        this->evaluatedExecutor_ = 0;
    }

    KeybindMode CommandEvaluation::getKeybindMode()
    {
        if (this->state_ == CS_Shortcut_Params || this->state_ == CS_Shortcut_Finished)
        {
//            if (this->shortcut_ != 0)
//                return this->shortcut_->getKeybindMode();
        }
        else if (this->state_ == CS_Function_Params || this->state_ == CS_Function_Finished)
        {
//            if (this->function_ != 0)
//                return this->function_->getKeybindMode();
        }
        else if (this->state_ == CS_ConfigValueType || this->state_ == CS_ConfigValueFinished)
        {
//            return KeybindMode::onPress;
        }
        else if (this->state_ == CS_KeybindCommand || this->state_ == CS_KeybindFinished)
        {
//            return KeybindMode::onPress;
        }
        else
        {
//            return KeybindMode::onPress;
        }
        // FIXME: Had to insert a return statement
        return (KeybindMode)0;
    }

    bool CommandEvaluation::isValid() const
    {
        if (this->state_ == CS_Shortcut_Params || this->state_ == CS_Shortcut_Finished)
        {
            return this->shortcut_;
        }
        else if (this->state_ == CS_Function_Params || this->state_ == CS_Function_Finished)
        {
            return (this->functionclass_ && this->function_);
        }
        else if (this->state_ == CS_ConfigValueType || this->state_ == CS_ConfigValueFinished)
        {
            return (this->configvalueclass_ && this->configvalue_);
        }
        else if (this->state_ == CS_KeybindCommand || this->state_ == CS_KeybindFinished)
        {
            return this->key_;
        }
        else
        {
            return false;
        }
    }

    void CommandEvaluation::evaluateParams()
    {
        this->bEvaluatedParams_ = false;
        this->evaluatedExecutor_ = 0;

        for (unsigned int i = 0; i < MAX_FUNCTOR_ARGUMENTS; i++)
            this->param_[i] = MT_null;

        if (this->state_ == CS_Shortcut_Params || this->state_ == CS_Shortcut_Finished)
        {
            if (this->shortcut_)
            {
                if (this->tokens_.size() <= 1)
                {
                    if (this->shortcut_->evaluate(this->getAdditionalParameter(), this->param_, " "))
                    {
                        this->bEvaluatedParams_ = true;
                        this->evaluatedExecutor_ = this->shortcut_;
                    }
                }
                else if (this->tokens_.size() > 1)
                {
                    if (this->shortcut_->evaluate(this->tokens_.subSet(1).join() + this->getAdditionalParameter(), this->param_, " "))
                    {
                        this->bEvaluatedParams_ = true;
                        this->evaluatedExecutor_ = this->shortcut_;
                    }
                }
            }
        }
        else if (this->state_ == CS_Function_Params || this->state_ == CS_Function_Finished)
        {
            if (this->function_)
            {
                if (this->tokens_.size() <= 2)
                {
                    if (this->function_->evaluate(this->getAdditionalParameter(), this->param_, " "))
                    {
                        this->bEvaluatedParams_ = true;
                        this->evaluatedExecutor_ = this->function_;
                    }
                }
                else if (this->tokens_.size() > 2)
                {
                    if (this->function_->evaluate(this->tokens_.subSet(2).join() + this->getAdditionalParameter(), this->param_, " "))
                    {
                        this->bEvaluatedParams_ = true;
                        this->evaluatedExecutor_ = this->function_;
                    }
                }
            }
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
        if (this->state_ == CS_Shortcut_Params || this->state_ == CS_Shortcut_Finished)
        {
            if (this->shortcut_)
                return this->shortcut_->hasReturnvalue();
        }
        else if (this->state_ == CS_Function_Params || this->state_ == CS_Function_Finished)
        {
            if (this->function_)
                return this->function_->hasReturnvalue();
        }

        return MT_null;
    }

    MultiTypeMath CommandEvaluation::getReturnvalue() const
    {
        if (this->state_ == CS_Shortcut_Params || this->state_ == CS_Shortcut_Finished)
        {
            if (this->shortcut_)
                return this->shortcut_->getReturnvalue();
        }
        else if (this->state_ == CS_Function_Params || this->state_ == CS_Function_Finished)
        {
            if (this->function_)
                return this->function_->getReturnvalue();
        }

        return MT_null;
    }
}
