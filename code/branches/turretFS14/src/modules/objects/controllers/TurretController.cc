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
 *      Martin Mueller
 *   Co-authors:
 *      ...
 *
 */

 #include "TurretController.h"

 namespace orxonox
 {
 	RegisterClass(TurretController);

 	TurretController::TurretController(Context* context) : ArtificialController(context)
 	{
 		RegisterObject(TurretController);
 		counter = 0;
 		flag = false;
 	}

 	TurretController::~TurretController()
 	{

 	}

 	void TurretController::tick(float dt)
 	{
 		counter += dt;
 		if(counter >= 10)
 		{
 			counter = 0;
 			flag = !flag;
 			orxout() << "Direction change" << endl;
 		}
 		if(flag)
 		{
 			this->getControllableEntity()->rotatePitch(10*dt);
 			//this->getControllableEntity()->rotateYaw(10*dt);
 		}
 		else
 		{
 			this->getControllableEntity()->rotatePitch(-10*dt);
 			//this->getControllableEntity()->rotateYaw(-10*dt);
 		}
 	}
 }