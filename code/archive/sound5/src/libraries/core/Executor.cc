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

#include "util/Convert.h"
#include "Language.h"

namespace orxonox
{
    Executor::Executor(Functor* functor, const std::string& name)
    {
        this->functor_ = functor;
        this->name_ = name;

        this->bAddedDescription_ = false;
        this->bAddedDescriptionReturnvalue_ = false;

        this->bAddedDescriptionParam_[0] = false;
        this->bAddedDescriptionParam_[1] = false;
        this->bAddedDescriptionParam_[2] = false;
        this->bAddedDescriptionParam_[3] = false;
        this->bAddedDescriptionParam_[4] = false;

        this->bAddedDefaultValue_[0] = false;
        this->bAddedDefaultValue_[1] = false;
        this->bAddedDefaultValue_[2] = false;
        this->bAddedDefaultValue_[3] = false;
        this->bAddedDefaultValue_[4] = false;
    }

    Executor::~Executor()
    {
        delete this->functor_;
    }

    bool Executor::parse(const std::string& params, const std::string& delimiter) const
    {
        EXECUTOR_PARSE(normal);
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
            else if (this->bAddedDefaultValue_[0])
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
                if (!this->bAddedDefaultValue_[i])
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

    Executor& Executor::setDescription(const std::string& description)
    {
        if (!this->bAddedDescription_)
        {
            this->description_ = std::string("ExecutorDescription::" + this->name_ + "::function");
            AddLanguageEntry(this->description_, description);
            this->bAddedDescription_ = true;
        }
        return (*this);
    }

    const std::string& Executor::getDescription() const
    {
        return GetLocalisation(this->description_);
    }

    Executor& Executor::setDescriptionParam(unsigned int param, const std::string& description)
    {
        if (param < MAX_FUNCTOR_ARGUMENTS)
        {
            if (!this->bAddedDescriptionParam_[param])
            {
                std::string paramnumber;
                if (!convertValue(&paramnumber, param))
                    return (*this);

                this->descriptionParam_[param] = std::string("ExecutorDescription::" + this->name_ + "::param" + paramnumber);
                AddLanguageEntry(this->descriptionParam_[param], description);
                this->bAddedDescriptionParam_[param] = true;
            }
        }
        return (*this);
    }

    const std::string& Executor::getDescriptionParam(unsigned int param) const
    {
        if (param < MAX_FUNCTOR_ARGUMENTS)
            return GetLocalisation(this->descriptionParam_[param]);

        return this->descriptionParam_[0];
    }

    Executor& Executor::setDescriptionReturnvalue(const std::string& description)
    {
        if (!this->bAddedDescriptionReturnvalue_)
        {
            this->descriptionReturnvalue_ = std::string("ExecutorDescription::" + this->name_ + "::returnvalue");
            AddLanguageEntry(this->descriptionReturnvalue_, description);
            this->bAddedDescriptionReturnvalue_ = true;
        }
        return (*this);
    }

    const std::string& Executor::getDescriptionReturnvalue(int param) const
    {
        return GetLocalisation(this->descriptionReturnvalue_);
    }

    Executor& Executor::setDefaultValues(const MultiType& param1)
    {
        this->defaultValue_[0] = param1;
        this->bAddedDefaultValue_[0] = true;

        return (*this);
    }

    Executor& Executor::setDefaultValues(const MultiType& param1, const MultiType& param2)
    {
        this->defaultValue_[0] = param1;
        this->bAddedDefaultValue_[0] = true;
        this->defaultValue_[1] = param2;
        this->bAddedDefaultValue_[1] = true;

        return (*this);
    }

    Executor& Executor::setDefaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3)
    {
        this->defaultValue_[0] = param1;
        this->bAddedDefaultValue_[0] = true;
        this->defaultValue_[1] = param2;
        this->bAddedDefaultValue_[1] = true;
        this->defaultValue_[2] = param3;
        this->bAddedDefaultValue_[2] = true;

        return (*this);
    }

    Executor& Executor::setDefaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4)
    {
        this->defaultValue_[0] = param1;
        this->bAddedDefaultValue_[0] = true;
        this->defaultValue_[1] = param2;
        this->bAddedDefaultValue_[1] = true;
        this->defaultValue_[2] = param3;
        this->bAddedDefaultValue_[2] = true;
        this->defaultValue_[3] = param4;
        this->bAddedDefaultValue_[3] = true;

        return (*this);
    }

    Executor& Executor::setDefaultValues(const MultiType& param1, const MultiType& param2, const MultiType& param3, const MultiType& param4, const MultiType& param5)
    {
        this->defaultValue_[0] = param1;
        this->bAddedDefaultValue_[0] = true;
        this->defaultValue_[1] = param2;
        this->bAddedDefaultValue_[1] = true;
        this->defaultValue_[2] = param3;
        this->bAddedDefaultValue_[2] = true;
        this->defaultValue_[3] = param4;
        this->bAddedDefaultValue_[3] = true;
        this->defaultValue_[4] = param5;
        this->bAddedDefaultValue_[4] = true;

        return (*this);
    }

    Executor& Executor::setDefaultValue(unsigned int index, const MultiType& param)
    {
        if (index < MAX_FUNCTOR_ARGUMENTS)
        {
            this->defaultValue_[index] = param;
            this->bAddedDefaultValue_[index] = true;
        }
        return (*this);
    }

    bool Executor::allDefaultValuesSet() const
    {
        for (unsigned int i = 0; i < this->functor_->getParamCount(); i++)
            if (!this->bAddedDefaultValue_[i])
                return false;

        return true;
    }
}
