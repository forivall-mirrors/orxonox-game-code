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
 *      Benjamin Grauer
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 */

/*!
    @file Convert.h
    @brief Definition and Implementation of the Convert class.
*/

#ifndef _Convert_H__
#define _Convert_H__

#include "UtilPrereqs.h"

#include <string>
#include <sstream>
#include <istream>
#include <boost/static_assert.hpp>

#include "Math.h"
#include "Debug.h"
#include "SubString.h"

///////////////////////////////////
// Explicit Conversion Functions //
///////////////////////////////////

inline bool explicitConversion(std::string* output, const char input)
{
    return true;
}
inline bool explicitConversion(std::string* output, const unsigned char input)
{
    return true;
}
inline bool explicitConversion(char* output, const std::string input)
{
    return true;
}
inline bool explicitConversion(unsigned char* output, const std::string input)
{
    return true;
}

///////////////////////////
// Static type detection //
///////////////////////////

/* The idea to use the sizeof() operator on return functions to determine function existance
   is described in 'Moder C++ design' by Alexandrescu (2001). */

template <int a, int b>
struct TemplateDebugger
{
    static int debug(int c, int d) { BOOST_STATIC_ASSERT(0); }
};

namespace conversionTests
{

    // Two struct with very different sizes
    struct VerySmallStruct { char dummy[1]; };
    struct VeryBigStruct   { char dummy[1024]; }; // Big is surely larger than Small, even with alignments
}

namespace generalFunctionTemplate
{
    // Keep this function out of conversion namespaces because the compiler gives a general
    // template in the same namesapace a higher priority than any specialisation.
    // This function simply accepts anything but has lower priority to specialisations.
    // It can be identified by the larger return value.
    template <class AnyToType, class AnyFromType>
    conversionTests::VeryBigStruct explicitConversion(AnyToType* output, const AnyFromType input)
    {
        // The function is exposed globally because of the "using namespace" in conversionTests.
        // We have to make sure noboy uses it, so a good compiler error would be nice.
        *output = (AnyToType)input; // Do not use this function!
        BOOST_STATIC_ASSERT(0); // just to be sure
    }
}

namespace conversionTests
{
    using namespace generalFunctionTemplate; // Why in separate namespace? See above

    template <class Any>
    conversionTests::VeryBigStruct operator<<(std::ostream& outstream, const Any anything);
    template <class Any>
    conversionTests::VeryBigStruct operator>>(std::istream& instream,  const Any anything);

    template <class FromType, class ToType>
    class ImplicitConversion
    {
    private:
        static VerySmallStruct test(ToType); // only accepts ToType, but is preferred over '...'
        static VeryBigStruct   test(...);    // accepts anything
        static FromType object; // helper object to handle private c'tor and d'tor
    public:
        enum { exists = sizeof(test(object)) == sizeof(VerySmallStruct) };
    };

    template <class FromType, class ToType, int asdf>
    class ExplicitConversion
    {
    private:
        static FromType objectFromType; // helper object to handle private c'tor and d'tor
        static ToType   objectToType;   // helper object to handle private c'tor and d'tor
    public:
        enum { exists = sizeof(explicitConversion(&objectToType, objectFromType)) == sizeof(VerySmallStruct) };
        static void test();
    };

    template <int asdf>
    class ExplicitConversion<float, int, asdf>
    {
    private:
        static float objectFromType; // helper object to handle private c'tor and d'tor
        static int   objectToType;   // helper object to handle private c'tor and d'tor
    public:
        enum { exists = sizeof(explicitConversion(&objectToType, objectFromType)) == sizeof(VerySmallStruct) };
        static void test() { TemplateDebugger<sizeof(explicitConversion(&objectToType, objectFromType)), sizeof(VerySmallStruct)>::debug(1,2); }
    };

    template <class Type>
    class IStringStreamOperator
    {
        static std::istringstream istream_; // helper object to perform the '>>' operation
        static Type object;                 // helper object to handle private c'tor and d'tor
    public:
        enum { exists = (sizeof(istream_ >> object) < sizeof(VerySmallStruct) + 512) };
    };

    template <class Type>
    class OStringStreamOperator
    {
        static std::ostringstream ostream_; // helper object to perform the '<<' operation
        static Type object;                 // helper object to handle private c'tor and d'tor
    public:
        enum { exists = (sizeof(ostream_ << object) < sizeof(VerySmallStruct) + 512) };
    };
}

