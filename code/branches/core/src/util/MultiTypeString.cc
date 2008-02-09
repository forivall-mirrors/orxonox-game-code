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
 *   Inspiration: MultiType by Benjamin Grauer
 */

#include "MultiTypeString.h"

MultiTypeString::MultiTypeString(MultiType type) : MultiTypePrimitive(type)
{
    if (type == MT_constchar)
        this->string_ = std::string("");
    else if (type == MT_string)
        this->string_ = std::string("");
}

MultiTypeString::MultiTypeString(const MultiTypeString& mts)
{
    this->type_ = mts.type_;
    this->value_ = mts.value_;
}

MultiTypeString& MultiTypeString::operator=(const MultiTypeString& mts)
{
    this->type_ = mts.type_;
    this->value_ = mts.value_;
    return *this;
}

bool MultiTypeString::operator==(const MultiTypeString& mts) const
{
    if (!MultiTypePrimitive::operator==(mts) && this->type_ == mts.type_)
    {
        if (this->type_ == MT_constchar)
            return (this->string_ == mts.string_);
        else if (this->type_ == MT_string)
            return (this->string_ == mts.string_);
    }

    return false;
}

bool MultiTypeString::operator!=(const MultiTypeString& mts) const
{
    if (MultiTypePrimitive::operator==(mts) && this->type_ == mts.type_)
    {
        if (this->type_ == MT_constchar)
            return (this->string_ != mts.string_);
        else if (this->type_ == MT_string)
            return (this->string_ != mts.string_);
    }

    return true;
}

void MultiTypeString::setValue(const MultiTypeString& mts)
{
    this->type_ = mts.type_;
    this->value_ = mts.value_;
}
