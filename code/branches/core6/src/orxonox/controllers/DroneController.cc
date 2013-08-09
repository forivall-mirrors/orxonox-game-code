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

    const float DroneController::ACTION_INTERVAL = 1.0f;

    DroneController::DroneController(Context* context) : ArtificialController(context)
    {
        RegisterObject(DroneController);

        this->owner_ = 0;
        this->drone_ = 0;
        this->isShooting_ = false;
        this->setAccuracy(10);

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
    /* PORTALS workaround:
    if the owner uses a portal -> distance between owner and drone is huge -> is detected by drone
    -> drone searches for portal -> drone adds portal as waypoint -> drone flies towards portal //ignores owner
    -> if the drone used a portal, then the distance to the owner is small -> remove waypoint // back to normal mode

    */
    void DroneController::tick(float dt)
    {
        if (this->getDrone() && this->getOwner())
        {
            if (this->waypoints_.size() > 0 ) //Waypoint functionality: Drone should follow it's master through portals
            {// Idea: after using the the portal, the master is far away.
                WorldEntity* wPoint = this->waypoints_[this->waypoints_.size()-1];
                if(wPoint)
                {
                    float distanceToOwnerSquared = (this->getDrone()->getWorldPosition() - this->getOwner()->getWorldPosition()).squaredLength();
                    this->absoluteMoveToPosition(wPoint->getWorldPosition()); //simplified function - needs WORKAROUND
                    if (distanceToOwnerSquared <= 90000.0f) //WORKAROUND: if the Drone is again near its owner, the portal has been used correctly.
                    {
                        this->waypoints_.pop_back(); // if goal is reached, remove it from the list
                        this->positionReached(); //needed??
                    }

                }
                else
                    this->waypoints_.pop_back(); // remove invalid waypoints
            }
            else
            {
                if (this->target_)
                {
                    float distanceToTargetSquared = (this->getDrone()->getWorldPosition() - this->target_->getWorldPosition()).squaredLength();
                    if (distanceToTargetSquared < (this->getDrone()->getMaxShootingRange()*this->getDrone()->getMaxShootingRange()))
                    {
                       this->isShooting_ = true;
                       this->aimAtTarget();
                       if(!this->friendlyFire())
                           this->getDrone()->fire(0);
                    }
               }


                float maxDistanceSquared = this->getDrone()->getMaxDistanceToOwner()*this->getDrone()->getMaxDistanceToOwner();
                float minDistanceSquared = this->getDrone()->getMinDistanceToOwner()*this->getDrone()->getMinDistanceToOwner();
                if((this->getDrone()->getWorldPosition() - this->getOwner()->getWorldPosition()).squaredLength()  > 20.0f*maxDistanceSquared)
                {//FIX: if the drone's owner uses portal, the drone searches for the portal & adds it as a waypoint.
                    this->updatePointsOfInterest("PortalEndPoint", 500.0f); //possible conflict: speed-pickup
                }
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

    bool DroneController::friendlyFire()
    {   ControllableEntity* droneEntity_ = this->getControllableEntity();
        if (!droneEntity_) return false;
        if(!owner_) return false;
        if(this->bHasTargetPosition_)
        {
            Vector3 ownerPosition_ = owner_->getPosition();
            Vector3 toOwner_ = owner_->getPosition() - droneEntity_->getPosition();
            Vector3 toTarget_ = targetPosition_ - droneEntity_->getPosition();
            if(toTarget_.length() < toOwner_.length()) return false; //owner is far away = in safty
            float angleToOwner = getAngle(droneEntity_->getPosition(), droneEntity_->getOrientation() * WorldEntity::FRONT, ownerPosition_);
            float angleToTarget = getAngle(droneEntity_->getPosition(), droneEntity_->getOrientation() * WorldEntity::FRONT, targetPosition_);
            float angle = angleToOwner - angleToTarget;//angle between target and owner, observed by the drone
            if(std::sin(angle)*toOwner_.length() < 5.0f)//calculate owner's distance to shooting line
            return true;
        }
        return false;//Default return value: Usually there is no friendlyFire
    }
}
