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

MultiTypePrimitive::MultiTypePrimitive(MultiType type)
{
    this->type_ = type;

    if (type == MT_int)
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
        this->value_.int_ = 0;
}

MultiTypePrimitive::MultiTypePrimitive(const MultiTypePrimitive& mtp)
{
    this->type_ = mtp.type_;
    this->value_ = mtp.value_;
}

MultiTypePrimitive& MultiTypePrimitive::operator=(const MultiTypePrimitive& mtp)
{
    this->type_ = mtp.type_;
    this->value_ = mtp.value_;
    return *this;
}

bool MultiTypePrimitive::operator==(const MultiTypePrimitive& mtp) const
{
    if (this->type_ == mtp.type_)
    {
        if (this->type_ == MT_int)
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
        if (this->type_ == MT_int)
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

void MultiTypePrimitive::setValue(const MultiTypePrimitive& mtp)
{
    this->type_ = mtp.type_;
    this->value_ = mtp.value_;
}

std::ostream& operator<<(std::ostream& out, const MultiTypePrimitive& mtp)
{
    if (mtp.isA(MT_int))
        out << mtp.getInt();
    else if (mtp.isA(MT_uint))
        out << mtp.getUnsignedInt();
    else if (mtp.isA(MT_char))
        out << mtp.getChar();
    else if (mtp.isA(MT_uchar))
        out << mtp.getUnsignedChar();
    else if (mtp.isA(MT_short))
        out << mtp.getShort();
    else if (mtp.isA(MT_ushort))
        out << mtp.getUnsignedShort();
    else if (mtp.isA(MT_long))
        out << mtp.getLong();
    else if (mtp.isA(MT_ulong))
        out << mtp.getUnsignedLong();
    else if (mtp.isA(MT_float))
        out << mtp.getFloat();
    else if (mtp.isA(MT_double))
        out << mtp.getDouble();
    else if (mtp.isA(MT_longdouble))
        out << mtp.getLongDouble();
    else if (mtp.isA(MT_bool))
        out << mtp.getBool();

    return out;
}
