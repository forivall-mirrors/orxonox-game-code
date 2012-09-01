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

/*!
    @file
    @brief Definition and Implementation of the Convert class.
*/

#ifndef _Converter_H__
#define _Converter_H__

#include "UtilPrereqs.h"

#include <string>
#include <sstream>
#include <typeinfo>

#include "Debug.h"
#include "TemplateUtils.h"

////////////////////////////////////
//// ACTUAL CONVERSION SEQUENCE ////
////////////////////////////////////
/*
    There is a distinct priority when choosing the right conversion function:
    Overwrite:
    1. (Partial) template specialisation of ConverterExplicit::convert()
    Fallbacks:
    2. Any possible implicit conversion. This includes 'FooBar' --> 'int' if FooBar defines operator float().
    3. Global or member operators for stringstream when converting from or to std::string (or FROM const char*)
    4. (Partial) template specialisation of ConverterFallback::convert()
    5. Function that simply displays "Could not convert value" with type information obtained from typeid().

    Notes:
    There has to be an exact type match when using template specialisations.
    Template specialisations can be defined after including this file. Any implicit cast function or iostream
    operator has to be declared BEFORE this file gets parsed.

    Defining your own functions:
    There are obviously 4 ways to specifiy a user defined conversion. What should I use?

    Usually, ConverterFallback fits quite well. You won't have to deal with the conversion from
    'MyClass' to 'MyClass' by using another explicit template specialisation to avoid ambiguities.

    However if you want to overwrite an implicit conversion or an iostream operator, you really need to
    make use of ConverterExplicit.
*/

namespace orxonox
{
    namespace detail
    {
        //! Little template that maps integers to entire types (Alexandrescu 2001)
        template <int I>
        struct Int2Type { };
    }


    ///////////////////
    // No Conversion //
    ///////////////////

    // Default template. No conversion available at all.
    template <class FromType, class ToType>
    struct ConverterFallback
    {
        FORCEINLINE static bool convert(ToType* output, const FromType& input)
        {
            COUT(2) << "Could not convert value of type " << typeid(FromType).name()
                    << " to type " << typeid(ToType).name() << std::endl;
            return false;
        }
    };

