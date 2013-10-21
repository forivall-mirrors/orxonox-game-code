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
 *      Damian 'Mozork' Frick
 *
 */

#include "AutonomousDroneController.h"
#include "worldentities/AutonomousDrone.h"
#include "util/Math.h"
#include <core/input/InputManager.h>
#include <core/input/InputHandler.h>
#include <core/input/InputState.h>

namespace orxonox
{

    /**
    @brief
        Constructor.
    @param context
        The context of this object.
    */
	RegisterClass(AutonomousDroneController);

	float time=0;
    AutonomousDroneController::AutonomousDroneController(Context* context) : Controller(context)
    {
        //TODO: Place your code here:
        // Make sure to register the object and create the factory.
    	RegisterObject(AutonomousDroneController);
        // This checks that our context really is a drone
        // and saves the pointer to the drone for the controlling commands
        /* NOTE from Sandro: This is currently broken */
        //AutonomousDrone* drone = dynamic_cast<AutonomousDrone*>(context);
        //assert(drone != NULL);
        //this->setControllableEntity(drone);
    }

    /**
    @brief
        Destructor.
    */
    AutonomousDroneController::~AutonomousDroneController()
    {

    }

    /**
    @brief
        The controlling happens here. This method defines what the controller has to do each tick.
    @param dt
        The duration of the tick.
    */
    void AutonomousDroneController::tick(float dt)
    {
        /* NOTE: Ugly hack by Sandro to make the tutorial work for the moment.
         * This will be reverted once the framework update is complete
         */
        //AutonomousDrone *myDrone = static_cast<AutonomousDrone*>(this->getControllableEntity());
        ObjectList<AutonomousDrone>::iterator it = ObjectList<AutonomousDrone>::begin();

        //TODO: Place your code here:
        // Steering commands
        // You can use the commands provided by the AutonomousDrone to steer it:
        // - moveFrontBack, moveRightLeft, moveUpDown
        // - rotatePitch, rotateYaw, rotateRoll
        // You will see, that the AutonomousDrone has two variants for each of these commands, one with a vector as input and one with just a float. Use the one with just the float as input.
        // Apply them to myDrone (e.g. myDrone->rotateYaw(..) )
        time+=dt;
        it->moveUpDown(sin(time*5)*600*dt);
        it->rotateYaw(300*dt);
        it->setPrimaryThrust(150);
        it->moveFrontBack(1200*dt);
		IntVector2 abs(10,50);
		IntVector2 rel(50, 20);
		IntVector2 clippingSize(1680, 1050);
		InputManager * myIM = &(InputManager::getInstance());
		InputState * gameState = myIM->getState("game");
		gameState->mouseMoved(abs, rel, clippingSize);
		orxout()<<"I'm a huge faggot";
    }



}