// shortcut without namespace
template <class FromType, class ToType>
struct ImplicitConversion
{ enum { exists = conversionTests::ImplicitConversion<FromType, ToType>::exists }; };

// shortcut without namespace
template <class FromType, class ToType>
struct ExplicitConversion
{ enum { exists = conversionTests::ExplicitConversion<FromType, ToType, 4>::exists }; };

// shortcut without namespace
template <class Type>
struct IStringStreamOperator
{ enum { exists = conversionTests::IStringStreamOperator<Type>::exists }; };

// shortcut without namespace
template <class Type>
struct OStringStreamOperator
{ enum { exists = conversionTests::OStringStreamOperator<Type>::exists }; };


///////////////////////////////
// Conversion Template Stuff //
///////////////////////////////

namespace conversion
{
    // Maps bools to types in order to use function overloading instead of template specialisation (Alexandrescu 2001)
    template <bool WhetherOrNot>
    struct ImplicitPossible { };
    template <bool WhetherOrNot>
    struct ExplicitPossible { };
    template <bool WhetherOrNot>
    struct StringStreamPossible { };

    // No Conversion possible, default template
    template <class ToType, class FromType, int Dummy>
    struct Converter
    {
        static bool convert(ToType* output, FromType input)
        {
            // Do not allow impossible conversions
            //(*output) = input; // this WILL trigger a compiler error
            //BOOST_STATIC_ASSERT(sizeof(ToType) == 0); // just to be sure..
            return false;
        }
    };


    ///////////////////////
    //Explicit Conversion//
    ///////////////////////

    // We can cast explicitely, this overwrites any other possible cast
    template <class ToType, class FromType>
    inline bool convert(ToType* output, const FromType& input, ExplicitPossible<true>)
    {
        // This function can by anywhere globally!
        //int a = TemplateDebugger<1,2>::debug(1,1);
        //conversionTests::ExplicitConversion<FromType, ToType, 4>::test();
        //BOOST_STATIC_ASSERT(0);
        return explicitConversion(output, input);
    }

    // No explict cast, try implicit
    template <class ToType, class FromType>
    inline bool convert(ToType* output, const FromType& input, ExplicitPossible<false>)
    {
        return convert(output, input, ImplicitPossible<ImplicitConversion<FromType, ToType>::exists>());
    }


    /////////////////
    //Implicit Cast//
    /////////////////

    // We can cast implicitely
    template <class ToType, class FromType>
    inline bool convert(ToType* output, const FromType& input, ImplicitPossible<true>)
    {
        (*output) = static_cast<ToType>(input);
        return true;
    }

    // No implicit cast, leave it up to << and >>
    template <class ToType, class FromType>
    inline bool convert(ToType* output, const FromType& input, ImplicitPossible<false>)
    {
        return Converter<ToType, FromType, 0>::convert(output, input);
    }


    /////////////////
    //OStringStream//
    /////////////////

    // Conversion via ostringstream
    template <class FromType>
    bool convertOStringStream(std::string* output, const FromType& input)
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

    // template that evaluates whether OStringStream is possible
    template <class FromType, int Dummy>
    struct Converter<std::string, FromType, Dummy>
    {
        // convert to std::string, probe for '<<' stringstream operator
        static bool convert(std::string* output, const FromType& input)
        {
            return convert(output, input, StringStreamPossible<OStringStreamOperator<FromType>::exists>());
            //conversion::OStringStreamOperator<FromType>::test();
        }

        // Conversion with ostringstream possible
        static bool convert(std::string* output, const FromType& input, StringStreamPossible<true>)
        {
            return convertOStringStream(output, input);
        }

        // Conversion with ostringstream not possible
        static bool convert(std::string* output, const FromType& input, StringStreamPossible<false>)
        {
            // Do not allow impossible conversions
            //(*output) = input; // this WILL trigger a compiler error
            //BOOST_STATIC_ASSERT(sizeof(ToType) == 0); // just to be sure..
            return false;
        }
    };


    /////////////////
    //IStringStream//
    /////////////////

    // conversion from std::string via istringstream
    template <class ToType>
    bool convertIStringStream(ToType* output, const std::string& input)
    {
        std::istringstream iss(input);
        if (iss >> (*output))
        {
            return true;
        }
        else
            return false;
    }

