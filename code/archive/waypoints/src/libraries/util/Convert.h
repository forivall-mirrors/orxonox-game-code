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
 *      Reto Grieder
 *      Fabian 'x3n' Landau
 *      Benjamin Grauer
 *   Co-authors:
 *      ...
 */

/**
    @defgroup Convert Conversion functions
    @ingroup Util
*/

/** Functions that convert values between different types.
@file
@ingroup Convert
@par Usage
    There are three ways to use the conversions depending on what you need. <br>
    - For simply converting values without having to know whether the conversion
      was successful (for instance float --> string), use orxonox::multi_cast
      which effectively works exactly like static_cast, etc.
      @code
        float input = 42.0;
        std::string output = multi_cast<std::string>(input);
      @endcode
    - If you care about whether the conversion was successful,
      use orxonox::convertValue.
      @code
        std::string input("3.4");
        float output;
        bool success = convertValue(&output, input);
      @endcode
    - If you care about success and if you can also feed a fallback value,
      use orxonox::convertValue.
      @code
        std::string input("3.4");
        float output;
        bool success = convertValue(&output, input, 0.0);
      @endcode
    - If success doesn't matter but you can feed a fallback value,
      use orxonox::getConvertedValue.
      @code
        std::string input("3.4");
        float output = getConvertedValue(input, 0.0);
      @endcode
@details
    The back end of these functions are the actual implementations for the
    specific conversions, for instance from Ogre::Vector3 to std::string and
    vice versa. Some of them also use the iostream operators. <br>
    The real deal is evaluating which function is needed for a conversion based
    on the input and output type. But there are lots of catches in conjunction
    with templates which explains why there are so many functions in this file.
    <br> <br>
@par Search Order
    Finding the right function is governed by priority rules: <br>
    -# (Partial) template specialisation of orxonox::ConverterExplicit::convert()
    -# An implicit conversion. This includes 'FooBar' to 'int' if FooBar
       defines operator int() or float().
    -# Global or member operators for iostream when converting from or
       to std::string (and FROM const char*)
    -# (Partial) template specialisation of orxonox::ConverterFallback::convert()
    -# Fallback function that displays "Could not convert value" with type
       information obtained from typeid().
@par Implementing conversion functions
    To do that you probably need to know a thing or two about the types
    involved. So, get ready with that. <br>
    Usually the best way to do it is specialising of the orxonox::ConverterFallback
    template, like this:
    @code
    template <>
    struct _UtilExport ConverterFallback<std::string, MyType>
    {
        static bool convert(MyType* output, const std::string& input)
        {
           ...
           return success;
        }
    };
    @endcode
    This piece of code converts an std::string to MyType and returns whether the
    conversion was successful. You can also use partial specialisation.<br>
    The advantage with orxonox::ConverterFallback is that it has a low priority
    meaning that when there is an implicit conversion or an iostream method, that
    comes first and you don't have to deal with it (and the accompanying
    function call ambiguity). <br>
    However sometimes you would like to explicitely replace such a conversion.
    That's where orxonox::ConverterExplicit comes in handy (for instance we
    replaced the operator << conversions for Ogre::VectorX with our own functions).
@note
    There has to be an exact type match when using template specialisations. <br>
    Template specialisations can be defined after including this file.
    But any implicit cast function or iostream operator has to be included
    in this file!
@par Understanding the Code
    In order to understand how the templates work, it is probably best to study
    the functions in order of calling. There are lots of comments explaining
    what happens, but you'll need to understand a deal about partial template
    specialisation and function headers are matched in C++.
*/

#ifndef _Converter_H__
#define _Converter_H__

#include "UtilPrereqs.h"

#include <string>
#include <sstream>
#include <typeinfo>
#include <loki/TypeManip.h>

#include "Output.h"
#include "ImplicitConversion.h"

// disable warnings about possible loss of data
#ifdef ORXONOX_COMPILER_MSVC
#  pragma warning(push)
#  pragma warning(disable:4244)
#endif

namespace orxonox
{
    ///////////////////
    // No Conversion //
    ///////////////////

    /// Default template. No conversion available at all.
    template <class FromType, class ToType>
    struct ConverterFallback
    {
        ORX_FORCEINLINE static bool convert(ToType* /*output*/, const FromType& /*input*/)
        {
            orxout(internal_warning) << "Could not convert value of type " << typeid(FromType).name()
                                     << " to type " << typeid(ToType).name() << endl;
            return false;
        }
    };

