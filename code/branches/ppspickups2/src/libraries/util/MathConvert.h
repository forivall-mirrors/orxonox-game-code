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
#include "Math.h"
#include "Convert.h"

namespace orxonox
{
    ////////////////////
    // Math to string //
    ////////////////////

    // Vector2 to std::string
    template <>
    struct ConverterExplicit<orxonox::Vector2, std::string>
    {
        FORCEINLINE static bool convert(std::string* output, const orxonox::Vector2& input)
        {
            std::ostringstream ostream;
            if (ostream << input.x << ',' << input.y)
            {
                (*output) = ostream.str();
                return true;
            }
            return false;
        }
    };

    // Vector3 to std::string
    template <>
    struct ConverterExplicit<orxonox::Vector3, std::string>
    {
        FORCEINLINE static bool convert(std::string* output, const orxonox::Vector3& input)
        {
            std::ostringstream ostream;
            if (ostream << input.x << ',' << input.y << ',' << input.z)
            {
                (*output) = ostream.str();
                return true;
            }
            return false;
        }
    };

    // Vector4 to std::string
    template <>
    struct ConverterExplicit<orxonox::Vector4, std::string>
    {
        FORCEINLINE static bool convert(std::string* output, const orxonox::Vector4& input)
        {
            std::ostringstream ostream;
            if (ostream << input.x << ',' << input.y << ',' << input.z << ',' << input.w)
            {
                (*output) = ostream.str();
                return true;
            }
            return false;
        }
    };

    // Quaternion to std::string
    template <>
    struct ConverterExplicit<orxonox::Quaternion, std::string>
    {
        FORCEINLINE static bool convert(std::string* output, const orxonox::Quaternion& input)
        {
            std::ostringstream ostream;
            if (ostream << input.w << ',' << input.x << ',' << input.y << ',' << input.z)
            {
                (*output) = ostream.str();
                return true;
            }
            return false;
        }
    };

    // ColourValue to std::string
    template <>
    struct ConverterExplicit<orxonox::ColourValue, std::string>
    {
        FORCEINLINE static bool convert(std::string* output, const orxonox::ColourValue& input)
        {
            std::ostringstream ostream;
            if (ostream << input.r << ',' << input.g << ',' << input.b << ',' << input.a)
            {
                (*output) = ostream.str();
                return true;
            }
            return false;
        }
    };


    ////////////////////
    // string to Math //
    ////////////////////

    // std::string to Vector2
    template <> struct _UtilExport ConverterFallback<std::string, orxonox::Vector2>
    { static bool convert(orxonox::Vector2*     output, const std::string& input); };
    // std::string to Vector3
    template <> struct _UtilExport ConverterFallback<std::string, orxonox::Vector3>
    { static bool convert(orxonox::Vector3*     output, const std::string& input); };
    // std::string to Vector4
    template <> struct _UtilExport ConverterFallback<std::string, orxonox::Vector4>
    { static bool convert(orxonox::Vector4*     output, const std::string& input); };
    // std::string to Quaternion
    template <> struct _UtilExport ConverterFallback<std::string, orxonox::Quaternion>
    { static bool convert(orxonox::Quaternion*  output, const std::string& input); };
    // std::string to ColourValue
    template <> struct _UtilExport ConverterFallback<std::string, orxonox::ColourValue>
    { static bool convert(orxonox::ColourValue* output, const std::string& input); };


    ///////////////////////////////
    // From and to Radian/Degree //
    ///////////////////////////////

    // From Radian
    template <class ToType>
    struct ConverterFallback<orxonox::Radian, ToType>
    {
        FORCEINLINE static bool convert(ToType* output, const orxonox::Radian& input)
        {
            return convertValue<Ogre::Real, ToType>(output, input.valueRadians());
        }
    };

    // From Degree
    template <class ToType>
    struct ConverterFallback<orxonox::Degree, ToType>
    {
        FORCEINLINE static bool convert(ToType* output, const orxonox::Degree& input)
        {
            return convertValue<Ogre::Real, ToType>(output, input.valueDegrees());
        }
    };

    // To Radian
    template <class FromType>
    struct ConverterFallback<FromType, orxonox::Radian>
    {
        FORCEINLINE static bool convert(orxonox::Radian* output, const FromType& input)
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
    };

    // To Degree
    template <class FromType>
    struct ConverterFallback<FromType, orxonox::Degree>
    {
        FORCEINLINE static bool convert(orxonox::Degree* output, const FromType& input)
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
    };
}

#endif /* _MathConvert_H__ */
