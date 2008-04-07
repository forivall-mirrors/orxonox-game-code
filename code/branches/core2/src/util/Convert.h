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


//////////
// Main //
//////////
/*
enum ConversionPreference
{
    CP_ToType,
    CP_FromType
};

// Helper classes to determine the preferred partial template specialization
class _AnyType_  {};
class _ToType_   {} static __to__;
class _FromType_ {} static __from__;
class _Explicit_ {} static __explicit__;


// The default convert functions
template <class FromType, class ToType>
static bool convert(ToType* output, const FromType& input, _ToType_* type)
{ std::cout << "default to" << std::endl; return false; }
template <class FromType, class ToType>
static bool convert(ToType* output, const FromType& input, _FromType_* type)
{ std::cout << "default from" << std::endl; return false; }
template <class FromType, class ToType>
static bool convert(ToType* output, const FromType& input, _Explicit_* type)
{ std::cout << "default explicit" << std::endl; return false; }


// The default convert function if both types are the same
template <class BothTypes>
static bool convert(BothTypes* output, const BothTypes& input, _Explicit_* type)
{ (*output) = input; return true; }


// The default conversion of primitives: A typecast (defined over two partial specialized templates to exclude all non-primitive types and classes)
template <class FromType, class ToType, class Type>
static bool convertDefault(ToType* output, const FromType& input, Type* type)
{ return false; }
#define CONVERT_PRIMITIVE_DEFAULT(primitive) \
template <class FromType> static bool convertDefault(primitive* output, const FromType& input, _ToType_* type) { return convertDefault(output, input, &__from__); } \
template <class ToType>   static bool convertDefault(ToType* output, const primitive& input, _FromType_* type) { (*output) = (ToType)input; return true; }
CONVERT_PRIMITIVE_DEFAULT(int)
CONVERT_PRIMITIVE_DEFAULT(unsigned int)
CONVERT_PRIMITIVE_DEFAULT(char)
CONVERT_PRIMITIVE_DEFAULT(unsigned char)
CONVERT_PRIMITIVE_DEFAULT(short)
CONVERT_PRIMITIVE_DEFAULT(unsigned short)
CONVERT_PRIMITIVE_DEFAULT(long)
CONVERT_PRIMITIVE_DEFAULT(unsigned long)
CONVERT_PRIMITIVE_DEFAULT(float)
CONVERT_PRIMITIVE_DEFAULT(double)
CONVERT_PRIMITIVE_DEFAULT(long double)
CONVERT_PRIMITIVE_DEFAULT(bool)

// Calls all four possibilities of converting two values: (the order of 2 and 3 can be changed by setting bPreferToType to false)
// 1) explicit specialization
// 2) partial specialization for ToType
// 3) partial specialization of the FromType
// 4) default conversion if available
template<class FromType, class ToType>
static bool convertValue(ToType* output, const FromType& input, ConversionPreference preference = CP_ToType)
{
std::cout << "1_1\n";
    if (convert(output, input, &__explicit__))
        return true;

std::cout << "1_2\n";
    if (preference == CP_ToType)
    {
std::cout << "1_3\n";
        if (convert(output, input, &__to__))
            return true;
std::cout << "1_4\n";
        if (convert(output, input, &__from__))
            return true;
std::cout << "1_5\n";
    }
    else
    {
std::cout << "1_6\n";
        if (convert(output, input, &__from__))
            return true;
std::cout << "1_7\n";
        if (convert(output, input, &__to__))
            return true;
std::cout << "1_8\n";
    }
std::cout << "1_9\n";
    return convertDefault(output, input, &__to__);
}
*/

    // Enum to declare the wanted conversion preference in case of equal type-levels
    enum ConversionPreference
    {
        CP_ToType,
        CP_FromType
    };

    // Helper classes to determine the preferred partial template specialization
    class _AnyType_  {};
    class _ToType_   {} static __to__;
    class _FromType_ {} static __from__;
    class _Explicit_ {} static __explicit__;


    // The default convert functions
    template <class FromType, class ToType>
    static bool convert(ToType* output, const FromType& input, _ToType_* type)
    { std::cout << "default to" << std::endl; return false; }
    template <class FromType, class ToType>
    static bool convert(ToType* output, const FromType& input, _FromType_* type)
    { std::cout << "default from" << std::endl; return false; }
    template <class FromType, class ToType>
    static bool convert(ToType* output, const FromType& input, _Explicit_* type)
    { std::cout << "default explicit" << std::endl; return false; }


    // The default convert function if both types are the same
    template <class BothTypes>
    static bool convert(BothTypes* output, const BothTypes& input, _Explicit_* type)
    { (*output) = input; return true; }


    // The possible levels
    #define __low__  0
    #define __mid__  1
    #define __high__ 2

    // Defines the levels of all types
    template <class T> struct ConverterLeveL          { enum { level = __high__ }; };
    template <> struct ConverterLeveL<std::string>    { enum { level = __mid__ }; };
    template <> struct ConverterLeveL<int>            { enum { level = __low__ }; };
    template <> struct ConverterLeveL<unsigned int>   { enum { level = __low__ }; };
    template <> struct ConverterLeveL<char>           { enum { level = __low__ }; };
    template <> struct ConverterLeveL<unsigned char>  { enum { level = __low__ }; };
    template <> struct ConverterLeveL<short>          { enum { level = __low__ }; };
    template <> struct ConverterLeveL<unsigned short> { enum { level = __low__ }; };
    template <> struct ConverterLeveL<long>           { enum { level = __low__ }; };
    template <> struct ConverterLeveL<unsigned long>  { enum { level = __low__ }; };
    template <> struct ConverterLeveL<float>          { enum { level = __low__ }; };
    template <> struct ConverterLeveL<double>         { enum { level = __low__ }; };
    template <> struct ConverterLeveL<long double>    { enum { level = __low__ }; };
    template <> struct ConverterLeveL<bool>           { enum { level = __low__ }; };


    // Calculates the preference based on the levels of FromType and ToType
    template <int from, int to>
    struct ConverterPreference
    {
        enum
        {
            max = (from > to) ? from : to,
            diff = ((to - from) > 1) ? 1 : (((to - from) < -1) ? -1 : to - from)
        };
    };


    // The default conversion: This usually does nothing
    template <int max, class FromType, class ToType, class Type>
    struct ConverterDefault
    {
        static bool convert(ToType* output, const FromType& input, Type* type)
        {
            return false;
        }
    };
    // The default conversion for primitives: A typecast (defined over two partial specialized templates to exclude all non-primitive types and classes)    template <int max, class FromType, class ToType>
    template <class FromType, class ToType, class Type>
    struct ConverterDefault<0, FromType, ToType, Type>
    {
        static bool convert(ToType* output, const FromType& input, Type* type)
        {
            (*output) = (ToType)input;
            return true;
        }
    };


    // Converter: Converts input of FromType into output of ToType
    template <int diff, int max, class FromType, class ToType>
    struct Converter
    {
        static bool convertValue(ToType* output, const FromType& input, ConversionPreference preference = CP_ToType)
        {
            return false;
        }
    };
    // Converter: FromType-level > ToType-level
    template <int max, class FromType, class ToType>
    struct Converter<-1, max, FromType, ToType>
    {
        static bool convertValue(ToType* output, const FromType& input, ConversionPreference preference = CP_ToType)
        {
            if (convert(output, input, &__explicit__))
                return true;
            if (convert(output, input, &__from__))
                return true;
            if (ConverterDefault<max, FromType, ToType, _ToType_>::convert(output, input, &__to__))
                return true;

            return false;
        }
    };
    // Converter: ToType-level > FromType-level
    template <int max, class FromType, class ToType>
    struct Converter<1, max, FromType, ToType>
    {
        static bool convertValue(ToType* output, const FromType& input, ConversionPreference preference = CP_ToType)
        {
            if (convert(output, input, &__explicit__))
                return true;
            if (convert(output, input, &__to__))
                return true;
            if (ConverterDefault<max, FromType, ToType, _ToType_>::convert(output, input, &__to__))
                return true;

            return false;
        }
    };
    // Converter: ToType-level = ToType-level
    template <int max, class FromType, class ToType>
    struct Converter<0, max, FromType, ToType>
    {
        static bool convertValue(ToType* output, const FromType& input, ConversionPreference preference = CP_ToType)
        {
            if (convert(output, input, &__explicit__))
                return true;

            if (preference == CP_ToType)
            {
                if (convert(output, input, &__to__))
                    return true;
                if (convert(output, input, &__from__))
                    return true;
            }
            else
            {
                if (convert(output, input, &__from__))
                    return true;
                if (convert(output, input, &__to__))
                    return true;
            }

            if (ConverterDefault<max, FromType, ToType, _ToType_>::convert(output, input, &__to__))
                return true;

            return false;
        }
    };


    // Calls the Converter::convertValue function with the correct template type parameters calculated by ConverterPreference
    template <class FromType, class ToType>
    static bool convertValue(ToType* output, const FromType& input, ConversionPreference preference = CP_ToType)
    {
        return Converter<ConverterPreference<ConverterLeveL<FromType>::level, ConverterLeveL<ToType>::level>::diff, ConverterPreference<ConverterLeveL<FromType>::level, ConverterLeveL<ToType>::level>::max, FromType, ToType>::convertValue(output, input, preference);
    }


