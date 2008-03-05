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

MultiTypeMath::operator orxonox::Vector2() const
{
    return (this->type_ == MT_vector2) ? this->vector2_ : ConvertValueAndReturn<MultiTypeMath, orxonox::Vector2>(*this);
}

MultiTypeMath::operator orxonox::Vector3() const
{
    return (this->type_ == MT_vector3) ? this->vector3_ : ConvertValueAndReturn<MultiTypeMath, orxonox::Vector3>(*this);
}

MultiTypeMath::operator orxonox::Quaternion() const
{
    return (this->type_ == MT_quaternion) ? this->quaternion_ : ConvertValueAndReturn<MultiTypeMath, orxonox::Quaternion>(*this);
}

MultiTypeMath::operator orxonox::ColourValue() const
{
    return (this->type_ == MT_colourvalue) ? this->colourvalue_ : ConvertValueAndReturn<MultiTypeMath, orxonox::ColourValue>(*this);
}

MultiTypeMath::operator orxonox::Radian() const
{
    return (this->type_ == MT_radian) ? this->radian_ : ConvertValueAndReturn<MultiTypeMath, orxonox::Radian>(*this);
}

MultiTypeMath::operator orxonox::Degree() const
{
    return (this->type_ == MT_degree) ? this->degree_ : ConvertValueAndReturn<MultiTypeMath, orxonox::Degree>(*this);
}

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
    if (mtm.isA(MT_vector2))
        out << mtm.getVector2();
    else if (mtm.isA(MT_vector3))
        out << mtm.getVector3();
    else if (mtm.isA(MT_colourvalue))
        out << mtm.getColourValue();
    else if (mtm.isA(MT_quaternion))
        out << mtm.getQuaternion();
    else if (mtm.isA(MT_radian))
        out << mtm.getRadian();
    else if (mtm.isA(MT_degree))
        out << mtm.getDegree();
    else
        out << ((MultiTypeString)mtm);

    return out;
}
