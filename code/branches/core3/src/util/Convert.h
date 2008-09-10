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

#include "Math.h"
#include "Debug.h"

// Gcc generates warnings when implicitely casting from float to int for instance.
// This is however exactly what convertValue does, so we need to suppress the warnings.
// They only occur in when using the ImplicitConversion template.
#if ORXONOX_COMPILER == ORXONOX_COMPILER_GCC
#  pragma GCC system_header
#endif

///////////////////////////////////
// Explicit Conversion Functions //
///////////////////////////////////


///////////////////////////////////////////////
// Static detection for conversion functions //
///////////////////////////////////////////////

/* The idea to use the sizeof() operator on return functions to determine function existance
   is described in 'Moder C++ design' by Alexandrescu (2001). */

namespace conversionTests
{
    // A struct that is guaranteed to be larger than any return type of our conversion functions.
    // So we simply add all the sizes of the return types plus a little bit more.
    struct VeryBigStruct
    {
        char intSize[sizeof(int)];
        char issSize[sizeof(std::istringstream)];
        char ossSize[sizeof(std::ostringstream)];
        char boolSize[sizeof(bool)];
        char addingMore[4096]; // just to be sure ;)
    };
}

namespace separate_namespace
{
    // We want to keep the templates for the convert functions out of global namespace so that there
    // are no ambiguities. These templates are never used anyway, they only serve to detect whether
    // there is a global funciton for a specific conversion or not.
    // Now why not putting this in namespace 'conversionTests'?
    // I can not tell for sure, but I think there is a bug in msvc 8. It chooses this general template
    // (if it was in 'conversionTests') over a better fitting function in global namespace.
    // The solution is to use another namespace and then apply the 'using' directive in 'conversionTests'.
    // Unfortunately there is a (somewhat documented) bug in msvc 8 that exposes namespace members
    // globally when applying the 'using' directive in a namespace. And it so happens that the bug is
    // triggered in this file. This unwanted global exposure hopefully doesn't cause ambiguities.
    template <class AnyToType, class AnyFromType>
    conversionTests::VeryBigStruct explicitConversion(AnyToType* output, const AnyFromType input)
    {
        // We have to make sure noboy uses it , so a good compiler error would be nice.
        *output = (AnyToType)input; // Do not use this function!
        // gcc does some syntax checking anyway. So return a correct value that is not a temporary.
	    return *(new conversionTests::VeryBigStruct());
    }
}

namespace conversionTests
{
    // Part of the msvc hack. See above in the namespace for more explanations.
    using namespace separate_namespace;

    // These operators simply accept anything but have lower priority than specialisations
    // or exact-match non-template functions.
    // They can be identified by the larger return value.
    // We put these in a seperate namespace to avoid conflicts with ambiguities.
    template <class Any>
    conversionTests::VeryBigStruct operator<<(std::ostream& outstream, const Any anything);
    template <class Any>
    conversionTests::VeryBigStruct operator>>(std::istream& instream,  const Any anything);

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
        // test(object) has only 'VerySmallStruct' return type iff the compiler doesn't choose test(...)
        enum { exists = !(sizeof(test(object)) == sizeof(VeryBigStruct)) };
    };

    template <class FromType, class ToType>
    class ExplicitConversion
    {
    private:
        ExplicitConversion(); ExplicitConversion(const ExplicitConversion&); ~ExplicitConversion();
        static FromType objectFromType; // helper object to handle private c'tor and d'tor
        static ToType   objectToType;   // helper object to handle private c'tor and d'tor
    public:
        enum { exists = !(sizeof(explicitConversion(&objectToType, objectFromType)) == sizeof(VeryBigStruct)) };
    };

    template <class Type>
    class IStringStreamOperator
    {
        IStringStreamOperator(); IStringStreamOperator(const IStringStreamOperator&); ~IStringStreamOperator();
        static std::istringstream istream_; // helper object to perform the '>>' operation
        static Type object;                 // helper object to handle private c'tor and d'tor
    public:
        enum { exists = !(sizeof(istream_ >> object) == sizeof(VeryBigStruct)) };
    };

    template <class Type>
    class OStringStreamOperator
    {
        OStringStreamOperator(); OStringStreamOperator(const OStringStreamOperator&); ~OStringStreamOperator();
        static std::ostringstream ostream_; // helper object to perform the '<<' operation
        static Type object;                 // helper object to handle private c'tor and d'tor
    public:
        enum { exists = !(sizeof(ostream_ << object) == sizeof(VeryBigStruct)) };
    };
}

/* Shortcuts because we usually don't have a namespace in util/ but need one here for the conversion tests*/