// Helper function: Calls convertValue with and without default value and returns true if the conversion was successful
template<class FromType, class ToType>
static bool ConvertValue(ToType* output, const FromType& input, ConversionPreference preference = CP_ToType)
{
    return convertValue(output, input, preference);
}
template<class FromType, class ToType>
static bool ConvertValue(ToType* output, const FromType& input, const ToType& fallback, ConversionPreference preference = CP_ToType)
{
    if (convertValue(output, input, preference))
        return true;

    (*output) = fallback;
    return false;
}

// Helper function: Calls convertValue with and without default value and returns the converted value
template<class FromType, class ToType>
static ToType getConvertedValue(const FromType& input, ConversionPreference preference = CP_ToType)
{
    ToType output = ToType();
    ConvertValue(&output, input, preference);
    return output;
}
template<class FromType, class ToType>
static ToType getConvertedValue(const FromType& input, const ToType& fallback, ConversionPreference preference = CP_ToType)
{
    ToType output = fallback;
    ConvertValue(&output, input, fallback, preference);
    return output;
}

/////////////////////
// SPECIALISATIONS //
/////////////////////

/*
template <class FromType>
static bool convertDefault(primitive* output, const FromType& input, _ToType_* type)
{

}

template <class ToType>
static bool convertDefault(ToType* output, const primitive& input, _FromType_* type)
{

}


template <>
static bool convertDefault(ToType* output, const FromType& input, _Explicit_* type)
{

}
*/

