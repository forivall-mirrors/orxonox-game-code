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

        this->dock_ = NULL;
        this->player_ = NULL;
        this->entity_ = NULL;
    }

    DockingController::~DockingController()
    {

    }

    void DockingController::tick(float dt)
    {
        ControllableEntity* entity = this->getControllableEntity();
        if (!entity)
            return;

        float distance = (this->dock_->getWorldPosition() - entity->getPosition()).length();
        Vector2 coord = get2DViewdirection(     // I don't understand this too
            entity->getPosition(),
            entity->getOrientation() * WorldEntity::FRONT,
            entity->getOrientation() * WorldEntity::UP,
            dock_->getWorldPosition()
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
        this->docking_ = docking;

        this->entity_ = this->player_->getControllableEntity();
        assert(this->entity_);

        if (docking)
        {
            COUT(4) << "DockingController::takeControl Taking over control." << std::endl;

            this->entity_->setDestroyWhenPlayerLeft(false);
            this->player_->pauseControl();
            this->entity_->setController(this);
            this->setControllableEntity(this->entity_);
        }
    }

    void DockingController::positionReached()
    {
        COUT(4) << "DockingController::positionReached() called." << std::endl;

        assert(this->player_);
        assert(this->dock_);

        // stop spaceship
        this->entity_->setPosition(this->dock_->getWorldPosition());
        this->entity_->setVelocity(0, 0, 0);
        this->entity_->setOrientation(this->dock_->getWorldOrientation());

        // give control back to player
        this->player_->startControl(this->entity_);
        this->setActive(false);
        this->controllableEntity_ = NULL;

        if (this->docking_)
            this->dock_->dockingAnimationFinished(this->player_);
        /*else
            dock->undockingAnimationFinished(player);*/

        this->destroy();
    }
}

