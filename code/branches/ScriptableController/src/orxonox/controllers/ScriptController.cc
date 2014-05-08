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
#include "infos/PlayerInfo.h"
#include "core/CoreIncludes.h"
#include "worldentities/ControllableEntity.h"
#include "core/LuaState.h"
#include <cmath>

namespace orxonox
{
    RegisterClass(ScriptController);

    //ScriptController::ScriptController(Context* context, ControllableEntity* CE) : ArtificialController(context)
    ScriptController::ScriptController(Context* context) : ArtificialController(context)
    {
        RegisterObject(ScriptController);
        //set_controlled(CE);
        this->ctrlid_ = 0;
    }

    void ScriptController::takeControl(int ctrlid)
    {
        orxout() << "ScriptController: Taking control" << endl;
        orxout() << "This-pointer: " << this << endl;
        this->ctrlid_ = ctrlid;
        this->entity_ = this->player_->getControllableEntity();
        assert(this->entity_);

        this->entity_->setDestroyWhenPlayerLeft(false);
        this->player_->pauseControl();
        this->entity_->setController(this);
        this->setControllableEntity(this->entity_);
    }

    /* Yet to be implemented and tested */
    //void ScriptController::yieldControl()
    //{
        //this->player_->startControl(this->entity_);
        //this->setActive(false);
        //this->controllableEntity_ = NULL;
    //}

    void ScriptController::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
    	//XMLPortParam(ScriptController, BaseObject, "lsrc", set_luasrc, xmlelement, mode);

    }

    const Vector3& ScriptController::getPosition()
    {
        return this->entity_->getPosition();
    }

    ScriptController* ScriptController::getScriptController()
    {
      /* Output a message that confirms this function was called */
      orxout() << "Great success!" << std::endl;

      /* Debugging: print all the scriptcontroller object pointers */
      for(ObjectList<ScriptController>::iterator it = 
        ObjectList<ScriptController>::begin(); 
        it != ObjectList<ScriptController>::end(); ++it)
      { orxout() << "Have object in list: " << *it << endl; }

      /* Find the first one with a nonzero ID */
      for(ObjectList<ScriptController>::iterator it = 
        ObjectList<ScriptController>::begin(); 
        it != ObjectList<ScriptController>::end(); ++it)
      { 
        // TODO: do some selection here. Currently just returns the first one
        if( (*it)->getID() > 0 )
          return *it; 
      
      }
      return NULL;
    }

    void ScriptController::tick(float dt)
    {
        /* If this controller has no entity entry, do nothing */
        if( !(this->entity_) )
          return;

        //orxout() << "Rotating!" << endl;

        //this->entity_->rotateYaw(-1.0f * 100.0f * dt);
        //this->entity_->rotatePitch(0.8f * 100.0f);

        SUPER(ScriptController, tick, dt);
    }


    void ScriptController::moveToPosition_beta(float x, float y, float z )
    {
        //const Vector3 local = this->getPosition();
        const Vector3 target = Vector3(100*x,100*y,100*z);
        //Vector3 way = target-local;
        orxout() << "Moving This-pointer: " << this << endl;
       
        
        this->entity_->lookAt(target);
        this->entity_->moveFrontBack(-1000*target.length());      

  
        /* This works fine */
        orxout()<<x<<"  "<<y<<"  "<<z<<endl;
    }


    /* TODO:    hilfs(zwischen)funktionen um lua eingabe zu ermoeglichen: zb moveToPosition(float...) weil in LUA wohl 
                kein vektor3 definierbar ist

                NB: viele noetige funktionen sind schon in artificial- bzw formationcontroller vorhanden 

                tick funktion?*/        



}