////////////
// String //
////////////

// convert to string
template <class FromType>
static bool convert(std::string* output, const FromType& input, _ToType_* type)
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

// convert from string
template <class ToType>
static bool convert(ToType* output, const std::string& input, _FromType_* type)
{
    std::istringstream iss(input);
    if (iss >> (*output))
        return true;
    else
        return false;
}


////////////////
// MULTITYPES //
////////////////

// convert from MultiTypePrimitive
template <class ToType>
static bool convert(ToType* output, const MultiTypePrimitive& input, _FromType_* type)
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

// convert from MultiTypeString
template <class ToType>
static bool convert(ToType* output, const MultiTypeString& input, _FromType_* type)
{
    if (input.getType() == MT_constchar)
        return ConvertValue(output, input.getConstChar());
    else if (input.getType() == MT_string)
        return ConvertValue(output, input.getString());
    else
        return ConvertValue(output, (MultiTypePrimitive)input);
}

// convert from MultiTypeMath
template <class ToType>
static bool convert(ToType* output, const MultiTypeMath& input, _FromType_* type)
{
    if (input.getType() == MT_vector2)
        return ConvertValue(output, input.getVector2(), CP_FromType);
    else if (input.getType() == MT_vector3)
        return ConvertValue(output, input.getVector3(), CP_FromType);
    else if (input.getType() == MT_quaternion)
        return ConvertValue(output, input.getQuaternion(), CP_FromType);
    else if (input.getType() == MT_colourvalue)
        return ConvertValue(output, input.getColourValue(), CP_FromType);
    else if (input.getType() == MT_radian)
        return ConvertValue(output, input.getRadian());
    else if (input.getType() == MT_degree)
        return ConvertValue(output, input.getDegree());
    else
        return ConvertValue(output, (MultiTypeString)input);
}


