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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      Benjamin Grauer
 *
 */

#ifndef _Util_String_H__
#define _Util_String_H__

#include <string>
#include <sstream>

#include "UtilPrereqs.h"

_UtilExport void        strip(std::string* str);
_UtilExport std::string getStripped(const std::string& str);

_UtilExport bool        isEmpty(const std::string& str);
_UtilExport bool        isComment(const std::string& str);
_UtilExport bool        isNumeric(const std::string& str);

_UtilExport void        lowercase(std::string* str);
_UtilExport std::string getLowercase(const std::string& str);

_UtilExport void        uppercase(std::string* str);
_UtilExport std::string getUppercase(const std::string& str);

_UtilExport int         nocaseCmp(const std::string& s1, const std::string& s2);
_UtilExport int         nocaseCmp(const std::string& s1, const std::string& s2, unsigned int len);

//! The Convert class has some static member functions to convert strings to values and values to strings.
class _UtilExport Convert
{
    public:
        /**
            @brief Converts a value of any type to a string.
            @param output The string to write the result in
            @param input The variable to convert
            @return True if the conversion succeded

            @example
            float f = 3.14;
            std::string output;
            bool success = Convert::ToString(&output, f);
        */
        template <typename T>
        static bool ToString(std::string* output, T input)
        {
            std::ostringstream oss;
            if (oss << input)
            {
                (*output) = oss.str();
                return true;
            }

            return false;
        }

        /**
            @brief Converts a value of any type to a string and assigns a defaultvalue if the conversion fails.
            @param output The string to write the result in
            @param input The variable to convert
            @param fallbackString The assigned string if the conversion fails.
            @return True if the conversion succeeded

            @example
            float f = 3.14;
            std::string output;
            bool success = Convert::ToString(&output, f, "0.000000");
        */
        template <typename T>
        static bool ToString(std::string* output, T input, const std::string& fallbackString)
        {
            if (Convert::ToString(output, input))
                return true;

            (*output) = fallbackString;
            return false;
        }

        /**
            @brief Converts a string to a value of any type.
            @param output The variable to assign the result to
            @param input The string to convert
            @return True if the conversion succeeded

            @example
            std::string input = "3.14";
            float f;
            bool success = string2Number(&f, input);
        */
        template <typename T>
        static bool FromString(T* output, const std::string& input)
        {
            std::istringstream iss(input);
            if (iss >> (*output))
                return true;

            return false;
        }

        /**
            @brief Converts a string to a value of any type.
            @param output The variable to assign the result to
            @param input The string to convert
            @param fallbackValue The assigned value if the conversion fails
            @return True if the conversion succeeded

            @example
            std::string input = "3.14";
            float f;
            bool success = string2Number(&f, input, 0.000000);
        */
        template <typename T>
        static bool FromString(T* output, const std::string& input, T fallbackValue)
        {
            if (Convert::FromString(output, input))
                return true;

            (*output) = fallbackValue;
            return false;
        }
};

#endif /* _Util_String_H__ */
