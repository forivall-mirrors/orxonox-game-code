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
 *      Benjamin Grauer
 *   Co-authors:
 *      Fabian 'x3n' Landau
 */

/*!
    @file Convert.h
    @brief Definition and Implementation of the Convert class.
*/

#ifndef _Convert_H__
#define _Convert_H__

#include <string>
#include <sstream>

#include "UtilPrereqs.h"
#include "Math.h"

// DEFAULT CLASS
template <typename FromType, typename ToType>
class Converter
{
  public:
    bool operator()(ToType* output, const FromType& input) const
    {
      return false;
    }
};

// PARTIAL SPECIALIZATION TO CONVERT TO STRINGS
template<typename FromType>
class Converter<FromType, std::string>
{
  public:
    bool operator()(std::string* output, const FromType& input) const
    {
      std::ostringstream oss;
      if (oss << input)
      {
        (*output) = oss.str();
        return true;
      }
      else
        return false;
    }
};

// PARTIAL SPECIALIZATION TO CONVERT FROM STRING
template<typename ToType>
class Converter<std::string, ToType>
{
  public:
    bool operator()(ToType* output, const std::string& input) const
    {
      std::istringstream iss(input);
      if (iss >> (*output))
        return true;
      else
        return false;
    }
};

// FUNCTION SO WE DO NOT HAVE TO TELL THE COMPILER ABOUT THE TYPE
template<typename FromType, typename ToType>
static bool ConvertValue(ToType* output, const FromType& input)
{
  Converter<FromType, ToType> converter;
  return converter(output, input);
}

// THE SAME, BUT WITH DEFAULT VALUE
template<typename FromType, typename ToType>
static bool ConvertValue(ToType* output, const FromType& input, const ToType& fallback)
{
  Converter<FromType, ToType> converter;
  if (converter(output, input))
    return true;

  (*output) = fallback;
  return false;
}



// MORE SPECIALISATIONS
// Vector2 to std::string
template <>
class Converter<orxonox::Vector2, std::string>
{
  public:
    bool operator()(std::string* output, const orxonox::Vector2& input) const
    {
      std::ostringstream ostream;
      if (ostream << input.x << "," << input.y)
      {
        (*output) = ostream.str();
        return true;
      }

      return false;
    }
};

// Vector3 to std::string
template <>
class Converter<orxonox::Vector3, std::string>
{
  public:
    bool operator()(std::string* output, const orxonox::Vector3& input) const
    {
      std::ostringstream ostream;
      if (ostream << input.x << "," << input.y << "," << input.z)
      {
        (*output) = ostream.str();
        return true;
      }

      return false;
    }
};

// Vector4 to std::string
template <>
class Converter<orxonox::Vector4, std::string>
{
  public:
    bool operator()(std::string* output, const orxonox::Vector4& input) const
    {
      std::ostringstream ostream;
      if (ostream << input.x << "," << input.y << "," << input.z << "," << input.w)
      {
        (*output) = ostream.str();
        return true;
      }

      return false;
    }
};

// Quaternion to std::string
template <>
class Converter<orxonox::Quaternion, std::string>
{
  public:
    bool operator()(std::string* output, const orxonox::Quaternion& input) const
    {
      std::ostringstream ostream;
      if (ostream << input.w << "," << input.x << "," << input.y << "," << input.z)
      {
        (*output) = ostream.str();
        return true;
      }

      return false;
    }
};

// ColourValue to std::string
template <>
class Converter<orxonox::ColourValue, std::string>
{
  public:
    bool operator()(std::string* output, const orxonox::ColourValue& input) const
    {
      std::ostringstream ostream;
      if (ostream << input.r << "," << input.g << "," << input.b << "," << input.a)
      {
        (*output) = ostream.str();
        return true;
      }

      return false;
    }
};

#endif /* _Convert_H__ */