/**
@brief
    Checks for an implicit conversion FromType --> TyType.
    This also works for user defined conversion operators.
    Usage: ImplicitConversion<FromType, ToType>::exists
*/
template <class FromType, class ToType>
struct ImplicitConversion
{ enum { exists = conversionTests::ImplicitConversion<FromType, ToType>::exists }; };

/**
@brief
    Checks for an explicit conversion FromType --> TyType via 'explicConversion()' function.
    There has to e an exact type match for a success!
    Usage: ExplicitConversion<FromType, ToType>::exists
*/
template <class FromType, class ToType>
struct ExplicitConversion
{ enum { exists = conversionTests::ExplicitConversion<FromType, ToType>::exists }; };

/**
@brief
    Checks for an explicit conversion std::string --> TyType via >> operator.
    There has to e an exact type match for a success!
    Usage: IStringStreamConversion<FromType, ToType>::exists
*/
template <class Type>
struct IStringStreamOperator
{ enum { exists = conversionTests::IStringStreamOperator<Type>::exists }; };

/**
@brief
    Checks for an explicit conversion std::string --> TyType via << operator.
    There has to e an exact type match for a success!
    Usage: OStringStreamConversion<FromType, ToType>::exists
*/
template <class Type>
struct OStringStreamOperator
{ enum { exists = conversionTests::OStringStreamOperator<Type>::exists }; };



////////////////////////////////////
//// ACTUAL CONVERSION SEQUENCE ////
////////////////////////////////////
/*
    There is a distinct priority when choosing the right conversion function:
    Overwrites:
    1. (Partial) template specialisation of ConverterExplicit::convert
    2. Global functions explicitConversion(ToType* output, const FromType input)
    Fallbacks:
    3. Any possible implicit conversion. This includes FooBar --> int if FooBar defines operator float().
    4. Global or member operators for stringstream when converting from or to std::string (or FROM const char*)
    5. Function that simply displays "Could not convert value" with information obtained from typeid().

    A note: There has to be an exact type match (or according to the rules of template spec.) except for 3.

    There are obviously a lot of ways to specifiy a user defined conversion. What should I use?
    When using any non-template function based conversion (implicit conversion, explicitConversion, << or >>)
    then you should consider that this function has to be defined prior to including this file.
    If you do not whish do do that, you will have to spcecialsize the ConverterExplicit template.
    There is a not so obvious advantage of the other way (non-template): You could declare each conversion function
    in the Prereqs file of the corresponding library. Then you can use the conversion everywhere.
    This is not possible with template specialisations even when defining them in this file (You would create
    a circular dependency when using a class from Core for instance, because complete template specialisations
    get compiled anyway (there is no template parameter)).
*/

// Put everything in a namespace to avoid unnecessary exposure
// Note that the textual order of the functions is in reverse.
namespace conversion
{
    // Maps bools to types in order to use function overloading instead of template specialisation (Alexandrescu 2001)
    template <bool WhetherOrNot>
    struct ImplicitPossible { };
    template <bool WhetherOrNot>
    struct ExplicitPossible { };
    template <bool WhetherOrNot>
    struct StringStreamPossible { };


    ///////////////////
    // No Conversion //
    ///////////////////

    // Default template, no Conversion possible
    template <class ToType, class FromType, int Dummy>
    struct ConverterSS
    {
        static bool convert(ToType* output, FromType input)
        {
            COUT(2) << "Could not convert value of type " << typeid(FromType).name()
                    << " to type " << typeid(ToType).name() << std::endl;
            return false;
        }
    };


    ///////////////////
    // OStringStream //
    ///////////////////

    // Conversion via ostringstream
    template <class FromType>
    inline bool convertOStringStream(std::string* output, const FromType& input)
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

    // template that evaluates whether OStringStream is possible for conversions to std::string
    template <class FromType, int Dummy>
    struct ConverterSS<std::string, FromType, Dummy>
    {
        // probe for '<<' stringstream operator
        static bool convert(std::string* output, const FromType& input)
        {
            const bool probe = OStringStreamOperator<FromType>::exists;
            return convert(output, input, StringStreamPossible<probe>());
        }
        // Conversion with ostringstream possible
        static bool convert(std::string* output, const FromType& input, StringStreamPossible<true>)
        {
            return convertOStringStream(output, input);
        }
        // Conversion with ostringstream not possible
        static bool convert(std::string* output, const FromType& input, StringStreamPossible<false>)
        {
            COUT(2) << "Could not convert value of type " << typeid(FromType).name()
                    << " to std::string" << std::endl;
            return false;
        }
    };


    ///////////////////
    // IStringStream //
    ///////////////////

