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

#include "MultiTypeMath.h"
#include "Convert.h"

MultiTypeMath::MultiTypeMath(MultiType type) : MultiTypeString(type)
{
    if (type == MT_vector2)
        this->vector2_ = orxonox::Vector2(0, 0);
    else if (type == MT_vector3)
        this->vector3_ = orxonox::Vector3(0, 0, 0);
    else if (type == MT_colourvalue)
        this->colourvalue_ = orxonox::ColourValue(0, 0, 0, 0);
    else if (type == MT_quaternion)
        this->quaternion_ = orxonox::Quaternion(1, 0, 0, 0);
    else if (type == MT_radian)
        this->radian_ = orxonox::Radian(0);
    else if (type == MT_degree)
        this->degree_ = orxonox::Degree(0);
}

bool MultiTypeMath::operator==(const MultiTypeMath& mtm) const
{
    if (!MultiTypeString::operator==(mtm) && this->type_ == mtm.type_)
    {
        if (this->type_ == MT_vector2)
            return (this->vector2_ == mtm.vector2_);
        else if (this->type_ == MT_vector3)
            return (this->vector3_ == mtm.vector3_);
        else if (this->type_ == MT_colourvalue)
            return (this->colourvalue_ == mtm.colourvalue_);
        else if (this->type_ == MT_quaternion)
            return (this->quaternion_ == mtm.quaternion_);
        else if (this->type_ == MT_radian)
            return (this->radian_ == mtm.radian_);
        else if (this->type_ == MT_degree)
            return (this->degree_ == mtm.degree_);
    }

    return false;
}

bool MultiTypeMath::operator!=(const MultiTypeMath& mtm) const
{
    if (MultiTypeString::operator==(mtm) && this->type_ == mtm.type_)
    {
        if (this->type_ == MT_vector2)
            return (this->vector2_ != mtm.vector2_);
        else if (this->type_ == MT_vector3)
            return (this->vector3_ != mtm.vector3_);
        else if (this->type_ == MT_colourvalue)
            return (this->colourvalue_ != mtm.colourvalue_);
        else if (this->type_ == MT_quaternion)
            return (this->quaternion_ != mtm.quaternion_);
        else if (this->type_ == MT_radian)
            return (this->radian_ != mtm.radian_);
        else if (this->type_ == MT_degree)
            return (this->degree_ != mtm.degree_);
    }

    return true;
}

MultiTypeMath::operator void*() const
{ return (this->type_ == MT_void) ? this->value_.void_ : getConvertedValue<MultiTypeMath, void*>(*this, 0); }
MultiTypeMath::operator int() const
{ return (this->type_ == MT_int) ? this->value_.int_ : getConvertedValue<MultiTypeMath, int>(*this, 0); }
MultiTypeMath::operator unsigned int() const
{ return (this->type_ == MT_uint) ? this->value_.uint_ : getConvertedValue<MultiTypeMath, unsigned int>(*this, 0); }
MultiTypeMath::operator char() const
{ return (this->type_ == MT_char) ? this->value_.char_ : getConvertedValue<MultiTypeMath, char>(*this, 0); }
MultiTypeMath::operator unsigned char() const
{ return (this->type_ == MT_uchar) ? this->value_.uchar_ : getConvertedValue<MultiTypeMath, unsigned char>(*this, 0); }
MultiTypeMath::operator short() const
{ return (this->type_ == MT_short) ? this->value_.short_ : getConvertedValue<MultiTypeMath, short>(*this, 0); }
MultiTypeMath::operator unsigned short() const
{ return (this->type_ == MT_ushort) ? this->value_.ushort_ : getConvertedValue<MultiTypeMath, unsigned short>(*this, 0); }
MultiTypeMath::operator long() const
{ return (this->type_ == MT_long) ? this->value_.long_ : getConvertedValue<MultiTypeMath, long>(*this, 0); }
MultiTypeMath::operator unsigned long() const
{ return (this->type_ == MT_ulong) ? this->value_.ulong_ : getConvertedValue<MultiTypeMath, unsigned long>(*this, 0); }
MultiTypeMath::operator float() const
{ return (this->type_ == MT_float) ? this->value_.float_ : getConvertedValue<MultiTypeMath, float>(*this, 0); }
MultiTypeMath::operator double() const
{ return (this->type_ == MT_double) ? this->value_.double_ : getConvertedValue<MultiTypeMath, double>(*this, 0); }
MultiTypeMath::operator long double() const
{ return (this->type_ == MT_longdouble) ? this->value_.longdouble_ : getConvertedValue<MultiTypeMath, long double>(*this, 0); }
MultiTypeMath::operator bool() const
{ return (this->type_ == MT_bool) ? this->value_.bool_ : getConvertedValue<MultiTypeMath, bool>(*this, 0); }
MultiTypeMath::operator std::string() const
{ return (this->type_ == MT_string) ? this->string_ : getConvertedValue<MultiTypeMath, std::string>(*this); }
MultiTypeMath::operator const char*() const
{ return ((this->type_ == MT_constchar) ? this->string_ : getConvertedValue<MultiTypeMath, std::string>(*this)).c_str(); }
MultiTypeMath::operator orxonox::Vector2() const
{ return (this->type_ == MT_vector2) ? this->vector2_ : getConvertedValue<MultiTypeMath, orxonox::Vector2>(*this); }
MultiTypeMath::operator orxonox::Vector3() const
{ return (this->type_ == MT_vector3) ? this->vector3_ : getConvertedValue<MultiTypeMath, orxonox::Vector3>(*this); }
MultiTypeMath::operator orxonox::Quaternion() const
{ return (this->type_ == MT_quaternion) ? this->quaternion_ : getConvertedValue<MultiTypeMath, orxonox::Quaternion>(*this); }
MultiTypeMath::operator orxonox::ColourValue() const
{ return (this->type_ == MT_colourvalue) ? this->colourvalue_ : getConvertedValue<MultiTypeMath, orxonox::ColourValue>(*this); }
MultiTypeMath::operator orxonox::Radian() const
{ return (this->type_ == MT_radian) ? this->radian_ : getConvertedValue<MultiTypeMath, orxonox::Radian>(*this); }
MultiTypeMath::operator orxonox::Degree() const
{ return (this->type_ == MT_degree) ? this->degree_ : getConvertedValue<MultiTypeMath, orxonox::Degree>(*this); }

