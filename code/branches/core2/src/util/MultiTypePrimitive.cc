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

#include "MultiTypePrimitive.h"
#include "Convert.h"

MultiTypePrimitive::MultiTypePrimitive(MultiType type)
{
    this->type_ = type;

    if (type == MT_void)
        this->value_.void_ = 0;
    else if (type == MT_int)
        this->value_.int_ = 0;
    else if (type == MT_uint)
        this->value_.uint_ = 0;
    else if (type == MT_char)
        this->value_.char_ = 0;
    else if (type == MT_uchar)
        this->value_.uchar_ = 0;
    else if (type == MT_short)
        this->value_.short_ = 0;
    else if (type == MT_ushort)
        this->value_.ushort_ = 0;
    else if (type == MT_long)
        this->value_.long_ = 0;
    else if (type == MT_ulong)
        this->value_.ulong_ = 0;
    else if (type == MT_float)
        this->value_.float_ = 0.0;
    else if (type == MT_double)
        this->value_.double_ = 0.0;
    else if (type == MT_longdouble)
        this->value_.longdouble_ = 0.0;
    else if (type == MT_bool)
        this->value_.bool_ = false;
    else
        this->value_.void_ = 0;
}

bool MultiTypePrimitive::operator==(const MultiTypePrimitive& mtp) const
{
    if (this->type_ == mtp.type_)
    {
        if (this->type_ == MT_void)
            return (this->value_.void_ == mtp.value_.void_);
        else if (this->type_ == MT_int)
            return (this->value_.int_ == mtp.value_.int_);
        else if (this->type_ == MT_uint)
            return (this->value_.uint_ == mtp.value_.uint_);
        else if (this->type_ == MT_char)
            return (this->value_.char_ == mtp.value_.char_);
        else if (this->type_ == MT_uchar)
            return (this->value_.uchar_ == mtp.value_.uchar_);
        else if (this->type_ == MT_short)
            return (this->value_.short_ == mtp.value_.short_);
        else if (this->type_ == MT_ushort)
            return (this->value_.ushort_ == mtp.value_.ushort_);
        else if (this->type_ == MT_long)
            return (this->value_.long_ == mtp.value_.long_);
        else if (this->type_ == MT_ulong)
            return (this->value_.ulong_ == mtp.value_.ulong_);
        else if (this->type_ == MT_float)
            return (this->value_.float_ == mtp.value_.float_);
        else if (this->type_ == MT_double)
            return (this->value_.double_ == mtp.value_.double_);
        else if (this->type_ == MT_longdouble)
            return (this->value_.longdouble_ == mtp.value_.longdouble_);
        else if (this->type_ == MT_bool)
            return (this->value_.bool_ == mtp.value_.bool_);
    }

    return false;
}

bool MultiTypePrimitive::operator!=(const MultiTypePrimitive& mtp) const
{
    if (this->type_ == mtp.type_)
    {
        if (this->type_ == MT_void)
            return (this->value_.void_ != mtp.value_.void_);
        else if (this->type_ == MT_int)
            return (this->value_.int_ != mtp.value_.int_);
        else if (this->type_ == MT_uint)
            return (this->value_.uint_ != mtp.value_.uint_);
        else if (this->type_ == MT_char)
            return (this->value_.char_ != mtp.value_.char_);
        else if (this->type_ == MT_uchar)
            return (this->value_.uchar_ != mtp.value_.uchar_);
        else if (this->type_ == MT_short)
            return (this->value_.short_ != mtp.value_.short_);
        else if (this->type_ == MT_ushort)
            return (this->value_.ushort_ != mtp.value_.ushort_);
        else if (this->type_ == MT_long)
            return (this->value_.long_ != mtp.value_.long_);
        else if (this->type_ == MT_ulong)
            return (this->value_.ulong_ != mtp.value_.ulong_);
        else if (this->type_ == MT_float)
            return (this->value_.float_ != mtp.value_.float_);
        else if (this->type_ == MT_double)
            return (this->value_.double_ != mtp.value_.double_);
        else if (this->type_ == MT_longdouble)
            return (this->value_.longdouble_ != mtp.value_.longdouble_);
        else if (this->type_ == MT_bool)
            return (this->value_.bool_ != mtp.value_.bool_);
    }

    return true;
}

