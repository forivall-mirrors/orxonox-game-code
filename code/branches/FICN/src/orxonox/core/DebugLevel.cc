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

#include "CoreIncludes.h"
#include "DebugLevel.h"
#include "Debug.h"

namespace orxonox
{
    DebugLevel* DebugLevel::pointer_s = 0;
    bool DebugLevel::bCreatingDebugLevelObject_s = false;

    DebugLevel::DebugLevel()
    {
        RegisterRootObject(DebugLevel);
        SetConfigValue(softDebugLevel_s, 2);
    }

    int DebugLevel::getSoftDebugLevel()
    {
        if (!pointer_s && !bCreatingDebugLevelObject_s)
        {
            bCreatingDebugLevelObject_s = true;
            pointer_s = new DebugLevel;
            bCreatingDebugLevelObject_s = false;
        }

        if (bCreatingDebugLevelObject_s)
            return 4;

        return pointer_s->softDebugLevel_s;
    }
}

int getSoftDebugLevel()
{
    return orxonox::DebugLevel::getSoftDebugLevel();
}
