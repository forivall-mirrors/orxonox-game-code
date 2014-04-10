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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "ScriptController.h"
#include "core/CoreIncludes.h"
#include "worldentities/ControllableEntity.h"

namespace orxonox
{
    RegisterClass(ScriptController);

    //ScriptController::ScriptController(Context* context, ControllableEntity* CE) : ArtificialController(context)
    ScriptController::ScriptController(Context* context) : ArtificialController(context)
    {
        RegisterObject(ScriptController);
        //set_controlled(CE);
    }

    void ScriptController::set_luasrc(std::string lsrc)
    {
        this->luasrc=lsrc;
    }

    void ScriptController::set_controlled(ControllableEntity* toControl)
    {
        this->controlled=toControl;
    }
    
    void ScriptController::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
    	//XMLPortParam(ScriptController, BaseObject, "lsrc", set_luasrc, xmlelement, mode);

    }

    void ScriptController::moveToPosition(const Vector3& target)
    {
        
        /*this->controlled_->rotateYaw(-sgn(coord.x)*coord.x*coord.x);
        this->controlled_->rotatePitch(sgn(coord.y)*coord.y*coord.y);*/
    }

    /*void WaypointController::tick(float dt)  //copied from waypointcontroller
    {
        if (!this->isActive())
            return;

        if (this->waypoints_.size() == 0 || !this->getControllableEntity())
            return;

        if (this->waypoints_[this->currentWaypoint_]->getWorldPosition().squaredDistance(this->getControllableEntity()->getPosition()) <= this->squaredaccuracy_)
            this->currentWaypoint_ = (this->currentWaypoint_ + 1) % this->waypoints_.size();

        this->moveToPosition(this->waypoints_[this->currentWaypoint_]->getWorldPosition());
    }*/

    /* TO DO
                in the constuctor: make accessible functions such as moveToPosition.. in LUA 
            ->tolua++ example: http://usefulgamedev.weebly.com/tolua-example.html*/



    //function to execute the luafile



}
