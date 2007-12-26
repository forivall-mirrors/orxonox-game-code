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

#include "CoreIncludes.h"
#include "Debug.h"
#include "DebugLevel.h"

namespace orxonox
{
    DebugLevel* DebugLevel::pointer_s = 0;                // Set the static member variable pointer_s to zero
    bool DebugLevel::bCreatingDebugLevelObject_s = false; // Set the static member variable bCreatingDebugLevelObject_s to false

    /**
        @brief Constructor: Registers the object and sets the debug level.
    */
    DebugLevel::DebugLevel()
    {
        RegisterRootObject(DebugLevel);
        SetConfigValue(softDebugLevel_s, 2);
    }

    /**
        @returns a pointer to the only existing instance of this class.
    */
    int DebugLevel::getSoftDebugLevel()
    {
        if (!pointer_s && !bCreatingDebugLevelObject_s)
        {
            // We need the bCreatingDebugLevelObject_s variable to avoid an infinite recursion:
            // Creating the object produces debug output and debug output needs the object.
            bCreatingDebugLevelObject_s = true;
            pointer_s = new DebugLevel;
            bCreatingDebugLevelObject_s = false;
        }
        else if (bCreatingDebugLevelObject_s)
            return 4;

        return pointer_s->softDebugLevel_s;
    }
}

/**
    @returns the soft debug level, stored in the only existing instance of the DebugLevel class, configured in the config-file.
*/
int getSoftDebugLevel()
{
    return orxonox::DebugLevel::getSoftDebugLevel();
}
