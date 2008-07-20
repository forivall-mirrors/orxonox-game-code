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
 *      Nicolas Perrenoud
 *   Co-authors:
 *      ...
 *
 */

/**
    @file Error.cc
    @brief Implementation of the Error class.
*/

#include "Error.h"
#include "Debug.h"

namespace orxonox
{
    Error::Error(std::string errorMsg, int errorCode)
    {
        COUT(1) << "############################ "<< std::endl
                            << "#         Error "<<errorCode<< "          #"<< std::endl
                            << "############################ "<< std::endl
                            << "An error occured in Orxonox: "<< std::endl;

        if (errorMsg=="")
        {
            switch (errorCode)
            {
                case 1:
                    errorMsg = "Some error!";
                    break;
                default:
                    errorMsg = "Unknown error!";
            }
        }
        COUT(1) << errorMsg << std::endl<< std::endl;
    }
}
