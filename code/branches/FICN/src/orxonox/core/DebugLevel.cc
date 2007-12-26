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
 *      ...
 *
 */

/*!
    @file DebugLevel.cc
    @brief Implementation of the DebugLevel class.
*/

#include "ConfigValueContainer.h"
#include "Debug.h"
#include "DebugLevel.h"

namespace orxonox
{
    /**
        @brief Constructor: Registers the object and sets the debug level.
    */
    DebugLevel::DebugLevel()
    {
        this->softDebugLevelContainer_ = new ConfigValueContainer(std::string("DebugLevel"), std::string("softDebugLevel_"), this->softDebugLevel_ = 2);
        this->softDebugLevel_ = this->softDebugLevelContainer_->getValue(this->softDebugLevel_);
    }
}

/**
    @returns the soft debug level, stored in the only existing instance of the DebugLevel class, configured in the config-file.
*/
int getSoftDebugLevel()
{
    return orxonox::DebugLevel::getSoftDebugLevel();
}
