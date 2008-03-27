/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
#include "Language.h"
#include "util/String.h"

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
        unsigned int paramCount = this->functor_->getParamCount();

        if (paramCount == 0)
        {
            (*this->functor_)();
        }
        else if (paramCount == 1)
        {
            (*this->functor_)(MultiTypeMath(params));
        }
        else
        {
            SubString tokens(params, delimiter, SubString::WhiteSpaces, false, '\\', '"', '(', ')', '\0');

            for (unsigned int i = tokens.size(); i < this->functor_->getParamCount(); i++)
                if (!this->bAddedDefaultValue_[i])
                    return false;

            MultiTypeMath param[paramCount];
            for (unsigned int i = 0; i < tokens.size(); i++)
                param[i] = tokens[i];
            for (unsigned int i = tokens.size(); i < paramCount; i++)
                param[i] = this->defaultValue_[i];

            switch(paramCount)
            {
                case 2:
                    (*this->functor_)(param[0], param[1]);
                    break;
                case 3:
                    (*this->functor_)(param[0], param[1], param[2]);
                    break;
                case 4:
                    (*this->functor_)(param[0], param[1], param[2], param[3]);
                    break;
                case 5:
                    (*this->functor_)(param[0], param[1], param[2], param[3], param[4]);
                    break;
            }
        }

        return true;
    }

    void Executor::setName(const std::string name)
    {
        this->name_ = name;
    }

    const std::string& Executor::getName() const
    {
        return this->name_;
    }

    void Executor::description(const std::string& description)
    {
        if (!this->bAddedDescription_)
        {
            this->description_ = std::string("ExecutorDescription::" + this->name_ + "::function");
            AddLanguageEntry(this->description_, description);
            this->bAddedDescription_ = true;
        }
    }

    const std::string& Executor::getDescription() const
    {
        return GetLocalisation(this->description_);
    }

    void Executor::descriptionParam(int param, const std::string& description)
    {
        if (param >= 0 && param < MAX_FUNCTOR_ARGUMENTS)
        {
            if (!this->bAddedDescriptionParam_[param])
            {
                std::string paramnumber;
                if (!Convert::ToString(&paramnumber, param))
                    return;

                this->descriptionParam_[param] = std::string("ExecutorDescription::" + this->name_ + "::param" + paramnumber);
                AddLanguageEntry(this->descriptionParam_[param], description);
                this->bAddedDescriptionParam_[param] = true;
            }
        }
    }

    const std::string& Executor::getDescriptionParam(int param) const
    {
        if (param >= 0 && param < MAX_FUNCTOR_ARGUMENTS)
            return GetLocalisation(this->descriptionParam_[param]);

        return this->descriptionParam_[0];
    }

    void Executor::descriptionReturnvalue(const std::string& description)
    {
        if (!this->bAddedDescriptionReturnvalue_)
        {
            this->descriptionReturnvalue_ = std::string("ExecutorDescription::" + this->name_ + "::returnvalue");
            AddLanguageEntry(this->descriptionReturnvalue_, description);
            this->bAddedDescriptionReturnvalue_ = true;
        }
    }

    const std::string& Executor::getDescriptionReturnvalue(int param) const
    {
        return GetLocalisation(this->descriptionReturnvalue_);
    }

    void Executor::setDefaultValues(const MultiTypeMath& param1)
    {
        this->defaultValue_[0] = param1;
        this->bAddedDefaultValue_[0] = true;
    }

    void Executor::setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2)
    {
        this->defaultValue_[0] = param1;
        this->bAddedDefaultValue_[0] = true;
        this->defaultValue_[1] = param2;
        this->bAddedDefaultValue_[1] = true;
    }

    void Executor::setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3)
    {
        this->defaultValue_[0] = param1;
        this->bAddedDefaultValue_[0] = true;
        this->defaultValue_[1] = param2;
        this->bAddedDefaultValue_[1] = true;
        this->defaultValue_[2] = param3;
        this->bAddedDefaultValue_[2] = true;
    }

    void Executor::setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4)
    {
        this->defaultValue_[0] = param1;
        this->bAddedDefaultValue_[0] = true;
        this->defaultValue_[1] = param2;
        this->bAddedDefaultValue_[1] = true;
        this->defaultValue_[2] = param3;
        this->bAddedDefaultValue_[2] = true;
        this->defaultValue_[3] = param4;
        this->bAddedDefaultValue_[3] = true;
    }

    void Executor::setDefaultValues(const MultiTypeMath& param1, const MultiTypeMath& param2, const MultiTypeMath& param3, const MultiTypeMath& param4, const MultiTypeMath& param5)
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
    }

    void Executor::setDefaultValue(int index, const MultiTypeMath& param)
    {
        if (index >= 0 && index < MAX_FUNCTOR_ARGUMENTS)
        {
            this->defaultValue_[index] = param;
            this->bAddedDefaultValue_[index] = true;
        }
    }
}
