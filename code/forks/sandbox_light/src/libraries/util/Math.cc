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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Implementation of several math-functions.
*/

#include "Math.h"

#include "MathConvert.h"
#include "SubString.h"

namespace orxonox
{
    /**
        @brief Function for writing a Radian to a stream.
    */
    std::ostream& operator<<(std::ostream& out, const orxonox::Radian& radian)
    {
        out << radian.valueRadians();
        return out;
    }

    /**
        @brief Function for reading a Radian from a stream.
    */
    std::istream& operator>>(std::istream& in, orxonox::Radian& radian)
    {
        float temp;
        in >> temp;
        radian = temp;
        return in;
    }

    /**
        @brief Function for writing a Degree to a stream.
    */
    std::ostream& operator<<(std::ostream& out, const orxonox::Degree& degree)
    {
        out << degree.valueDegrees();
        return out;
    }

    /**
        @brief Function for reading a Degree from a stream.
    */
    std::istream& operator>>(std::istream& in, orxonox::Degree& degree)
    {
        float temp;
        in >> temp;
        degree = temp;
        return in;
    }

    unsigned long getUniqueNumber()
    {
        static unsigned long aNumber = 135;
        return aNumber++;
    }


    //////////////////////////
    // Conversion functions //
    //////////////////////////

    // std::string to Vector2
    bool ConverterFallback<std::string, orxonox::Vector2>::convert(orxonox::Vector2* output, const std::string& input)
    {
        size_t opening_parenthesis, closing_parenthesis = input.find(')');
        if ((opening_parenthesis = input.find('(')) == std::string::npos)
            opening_parenthesis = 0;
        else
            opening_parenthesis++;

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis),
                         ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 2)
        {
            if (!convertValue(&(output->x), tokens[0]))
                return false;
            if (!convertValue(&(output->y), tokens[1]))
                return false;

            return true;
        }
        return false;
    }

    // std::string to Vector3
    bool ConverterFallback<std::string, orxonox::Vector3>::convert(orxonox::Vector3* output, const std::string& input)
    {
        size_t opening_parenthesis, closing_parenthesis = input.find(')');
        if ((opening_parenthesis = input.find('(')) == std::string::npos)
            opening_parenthesis = 0;
        else
            opening_parenthesis++;

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis),
                         ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 3)
        {
            if (!convertValue(&(output->x), tokens[0]))
                return false;
            if (!convertValue(&(output->y), tokens[1]))
                return false;
            if (!convertValue(&(output->z), tokens[2]))
                return false;

            return true;
        }
        return false;
    }

    // std::string to Vector4
    bool ConverterFallback<std::string, orxonox::Vector4>::convert(orxonox::Vector4* output, const std::string& input)
    {
        size_t opening_parenthesis, closing_parenthesis = input.find(')');
        if ((opening_parenthesis = input.find('(')) == std::string::npos)
            opening_parenthesis = 0;
        else
            opening_parenthesis++;

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis),
                         ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 4)
        {
            if (!convertValue(&(output->x), tokens[0]))
                return false;
            if (!convertValue(&(output->y), tokens[1]))
                return false;
            if (!convertValue(&(output->z), tokens[2]))
                return false;
            if (!convertValue(&(output->w), tokens[3]))
                return false;

            return true;
        }
        return false;
    }

    // std::string to Quaternion
    bool ConverterFallback<std::string, orxonox::Quaternion>::convert(orxonox::Quaternion* output, const std::string& input)
    {
        size_t opening_parenthesis, closing_parenthesis = input.find(')');
        if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 4)
        {
            if (!convertValue(&(output->w), tokens[0]))
                return false;
            if (!convertValue(&(output->x), tokens[1]))
                return false;
            if (!convertValue(&(output->y), tokens[2]))
                return false;
            if (!convertValue(&(output->z), tokens[3]))
                return false;

            return true;
        }
        return false;
    }

    // std::string to ColourValue
    bool ConverterFallback<std::string, orxonox::ColourValue>::convert(orxonox::ColourValue* output, const std::string& input)
    {
        size_t opening_parenthesis, closing_parenthesis = input.find(')');
        if ((opening_parenthesis = input.find('(')) == std::string::npos) { opening_parenthesis = 0; } else { opening_parenthesis++; }

        SubString tokens(input.substr(opening_parenthesis, closing_parenthesis - opening_parenthesis), ",", SubString::WhiteSpaces, false, '\\', true, '"', true, '\0', '\0', true, '\0');
        if (tokens.size() >= 3)
        {
            if (!convertValue(&(output->r), tokens[0]))
                return false;
            if (!convertValue(&(output->g), tokens[1]))
                return false;
            if (!convertValue(&(output->b), tokens[2]))
                return false;
            if (tokens.size() >= 4)
            {
                if (!convertValue(&(output->a), tokens[3]))
                    return false;
            }
            else
                output->a = 1.0;

            return true;
        }
        return false;
    }
}
