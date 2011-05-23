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

#include <cmath>

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
        ControllableEntity* entity = this->getControllableEntity();
        if (!entity)
            return;

        float distance = (dock->getWorldPosition() - entity->getPosition()).length();
        Vector2 coord = get2DViewdirection(     // I don't understand this too
            entity->getPosition(),
            entity->getOrientation() * WorldEntity::FRONT,
            entity->getOrientation() * WorldEntity::UP,
            dock->getWorldPosition()
        );

        // adjust direction of spaceship
        if (distance > 10)
        {
            entity->rotateYaw(-1.0f * 0.8f * sgn(coord.x) * coord.x*coord.x);
            entity->rotatePitch(0.8f * sgn(coord.y) * coord.y*coord.y);
        }

        /*// adjust speed
        if (distance < 200 && entity->getVelocity().squaredLength() > dock->getVelocity().squaredLength())
            entity->moveFrontBack(0.2f);
        else
            entity->moveFrontBack(0.8f);*/

        entity->moveFrontBack(0.5f * log(distance/10.0f));

        if (distance < 20)
            this->positionReached();

        SUPER(DockingController, tick, dt);
    }

    void DockingController::takeControl(bool docking)
    {
        this->docking = docking;

        entity = player->getControllableEntity();
        assert(entity);

        if (docking)
        {
            COUT(0) << "DockingController::takeControl Taking over control." << std::endl;

            entity->setDestroyWhenPlayerLeft(false);
            player->pauseControl();
            entity->setController(this);
            this->setControllableEntity(entity);
        }
    }

    void DockingController::positionReached()
    {
        COUT(0) << "DockingController::positionReached() called." << std::endl;

        assert(this->player);
        assert(this->dock);

        // stop spaceship
        dock->attach(entity);
        entity->setVelocity(0, 0, 0);
        entity->setOrientation(dock->getOrientation());

        // give control back to player
        player->startControl(entity);
        this->setActive(false);
        this->controllableEntity_ = NULL;

        if (docking)
            dock->dockingAnimationFinished(player);
        /*else
            dock->undockingAnimationFinished(player);*/

        this->destroy();
    }
}

