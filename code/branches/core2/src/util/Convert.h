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
#include "SubString.h"
#include "MultiTypeMath.h"

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

// THE SAME LIKE BEFORE, BUT NEEDS NO POINTER TO THE OUTPUT
template<typename FromType, typename ToType>
static ToType ConvertValueAndReturn(const FromType& input)
{
  ToType output;
  ConvertValue(&output, input);
  return output;
}

// THE SAME, BUT WITH DEFAULT VALUE
template<typename FromType, typename ToType>
static ToType ConvertValueAndReturn(const FromType& input, const FromType& fallback)
{
  ToType output;
  ConvertValue(&output, input, fallback);
  return output;
}

//////////////////////////
// MORE SPECIALISATIONS //
//////////////////////////

// STRING TO STRING
template<>
class Converter<std::string, std::string>
{
  public:
    bool operator()(std::string* output, const std::string& input) const
    {
        (*output) = std::string(input);
        return true;
    }
};

////////////////
// MULTITYPES //
////////////////

// PARTIAL SPECIALIZATION TO CONVERT FROM MULTITYPEPRIMITIVE
template<typename ToType>
class Converter<MultiTypePrimitive, ToType>
{
  public:
    bool operator()(ToType* output, const MultiTypePrimitive& input) const
    {
      if (input.getType() == MT_void)
        return ConvertValue(output, input.getVoid());
      else if (input.getType() == MT_int)
        return ConvertValue(output, input.getInt());
      else if (input.getType() == MT_uint)
        return ConvertValue(output, input.getUnsignedInt());
      else if (input.getType() == MT_char)
        return ConvertValue(output, input.getChar());
      else if (input.getType() == MT_uchar)
        return ConvertValue(output, input.getUnsignedChar());
      else if (input.getType() == MT_short)
        return ConvertValue(output, input.getShort());
      else if (input.getType() == MT_ushort)
        return ConvertValue(output, input.getUnsignedShort());
      else if (input.getType() == MT_long)
        return ConvertValue(output, input.getLong());
      else if (input.getType() == MT_ulong)
        return ConvertValue(output, input.getUnsignedLong());
      else if (input.getType() == MT_float)
        return ConvertValue(output, input.getFloat());
      else if (input.getType() == MT_double)
        return ConvertValue(output, input.getDouble());
      else if (input.getType() == MT_longdouble)
        return ConvertValue(output, input.getLongDouble());
      else if (input.getType() == MT_bool)
        return ConvertValue(output, input.getBool());
      else
        return false;
    }
};
template<>
class Converter<MultiTypePrimitive, std::string>
{
  public:
    bool operator()(std::string* output, const MultiTypePrimitive& input) const
    {
      if (input.getType() == MT_void)
        return ConvertValue(output, input.getVoid());
      else if (input.getType() == MT_int)
        return ConvertValue(output, input.getInt());
      else if (input.getType() == MT_uint)
        return ConvertValue(output, input.getUnsignedInt());
      else if (input.getType() == MT_char)
        return ConvertValue(output, input.getChar());
      else if (input.getType() == MT_uchar)
        return ConvertValue(output, input.getUnsignedChar());
      else if (input.getType() == MT_short)
        return ConvertValue(output, input.getShort());
      else if (input.getType() == MT_ushort)
        return ConvertValue(output, input.getUnsignedShort());
      else if (input.getType() == MT_long)
        return ConvertValue(output, input.getLong());
      else if (input.getType() == MT_ulong)
        return ConvertValue(output, input.getUnsignedLong());
      else if (input.getType() == MT_float)
        return ConvertValue(output, input.getFloat());
      else if (input.getType() == MT_double)
        return ConvertValue(output, input.getDouble());
      else if (input.getType() == MT_longdouble)
        return ConvertValue(output, input.getLongDouble());
      else if (input.getType() == MT_bool)
        return ConvertValue(output, input.getBool());
      else
        return false;
    }
};