    /// If all else fails, try a dynamic_cast for pointer types.
    template <class FromType, class ToType>
    struct ConverterFallback<FromType*, ToType*>
    {
        ORX_FORCEINLINE static bool convert(ToType** output, FromType* const input)
        {
            ToType* temp = dynamic_cast<ToType*>(input);
            if (temp)
            {
                *output = temp;
                return true;
            }
            else
                return false;
        }
    };
}


///////////////////////
// ConverterFallback //
///////////////////////

/** Fallback template for stringstream
@details
    Neither FromType nor ToType was std::string, therefore
    delegate to orxonox::ConverterFallback
*/
template <class FromType, class ToType>
struct ConverterStringStream
{
    ORX_FORCEINLINE static bool convert(ToType* output, const FromType& input)
    {
        return orxonox::ConverterFallback<FromType, ToType>::convert(output, input);
    }
};


/////////////
// OStream //
/////////////

/// Extra namespace to avoid exposing the iostream operators in it
namespace fallbackTemplates
{
    /// Fallback operator <<() (delegates to orxonox::ConverterFallback)
    template <class FromType>
    ORX_FORCEINLINE bool operator <<(std::ostream& outstream,  const FromType& input)
    {
        std::string temp;
        if (orxonox::ConverterFallback<FromType, std::string>::convert(&temp, input))
        {
            std::operator <<(outstream, temp);
            return true;
        }
        else
            return false;
    }
}

/// Template that evaluates whether we can convert to std::string via ostringstream
template <class FromType>
struct ConverterStringStream<FromType, std::string>
{
    ORX_FORCEINLINE static bool convert(std::string* output, const FromType& input)
    {
        using namespace fallbackTemplates;
        // this operator call only chooses fallbackTemplates::operator<<()
        // if there's no other fitting function
        std::ostringstream oss;
        // Note: std::ostream has operator!() to tell whether any error flag was set
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
    /// Fallback operator >>() (delegates to orxonox::ConverterFallback)
    template <class ToType>
    ORX_FORCEINLINE bool operator >>(std::istream& instream, ToType& output)
    {
        std::string input(static_cast<std::istringstream&>(instream).str());
        return orxonox::ConverterFallback<std::string, ToType>::convert(&output, input);
    }
}

/// Template that evaluates whether we can convert from std::string via istringstream
template <class ToType>
struct ConverterStringStream<std::string, ToType>
{
    ORX_FORCEINLINE static bool convert(ToType* output, const std::string& input)
    {
        using namespace fallbackTemplates;
        // this operator call chooses fallbackTemplates::operator>>()
        // only if there's no other fitting function
        std::istringstream iss(input);
        // Note: std::istream has operator!() to tell whether any error flag was set
        if (iss >> (*output))
        {
            return true;
        }
        else
            return false;
    }
};

namespace orxonox
{
    ///////////////////
    // Implicit Cast //
    ///////////////////

    /// %Template delegates to ::ConverterStringStream
    template <class FromType, class ToType>
    ORX_FORCEINLINE bool convertImplicitely(ToType* output, const FromType& input, Loki::Int2Type<false>)
    {
        return ConverterStringStream<FromType, ToType>::convert(output, input);
    }

    /// Makes an implicit cast from \a FromType to \a ToType
    template <class FromType, class ToType>
    ORX_FORCEINLINE bool convertImplicitely(ToType* output, const FromType& input, Loki::Int2Type<true>)
    {
        (*output) = static_cast<ToType>(input);
        return true;
    }


    ////////////////////////////////
    // ConverterExplicit Fallback //
    ////////////////////////////////

    /** Default template if no orxonox::ConverterExplicit is available
    @details
        Evaluates whether \a FromType can be implicitly converted to \a ToType
        by the use the ImplicitConversion magic.
    */
    template <class FromType, class ToType>
    struct ConverterExplicit
    {
        enum { probe = ImplicitConversion<FromType, ToType>::exists };
        ORX_FORCEINLINE static bool convert(ToType* output, const FromType& input)
        {
            // Use the probe's value to delegate to the right function
            return convertImplicitely(output, input, Loki::Int2Type<probe>());
        }
    };


