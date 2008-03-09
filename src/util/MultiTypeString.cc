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
#include "Convert.h"

MultiTypeString::MultiTypeString(MultiType type) : MultiTypePrimitive(type)
{
    if (type == MT_constchar)
        this->string_ = std::string("");
    else if (type == MT_string)
        this->string_ = std::string("");
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

MultiTypeString::operator int() const
{ return (this->type_ == MT_int) ? this->value_.int_ : ConvertValueAndReturn<MultiTypeString, int>(*this); }
MultiTypeString::operator unsigned int() const
{ return (this->type_ == MT_uint) ? this->value_.uint_ : ConvertValueAndReturn<MultiTypeString, unsigned int>(*this); }
MultiTypeString::operator char() const
{ return (this->type_ == MT_char) ? this->value_.char_ : ConvertValueAndReturn<MultiTypeString, char>(*this); }
MultiTypeString::operator unsigned char() const
{ return (this->type_ == MT_uchar) ? this->value_.uchar_ : ConvertValueAndReturn<MultiTypeString, unsigned char>(*this); }
MultiTypeString::operator short() const
{ return (this->type_ == MT_short) ? this->value_.short_ : ConvertValueAndReturn<MultiTypeString, short>(*this); }
MultiTypeString::operator unsigned short() const
{ return (this->type_ == MT_ushort) ? this->value_.ushort_ : ConvertValueAndReturn<MultiTypeString, unsigned short>(*this); }
MultiTypeString::operator long() const
{ return (this->type_ == MT_long) ? this->value_.long_ : ConvertValueAndReturn<MultiTypeString, long>(*this); }
MultiTypeString::operator unsigned long() const
{ return (this->type_ == MT_ulong) ? this->value_.ulong_ : ConvertValueAndReturn<MultiTypeString, unsigned long>(*this); }
MultiTypeString::operator float() const
{ return (this->type_ == MT_float) ? this->value_.float_ : ConvertValueAndReturn<MultiTypeString, float>(*this); }
MultiTypeString::operator double() const
{ return (this->type_ == MT_double) ? this->value_.double_ : ConvertValueAndReturn<MultiTypeString, double>(*this); }
MultiTypeString::operator long double() const
{ return (this->type_ == MT_longdouble) ? this->value_.longdouble_ : ConvertValueAndReturn<MultiTypeString, long double>(*this); }
MultiTypeString::operator bool() const
{ return (this->type_ == MT_bool) ? this->value_.bool_ : ConvertValueAndReturn<MultiTypeString, bool>(*this); }
MultiTypeString::operator std::string() const
{ return (this->type_ == MT_string) ? this->string_ : ConvertValueAndReturn<MultiTypeString, std::string>(*this); }
MultiTypeString::operator const char*() const
{ return ((this->type_ == MT_constchar) ? this->string_ : ConvertValueAndReturn<MultiTypeString, std::string>(*this)).c_str(); }

void MultiTypeString::setValue(const MultiTypeString& mts)
{
    MultiTypePrimitive::setValue(mts);
    this->string_ = mts.string_;
}

std::string MultiTypeString::toString() const
{
    if (this->type_ == MT_constchar)
        return this->string_;
    else if (this->type_ == MT_string)
        return this->string_;
    else
        return MultiTypePrimitive::toString();
}

bool MultiTypeString::fromString(const std::string value)
{
    if (this->type_ == MT_constchar)
        this->string_ = value;
    else if (this->type_ == MT_string)
        this->string_ = value;
    else
        return MultiTypePrimitive::fromString(value);

    return true;
}

std::ostream& operator<<(std::ostream& out, MultiTypeString& mts)
{
    out << mts.toString();
    return out;
}
