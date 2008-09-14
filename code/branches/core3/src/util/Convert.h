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
 *      Reto Grieder (direct conversion tests)
 *   Co-authors:
 *      ...
 */

/*!
    @file
    @brief Definition and Implementation of the Convert class.
*/

#ifndef _Converter_H__
#define _Converter_H__

#include "UtilPrereqs.h"

#include <string>
#include <sstream>
#include <istream>
#include <ostream>

#include "Debug.h"

// Gcc generates warnings when implicitely casting from float to int for instance.
// This is however exactly what convertValue does, so we need to suppress the warnings.
// They only occur in when using the ImplicitConversion template.
#if ORXONOX_COMPILER == ORXONOX_COMPILER_GNUC
#  pragma GCC system_header
#endif

///////////////////////////////////////////////
// Static detection for conversion functions //
///////////////////////////////////////////////
/* The idea to use the sizeof() operator on return functions to determine function existance
   is described in 'Moder C++ design' by Alexandrescu (2001). */

// disable warnings about possible loss of data
#if ORXONOX_COMPILER == ORXONOX_COMPILER_MSVC
#  pragma warning(push)
#  pragma warning(disable:4244)
#endif

namespace conversionTests
{
    // A struct that is guaranteed to be larger than any return type of our conversion functions.
    // So we simply add all the sizes of the return types plus a little bit more.
    struct VeryBigStruct
    {
        char intSize[sizeof(int)];
        char addingMore[4096]; // just to be sure ;)
    };

    template <class FromType, class ToType>
    class ImplicitConversion
    {
    private:
        ImplicitConversion(); ImplicitConversion(const ImplicitConversion&); ~ImplicitConversion();
        // Gets chosen only if there is an implicit conversion from FromType to ToType.
        static int test(ToType);
        // Accepts any argument. Why do we not use a template? The reason is that with templates,
        // the function above is only taken iff it is an exact type match. But since we want to
        // check for implicit conversion, we have to use the ellipsis.
        static VeryBigStruct   test(...);
        static FromType object; // helper object to handle private c'tor and d'tor
    public:
        // test(object) only has 'VerySmallStruct' return type iff the compiler doesn't choose test(...)
        enum { exists = !(sizeof(test(object)) == sizeof(VeryBigStruct)) };
    };
}

#if ORXONOX_COMPILER == ORXONOX_COMPILER_MSVC
#  pragma warning(pop)
#endif


////////////////////////////////////
//// ACTUAL CONVERSION SEQUENCE ////
////////////////////////////////////
/*
    There is a distinct priority when choosing the right conversion function:
    Overwrites:
    1. (Partial) template specialisation of ConverterExplicit::convert
    2. Global functions convertValue(ToType* output, const FromType input)
    Fallbacks:
    3. Any possible implicit conversion. This includes FooBar --> int if FooBar defines operator float().
    4. Global or member operators for stringstream when converting from or to std::string (or FROM const char*)
    5. Function that simply displays "Could not convert value" with information obtained from typeid().

    A note: There has to be an exact type match (or according to the rules of template spec.) except for 3.

    There are obviously a lot of ways to specifiy a user defined conversion. What should I use?
    When using any non-template function based conversion (implicit conversion, convertValue, << or >>)
    then you should consider that this function has to be defined prior to including this file.
    If you do not whish do do that, you will have to spcecialsize the ConverterExplicit template.
    There is a not so obvious advantage of the other way (non-template): You could declare each conversion function
    in the Prereqs file of the corresponding library. Then you can use the conversion everywhere.
    This is not possible with template specialisations even when defining them in this file (You would create
    a circular dependency when using a class from Core for instance, because complete template specialisations
    get compiled anyway (there is no template parameter)).
*/

namespace
{
    // little template that maps to ints to entire types (Alexandrescu 2001)
    template <int I>
    struct Int2Type { };
}


///////////////////
// No Conversion //
///////////////////

// Default template for fallbackConvert, no conversion possible
template <class ToType, class FromType>
inline bool fallbackConversion(ToType* output, FromType input)
{
    COUT(2) << "Could not convert value of type " << typeid(FromType).name()
            << " to type " << typeid(ToType).name() << std::endl;
    return false;
}


/////////////////////
// fallbackConvert //
/////////////////////

// Class template used when << or >> was not available with string conversions
// It is required to call not yet declared fallbackConvert functions.
template <class ToType, class FromType>
struct ConverterFallbackTemplate
{
    static bool convert(ToType* output, const FromType& input)
    {
        return fallbackConversion(output, input);
    }
};

// Default template for stringstream
template <class ToType, class FromType>
struct ConverterStringStream
{
    static bool convert(ToType* output, const FromType& input)
    {
        return fallbackConversion(output, input);
    }
};


/////////////
// OStream //
/////////////