    // template that evaluates whether IStringStream is possible
    template <class ToType, int Dummy>
    struct Converter<ToType, std::string, Dummy>
    {
        // convert from std::string, probe for '>>' stringstream operator
        static bool convert(ToType* output, const std::string& input)
        {
            return convert(output, input, StringStreamPossible<IStringStreamOperator<ToType>::exists>());
        }

        // Conversion with istringstream possible
        static bool convert(ToType* output, const std::string& input, StringStreamPossible<true>)
        {
            return convertIStringStream(output, input);
        }

        // Conversion with istringstream not possible
        static bool convert(ToType* output, const std::string& input, StringStreamPossible<false>)
        {
            // Do not allow impossible conversions
            //(*output) = input; // this WILL trigger a compiler error
            //BOOST_STATIC_ASSERT(sizeof(ToType) == 0); // just to be sure..
            return false;
        }
    };


    /////////////////
    //Special Cases//
    /////////////////

    // delegate conversion from const char* via string
    template <class ToType, int Dummy>
    struct Converter<ToType, const char*, Dummy>
    {
        // convert from const char* via std::string
        static bool convert(ToType* output, const char* input)
        { return Converter<ToType, std::string>::convert(output, input); }
    };
#if 0
    // conversion char to std::string leads to ambiguous operator <<
    template <int Dummy>
    struct Converter<std::string, char, Dummy>
    {
        static bool convert(std::string* output, const char input)
        { return convertOStringStream(output, input); }
    };

    // conversion unsigned char to std::string leads to ambiguous operator <<
    template <int Dummy>
    struct Converter<std::string, unsigned char, Dummy>
    {
        static bool convert(std::string* output, const char input)
        { return convertOStringStream(output, input); }
    };

    // conversion std::string to char leads to ambiguous operator >>
    template <int Dummy>
    struct Converter<char, std::string, Dummy>
    {
        static bool convert(char* output, const std::string input)
        { return convertIStringStream(output, input); }
    };

    // conversion std::string to unsigned char leads to ambiguous operator >>
    template <int Dummy>
    struct Converter<unsigned char, std::string, Dummy>
    {
        static bool convert(unsigned char* output, const std::string input)
        { return convertIStringStream(output, input); }
    };
#endif
}


////////////////////
//Public Functions//
////////////////////

/**
@brief
    Converts any value to any other as long as there exits a conversion.
    Otherwise, the conversion will generate a runtime warning.
*/
template <class ToType, class FromType>
inline bool convertValue(ToType* output, const FromType& input)
{
    // check whether we can convert one type to the other explicitely.
    //conversionTests::ExplicitConversion<FromType, ToType, 4>::test();
    return conversion::convert(output, input,
        conversion::ExplicitPossible<ExplicitConversion<FromType, ToType>::exists>());
}

// Helper function: Calls convertValue with and without default value and returns true if the conversion was successful
template<class FromType, class ToType>
static bool ConvertValue(ToType* output, const FromType& input)
{
    return convertValue(output, input);
}
template<class FromType, class ToType>
static bool ConvertValue(ToType* output, const FromType& input, const ToType& fallback)
{
    if (convertValue(output, input))
        return true;

    (*output) = fallback;
    return false;
}

// Helper function: Calls convertValue with and without default value and returns the converted value
template<class FromType, class ToType>
static ToType getConvertedValue(const FromType& input)
{
    ToType output = ToType();
    ConvertValue(&output, input);
    return output;
}
template<class FromType, class ToType>
static ToType getConvertedValue(const FromType& input, const ToType& fallback)
{
    ToType output = fallback;
    ConvertValue(&output, input, fallback);
    return output;
}


/////////////////////
// SPECIALIZATIONS //
/////////////////////

/////////////
// SAMPLES //
/////////////
/*
// convert everything to xyz
template <class FromType>
struct ConverterSpecialized<FromType, xyz, _ToType_>
{
    enum { specialized = true };
    static bool convert(xyz* output, const FromType& input)
    { return ...; }
};

// convert xyz to everything
template <class ToType>
struct ConverterSpecialized<xyz, ToType, _FromType_>
{
    enum { specialized = true };
    static bool convert(ToType* output, const xyz& input)
    { return ...; }
};

// convert abc to xyz
template <>
struct ConverterSpecialized<abc, xyz, _Explicit_>
{
    enum { specialized = true };
    static bool convert(xyz* output, const abc& input)
    { return ...; }
};
*/


