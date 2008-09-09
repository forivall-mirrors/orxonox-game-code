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
 *   Co-authors:
 *      ...
 *
 */

/**
@file
@brief
    Implementation of the Exception class.
*/

#include "Exception.h"

namespace orxonox
{
    Exception::Exception(const std::string& description, int lineNumber,
                         const char* fileName, const char* functionName)
        : description_(description)
        , lineNumber_(lineNumber)
        , functionName_(functionName)
        , fileName_(fileName)
    { }

    Exception::Exception(const std::string& description)
        : description_(description)
        , lineNumber_(0)
        , functionName_("")
        , fileName_("")
    { }

    const std::string& Exception::getFullDescription() const
    {
        if (fullDescription_ == "")
        {
            std::ostringstream fullDesc;

            fullDesc << this->getTypeName() << "_EXCEPTION";

            if (this->fileName_ != "")
            {
                fullDesc << " in " << this->fileName_;
                if (this->lineNumber_)
                    fullDesc << "(" << this->lineNumber_ << ")";
            }

            if (this->functionName_ != "")
                fullDesc << " in function '" << this->functionName_ << "'";

            fullDesc << ": ";
            if (this->description_ != "")
                fullDesc << this->description_;
            else
                fullDesc << "No description available.";

            this->fullDescription_ = std::string(fullDesc.str());
        }

        return fullDescription_;
    }


}
