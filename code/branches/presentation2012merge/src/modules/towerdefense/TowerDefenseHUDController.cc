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
 *
 *   Co-authors:
 *      ...
 */

#include "TowerDefenseHUDController.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Convert.h"


namespace orxonox
{
    CreateFactory(TowerDefenseHUDController);

    TowerDefenseHUDController::TowerDefenseHUDController(BaseObject* creator) : OverlayText(creator)
    {
        RegisterObject(TowerDefenseHUDController);
    }

    TowerDefenseHUDController::~TowerDefenseHUDController()
    {

    }

    void TowerDefenseHUDController::tick(float dt)
    {
        SUPER(TowerDefenseHUDController, tick, dt);
    }

    void TowerDefenseHUDController::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(TowerDefenseHUDController, XMLPort, xmlelement, mode);
    }

    void TowerDefenseHUDController::changedOwner()
    {
        SUPER(TowerDefenseHUDController, changedOwner);
        /*
        if (this->getOwner() != NULL && this->getOwner()->getGametype())
            this->owner_ = orxonox_cast<Pong*>(this->getOwner()->getGametype().get());
        else
            this->owner_ = 0;
        */
    }

}
