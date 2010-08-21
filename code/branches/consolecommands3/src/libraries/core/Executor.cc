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

namespace orxonox
{
    int Functor::instances_s = 0;
    int Executor::instances_s = 0;

    Executor::Executor(const FunctorPtr& functor, const std::string& name)
    {
        this->functor_ = functor;
        this->name_ = name;
        ++instances_s; COUT(0) << "executor ++: " << instances_s << std::endl;
    }

    Executor::~Executor()
    {
        --instances_s; COUT(0) << "executor --: " << instances_s << std::endl;
    }

    MultiType Executor::parse(const std::string& params, bool* success, const std::string& delimiter) const
    {
        if (success)
            *success = true;

        unsigned int paramCount = this->functor_->getParamCount();

        if (paramCount == 0)
        {
            COUT(5) << "Calling Executor " << this->name_ << " through parser without parameters." << std::endl;
            return (*this->functor_)();
        }
        else if (paramCount == 1)
        {
            const std::string& temp = getStripped(params);
            if (!temp.empty())
            {
                COUT(5) << "Calling Executor " << this->name_ << " through parser with one parameter, using whole string: " << params << std::endl;
                return (*this->functor_)(MultiType(params));
            }
            else if (!this->defaultValue_[0].null())
            {
                COUT(5) << "Calling Executor " << this->name_ << " through parser with one parameter, using default value: " << this->defaultValue_[0] << std::endl;
                return (*this->functor_)(this->defaultValue_[0]);
            }
            else
            {
                COUT(2) << "Warning: Can't call executor " << this->name_ << " through parser: Not enough parameters or default values given (input: " << temp << ")." << std::endl;
                if (success)
                    *success = false;
                return MT_Type::Null;
            }
        }
        else
        {
            SubString tokens(params, delimiter, SubString::WhiteSpaces, false, '\\', true, '"', true, '(', ')', true, '\0');

            for (unsigned int i = tokens.size(); i < this->functor_->getParamCount(); i++)
            {
                if (this->defaultValue_[i].null())
                {
                    COUT(2) << "Warning: Can't call executor " << this->name_ << " through parser: Not enough parameters or default values given (input:" << params << ")." << std::endl;
                    if (success)
                        *success = false;
                    return MT_Type::Null;
                }
            }

            MultiType param[MAX_FUNCTOR_ARGUMENTS];
            COUT(5) << "Calling Executor " << this->name_ << " through parser with " << paramCount << " parameters, using " << tokens.size() << " tokens (";
            for (unsigned int i = 0; i < tokens.size() && i < MAX_FUNCTOR_ARGUMENTS; i++)
            {
                param[i] = tokens[i];
                if (i != 0)
                {
                    COUT(5) << ", ";
                }
                COUT(5) << tokens[i];
            }
            COUT(5) << ") and " << std::max((int)paramCount - (int)tokens.size(), 0) << " default values (";
            for (unsigned int i = tokens.size(); i < paramCount; i++)
            {
                param[i] = this->defaultValue_[i];
                if (i != 0)
                {
                    COUT(5) << ", ";
                }
                COUT(5) << this->defaultValue_[i];
            }
            COUT(5) << ")." << std::endl;

            if ((tokens.size() > paramCount) && (this->functor_->getTypenameParam(paramCount - 1) == "string"))
                param[paramCount - 1] = tokens.subSet(paramCount - 1).join();

            switch(paramCount)
            {
                case 2:
                    return (*this->functor_)(param[0], param[1]);
                case 3:
                    return (*this->functor_)(param[0], param[1], param[2]);
                case 4:
                    return (*this->functor_)(param[0], param[1], param[2], param[3]);
                case 5:
                    return (*this->functor_)(param[0], param[1], param[2], param[3], param[4]);
            }
        }

        return MT_Type::Null;
    }

    bool Executor::evaluate(const std::string& params, MultiType param[5], const std::string& delimiter) const
    {
        unsigned int paramCount = this->functor_->getParamCount();

        if (paramCount == 1)
        {
            // only one param: check if there are params given, otherwise try to use default values
            if (!getStripped(params).empty())
            {
                param[0] = params;
                this->functor_->evaluateParam(0, param[0]);
                return true;
            }
            else if (!this->defaultValue_[0].null())
            {
                param[0] = this->defaultValue_[0];
                this->functor_->evaluateParam(0, param[0]);
                return true;
            }
            return false;
        }
        else
        {
            // more than one param
            SubString tokens(params, delimiter, SubString::WhiteSpaces, false, '\\', true, '"', true, '(', ')', true, '\0');

            // if there are not enough params given, check if there are default values
            for (unsigned int i = tokens.size(); i < this->functor_->getParamCount(); i++)
                if (this->defaultValue_[i].null())
                    return false;

            // assign all given arguments to the multitypes
            for (unsigned int i = 0; i < std::min(tokens.size(), MAX_FUNCTOR_ARGUMENTS); i++)
                param[i] = tokens[i];

            // fill the remaining multitypes with default values
            for (unsigned int i = tokens.size(); i < std::min(paramCount, MAX_FUNCTOR_ARGUMENTS); i++)
                param[i] = this->defaultValue_[i];

            // evaluate the param types through the functor
            for (unsigned int i = 0; i < std::min(paramCount, MAX_FUNCTOR_ARGUMENTS); i++)
                this->functor_->evaluateParam(i, param[i]);

            return true;
        }
    }

    Executor& Executor::setDefaultValues(const MultiType& param1)
    {
        this->defaultValue_[0] = param1;

        return (*this);
    }

    Executor& Executor::setDefaultValues(const MultiType& param1, const MultiType& param2)
    {
        this->defaultValue_[0] = param1;
        this->defaultValue_[1] = param2;

        return (*this);
    }

    Executor& Executor::setDefaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3)
    {
        this->defaultValue_[0] = param1;
        this->defaultValue_[1] = param2;
        this->defaultValue_[2] = param3;

        return (*this);
    }

    Executor& Executor::setDefaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4)
    {
        this->defaultValue_[0] = param1;
        this->defaultValue_[1] = param2;
        this->defaultValue_[2] = param3;
        this->defaultValue_[3] = param4;

        return (*this);
    }

    Executor& Executor::setDefaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5)
    {
        this->defaultValue_[0] = param1;
        this->defaultValue_[1] = param2;
        this->defaultValue_[2] = param3;
        this->defaultValue_[3] = param4;
        this->defaultValue_[4] = param5;

        return (*this);
    }

    Executor& Executor::setDefaultValue(unsigned int index, const MultiType& param)
    {
        if (index < MAX_FUNCTOR_ARGUMENTS)
            this->defaultValue_[index] = param;

        return (*this);
    }

    bool Executor::allDefaultValuesSet() const
    {
        for (unsigned int i = 0; i < this->functor_->getParamCount(); i++)
            if (this->defaultValue_[i].null())
                return false;

        return true;
    }
}