    // conversion from std::string via istringstream
    template <class ToType>
    inline bool convertIStringStream(ToType* output, const std::string& input)
    {
        std::istringstream iss(input);
        if (iss >> (*output))
        {
            return true;
        }
        else
            return false;
    }

    // template that evaluates whether IStringStream is possible for conversions from std::string
    template <class ToType, int Dummy>
    struct ConverterSS<ToType, std::string, Dummy>
    {
        // probe for '>>' stringstream operator
        static bool convert(ToType* output, const std::string& input)
        {
            const bool probe = IStringStreamOperator<ToType>::exists;
            return convert(output, input, StringStreamPossible<probe>());
        }
        // Conversion with istringstream possible
        static bool convert(ToType* output, const std::string& input, StringStreamPossible<true>)
        {
            return convertIStringStream(output, input);
        }
        // Conversion with istringstream not possible
        static bool convert(ToType* output, const std::string& input, StringStreamPossible<false>)
        {
            COUT(2) << "Could not convert std::string value to type " << typeid(ToType).name() << std::endl;
            return false;
        }
    };


    ///////////////////
    // Implicit Cast //
    ///////////////////

    // We can cast implicitely
    template <class ToType, class FromType>
    inline bool convert(ToType* output, const FromType& input, ImplicitPossible<true>)
    {
        (*output) = static_cast<ToType>(input);
        return true;
    }

    // No implicit cast, leave it up to << and >> via template spcialisation
    template <class ToType, class FromType>
    inline bool convert(ToType* output, const FromType& input, ImplicitPossible<false>)
    {
        return ConverterSS<ToType, FromType, 0>::convert(output, input);
    }


    /////////////////////////
    // Explicit Conversion //
    /////////////////////////

    // We can convert explicitely via function overloading
    template <class ToType, class FromType>
    inline bool convert(ToType* output, const FromType& input, ExplicitPossible<true>)
    {
        // This function can by anywhere globally!
        return explicitConversion(output, input);
    }

    // No explict conversion via explicitConversion(), try implicit cast
    template <class ToType, class FromType>
    inline bool convert(ToType* output, const FromType& input, ExplicitPossible<false>)
    {
        const bool probe = ImplicitConversion<FromType, ToType>::exists;
        return convert(output, input, ImplicitPossible<probe>());
    }
}

// We usually don't have a namespace in util/ but it would still be desirable to lock
// everything internal away in namespace conversion.
// However this template can be specialised everywhere.

// Template that is used when no explicit template specialisation is available
// try explicitConversion() function next.
template <class ToType, class FromType>
struct ConverterExplicit
{
    static bool convert(ToType* output, const FromType& input)
    {
        // check for explicit conversion via function overloading
        const bool probe = ExplicitConversion<FromType, ToType>::exists;
        return conversion::convert(output, input, conversion::ExplicitPossible<probe>());
    }
};


//////////////////////
// Public Functions //
//////////////////////

/**
@brief
    Converts any value to any other as long as there exits a conversion.
    Otherwise, the conversion will generate a runtime warning.
    For information about the different conversion methods (user defined too), see the section
    'Actual conversion sequence' in this file above.
*/
template <class ToType, class FromType>
inline bool convertValue(ToType* output, const FromType& input)
{
    // check whether we can convert one type to the other explicitely via explicit template specialisations
    return ConverterExplicit<ToType, FromType>::convert(output, input);
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


///////////////////////////////////////
// Explicit Template Specialisations //
///////////////////////////////////////

// delegate conversion from const char* to std::string
template <class ToType>
struct ConverterExplicit<ToType, const char*>
{
    static bool convert(ToType* output, const char* input)
    {
        return ConverterExplicit<ToType, std::string>::convert(output, input);
    }
};

// These conversions would exhibit ambiguous << or >> operators when using stringstream
template <> struct ConverterExplicit<std::string, char>
{
    static bool convert(std::string* output, const char input)
    {
        *output = std::string(1, input);
        return true;
    }
};
template <> struct ConverterExplicit<std::string, unsigned char>
{
    static bool convert(std::string* output, const unsigned char input)
    {
        *output = std::string(1, input);
        return true;
    }
};
template <> struct ConverterExplicit<char, std::string>
{
    static bool convert(char* output, const std::string input)
    {
        if (input != "")
            *output = input[0];
        else
            *output = '\0';
        return true;
    }
};
template <> struct ConverterExplicit<unsigned char, std::string>
{
    static bool convert(unsigned char* output, const std::string input)
    {
        if (input != "")
            *output = input[0];
        else
            *output = '\0';
        return true;
    }
};

#endif /* _Convert_H__ */
