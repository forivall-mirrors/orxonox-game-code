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

#include "TeamTargetProxy.h"
#include "worldentities/pawns/Pawn.h"

 namespace orxonox
 {
 	RegisterClass(TeamTargetProxy);

 	TeamTargetProxy::TeamTargetProxy(Context* context) : FormationController(context)
 	{
 		RegisterObject(TeamTargetProxy);

 		this->once_ = false;
 	}

 	TeamTargetProxy::~TeamTargetProxy()
 	{
 	}

 	void TeamTargetProxy::tick(float dt)
 	{
	    if (!this->isActive() || !this->getControllableEntity())
	        return;

        ControllableEntity* parent = orxonox_cast<ControllableEntity*> (this->getControllableEntity()->getParent());
 		


        if(parent)
        {

            if(!this->once_)
            {
                //Set the same team
                if(parent)
                {
                    Controller* parentcontroller = parent->getController();
                    if(parentcontroller)
                    {
                        this->setTeam(parentcontroller->getTeam());
                    }
                    else
                    {
                        this->setTeam(parent->getTeam());
                    }
                    this->getControllableEntity()->setTeam(parent->getTeam());
                }
                this->once_ = true;
            }

            Pawn* parenttarget = orxonox_cast<Pawn*>(parent->getTarget());
            if(parenttarget)
            {
                this->setTarget(parenttarget);
                this->getControllableEntity()->setTarget(parenttarget);
            }
        }

 	}
 }