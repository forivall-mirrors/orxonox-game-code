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

#include "RocketController.h"
#include "worldentities/Rocket.h"
#include "util/Math.h"


namespace orxonox
{
	CreateFactory(RocketController);
    /**
    @brief
        Constructor.
    */
    RocketController::RocketController(BaseObject* creator) : Controller(creator)
    {
        // Place your code here:
		RegisterObject(RocketController);
        // - make sure to register the object in the factory
        // - do any kind of initialisation
        
        // this checks that our creator really is a Rocket
        // and saves the pointer to the Rocket for the controlling commands
        assert(dynamic_cast<Rocket*>(creator)!=0);
        this->setControllableEntity(dynamic_cast<Rocket*>(creator));
		Rocket *myRocket = static_cast<Rocket*>(this->getControllableEntity());
		//myRocket->setVelocity(Vector3(0,0,-60));
    }

    RocketController::~RocketController()
    {
    }

    /**
    @brief
        The controlling happens here. This method defines what the controller has to do each tick.
    @param dt
        The duration of the tick.
    */
    void RocketController::tick(float dt)
    {	
        // Place your code here:
        // - steering commands
        //Rocket *myRocket = static_cast<Rocket*>(this->getControllableEntity());
       
        // you can use the following commands for steering 
        // - moveFrontBack, moveRightLeft, moveUpDown 
        // - rotatePitch, rotateYaw, rotateRoll 
        // - apply the to myRocket (e.g. myRocket->rotateYaw(..) ) 

    }
}
