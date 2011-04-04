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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

/**
    @file DockToShip.cc
    @brief Implementation of the DockToShip class.
*/

#include "DockingEffect.h"
#include "DockToShip.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    CreateFactory(DockToShip);

    DockToShip::DockToShip(BaseObject* creator) : DockingEffect(creator)
    {
        RegisterObject(DockToShip);
        COUT(0) << "DockToShip instance created.." << endl;
    }

    DockToShip::~DockToShip()
    {

    }

    bool DockToShip::docking(PlayerInfo* player)
    {
        COUT(0) << "DockToShip::docking" << endl;
        return true;
    }

    bool DockToShip::attach(PlayerInfo* player)
    {
        COUT(0) << "DockToShip::attach" << endl;
        return true;
    }

    bool DockToShip::release(PlayerInfo* player)
    {
        COUT(0) << "DockToShip::release" << endl;
        return true;
    }
}
