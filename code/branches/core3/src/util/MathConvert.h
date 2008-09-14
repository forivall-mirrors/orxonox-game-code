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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 */

/**
    @file
    @brief
        Math conversion functions. Definitions are in Math.cc
*/

#ifndef _MathConvert_H__
#define _MathConvert_H__

#include "UtilPrereqs.h"
#include "Convert.h"
#include "Math.h"


////////////////////
// Math to string //
////////////////////

// Vector2 to std::string
inline bool explicitConversion(std::string* output, const orxonox::Vector2& input)
{
    std::ostringstream ostream;
    if (ostream << input.x << "," << input.y)
    {
        (*output) = ostream.str();
        return true;
    }
    return false;
}

// Vector3 to std::string
inline bool explicitConversion(std::string* output, const orxonox::Vector3& input)
{
    std::ostringstream ostream;
    if (ostream << input.x << "," << input.y << "," << input.z)
    {
        (*output) = ostream.str();
        return true;
    }
    return false;
}

// Vector4 to std::string
inline bool explicitConversion(std::string* output, const orxonox::Vector4& input)
{
    std::ostringstream ostream;
    if (ostream << input.x << "," << input.y << "," << input.z << "," << input.w)
    {
        (*output) = ostream.str();
        return true;
    }
    return false;
}

// Quaternion to std::string
inline bool explicitConversion(std::string* output, const orxonox::Quaternion& input)
{
    std::ostringstream ostream;
    if (ostream << input.w << "," << input.x << "," << input.y << "," << input.z)
    {
        (*output) = ostream.str();
        return true;
    }
    return false;
}

// ColourValue to std::string
inline bool explicitConversion(std::string* output, const orxonox::ColourValue& input)
{
    std::ostringstream ostream;
    if (ostream << input.r << "," << input.g << "," << input.b << "," << input.a)
    {
        (*output) = ostream.str();
        return true;
    }
    return false;
}


////////////////////
// string to Math //
////////////////////

// std::string to Vector2
_UtilExport bool fallbackConversion(orxonox::Vector2* output, const std::string& input);
// std::string to Vector3
_UtilExport bool fallbackConversion(orxonox::Vector3* output, const std::string& input);
// std::string to Vector4
_UtilExport bool fallbackConversion(orxonox::Vector4* output, const std::string& input);
// std::string to Quaternion
_UtilExport bool fallbackConversion(orxonox::Quaternion* output, const std::string& input);
// std::string to ColourValue
_UtilExport bool fallbackConversion(orxonox::ColourValue* output, const std::string& input);


///////////////////////////////
// From and to Radian/Degree //
///////////////////////////////

// From Radian
template <class ToType>
inline bool fallbackConversion(ToType* output, const orxonox::Radian input)
{
    return convertValue<ToType, Ogre::Real>(output, input.valueRadians()); 
}

// From Degree
template <class ToType>
inline bool fallbackConversion(ToType* output, const orxonox::Degree input)
{
    return convertValue<ToType, Ogre::Real>(output, input.valueDegrees()); 
}

// To Radian
template <class FromType>
inline bool fallbackConversion(orxonox::Radian* output, const FromType input)
{
    float temp;
    if (convertValue(&temp, input))
    {
        *output = temp;
        return true;
    }
    else
        return false;
}

// To Degree
template <class FromType>
inline bool fallbackConversion(orxonox::Degree* output, const FromType input)
{
    float temp;
    if (convertValue(&temp, input))
    {
        *output = temp;
        return true;
    }
    else
        return false;
}

#endif /* _MathConvert_H__ */