    // If all else fails, try a dynamic_cast for pointer types.
    template <class FromType, class ToType>
    struct ConverterFallback<FromType*, ToType*>
    {
        FORCEINLINE static bool convert(ToType** output, FromType* const input)
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

// Default template for stringstream
template <class FromType, class ToType>
struct ConverterStringStream
{
    FORCEINLINE static bool convert(ToType* output, const FromType& input)
    {
        return orxonox::ConverterFallback<FromType, ToType>::convert(output, input);
    }
};


/////////////
// OStream //
/////////////

namespace fallbackTemplates
{
    template <class FromType>
    FORCEINLINE bool operator <<(std::ostream& outstream,  const FromType& input)
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

// template that evaluates whether we can convert to std::string via ostringstream
template <class FromType>
struct ConverterStringStream<FromType, std::string>
{
    FORCEINLINE static bool convert(std::string* output, const FromType& input)
    {
        using namespace fallbackTemplates;
        // this operator call only chooses fallbackTemplates::operator<< if there's no other fitting function
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
    FORCEINLINE bool operator >>(std::istream& instream, ToType& output)
    {
        return orxonox::ConverterFallback<std::string, ToType>
            ::convert(&output, static_cast<std::istringstream&>(instream).str());
    }
}

// template that evaluates whether we can convert from std::string via ostringstream
template <class ToType>
struct ConverterStringStream<std::string, ToType>
{
    FORCEINLINE static bool convert(ToType* output, const std::string& input)
    {
        using namespace fallbackTemplates;
        std::istringstream iss(input);
        // this operator call only chooses fallbackTemplates::operator>> if there's no other fitting function
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

    // implicit cast not possible, try stringstream conversion next
    template <class FromType, class ToType>
    FORCEINLINE bool convertImplicitely(ToType* output, const FromType& input, detail::Int2Type<false>)
    {
        return ConverterStringStream<FromType, ToType>::convert(output, input);
    }

    // We can cast implicitely
    template <class FromType, class ToType>
    FORCEINLINE bool convertImplicitely(ToType* output, const FromType& input, detail::Int2Type<true>)
    {
        (*output) = static_cast<ToType>(input);
        return true;
    }


    ////////////////////////////////
    // ConverterExplicit Fallback //
    ////////////////////////////////

    // Default template if no specialisation is available
    template <class FromType, class ToType>
    struct ConverterExplicit
    {
        enum { probe = ImplicitConversion<FromType, ToType>::exists };
        FORCEINLINE static bool convert(ToType* output, const FromType& input)
        {
            // Try implict cast and probe first. If a simple cast is not possible, it will not compile
            // We therefore have to out source it into another template function
            return convertImplicitely(output, input, detail::Int2Type<probe>());
        }
    };


    //////////////////////
    // Public Functions //
    //////////////////////

    /**
    @brief
        Converts any value to any other as long as there exists a conversion.
        Otherwise, the conversion will generate a runtime warning and return false.
        For information about the different conversion methods (user defined too), see the section
        'Actual conversion sequence' in this file above.
    */
    template <class FromType, class ToType>
    FORCEINLINE bool convertValue(ToType* output, const FromType& input)
    {
        return ConverterExplicit<FromType, ToType>::convert(output, input);
    }

    // Calls convertValue and returns true if the conversion was successful.
    // Otherwise the fallback is used.
    /**
    @brief
        Converts any value to any other as long as there exists a conversion.
        Otherwise, the conversion will generate a runtime warning and return false.
        For information about the different conversion methods (user defined too), see the section
        'Actual conversion sequence' in this file above.
        If the conversion doesn't succeed, 'fallback' is written to '*output'.
    @param fallback
        A default value that gets written to '*output' if there is no conversion.
    */
    template<class FromType, class ToType>
    FORCEINLINE bool convertValue(ToType* output, const FromType& input, const ToType& fallback)
    {
        if (convertValue(output, input))
            return true;
        else
        {
            (*output) = fallback;
            return false;
        }
    }

    // Directly returns the converted value, even if the conversion was not successful.
    template<class FromType, class ToType>
    FORCEINLINE ToType getConvertedValue(const FromType& input)
    {
        ToType output;
        convertValue(&output, input);
        return output;
    }

    // Directly returns the converted value, but uses the fallback on failure.
    template<class FromType, class ToType>
    FORCEINLINE ToType getConvertedValue(const FromType& input, const ToType& fallback)
    {
        ToType output;
        convertValue(&output, input, fallback);
        return output;
    }

    // Like getConvertedValue, but the template argument order is in reverse.
    // That means you can call it exactly like static_cast<ToType>(fromTypeValue).
    template<class ToType, class FromType>
    FORCEINLINE ToType multi_cast(const FromType& input)
    {
        ToType output;
        convertValue(&output, input);
        return output;
    }

    ////////////////////////////////
    // Special string conversions //
    ////////////////////////////////

    // Delegate conversion from const char* to std::string
    template <class ToType>
    struct ConverterExplicit<const char*, ToType>
    {
        FORCEINLINE static bool convert(ToType* output, const char* input)
        {
            return convertValue<std::string, ToType>(output, input);
        }
    };

    // These conversions would exhibit ambiguous << or >> operators when using stringstream
    template <>
    struct ConverterExplicit<char, std::string>
    {
        FORCEINLINE static bool convert(std::string* output, const char input)
        {
            *output = input;
            return true;
        }
    };
    template <>
    struct ConverterExplicit<unsigned char, std::string>
    {
        FORCEINLINE static bool convert(std::string* output, const unsigned char input)
        {
            *output = input;
            return true;
        }
    };
    template <>
    struct ConverterExplicit<std::string, char>
    {
        FORCEINLINE static bool convert(char* output, const std::string& input)
        {
            if (!input.empty())
                *output = input[0];
            else
                *output = '\0';
            return true;
        }
    };
    template <>
    struct ConverterExplicit<std::string, unsigned char>
    {
        FORCEINLINE static bool convert(unsigned char* output, const std::string& input)
        {
            if (!input.empty())
                *output = input[0];
            else
                *output = '\0';
            return true;
        }
    };


    // bool to std::string
    template <>
    struct ConverterExplicit<bool, std::string>
    {
        FORCEINLINE static bool convert(std::string* output, const bool& input)
        {
            if (input)
              *output = "true";
            else
              *output = "false";
            return true;
        }
    };

    // Declarations to avoid StringUtils.h include
    _UtilExport std::string removeTrailingWhitespaces(const std::string& str);
    _UtilExport std::string getLowercase(const std::string& str);

    // std::string to bool
    template <>
    struct ConverterExplicit<std::string, bool>
    {
        static bool convert(bool* output, const std::string& input)
        {
            const std::string& stripped = getLowercase(removeTrailingWhitespaces(input));
            if (stripped == "true" || stripped == "on" || stripped == "yes")
            {
                *output = true;
                return true;
            }
            else if (stripped == "false" || stripped == "off" || stripped == "no")
            {
                *output = false;
                return true;
            }

            std::istringstream iss(input);
            if (iss >> (*output))
                return true;
            else
                return false;
        }
    };
}

#endif /* _Convert_H__ */
