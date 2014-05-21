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

    ScriptController::ScriptController(Context* context) : ArtificialController(context)
    {
        RegisterObject(ScriptController);

        /* By default, this controller has ID 0, which means it is not assigned
         * to anything yet. 
         */
        this->ctrlid_ = 0;


        /* Set default values for all variables */
        /* - pointers to zero */
        this->player_ = NULL;
        this->entity_ = NULL;

        /* - times */
        this->scTime = 0.0f;
        this->timeToTarget = 0.0f;
        this->eventTime = 0.0f;

        /* - Points in space */
        this->target = Vector3(0,0,0);
        this->startpos = Vector3(0,0,0);
        this->lookAtPosition = Vector3(0,0,0);

        /* - Processing flag */
        this->processing = false;

        /* - Counters */
        this->eventno = 0;

    }

    void ScriptController::takeControl(int ctrlid)
    {
        /* Output some debugging information */
        orxout() << "ScriptController: Taking control" << endl;
        orxout() << "This-pointer: " << this << endl;

        /* Set the controller ID (the argument here should be nonzero) */
        this->ctrlid_ = ctrlid;

        /* Store the entity pointer in a private variable */
        this->entity_ = this->player_->getControllableEntity();
        assert(this->entity_);
         
        /* Add the controller here to this entity. Apparently this still leaves
         * any preexisting human controllers in place. 
         */
        this->entity_->setDestroyWhenPlayerLeft(false);
        this->player_->pauseControl();
        this->entity_->setController(this);
        this->setControllableEntity(this->entity_);
        this->entity_->mouseLook();
        this->entity_->setVisible(false);
    }

    /* Yet to be implemented and tested */
    //void ScriptController::yieldControl()
    //{
        //this->player_->startControl(this->entity_);
        //this->setActive(false);
        //this->controllableEntity_ = NULL;
    //}

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
        { orxout() << "Controller to return: " << *it << endl;
          return *it; 
        }
      
      }
      return NULL;
    }

    void ScriptController::execute(event ev)
    {
        orxout() << "Executing event " << ev.fctName 
          << " with parameters:\n " 
          << ev.x1 << " " << ev.y1 << " " << ev.z1 << "\n"
          << ev.x2 << " " << ev.y2 << " " << ev.z2 << "\n"
          << ev.duration << endl;

        this->eventTime = 0.0f;
        this->startpos = this->entity_->getPosition();
        this->processing = true;

        if(ev.fctName == "mal")
          moveAndLook(ev.x1, ev.y1, ev.z1, ev.x2, ev.y2, ev.z2, ev.duration);
    }


    void ScriptController::tick(float dt)
    {
        /* Call the tick function of the classes we derive from */
        SUPER(ScriptController, tick, dt);

        /* If this controller has no entity entry, do nothing */
        if( !(this->entity_) )
          return;

        //orxout() << "Size 0: " << this->eventList.size() << endl;

        /* See if time has come for the next event to be run */
        if(this->eventList.size() > 0 && this->eventList[0].eventTime <= scTime)
        {
          /* Execute the next event on the list */
          this->execute(this->eventList[0]);
          this->eventList.erase(this->eventList.begin());
          this->eventno -= 1;
          //orxout() << "Size 1: " << this->eventList.size() << endl;
          //orxout() << "Eventno is now: " << this->eventno << endl;
        }

        /* Update the local timers in this object */
        scTime += dt;
        eventTime += dt;

        /* If we've arrived at the target, stop processing */
        if( eventTime > timeToTarget && this->processing == true)
        { this->processing = false;

          //orxout() << "Size 4: " << this->eventList.size() << endl;
          //orxout() << "Eventno: " << this->eventno << endl;
          
          if( this->eventno == 0 )
          {
            this->entity_->mouseLook();
            this->entity_->setVisible(true);
          }
        }

        /* Get a variable that specifies how far along the trajectory
         * we are 
         */
        float dl = eventTime / timeToTarget; 

        /* Do some moving */
        if( this->processing )
        { 
          /* Set the position to the correct place in the trajectory */
          this->entity_->setPosition( (1-dl)*startpos + dl * target );

          /* Look at the specified position */
          this->entity_->lookAt(lookAtPosition);

          /* Force mouse look */
          if( this->entity_->isInMouseLook() == false )
            this->entity_->mouseLook();
        }
    }

    void ScriptController::moveAndLook(
      float xm, float ym, float zm,
      float xl, float yl, float zl,
      float t )
    {
      orxout()<<"moveAndLook executed"<<endl;

      /* Set the local variables required for this event */
      this->target = Vector3(xm,ym,zm);
      this->lookAtPosition = Vector3(xl,yl,zl);
      this->timeToTarget = t;


      orxout() << "Moving This-pointer: " << this << endl;

      if(this->entity_ != NULL)
        orxout()<<"not-NULL-entity"<<endl;
      else 
        return;

      if(this->player_ != NULL)
        orxout()<<"not-NULL-player"<<endl;
      else 
        return;
    }

    void ScriptController::eventScheduler(std::string instruction, 
      float x1, float y1, float z1, 
      float x2, float y2, float z2, 
      float duration, float executionTime)
    {
      /* put data (from LUA) into time-sorted eventList*/ 
      /* Nimmt den befehl und die argumente aus luascript und ertellt einen
       * struct pro event, diese structs werden sortiert nach eventTime
       */
      struct event tmp;

      /* Fill the structure with all the provided information */
      tmp.fctName = instruction;
      tmp.x1 = x1; tmp.y1 = y1; tmp.z1 = z1;
      tmp.x2 = x2; tmp.y2 = y2; tmp.z2 = z2;
      tmp.duration = duration;
      tmp.eventTime = executionTime;

      orxout() << tmp.fctName << endl;

      /* Add the created event to the event list */
      if(eventList.size()==0)
      { /* The list is still empty, just add it */
        orxout() << "eventList empty (01)" << endl;
        eventList.insert(eventList.begin(), tmp);
        this->eventno += 1;
        return; /* Nothing more to do, the event was added */
      }

      /* Event was not added yet since the list was not empty. Walk through
       * the list of events and add it so that the events are correctly in
       * order.
       */
      for (std::vector<event>::iterator it=eventList.begin(); it<eventList.end(); it++)
      { if(tmp.eventTime < it->eventTime)
        { eventList.insert(it,tmp);
          this->eventno += 1;
          orxout()<<"new event added"<<endl;
          return;
        }
      }

      /* If the event was still not added here, it belongs in the end of the list */
      eventList.insert(eventList.end(), tmp);
      this->eventno += 1;

    }
}
