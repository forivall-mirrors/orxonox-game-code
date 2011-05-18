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
 *      Sven Stucki
 *   Co-authors:
 *      ...
 *
 */

#include "DockingController.h"

#include "infos/PlayerInfo.h"
#include "worldentities/ControllableEntity.h"
#include "Dock.h"

namespace orxonox
{
    CreateFactory(DockingController);

    DockingController::DockingController(BaseObject* creator) : ArtificialController(creator)
    {
        RegisterObject(DockingController);
    }

    DockingController::~DockingController()
    {

    }

    void DockingController::tick(float dt)
    {
        this->moveToTargetPosition();

        SUPER(DockingController, tick, dt);
    }

    void DockingController::positionReached()
    {
        // TODO; Give control back to player
        PlayerInfo* player = this->entity->getPlayer();
        assert(player);

        if (docking)
            dock->dockingAnimationFinished(player);
        else
            dock->undockingAnimationFinished(player);
    }
}

