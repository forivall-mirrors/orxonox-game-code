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

#include "Math.h"

/**
    @brief Function for writing to a stream.
*/
std::ostream& operator<<(std::ostream& out, const orxonox::Radian& radian)
{
    out << radian.valueRadians();
    return out;
}

/**
    @brief Function for reading from a stream.
*/
std::istream& operator>>(std::istream& in, orxonox::Radian& radian)
{
    float temp;
    in >> temp;
    radian = temp;
    return in;
}

/**
    @brief Function for writing to a stream.
*/
std::ostream& operator<<(std::ostream& out, const orxonox::Degree& degree)
{
    out << degree.valueDegrees();
    return out;
}

/**
    @brief Function for reading from a stream.
*/
std::istream& operator>>(std::istream& in, orxonox::Degree& degree)
{
    float temp;
    in >> temp;
    degree = temp;
    return in;
}
