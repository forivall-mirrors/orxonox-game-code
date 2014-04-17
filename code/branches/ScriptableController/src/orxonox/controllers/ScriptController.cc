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
#include <cmath>

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

    const Vector3& ScriptController::getPosition()
    {
        return this->controlled->getPosition();
    }

    void ScriptController::moveToPosition_beta(float x, float y, float z )
    {

       const Vector3 local=getPosition();
       const Vector3 target=Vector3(x,y,z);

       Vector3 way=target-local;

       
       this->controlled->lookAt(target);

        this->controlled->moveFrontBack(way.length());      


        /*orxout()<<x<<"  "<<y<<"  "<<z<<endl;*/



         
    }

    /* TODO:    hilfs(zwischen)funktionen um lua eingabe zu ermoeglichen: zb moveToPosition(float...) weil in LUA wohl 
                kein vektor3 definierbar ist

                NB: viele noetige funktionen sind schon in artificial- bzw formationcontroller vorhanden 

                tick funktion?*/        



}
