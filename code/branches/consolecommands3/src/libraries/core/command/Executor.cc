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
 *   Inspiration: Executor by Benjamin Grauer
 */

#include "Executor.h"

#include <algorithm>

#include "util/Convert.h"
#include "util/Debug.h"
#include "util/StringUtils.h"
#include "util/SubString.h"
#include "CommandExecutor.h"

namespace orxonox
{
    Executor::Executor(const FunctorPtr& functor, const std::string& name)
    {
        this->functor_ = functor;
        this->name_ = name;
    }

    Executor::Executor(const Executor& other) : name_(other.name_), defaultValue_(other.defaultValue_)
    {
        this->functor_ = other.functor_->clone();
    }

    Executor::~Executor()
    {
    }

    MultiType Executor::parse(const std::string& arguments, int* error, const std::string& delimiter, bool bPrintError) const
    {
        return this->parse(SubString(arguments, delimiter, SubString::WhiteSpaces, false, '\\', true, '"', true, '(', ')', true, '\0'), error, delimiter, bPrintError);
    }

    MultiType Executor::parse(const SubString& arguments, int* error, const std::string& delimiter, bool bPrintError) const
    {
        MultiType param[MAX_FUNCTOR_ARGUMENTS];
        unsigned int paramCount = this->evaluateParams(arguments, param, error, delimiter);

        if (error && *error)
        {
            if (bPrintError)
                COUT(2) << "Warning: Can't call executor " << this->name_ << " through parser: Not enough parameters or default values given (input: " << arguments.join() << ")." << std::endl;
            return MT_Type::Null;
        }

        COUT(5) << "Executor::parse: \"" << arguments.join(delimiter) << "\" -> " << paramCount << " params: " << param[0] << " / " << param[1] << " / " << param[2] << " / " << param[3] << " / " << param[4] << std::endl;

        switch (paramCount)
        {
            case 0:  return (*this->functor_)();
            case 1:  return (*this->functor_)(param[0]);
            case 2:  return (*this->functor_)(param[0], param[1]);
            case 3:  return (*this->functor_)(param[0], param[1], param[2]);
            case 4:  return (*this->functor_)(param[0], param[1], param[2], param[3]);
            case 5:
            default: return (*this->functor_)(param[0], param[1], param[2], param[3], param[4]);
        }
    }

    int Executor::evaluateParams(const SubString& arguments, MultiType param[MAX_FUNCTOR_ARGUMENTS], int* error, const std::string& delimiter) const
    {
        unsigned int paramCount = this->functor_->getParamCount();
        unsigned int argumentCount = arguments.size();

        // if there are not enough params given, check if there are default values
        for (unsigned int i = argumentCount; i < paramCount; i++)
        {
            if (this->defaultValue_[i].null())
            {
                if (error)
                    *error = CommandExecutor::Incomplete;
                return 0;
            }
        }

        // assign all given arguments to the multitypes
        for (unsigned int i = 0; i < std::min(std::min(argumentCount, paramCount), MAX_FUNCTOR_ARGUMENTS); i++)
            param[i] = arguments[i];

        // fill the remaining multitypes with default values
        for (unsigned int i = argumentCount; i < std::min(paramCount, MAX_FUNCTOR_ARGUMENTS); i++)
            param[i] = this->defaultValue_[i];

        // assign the remaining arguments all to the last parameter if it is a string
        if ((paramCount <= MAX_FUNCTOR_ARGUMENTS) &&(argumentCount > paramCount) && (paramCount == 1 || this->functor_->getTypenameParam(paramCount - 1) == "string"))
            param[paramCount - 1] = arguments.subSet(paramCount - 1).join(delimiter);

        // evaluate the param types through the functor
        for (unsigned int i = 0; i < std::min(paramCount, MAX_FUNCTOR_ARGUMENTS); i++)
            this->functor_->evaluateParam(i, param[i]);

        if (error)
            *error = CommandExecutor::Success;
        return paramCount;
    }

    void Executor::setDefaultValues(const MultiType& param1)
    {
        this->defaultValue_[0] = param1;
    }

    void Executor::setDefaultValues(const MultiType& param1, const MultiType& param2)
    {
        this->defaultValue_[0] = param1;
        this->defaultValue_[1] = param2;
    }

    void Executor::setDefaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3)
    {
        this->defaultValue_[0] = param1;
        this->defaultValue_[1] = param2;
        this->defaultValue_[2] = param3;
    }

    void Executor::setDefaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4)
    {
        this->defaultValue_[0] = param1;
        this->defaultValue_[1] = param2;
        this->defaultValue_[2] = param3;
        this->defaultValue_[3] = param4;
    }

    void Executor::setDefaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5)
    {
        this->defaultValue_[0] = param1;
        this->defaultValue_[1] = param2;
        this->defaultValue_[2] = param3;
        this->defaultValue_[3] = param4;
        this->defaultValue_[4] = param5;
    }

    void Executor::setDefaultValue(unsigned int index, const MultiType& param)
    {
        if (index < MAX_FUNCTOR_ARGUMENTS)
            this->defaultValue_[index] = param;
    }

    bool Executor::allDefaultValuesSet() const
    {
        for (unsigned int i = 0; i < this->functor_->getParamCount(); i++)
            if (this->defaultValue_[i].null())
                return false;

        return true;
    }
}
