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
#include "worldentities/ControllableEntity.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
	RegisterClass(TeamTargetProxy);

   /**
       @brief 
       Sets default values for all variables.

       @param context
       The context
    */
	TeamTargetProxy::TeamTargetProxy(Context* context) : FormationController(context)
 	{
 		RegisterObject(TeamTargetProxy);

 		this->once_ = false;
 	}

    /**
        @brief 
        Destructor. Nothing to see here.
     */
 	TeamTargetProxy::~TeamTargetProxy()
 	{
 	}

    /**
        @brief
        Copies the team and the target from the parent.

        That's all there is.
    */
 	void TeamTargetProxy::tick(float dt)
 	{
	    if (!this->isActive() || !this->getControllableEntity())
	        return;

        ControllableEntity* parent = orxonox_cast<ControllableEntity*> (this->getControllableEntity()->getParent());

        if(this->getTeam() != -1 && !this->once_ && parent)
        {
            orxout(internal_warning) << "TeamTargetProxy: Team already set, may result in undesired behaviour. Will get overridden by the parent's team." << endl;
        }

        if(!this->once_)
            this->once_ = true;

        //Teams aren't set immediately, after creation, so we have to check every tick...
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

        if(parent)
        {
            Pawn* parenttarget = orxonox_cast<Pawn*>(parent->getTarget());
            if(parenttarget)
            {
                this->setTarget(parenttarget);
                this->getControllableEntity()->setTarget(parenttarget);
            }
        }
 	}
}