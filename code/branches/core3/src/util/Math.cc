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
 */

#include <OgrePlane.h>

#include "Math.h"
#include "Convert.h"

/**
    @brief Function for writing to a stream.
*/
std::ostream& operator<<(std::ostream& out, const orxonox::Radian& radian)
{
    out << radian.valueRadians();
    return out;
}

/**
    @brief Function for reading from a stream.
*/
std::istream& operator>>(std::istream& in, orxonox::Radian& radian)
{
    float temp;
    in >> temp;
    radian = temp;
    return in;
}

/**
    @brief Function for writing to a stream.
*/
std::ostream& operator<<(std::ostream& out, const orxonox::Degree& degree)
{
    out << degree.valueDegrees();
    return out;
}

/**
    @brief Function for reading from a stream.
*/
std::istream& operator>>(std::istream& in, orxonox::Degree& degree)
{
    float temp;
    in >> temp;
    degree = temp;
    return in;
}


float getAngle(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& otherposition)
{
    orxonox::Vector3 distance = otherposition - myposition;
    return acos(mydirection.dotProduct(distance) / distance.length());
}

orxonox::Vector2 get2DViewdirection(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& myorthonormal, const orxonox::Vector3& otherposition)
{
    orxonox::Vector3 distance = otherposition - myposition;

    // project difference vector on our plane
    orxonox::Vector3 projection = Ogre::Plane(mydirection, myposition).projectVector(distance);
    float angle = acos(myorthonormal.dotProduct(projection) / projection.length());

    if ((mydirection.crossProduct(myorthonormal)).dotProduct(distance) > 0)
        return orxonox::Vector2(sin(angle), cos(angle));
    else
        return orxonox::Vector2(-sin(angle), cos(angle));
}

orxonox::Vector2 get2DViewcoordinates(const orxonox::Vector3& myposition, const orxonox::Vector3& mydirection, const orxonox::Vector3& myorthonormal, const orxonox::Vector3& otherposition)
{
    orxonox::Vector3 distance = otherposition - myposition;

    // project difference vector on our plane
    orxonox::Vector3 projection = Ogre::Plane(mydirection, myposition).projectVector(distance);
    float angle = acos(myorthonormal.dotProduct(projection) / projection.length());
    float radius = acos(mydirection.dotProduct(distance) / distance.length()) / Ogre::Math::PI;

    if ((mydirection.crossProduct(myorthonormal)).dotProduct(distance) > 0)
        return orxonox::Vector2(sin(angle) * radius, cos(angle) * radius);
    else
        return orxonox::Vector2(-sin(angle) * radius, cos(angle) * radius);
}

orxonox::Vector3 getPredictedPosition(const orxonox::Vector3& myposition, float projectilespeed, const orxonox::Vector3& targetposition, const orxonox::Vector3& targetvelocity)
{
    float squaredProjectilespeed = projectilespeed * projectilespeed;
    orxonox::Vector3 distance = targetposition - myposition;
    float a = distance.squaredLength();
    float b = 2 * (distance.x + distance.y + distance.z) * (targetvelocity.x + targetvelocity.y + targetvelocity.z);
    float c = targetvelocity.squaredLength();

    float temp = 4*squaredProjectilespeed*c + a*a - 4*b*c;
    if (temp < 0)
        return orxonox::Vector3::ZERO;

    temp = sqrt(temp);
    float time = (temp + a) / (2 * (squaredProjectilespeed - b));
    return (targetposition + targetvelocity * time);
}

// std::string to Vector2
bool explicitConversion(orxonox::Vector2* output, const std::string& input)
{
    unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
    if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

    SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
    if (tokens.size() >= 2)
    {
        if (!ConvertValue(&(output->x), tokens[0]))
            return false;
        if (!ConvertValue(&(output->y), tokens[1]))
            return false;

        return true;
    }
    return false;
}

// std::string to Vector3
bool explicitConversion(orxonox::Vector3* output, const std::string& input)
{
    unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
    if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

    SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
    if (tokens.size() >= 3)
    {
        if (!ConvertValue(&(output->x), tokens[0]))
            return false;
        if (!ConvertValue(&(output->y), tokens[1]))
            return false;
        if (!ConvertValue(&(output->z), tokens[2]))
            return false;

        return true;
    }
    return false;
}

// std::string to Vector4
bool explicitConversion(orxonox::Vector4* output, const std::string& input)
{
    unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
    if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

    SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
    if (tokens.size() >= 4)
    {
        if (!ConvertValue(&(output->x), tokens[0]))
            return false;
        if (!ConvertValue(&(output->y), tokens[1]))
            return false;
        if (!ConvertValue(&(output->z), tokens[2]))
            return false;
        if (!ConvertValue(&(output->w), tokens[3]))
            return false;

        return true;
    }
    return false;
}

// std::string to Quaternion
bool explicitConversion(orxonox::Quaternion* output, const std::string& input)
{
    unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
    if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

    SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
    if (tokens.size() >= 4)
    {
        if (!ConvertValue(&(output->w), tokens[0]))
            return false;
        if (!ConvertValue(&(output->x), tokens[1]))
            return false;
        if (!ConvertValue(&(output->y), tokens[2]))
            return false;
        if (!ConvertValue(&(output->z), tokens[3]))
            return false;

        return true;
    }
    return false;
}

// std::string to ColourValue
bool explicitConversion(orxonox::ColourValue* output, const std::string& input)
{
    unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
    if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

    SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
    if (tokens.size() >= 4)
    {
        if (!ConvertValue(&(output->r), tokens[0]))
            return false;
        if (!ConvertValue(&(output->g), tokens[1]))
            return false;
        if (!ConvertValue(&(output->b), tokens[2]))
            return false;
        if (!ConvertValue(&(output->a), tokens[3]))
            return false;

        return true;
    }
    return false;
}