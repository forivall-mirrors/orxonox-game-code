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
 *   Co-authors:
 *      ...
 */

#include "Convert.h"
#include "StringUtils.h"

namespace orxonox
{
    bool ConverterExplicit<std::string, bool>::convert(bool* output, const std::string& input)
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
}