    //////////////////////
    // Public Functions //
    //////////////////////

    /**
    @brief
        Converts any value to any other as long as there exists a conversion.
    @details
        Otherwise, the conversion will generate a runtime warning and return false.
    @see Convert.h
    @param output
        A pointer to the variable where the converted value will be stored
    @param input
        The original value
    */
    template <class FromType, class ToType>
    ORX_FORCEINLINE bool convertValue(ToType* output, const FromType& input)
    {
        return ConverterExplicit<FromType, ToType>::convert(output, input);
    }

    // Calls convertValue and returns true if the conversion was successful.
    // Otherwise the fallback is used.
    /**
    @brief
        Converts any value to any other as long as there exists a conversion.
        Otherwise, the conversion will generate a runtime warning and return false.
        If the conversion doesn't succeed, \a fallback is written to \a output.
    @see Convert.h
    @param output
        A pointer to the variable where the converted value will be stored
    @param input
        The original value
    @param fallback
        A default value that gets written to '*output' if there is no conversion.
    */
    template<class FromType, class ToType>
    ORX_FORCEINLINE bool convertValue(ToType* output, const FromType& input, const ToType& fallback)
    {
        if (convertValue(output, input))
            return true;
        else
        {
            (*output) = fallback;
            return false;
        }
    }

    /// Directly returns the converted value, but uses the fallback on failure. @see convertValue
    template<class FromType, class ToType>
    ORX_FORCEINLINE ToType getConvertedValue(const FromType& input, const ToType& fallback)
    {
        ToType output;
        convertValue(&output, input, fallback);
        return output;
    }

    /**
    @brief
        Converts any value to any other as long as there exists a conversion.
    @details
        Use exactly the way you use static_cast, etc. <br>
        A failed conversion will return a default instance of \a ToType
        (possibly uninitialised)
    @see Convert.h
    @param input
        The original value
    */
    template<class ToType, class FromType>
    ORX_FORCEINLINE ToType multi_cast(const FromType& input)
    {
        ToType output;
        convertValue(&output, input);
        return output;
    }

    ////////////////////////////////
    // Special string conversions //
    ////////////////////////////////

    /// Delegates conversion from const char* to std::string
    template <class ToType>
    struct ConverterExplicit<const char*, ToType>
    {
        ORX_FORCEINLINE static bool convert(ToType* output, const char* input)
        {
            return convertValue<std::string, ToType>(output, input);
        }
    };

    /// Conversion would exhibit ambiguous << or >> operators when using iostream
    template <>
    struct ConverterExplicit<char, std::string>
    {
        ORX_FORCEINLINE static bool convert(std::string* output, const char input)
        {
            *output = input;
            return true;
        }
    };
    /// Conversion would exhibit ambiguous << or >> operators when using iostream
    template <>
    struct ConverterExplicit<unsigned char, std::string>
    {
        ORX_FORCEINLINE static bool convert(std::string* output, const unsigned char input)
        {
            *output = input;
            return true;
        }
    };
    /// Conversion would exhibit ambiguous << or >> operators when using iostream
    template <>
    struct ConverterExplicit<std::string, char>
    {
        ORX_FORCEINLINE static bool convert(char* output, const std::string& input)
        {
            if (!input.empty())
                *output = input[0];
            else
                *output = '\0';
            return true;
        }
    };
    /// Conversion would exhibit ambiguous << or >> operators when using iostream
    template <>
    struct ConverterExplicit<std::string, unsigned char>
    {
        ORX_FORCEINLINE static bool convert(unsigned char* output, const std::string& input)
        {
            if (!input.empty())
                *output = input[0];
            else
                *output = '\0';
            return true;
        }
    };


    /// Conversion from bool to std::string
    template <>
    struct ConverterExplicit<bool, std::string>
    {
        ORX_FORCEINLINE static bool convert(std::string* output, const bool& input)
        {
            if (input)
              *output = "true";
            else
              *output = "false";
            return true;
        }
    };

    /// Conversion from std::string to bool
    template <>
    struct _UtilExport ConverterExplicit<std::string, bool>
    {
        static bool convert(bool* output, const std::string& input);
    };
}

// Reinstate warnings
#ifdef ORXONOX_COMPILER_MSVC
#  pragma warning(pop)
#endif

#endif /* _Convert_H__ */
