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

#include "OrxonoxStableHeaders.h"
#include "DroneController.h"
#include "objects/worldentities/Drone.h"
#include "util/Math.h"


namespace orxonox
{
    DroneController::DroneController(BaseObject* creator) : Controller(creator)
    {
        // Place your code here:
        // - make sure to register the object in the factory
        // - do any kind of initialisation
        RegisterObject(DroneController);
        
        
        
        // this checks that our creator really is a drone
        // and saves the pointer to the drone for the controlling commands
        assert(dynamic_cast<Drone*>(creator)!=0);
        this->setControllableEntity(dynamic_cast<Drone*>(creator));
    }

    DroneController::~DroneController()
    {
    }

    void DroneController::tick(float dt)
    {
        // Place your code here:
        // - steering commands
        Drone *myDrone = static_cast<Drone*>(this->getControllableEntity());
        // you can use the following commands for steering
        // - moveFrontBack, moveRightLeft, moveUpDown
        // - rotatePitch, rotateYaw, rotateRoll
        // - apply the to myDrone (e.g. myDrone->rotateYaw(..) )
        
    }
}
