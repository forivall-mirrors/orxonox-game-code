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
 *      Oli Scheuss
 *   Co-authors:
 *      ...
 *
 */

#include "DroneController.h"
#include "worldentities/Drone.h"
#include "util/Math.h"

#include "core/CoreIncludes.h"
#include "core/command/Executor.h"
#include "worldentities/ControllableEntity.h"

namespace orxonox
{
    /**
    @brief
        Constructor.
    */
    CreateFactory(DroneController);

    static const float ACTION_INTERVAL = 1.0f;

    DroneController::DroneController(BaseObject* creator) : ArtificialController(creator)
    {
        RegisterObject(DroneController);

        this->owner_ = 0;
        this->drone_ = 0;
        this->isShooting_ = false;

        this->actionTimer_.setTimer(ACTION_INTERVAL, true, createExecutor(createFunctor(&DroneController::action, this)));

        this->owner_.setCallback(createFunctor(&DroneController::ownerDied, this));
    }

    DroneController::~DroneController()
    {
    }

    void DroneController::setOwner(Pawn* owner){
        this->owner_ = owner;
    }

    void DroneController::setDrone(Drone* drone)
    {
        this->drone_ = drone;
        this->setControllableEntity(drone);
    }

    void DroneController::action()
    {
        float random;
        float maxrand = 100.0f / ACTION_INTERVAL;
        float distanceToTargetSquared = 0;

        if (this->target_)
            distanceToTargetSquared = (getDrone()->getWorldPosition() - this->target_->getWorldPosition()).squaredLength();

        random = rnd(maxrand);
        if ( random < 30 || (!this->target_) || distanceToTargetSquared > (this->getDrone()->getMaxShootingRange()*this->getDrone()->getMaxShootingRange()))
            this->searchNewTarget();
    }

    /**
    @brief
        The controlling happens here. This method defines what the controller has to do each tick.
    @param dt
        The duration of the tick.
    */
    void DroneController::tick(float dt)
    {
        if (this->getDrone() && this->getOwner())
        {
            if (this->target_)
            {
                float distanceToTargetSquared = (this->getDrone()->getWorldPosition() - this->target_->getWorldPosition()).squaredLength();
                if (distanceToTargetSquared < (this->getDrone()->getMaxShootingRange()*this->getDrone()->getMaxShootingRange()))
                {
                    this->isShooting_ = true;
                    this->aimAtTarget();
                    this->getDrone()->fire(0);
                }
            }

            float maxDistanceSquared = this->getDrone()->getMaxDistanceToOwner()*this->getDrone()->getMaxDistanceToOwner();
            float minDistanceSquared = this->getDrone()->getMinDistanceToOwner()*this->getDrone()->getMinDistanceToOwner();
            if ((this->getDrone()->getWorldPosition() - this->getOwner()->getWorldPosition()).squaredLength()  > maxDistanceSquared)
            {
                this->moveToPosition(this->getOwner()->getWorldPosition()); //fly towards owner
            }
            else if((this->getDrone()->getWorldPosition() - this->getOwner()->getWorldPosition()).squaredLength() < minDistanceSquared)
            {
                this->moveToPosition(-this->getOwner()->getWorldPosition()); //fly away from owner
            }
            else if (!this->isShooting_)
            {
                float random = rnd(2.0f);
                float randomSelection = rnd(6.0f);
                if((int)randomSelection==0) drone_->moveUpDown(random);
                else if((int)randomSelection==1) drone_->moveRightLeft(random);
                else if((int)randomSelection==2) drone_->moveFrontBack(random);
                else if((int)randomSelection==3) drone_->rotateYaw(random);
                else if((int)randomSelection==4) drone_->rotatePitch(random);
                else if((int)randomSelection==5) drone_->rotateRoll(random);
            }

            this->isShooting_ = false;
        }

        SUPER(AIController, tick, dt);

    }

    void DroneController::ownerDied()
    {
//         if (this->target_) {            //Drone has some kind of Stockholm-Syndrom --> gets attached to Owners Killer
//             this->setOwner(target_);
//             this->searchNewTarget();
//         }
        if (this->drone_)
            this->drone_->destroy();
        else
            this->destroy();
    }
}