// PARTIAL SPECIALIZATION TO CONVERT FROM MULTITYPESTRING
template<typename ToType>
class Converter<MultiTypeString, ToType>
{
  public:
    bool operator()(ToType* output, const MultiTypeString& input) const
    {
      if (input.getType() == MT_constchar)
        return ConvertValue(output, input.getConstChar());
      else if (input.getType() == MT_string)
        return ConvertValue(output, input.getString());
      else if (input.getType() == MT_xmlelement)
        return ConvertValue(output, input.getXMLElement());
      else
        return ConvertValue(output, (MultiTypePrimitive)input);
    }
};
template<>
class Converter<MultiTypeString, std::string>
{
  public:
    bool operator()(std::string* output, const MultiTypeString& input) const
    {
      if (input.getType() == MT_constchar)
        return ConvertValue(output, input.getConstChar());
      else if (input.getType() == MT_string)
        return ConvertValue(output, input.getString());
      else if (input.getType() == MT_xmlelement)
        return ConvertValue(output, input.getXMLElement());
      else
        return ConvertValue(output, (MultiTypePrimitive)input);
    }
};

// PARTIAL SPECIALIZATION TO CONVERT FROM MULTITYPEMATH
template<typename ToType>
class Converter<MultiTypeMath, ToType>
{
  public:
    bool operator()(ToType* output, const MultiTypeMath& input) const
    {
      if (input.getType() == MT_vector2)
        return ConvertValue(output, input.getVector2());
      else if (input.getType() == MT_vector3)
        return ConvertValue(output, input.getVector3());
      else if (input.getType() == MT_quaternion)
        return ConvertValue(output, input.getQuaternion());
      else if (input.getType() == MT_colourvalue)
        return ConvertValue(output, input.getColourValue());
      else if (input.getType() == MT_radian)
        return ConvertValue(output, input.getRadian());
      else if (input.getType() == MT_degree)
        return ConvertValue(output, input.getDegree());
      else
        return ConvertValue(output, (MultiTypeString)input);
    }
};
template<>
class Converter<MultiTypeMath, std::string>
{
  public:
    bool operator()(std::string* output, const MultiTypeMath& input) const
    {
      if (input.getType() == MT_vector2)
        return ConvertValue(output, input.getVector2());
      else if (input.getType() == MT_vector3)
        return ConvertValue(output, input.getVector3());
      else if (input.getType() == MT_quaternion)
        return ConvertValue(output, input.getQuaternion());
      else if (input.getType() == MT_colourvalue)
        return ConvertValue(output, input.getColourValue());
      else if (input.getType() == MT_radian)
        return ConvertValue(output, input.getRadian());
      else if (input.getType() == MT_degree)
        return ConvertValue(output, input.getDegree());
      else
        return ConvertValue(output, (MultiTypeString)input);
    }
};


////////////////////
// MATH TO STRING //
////////////////////

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


////////////////////
// STRING TO MATH //
////////////////////

// std::string to Vector2
template <>
class Converter<std::string, orxonox::Vector2>
{
  public:
    bool operator()(orxonox::Vector2* output, const std::string& input) const
    {
      unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
      if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

      SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', '"', '\0', '\0', '\0');

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
};

// std::string to Vector3
template <>
class Converter<std::string, orxonox::Vector3>
{
  public:
    bool operator()(orxonox::Vector3* output, const std::string& input) const
    {
      unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
      if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

      SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', '"', '\0', '\0', '\0');

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
};

// std::string to Vector4
template <>
class Converter<std::string, orxonox::Vector4>
{
  public:
    bool operator()(orxonox::Vector4* output, const std::string& input) const
    {
      unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
      if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

      SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', '"', '\0', '\0', '\0');

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
};

// std::string to Quaternion
template <>
class Converter<std::string, orxonox::Quaternion>
{
  public:
    bool operator()(orxonox::Quaternion* output, const std::string& input) const
    {
      unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
      if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

      SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', '"', '\0', '\0', '\0');

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
};

// std::string to ColourValue
template <>
class Converter<std::string, orxonox::ColourValue>
{
  public:
    bool operator()(orxonox::ColourValue* output, const std::string& input) const
    {
      unsigned int opening_parenthesis, closing_parenthesis = input.find(')');
      if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

      SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', '"', '\0', '\0', '\0');

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
};


////////////////
// XMLElement //
////////////////

// orxonox::Element to std::string
template <>
class Converter<orxonox::Element, std::string>
{
  public:
    bool operator()(std::string* output, const orxonox::Element& input) const
    {
      std::ostringstream ostream;
      if (ostream << input)
      {
        (*output) = ostream.str();
        return true;
      }

      return false;
    }
};

// std::string to orxonox::Element
template <>
class Converter<std::string, orxonox::Element>
{
  public:
    bool operator()(orxonox::Element* output, const std::string& input) const
    {
      std::istringstream istream(input);
      if (istream >> (*output))
        return true;

      return false;
    }
};


#endif /* _Convert_H__ */
