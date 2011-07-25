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
 *   Inspiration: MultiType by Benjamin Grauer
 */

#include "MultiTypeString.h"
#include "Convert.h"

MultiTypeString::MultiTypeString(MultiType type) : MultiTypePrimitive(type)
{
    // Nothing to do for string
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

bool MultiTypeString::operator==(const MultiTypePrimitive& mtp) const
{
    return MultiTypePrimitive::operator==(mtp);
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

bool MultiTypeString::operator!=(const MultiTypePrimitive& mtp) const
{
    return MultiTypePrimitive::operator!=(mtp);
}

MultiTypeString::operator void*() const
{ return (this->type_ == MT_void) ? this->value_.void_ : getConvertedValue<MultiTypeString, void*>(*this, 0); }
MultiTypeString::operator int() const
{ return (this->type_ == MT_int) ? this->value_.int_ : getConvertedValue<MultiTypeString, int>(*this, 0); }
MultiTypeString::operator unsigned int() const
{ return (this->type_ == MT_uint) ? this->value_.uint_ : getConvertedValue<MultiTypeString, unsigned int>(*this, 0); }
MultiTypeString::operator char() const
{ return (this->type_ == MT_char) ? this->value_.char_ : getConvertedValue<MultiTypeString, char>(*this, 0); }
MultiTypeString::operator unsigned char() const
{ return (this->type_ == MT_uchar) ? this->value_.uchar_ : getConvertedValue<MultiTypeString, unsigned char>(*this, 0); }
MultiTypeString::operator short() const
{ return (this->type_ == MT_short) ? this->value_.short_ : getConvertedValue<MultiTypeString, short>(*this, 0); }
MultiTypeString::operator unsigned short() const
{ return (this->type_ == MT_ushort) ? this->value_.ushort_ : getConvertedValue<MultiTypeString, unsigned short>(*this, 0); }
MultiTypeString::operator long() const
{ return (this->type_ == MT_long) ? this->value_.long_ : getConvertedValue<MultiTypeString, long>(*this, 0); }
MultiTypeString::operator unsigned long() const
{ return (this->type_ == MT_ulong) ? this->value_.ulong_ : getConvertedValue<MultiTypeString, unsigned long>(*this, 0); }
MultiTypeString::operator float() const
{ return (this->type_ == MT_float) ? this->value_.float_ : getConvertedValue<MultiTypeString, float>(*this, 0); }
MultiTypeString::operator double() const
{ return (this->type_ == MT_double) ? this->value_.double_ : getConvertedValue<MultiTypeString, double>(*this, 0); }
MultiTypeString::operator long double() const
{ return (this->type_ == MT_longdouble) ? this->value_.longdouble_ : getConvertedValue<MultiTypeString, long double>(*this, 0); }
MultiTypeString::operator bool() const
{ return (this->type_ == MT_bool) ? this->value_.bool_ : getConvertedValue<MultiTypeString, bool>(*this, 0); }
MultiTypeString::operator std::string() const
{ return (this->type_ == MT_string) ? this->string_ : getConvertedValue<MultiTypeString, std::string>(*this); }
MultiTypeString::operator const char*() const
{ return ((this->type_ == MT_constchar) ? this->string_ : getConvertedValue<MultiTypeString, std::string>(*this)).c_str(); }

void MultiTypeString::setValue(const MultiTypeString& mts)
{
    MultiTypePrimitive::setValue(mts);
    this->string_ = mts.string_;
}

void MultiTypeString::setValue(const MultiTypePrimitive& mtp)
{
    MultiTypePrimitive::setValue(mtp);
}

std::string MultiTypeString::getTypename() const
{
    if (this->type_ == MT_constchar)
        return "string";
    else if (this->type_ == MT_string)
        return "string";
    else
        return MultiTypePrimitive::getTypename();
}

std::string MultiTypeString::toString() const
{
    std::string output;

    if (this->type_ == MT_constchar)
        return this->string_;
    else if (this->type_ == MT_string)
        return this->string_;
    else
        return MultiTypePrimitive::toString();

    // FIXME: unreachable code
    // return output;
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

bool MultiTypeString::assimilate(const MultiTypeString& mts, const MultiTypeString& defvalue)
{
    if (this->type_ == MT_void)
        return ConvertValue(&this->value_.void_, mts, defvalue.value_.void_);
    else if (this->type_ == MT_int)
        return ConvertValue(&this->value_.int_, mts, defvalue.value_.int_);
    else if (this->type_ == MT_uint)
        return ConvertValue(&this->value_.uint_, mts, defvalue.value_.uint_);
    else if (this->type_ == MT_char)
        return ConvertValue(&this->value_.char_, mts, defvalue.value_.char_);
    else if (this->type_ == MT_uchar)
        return ConvertValue(&this->value_.uchar_, mts, defvalue.value_.uchar_);
    else if (this->type_ == MT_short)
        return ConvertValue(&this->value_.short_, mts, defvalue.value_.short_);
    else if (this->type_ == MT_ushort)
        return ConvertValue(&this->value_.ushort_, mts, defvalue.value_.ushort_);
    else if (this->type_ == MT_long)
        return ConvertValue(&this->value_.long_, mts, defvalue.value_.long_);
    else if (this->type_ == MT_ulong)
        return ConvertValue(&this->value_.ulong_, mts, defvalue.value_.ulong_);
    else if (this->type_ == MT_float)
        return ConvertValue(&this->value_.float_, mts, defvalue.value_.float_);
    else if (this->type_ == MT_double)
        return ConvertValue(&this->value_.double_, mts, defvalue.value_.double_);
    else if (this->type_ == MT_longdouble)
        return ConvertValue(&this->value_.longdouble_, mts, defvalue.value_.longdouble_);
    else if (this->type_ == MT_bool)
        return ConvertValue(&this->value_.bool_, mts, defvalue.value_.bool_);
    else if (this->type_ == MT_constchar)
        return ConvertValue(&this->string_, mts, defvalue.string_);
    else if (this->type_ == MT_string)
        return ConvertValue(&this->string_, mts, defvalue.string_);
    else
        return false;
}

std::ostream& operator<<(std::ostream& out, MultiTypeString& mts)
{
    out << mts.toString();
    return out;
}
