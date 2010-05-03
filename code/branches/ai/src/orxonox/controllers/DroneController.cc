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
#include "core/Executor.h"
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
        // - do any kind of initialisation

        // this checks that our creator really is a drone
        // and saves the pointer to the drone for the controlling commands

    
        RegisterObject(DroneController);

        this->owner_ = 0;
        this->drone_ = 0;

        this->actionTimer_.setTimer(ACTION_INTERVAL, true, createExecutor(createFunctor(&DroneController::action, this)));
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

        const Vector3& ownerPosition = getOwner()->getWorldPosition();
        const Vector3& dronePosition = getDrone()->getWorldPosition();

        const Vector3& locOwnerDir = getDrone()->getOrientation().UnitInverse()*(dronePosition-ownerPosition); //Vector from Drone To Owner out of drones local coordinate system

        int distance = sqrt( (ownerPosition.x-dronePosition.x)*(ownerPosition.x-dronePosition.x)
                           + (ownerPosition.y-dronePosition.y)*(ownerPosition.y-dronePosition.y)
                           + (ownerPosition.z-dronePosition.z)*(ownerPosition.z-dronePosition.z)); //distance to Owner

        if (distance > 500) { //TODO: variable implementation of maxdistance
            drone_->moveUpDown(-locOwnerDir.y);
            drone_->moveFrontBack(locOwnerDir.z);
            drone_->moveRightLeft(-locOwnerDir.x);
        }

        COUT(0) << "Owner: " << ownerPosition << endl;
        COUT(0) << "Drone: " << dronePosition << endl;
        COUT(0) << "Distance: " << distance << endl;
        COUT(0) << "locDrone: " << locOwnerDir << endl;


/*
        // search enemy
        random = rnd(maxrand);
        if (random < 15 && (!this->target_))
            this->searchNewTarget();

        // forget enemy
        random = rnd(maxrand);
        if (random < 5 && (this->target_))
            this->forgetTarget();

        // next enemy
        random = rnd(maxrand);
        if (random < 10 && (this->target_))
            this->searchNewTarget();

        //fly somewhere
        random = rnd(maxrand);
        if (random < 50 && (!this->bHasTargetPosition_ && !this->target_))
            this->searchRandomTargetPosition(); 
 
        // stop flying
        random = rnd(maxrand);
        if (random < 10 && (this->bHasTargetPosition_ && !this->target_))
            this->bHasTargetPosition_ = false;

        // fly somewhere else
        random = rnd(maxrand);
        if (random < 30 && (this->bHasTargetPosition_ && !this->target_))
            this->searchRandomTargetPosition();

        // shoot
        random = rnd(maxrand);
        if (random < 75 && (this->target_ && !this->bShooting_))
            this->bShooting_ = true;

        // stop shooting
        random = rnd(maxrand);
        if (random < 25 && (this->bShooting_)) 
            this->bShooting_ = false; */
    }


    /**
    @brief
        The controlling happens here. This method defines what the controller has to do each tick.
    @param dt
        The duration of the tick.
    */
    void DroneController::tick(float dt)
    {
        

	Drone *myDrone = static_cast<Drone*>(this->getControllableEntity());

        if(myDrone != NULL) {

        setTargetPosition(this->getControllableEntity()->getPosition());

/*      myDrone->setRotationThrust(25);
	myDrone->setAuxilaryThrust(30);
	myDrone->rotateYaw(10*dt); */
        }

        SUPER(AIController, tick, dt);

        // you can use the following commands for steering 
        // - moveFrontBack, moveRightLeft, moveUpDown 
        // - rotatePitch, rotateYaw, rotateRoll 
        // - apply the to myDrone (e.g. myDrone->rotateYaw(..) ) 

    }
}
