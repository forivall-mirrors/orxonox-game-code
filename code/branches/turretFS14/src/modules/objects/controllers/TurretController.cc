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
#include "worldentities/pawns/Pawn.h"
#include "Turret.h"

 namespace orxonox
 {
 	RegisterClass(TurretController);

 	TurretController::TurretController(Context* context) : ArtificialController(context)
 	{
 		RegisterObject(TurretController);
 	}

 	TurretController::~TurretController()
 	{

 	}

 	void TurretController::searchTarget()
 	{
        Turret* turret = orxonox_cast<Turret*>(this->getControllableEntity());
        if(target_ && turret->isInRange(target_->getPosition()))
        {
        	return;
        }
        else
        {
        	this->forgetTarget();
        }


        ControllableEntity* parent = orxonox_cast<ControllableEntity*>(turret->getParent());
        if(parent)
        {
        	Pawn* parenttarget = orxonox_cast<Pawn*>(parent->getTarget());
        	if(parenttarget && turret->isInRange(parenttarget->getPosition()))
        	{
        		this->setTarget(parenttarget);
        		turret->setTarget(parenttarget);
        		return;
        	}
        }

  		for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
        {
        	Pawn* entity = orxonox_cast<Pawn*>(*it);
            if (ArtificialController::sameTeam(this->getControllableEntity(), entity, this->getGametype()))
            	continue;

            if(turret->isInRange(entity->getPosition()))
            {
            	this->setTarget(entity);
            	turret->setTarget(entity);
            	break;
            }
    	}		
 	}

 	void TurretController::tick(float dt)
 	{
        if (!this->isActive() || !this->getControllableEntity())
            return;
 		this->searchTarget();
 		this->getControllableEntity()->rotatePitch(0.2);
 		/*if(target_)
 		{
 			this->aimAtTarget();
 			//It says move, but really it only turns
 			this->moveToTargetPosition();
 			if(this->isLookingAtTarget(Degree(5).valueRadians()))
 			{
 				orxout() << 42 << endl;
 			}
 		}*/
 	}
 }