void MultiTypeMath::setValue(const MultiTypeMath& mtm)
{
    MultiTypeString::setValue(mtm);
    this->vector2_ = mtm.vector2_;
    this->vector3_ = mtm.vector3_;
    this->quaternion_ = mtm.quaternion_;
    this->colourvalue_ = mtm.colourvalue_;
    this->radian_ = mtm.radian_;
    this->degree_ = mtm.degree_;
}

std::string MultiTypeMath::getTypename() const
{
    if (this->type_ == MT_vector2)
        return "Vector2";
    else if (this->type_ == MT_vector3)
        return "Vector3";
    else if (this->type_ == MT_colourvalue)
        return "ColourValue";
    else if (this->type_ == MT_quaternion)
        return "Quaternion";
    else if (this->type_ == MT_radian)
        return "Radian";
    else if (this->type_ == MT_degree)
        return "Degree";
    else
        return MultiTypeString::getTypename();
}

std::string MultiTypeMath::toString() const
{
    std::string output;

    if (this->type_ == MT_vector2)
        ConvertValue(&output, this->vector2_);
    else if (this->type_ == MT_vector3)
        ConvertValue(&output, this->vector3_);
    else if (this->type_ == MT_colourvalue)
        ConvertValue(&output, this->colourvalue_);
    else if (this->type_ == MT_quaternion)
        ConvertValue(&output, this->quaternion_);
    else if (this->type_ == MT_radian)
        ConvertValue(&output, this->radian_);
    else if (this->type_ == MT_degree)
        ConvertValue(&output, this->degree_);
    else
        return MultiTypeString::toString();

    return output;
}

bool MultiTypeMath::fromString(const std::string value)
{
    if (this->type_ == MT_vector2)
        return ConvertValue(&this->vector2_, value, orxonox::Vector2(0, 0));
    else if (this->type_ == MT_vector3)
        return ConvertValue(&this->vector3_, value, orxonox::Vector3(0, 0, 0));
    else if (this->type_ == MT_colourvalue)
        return ConvertValue(&this->colourvalue_, value, orxonox::ColourValue(0, 0, 0, 0));
    else if (this->type_ == MT_quaternion)
        return ConvertValue(&this->quaternion_, value, orxonox::Quaternion(1, 0, 0, 0));
    else if (this->type_ == MT_radian)
        return ConvertValue(&this->radian_, value, orxonox::Radian(0));
    else if (this->type_ == MT_degree)
        return ConvertValue(&this->degree_, value, orxonox::Degree(0));
    else
        return MultiTypeString::fromString(value);
}

std::ostream& operator<<(std::ostream& out, MultiTypeMath& mtm)
{
    out << mtm.toString();
    return out;
}