MultiTypePrimitive::operator void*() const
{ return (this->type_ == MT_void) ? this->value_.void_ : ConvertValueAndReturn<MultiTypePrimitive, void*>(*this); }
MultiTypePrimitive::operator int() const
{ return (this->type_ == MT_int) ? this->value_.int_ : ConvertValueAndReturn<MultiTypePrimitive, int>(*this); }
MultiTypePrimitive::operator unsigned int() const
{ return (this->type_ == MT_uint) ? this->value_.uint_ : ConvertValueAndReturn<MultiTypePrimitive, unsigned int>(*this); }
MultiTypePrimitive::operator char() const
{ return (this->type_ == MT_char) ? this->value_.char_ : ConvertValueAndReturn<MultiTypePrimitive, char>(*this); }
MultiTypePrimitive::operator unsigned char() const
{ return (this->type_ == MT_uchar) ? this->value_.uchar_ : ConvertValueAndReturn<MultiTypePrimitive, unsigned char>(*this); }
MultiTypePrimitive::operator short() const
{ return (this->type_ == MT_short) ? this->value_.short_ : ConvertValueAndReturn<MultiTypePrimitive, short>(*this); }
MultiTypePrimitive::operator unsigned short() const
{ return (this->type_ == MT_ushort) ? this->value_.ushort_ : ConvertValueAndReturn<MultiTypePrimitive, unsigned short>(*this); }
MultiTypePrimitive::operator long() const
{ return (this->type_ == MT_long) ? this->value_.long_ : ConvertValueAndReturn<MultiTypePrimitive, long>(*this); }
MultiTypePrimitive::operator unsigned long() const
{ return (this->type_ == MT_ulong) ? this->value_.ulong_ : ConvertValueAndReturn<MultiTypePrimitive, unsigned long>(*this); }
MultiTypePrimitive::operator float() const
{ return (this->type_ == MT_float) ? this->value_.float_ : ConvertValueAndReturn<MultiTypePrimitive, float>(*this); }
MultiTypePrimitive::operator double() const
{ return (this->type_ == MT_double) ? this->value_.double_ : ConvertValueAndReturn<MultiTypePrimitive, double>(*this); }
MultiTypePrimitive::operator long double() const
{ return (this->type_ == MT_longdouble) ? this->value_.longdouble_ : ConvertValueAndReturn<MultiTypePrimitive, long double>(*this); }
MultiTypePrimitive::operator bool() const
{ return (this->type_ == MT_bool) ? this->value_.bool_ : ConvertValueAndReturn<MultiTypePrimitive, bool>(*this); }

void MultiTypePrimitive::setValue(const MultiTypePrimitive& mtp)
{
    this->type_ = mtp.type_;
    this->value_ = mtp.value_;
}

std::string MultiTypePrimitive::getTypename() const
{
    if (this->type_ == MT_void)
        return "pointer";
    else if (this->type_ == MT_int)
        return "int";
    else if (this->type_ == MT_uint)
        return "unsigned int";
    else if (this->type_ == MT_char)
        return "char";
    else if (this->type_ == MT_uchar)
        return "unsigned char";
    else if (this->type_ == MT_short)
        return "short";
    else if (this->type_ == MT_ushort)
        return "unsigned short";
    else if (this->type_ == MT_long)
        return "long";
    else if (this->type_ == MT_ulong)
        return "unsigned long";
    else if (this->type_ == MT_float)
        return "float";
    else if (this->type_ == MT_double)
        return "double";
    else if (this->type_ == MT_longdouble)
        return "long double";
    else if (this->type_ == MT_bool)
        return "bool";
    else
        return "unknown";
}

std::string MultiTypePrimitive::toString() const
{
    std::string output;

    if (this->type_ == MT_void)
        ConvertValue(&output, this->value_.void_);
    else if (this->type_ == MT_int)
        ConvertValue(&output, this->value_.int_);
    else if (this->type_ == MT_uint)
        ConvertValue(&output, this->value_.uint_);
    else if (this->type_ == MT_char)
        ConvertValue(&output, this->value_.char_);
    else if (this->type_ == MT_uchar)
        ConvertValue(&output, this->value_.uchar_);
    else if (this->type_ == MT_short)
        ConvertValue(&output, this->value_.short_);
    else if (this->type_ == MT_ushort)
        ConvertValue(&output, this->value_.ushort_);
    else if (this->type_ == MT_long)
        ConvertValue(&output, this->value_.long_);
    else if (this->type_ == MT_ulong)
        ConvertValue(&output, this->value_.ulong_);
    else if (this->type_ == MT_float)
        ConvertValue(&output, this->value_.float_);
    else if (this->type_ == MT_double)
        ConvertValue(&output, this->value_.double_);
    else if (this->type_ == MT_longdouble)
        ConvertValue(&output, this->value_.longdouble_);
    else if (this->type_ == MT_bool)
        ConvertValue(&output, this->value_.bool_);

    return output;
}

bool MultiTypePrimitive::fromString(const std::string value)
{
    if (this->type_ == MT_void)
        return ConvertValue(&this->value_.void_, value, (void*)0);
    else if (this->type_ == MT_int)
        return ConvertValue(&this->value_.int_, value, (int)0);
    else if (this->type_ == MT_uint)
        return ConvertValue(&this->value_.uint_, value, (unsigned int)0);
    else if (this->type_ == MT_char)
        return ConvertValue(&this->value_.char_, value, (char)0);
    else if (this->type_ == MT_uchar)
        return ConvertValue(&this->value_.uchar_, value, (unsigned char)0);
    else if (this->type_ == MT_short)
        return ConvertValue(&this->value_.short_, value, (short)0);
    else if (this->type_ == MT_ushort)
        return ConvertValue(&this->value_.ushort_, value, (unsigned short)0);
    else if (this->type_ == MT_long)
        return ConvertValue(&this->value_.long_, value, (long)0);
    else if (this->type_ == MT_ulong)
        return ConvertValue(&this->value_.ulong_, value, (unsigned long)0);
    else if (this->type_ == MT_float)
        return ConvertValue(&this->value_.float_, value, (float)0.0);
    else if (this->type_ == MT_double)
        return ConvertValue(&this->value_.double_, value, (double)0.0);
    else if (this->type_ == MT_longdouble)
        return ConvertValue(&this->value_.longdouble_, value, (long double)0.0);
    else if (this->type_ == MT_bool)
        return ConvertValue(&this->value_.bool_, value, false);
    else
        return false;
}

std::ostream& operator<<(std::ostream& out, const MultiTypePrimitive& mtp)
{
    out << mtp.toString();
    return out;
}