////////////////////
// MATH TO STRING //
////////////////////

// Vector2 to std::string
template <>
static bool convert(std::string* output, const orxonox::Vector2& input, _Explicit_* type)
{
    std::ostringstream ostream;
    if (ostream << input.x << "," << input.y)
    {
        (*output) = ostream.str();
        return true;
    }
    return false;
}

/*
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
*/
// Vector3 to std::string
template <>
static bool convert(std::string* output, const orxonox::Vector3& input, _Explicit_* type)
{
    std::ostringstream ostream;
    if (ostream << input.x << "," << input.y << "," << input.z)
    {
        (*output) = ostream.str();
        return true;
    }
    return false;
}

/*
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
*/
// Vector4 to std::string
template <>
static bool convert(std::string* output, const orxonox::Vector4& input, _Explicit_* type)
{
    std::ostringstream ostream;
    if (ostream << input.x << "," << input.y << "," << input.z << "," << input.w)
    {
        (*output) = ostream.str();
        return true;
    }
    return false;
}
/*
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
*/
// Quaternion to std::string
template <>
static bool convert(std::string* output, const orxonox::Quaternion& input, _Explicit_* type)
{
    std::ostringstream ostream;
    if (ostream << input.w << "," << input.x << "," << input.y << "," << input.z)
    {
        (*output) = ostream.str();
        return true;
    }
    return false;
}
/*
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
*/
// ColourValue to std::string
template <>
static bool convert(std::string* output, const orxonox::ColourValue& input, _Explicit_* type)
{
    std::ostringstream ostream;
    if (ostream << input.r << "," << input.g << "," << input.b << "," << input.a)
    {
        (*output) = ostream.str();
        return true;
    }
    return false;
}
/*
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
*/


////////////////////
// STRING TO MATH //
////////////////////

// std::string to Vector2
template <>
static bool convert(orxonox::Vector2* output, const std::string& input, _Explicit_* type)
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
/*
// std::string to Vector2
template <>
class Converter<std::string, orxonox::Vector2>
{
  public:
    bool operator()(orxonox::Vector2* output, const std::string& input) const
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
};
*/
// std::string to Vector3
template <>
static bool convert(orxonox::Vector3* output, const std::string& input, _Explicit_* type)
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
/*
// std::string to Vector3
template <>
class Converter<std::string, orxonox::Vector3>
{
  public:
    bool operator()(orxonox::Vector3* output, const std::string& input) const
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
};
*/
// std::string to Vector4
template <>
static bool convert(orxonox::Vector4* output, const std::string& input, _Explicit_* type)
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
/*
// std::string to Vector4
template <>
class Converter<std::string, orxonox::Vector4>
{
  public:
    bool operator()(orxonox::Vector4* output, const std::string& input) const
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
};
*/
// std::string to Quaternion
template <>
static bool convert(orxonox::Quaternion* output, const std::string& input, _Explicit_* type)
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
/*
// std::string to Quaternion
template <>
class Converter<std::string, orxonox::Quaternion>
{
  public:
    bool operator()(orxonox::Quaternion* output, const std::string& input) const
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
};
*/
// std::string to ColourValue
template <>
static bool convert(orxonox::ColourValue* output, const std::string& input, _Explicit_* type)
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
/*
// std::string to ColourValue
template <>
class Converter<std::string, orxonox::ColourValue>
{
  public:
    bool operator()(orxonox::ColourValue* output, const std::string& input) const
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
};
*/


#endif /* _Convert_H__ */
