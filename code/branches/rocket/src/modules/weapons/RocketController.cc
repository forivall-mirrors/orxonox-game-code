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
 *     Gabriel Nadler, Originalfile: Oli Scheuss
 *   Co-authors:
 *      ...
 *
 */

#include "RocketController.h"
#include "projectiles/SimpleRocket.h" 
#include "util/Math.h"
#include "weapons/projectiles/SimpleRocket.h"


namespace orxonox
{
    /**
    @brief
        Constructor.
    */
    RocketController::RocketController(BaseObject* creator) : Controller(creator)
    {

        RegisterObject(RocketController);
		SimpleRocket* rocket = new SimpleRocket(this);
		rocket->setController(this);
		this->setControllableEntity(dynamic_cast<ControllableEntity*> (rocket));

    }


    /**
    @brief
        The controlling happens here. This method defines what the controller has to do each tick.
    @param dt
        The duration of the tick.
    */
    void RocketController::tick(float dt)
    {

        SimpleRocket *rocket = static_cast<SimpleRocket*>(this->getControllableEntity());


		rocket->rotateYaw(0.2);
			//rocket->moveFrontBack(2);

    }


	RocketController::~RocketController() {

		delete this->getControllableEntity();

	}



}