/////////////////
// CONST CHAR* //
/////////////////
/*
// convert from const char* --> use conversions with std::string
template <class ToType, class Type>
struct ConverterSpecialized<const char*, ToType, Type>
{
    enum { specialized = true };
    static bool convert(ToType* output, const char* input)
    {
        return ConverterSpecialized<std::string, ToType, Type>::convert(output, input);
    }
};

// convert from const char* --> use conversions with std::string
template <>
struct ConverterSpecialized<const char*, std::string, _ToType_>
{
    enum { specialized = true };
    static bool convert(std::string* output, const char* input)
    {
        *output = input;
        return true;
    }
};

// convert from const char* _Explicit_ --> use conversions with std::string
//template <class ToType>
//struct ConverterSpecialized<const char*, ToType, _Explicit_>
//{
//    enum { specialized = true };
//    static bool convert(ToType* output, const char* input)
//    {
//        return ConverterSpecialized<std::string, ToType, _Explicit_>::convert(output, input);
//    }
//};

// convert from char* without const is not allowed
//template <class ToType, class Type>
//struct ConverterSpecialized<char*, ToType, Type>
//{
//    enum { specialized = true };
//    static bool convert(ToType* output, const char* input)
//    {
//        BOOST_STATIC_ASSERT(sizeof(ToType) == 0);
//    }
//};

// No support for char* without const
//template <class ToType, class Type>
//struct ConverterSpecialized<char*, ToType, Type>
//{
//    enum { specialized = true };
//    static bool convert(ToType* output, const char* input)
//    {
//        BOOST_STATIC_ASSERT(sizeof(ToType) == 0);
//    }
//};

// convert to const char* is not supported (possible memory leak)
template <class FromType, class Type>
struct ConverterSpecialized<FromType, const char*, Type>
{
    enum { specialized = true };
    static bool convert(const char** output, const FromType& input)
    {
        BOOST_STATIC_ASSERT(sizeof(FromType) == 0);
    }
};

// convert to char* is not supported (possible memory leak)
// Note: It actually does need both specializations for const char* and char*
//template <class FromType, class Type>
//struct ConverterSpecialized<FromType, char*, Type>
//{
//    enum { specialized = true };
//    static bool convert(char** output, const FromType& input)
//    {
//        BOOST_STATIC_ASSERT(sizeof(FromType) == 0);
//    }
//};


////////////////////
// MATH TO STRING //
////////////////////

// Vector2 to std::string
template <>
struct ConverterSpecialized<orxonox::Vector2, std::string, _Explicit_>
{
    enum { specialized = true };
    static bool convert(std::string* output, const orxonox::Vector2& input)
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
struct ConverterSpecialized<orxonox::Vector3, std::string, _Explicit_>
{
    enum { specialized = true };
    static bool convert(std::string* output, const orxonox::Vector3& input)
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
struct ConverterSpecialized<orxonox::Vector4, std::string, _Explicit_>
{
    enum { specialized = true };
    static bool convert(std::string* output, const orxonox::Vector4& input)
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
struct ConverterSpecialized<orxonox::Quaternion, std::string, _Explicit_>
{
    enum { specialized = true };
    static bool convert(std::string* output, const orxonox::Quaternion& input)
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
struct ConverterSpecialized<orxonox::ColourValue, std::string, _Explicit_>
{
    enum { specialized = true };
    static bool convert(std::string* output, const orxonox::ColourValue& input)
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
struct ConverterSpecialized<std::string, orxonox::Vector2, _Explicit_>
{
    enum { specialized = true };
    static bool convert(orxonox::Vector2* output, const std::string& input)
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

// std::string to Vector3
template <>
struct ConverterSpecialized<std::string, orxonox::Vector3, _Explicit_>
{
    enum { specialized = true };
    static bool convert(orxonox::Vector3* output, const std::string& input)
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

// std::string to Vector4
template <>
struct ConverterSpecialized<std::string, orxonox::Vector4, _Explicit_>
{
    enum { specialized = true };
    static bool convert(orxonox::Vector4* output, const std::string& input)
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

// std::string to Quaternion
template <>
struct ConverterSpecialized<std::string, orxonox::Quaternion, _Explicit_>
{
    enum { specialized = true };
    static bool convert(orxonox::Quaternion* output, const std::string& input)
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

// std::string to ColourValue
template <>
struct ConverterSpecialized<std::string, orxonox::ColourValue, _Explicit_>
{
    enum { specialized = true };
    static bool convert(orxonox::ColourValue* output, const std::string& input)
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