namespace fallbackTemplates
{
    template <class FromType>
    inline bool operator <<(std::ostream& outstream,  const FromType& input)
    {
        std::string temp;
        if (ConverterFallbackTemplate<std::string, FromType>::convert(&temp, input))
        {
            std::operator <<(outstream, temp);
            return true;
        }
        else
            return false;
    }
}

// template that evaluates whether OStringStream is possible for conversions to std::string
template <class FromType>
struct ConverterStringStream<std::string, FromType>
{
    static bool convert(std::string* output, const FromType& input)
    {
        using namespace fallbackTemplates;
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


/////////////
// IStream //
/////////////

namespace fallbackTemplates
{
    template <class ToType>
    inline bool operator >>(std::istream& instream, ToType& output)
    {
        return ConverterFallbackTemplate<ToType, std::string>
            ::convert(&output, static_cast<std::istringstream&>(instream).str());
    }
}

// template that evaluates whether IStringStream is possible for conversions from std::string
template <class ToType>
struct ConverterStringStream<ToType, std::string>
{
    static bool convert(ToType* output, const std::string& input)
    {
        using namespace fallbackTemplates;
        std::istringstream iss(input);
        if (iss >> (*output))
        {
            return true;
        }
        else
            return false;
    }
};


///////////////////
// Implicit Cast //
///////////////////

// We can cast implicitely
template <class ToType, class FromType>
inline bool convertImplicitely(ToType* output, const FromType& input, ::Int2Type<true>)
{
    (*output) = static_cast<ToType>(input);
    return true;
}

// static cast no possible, try stringstream conversion next
template <class ToType, class FromType>
inline bool convertImplicitely(ToType* output, const FromType& input, ::Int2Type<false>)
{
    return ConverterStringStream<ToType, FromType>::convert(output, input);
}


///////////////////////
// Explicit Fallback //
///////////////////////

template <class ToType, class FromType>
inline bool explicitConversion(ToType* output, const FromType& input)
{
    // try implict conversion by probing first because we use '...' instead of a template
    const bool probe = conversionTests::ImplicitConversion<FromType, ToType>::exists;
    return convertImplicitely(output, input, ::Int2Type<probe>());
}


// Indirect calls over a class template so we can call functions not yet declared.
template <class ToType, class FromType>
struct ConverterExplicitTemplate
{
    static bool convert(ToType* output, const FromType& input)
    {
        return explicitConversion(output, input);
    }
};


//////////////////////
// Public Functions //
//////////////////////

/**
@brief
    Converts any value to any other as long as there exists a conversion.
    Otherwise, the conversion will generate a runtime warning.
    For information about the different conversion methods (user defined too), see the section
    'Actual conversion sequence' in this file above.
@note
    This function is only a fallback if there is no appropriate 'convertValue' function.
*/
template <class ToType, class FromType>
inline bool convertValue(ToType* output, const FromType& input)
{
    return ConverterExplicitTemplate<ToType, FromType>::convert(output, input);
}

// For compatibility reasons. The same, but with capital ConvertValue
template<class FromType, class ToType>
inline bool ConvertValue(ToType* output, const FromType& input)
{
    return convertValue(output, input);
}

// Calls convertValue and returns true if the conversion was successful.
// Otherwise the fallback is used.
template<class FromType, class ToType>
inline bool ConvertValue(ToType* output, const FromType& input, const ToType& fallback)
{
    if (convertValue(output, input))
        return true;

    (*output) = fallback;
    return false;
}

// Directly returns the converted value, even if the conversion was not successful.
template<class FromType, class ToType>
inline ToType getConvertedValue(const FromType& input)
{
    ToType output;
    ConvertValue(&output, input);
    return output;
}

// Directly returns the converted value, but uses the fallback on failure.
template<class FromType, class ToType>
inline ToType getConvertedValue(const FromType& input, const ToType& fallback)
{
    ToType output;
    ConvertValue(&output, input, fallback);
    return output;
}

// Like getConvertedValue, but the template argument order is in reverse.
// That means you can call it exactly like static_cast<ToType>(fromTypeValue).
template<class ToType, class FromType>
inline ToType conversion_cast(const FromType& input)
{
    ToType output;
    ConvertValue(&output, input);
    return output;
}

// Like conversion_cast above, but uses a fallback on failure.
template<class ToType, class FromType>
inline ToType conversion_cast(const FromType& input, const ToType& fallback)
{
    ToType output;
    ConvertValue(&output, input, fallback);
    return output;
}


////////////////////////////////
// Special string conversions //
////////////////////////////////

// delegate conversion from const char* to std::string
template <class ToType>
inline bool explicitConversion(ToType* output, const char* input)
{
    return convertValue<ToType, std::string>(output, input);
}

// These conversions would exhibit ambiguous << or >> operators when using stringstream
inline bool explicitConversion(std::string* output, const char input)
{
    *output = std::string(1, input);
    return true;
}
inline bool explicitConversion(std::string* output, const unsigned char input)
{
    *output = std::string(1, input);
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

#endif /* _Convert_H__ */
