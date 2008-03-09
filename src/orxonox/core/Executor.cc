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
    }

    Executor::~Executor()
    {
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
        if (param > 0 && param <= 5)
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
        if (param > 0 && param <= 5)
        {
            return GetLocalisation(this->descriptionParam_[param]);
        }

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
}
