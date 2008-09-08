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

// These conversions exhibit ambiguous << or >> operators when using stringstream
inline bool explicitConversion(std::string* output, const char input)
{
    *output = std::string(input, 1);
    return true;
}
inline bool explicitConversion(std::string* output, const unsigned char input)
{
    *output = std::string(input, 1);
    return true;
}
inline bool explicitConversion(char* output, const std::string input)
{
    if (input != "")
        *output = input[0];
    else
        *output = '\0';
    return true;
}
inline bool explicitConversion(unsigned char* output, const std::string input)
{
    if (input != "")
        *output = input[0];
    else
        *output = '\0';
    return true;
}

///////////////////////////
// Static type detection //
///////////////////////////

/* The idea to use the sizeof() operator on return functions to determine function existance
   is described in 'Moder C++ design' by Alexandrescu (2001). */

//template <int a, int b>
//struct TemplateDebugger
//{
//    static int debug(int c, int d) { return 0; } //BOOST_STATIC_ASSERT(0); }
//};

namespace conversionTests
{

    // Two struct with very different sizes
    struct VerySmallStruct { char dummy[1]; };
    struct VeryBigStruct   { char dummy[1024]; }; // Big is surely larger than Small, even with alignments
}

//namespace generalFunctionTemplate
//{
    // Keep this function out of conversion namespaces because the compiler gives a general
    // template in the same namesapace a higher priority than any specialisation.
    // This function simply accepts anything but has lower priority than specialisations.
    // It can be identified by the larger return value.
    template <class AnyToType, class AnyFromType>
    conversionTests::VeryBigStruct explicitConversion(AnyToType* output, const AnyFromType input)
    {
        // The function is exposed globally because of the "using namespace" in conversionTests.
        // We have to make sure noboy uses it, so a good compiler error would be nice.
        *output = (AnyToType)input; // Do not use this function!
        //BOOST_STATIC_ASSERT(0); // just to be sure
		return *(new conversionTests::VeryBigStruct());
    }
    //conversionTests::VeryBigStruct explicitConversion(...);
//}

namespace conversionTests
{
    //using namespace generalFunctionTemplate; // Why in separate namespace? See above

    // This operators simply accept anything but have lower priority than specialisations.
    // It can be identified by the larger return value.
    template <class Any>
    conversionTests::VeryBigStruct operator<<(std::ostream& outstream, const Any anything);
    template <class Any>
    conversionTests::VeryBigStruct operator>>(std::istream& instream,  const Any anything);

    // checks for implicit conversion
    template <class FromType, class ToType>
    class ImplicitConversion
    {
    private:
        static VerySmallStruct test(ToType); // only accepts ToType, but is preferred over '...'
		//template <class AnyType>
        static VeryBigStruct   test(...);//const AnyType anything);    // accepts anything
        static FromType object; // helper object to handle private c'tor and d'tor
    public:
        enum { exists = sizeof(test(object)) == sizeof(VerySmallStruct) };
    };

    // checks for explicit conversion with explicitConversion()
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

    //template <int asdf>
    //class ExplicitConversion<float, int, asdf>
    //{
    //private:
    //    static float objectFromType; // helper object to handle private c'tor and d'tor
    //    static int   objectToType;   // helper object to handle private c'tor and d'tor
    //public:
    //    enum { exists = sizeof(explicitConversion(&objectToType, objectFromType)) == sizeof(VerySmallStruct) };
    //    static void test() { TemplateDebugger<sizeof(explicitConversion(&objectToType, objectFromType)), sizeof(VerySmallStruct)>::debug(1,2); }
    //};

    // checks for conversion via istringstream
    template <class Type>
    class IStringStreamOperator
    {
        static std::istringstream istream_; // helper object to perform the '>>' operation
        static Type object;                 // helper object to handle private c'tor and d'tor
    public:
        enum { exists = (sizeof(istream_ >> object) < sizeof(VerySmallStruct) + 512) };
    };

    // checks for conversion via ostringstream
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
}


///////////////////////
//Explicit Conversion//
///////////////////////

namespace conversion
{
    // We can cast explicitely via function overloading, this overwrites any other possible cast
    template <class ToType, class FromType>
    inline bool convert(ToType* output, const FromType& input, ExplicitPossible<true>)
    {
        // This function can by anywhere globally!
        //int a = TemplateDebugger<1,2>::debug(1,1);
        //conversionTests::ExplicitConversion<FromType, ToType, 4>::test();
        //BOOST_STATIC_ASSERT(0);
        return explicitConversion(output, input);
    }

    // No function explict conversion, try implicit cast
    template <class ToType, class FromType>
    inline bool convert(ToType* output, const FromType& input, ExplicitPossible<false>)
    {
        return convert(output, input, ImplicitPossible<ImplicitConversion<FromType, ToType>::exists>());
        //return ConverterSpecialised<ToType, FromType>::convert(output, input);
    }
}

// template that is used when no explicit template specialisation is available
template <class ToType, class FromType>
struct ConverterSpecialised
{
    static bool convert(ToType* output, const FromType& input)
    {
        // check for explicit conversion via function overloading
        return conversion::convert(output, input,
            conversion::ExplicitPossible<ExplicitConversion<FromType, ToType>::exists>());
    }
};

namespace conversion
{
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


    ///////////////
    //const char*//
    ///////////////

    // delegate conversion from const char* via string
    template <class ToType, int Dummy>
    struct Converter<ToType, const char*, Dummy>
    {
        // convert from const char* via std::string
        static bool convert(ToType* output, const char* input)
        { return Converter<ToType, std::string, Dummy>::convert(output, input); }
    };
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
    // check whether we can convert one type to the other explicitely via function overloading
    //conversionTests::ExplicitConversion<FromType, ToType, 4>::test();
    return ConverterSpecialised<ToType, FromType>::convert(output, input);
    //return conversion::convert(output, input,
    //    conversion::ExplicitPossible<ExplicitConversion<FromType, ToType>::exists>());
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

#endif /* _Convert_H__ */
