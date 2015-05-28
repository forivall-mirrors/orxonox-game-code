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

 /*
  * Currently available lua commands:
  *
  * IMPORTANT: ALL COMMANDS DO REQUIRE 7 PARAMETERS TO BE PROVIDED. FILL UP WITH ZEROES IN UNIMPORTANT PLACES.
  *
  * Command             | Abbreviation | Parameter 1          | '' 2     | '' 3    | '' 4                 | '' 5     | '' 6     | '' 7
  *
  * "Move And Look"     | mal          | GoTo X Coordinate    | '' Y ''  | '' Z '' | LookAt X Coordinate  |  '' Y '' |  '' Y '' | Duration
  * "Rotate And Look"   | ral          | GoTo X Coordinate    | '' Y ''  | '' Z '' | Axis (1=x, 2=z, 3=z) |     -    |     -    | Duration
  * "Spiral"            | spi          | GoTo X Coordinate    | '' Y ''  | '' Z '' |          -           |     -    |     -    | Duration
  * "Transition Look"   | chl          | From X Coordinate    | '' Y ''  | '' Z '' | To X Coordinate      |  '' Y '' |  '' Y '' | Duration
  * "Idle (Do nothing)" | idle         | Duration
  */

// TODO: Which library can this be found in?

#define M_PI 3.14159265358979323846 /* pi */

#include "ScriptController.h"
#include "infos/PlayerInfo.h"
#include "core/CoreIncludes.h"
#include "worldentities/ControllableEntity.h"
#include "core/LuaState.h"
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
        this->eventTime = 0.0f;

        /* - Points in space */
        this->startpos = Vector3(0,0,0);
        //this->lookAtPosition = Vector3(0,0,0);

        /* - Processing flag */
        this->processing = false;

        /* - Counters */
        this->eventno = 0;

        /* - First "previous event" scheduled at t=0 */
        /* - Needed for automatically updating event times */
        this->prevEventTime = 0;

        /* hack */
        this->deltat = 0;
    }

    void ScriptController::takeControl(int ctrlid)
    {
        /* Output some debugging information */
        orxout(verbose) << "ScriptController: Taking control" << endl;
        orxout(verbose) << "This-pointer: " << this << endl;

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

    const Vector3& ScriptController::getPosition()
    {
      return this->entity_->getPosition();
    }

    ScriptController* ScriptController::getScriptController()
    {
      /* Output a message that confirms this function was called */
      orxout(verbose) << "Great success!" << std::endl;

      /* Debugging: print all the scriptcontroller object pointers */
      for(ObjectList<ScriptController>::iterator it = 
        ObjectList<ScriptController>::begin(); 
        it != ObjectList<ScriptController>::end(); ++it)
      { orxout(verbose) << "Have object in list: " << *it << endl; }

      /* Find the first one with a nonzero ID */
      for(ObjectList<ScriptController>::iterator it = 
        ObjectList<ScriptController>::begin(); 
        it != ObjectList<ScriptController>::end(); ++it)
      { 
        // TODO: do some selection here. Currently just returns the first one
        if( (*it)->getID() > 0 )
        { orxout(verbose) << "Controller to return: " << *it << endl;
          return *it; 
        }
      
      }
      return NULL;
    }

    void ScriptController::execute(event ev)
    {
        /* Debugging output */
        //orxout() << "Executing event " << ev.fctName 
          //<< " with parameters:\n " 
          //<< ev.x1 << " " << ev.y1 << " " << ev.z1 << "\n"
          //<< ev.x2 << " " << ev.y2 << " " << ev.z2 << "\n"
          //<< ev.duration << endl;

        /* Event is starting, hence set the time to 0 */
        this->eventTime = 0.0f;
        this->processing = true;

        /* Copy the event into the currentEvent holder */
        this->currentEvent = ev;

        /* Store starting position */
        this->startpos = this->entity_->getPosition();
    }


    void ScriptController::tick(float dt)
    {
        /* hack */
        this->deltat = dt;

        /* Call the tick function of the classes we derive from */
        SUPER(ScriptController, tick, dt);

        /* If this controller has no entity entry, do nothing */
        if( !(this->entity_) ) return;

        /* See if time has come for the next event to be run */
        if(this->eventList.size() > 0 && this->eventList[0].eventTime <= scTime)
        { /* Execute the next event on the list */
          this->execute(this->eventList[0]);
          this->eventList.erase(this->eventList.begin());
          this->eventno -= 1;
        }

        /* Update the local timers in this object */
        scTime += dt; eventTime += dt;

        /* If we've arrived at the target, stop processing */
        if( eventTime > currentEvent.duration && this->processing == true)
        { this->processing = false;

          /* If we reached the last event, also reenable the normal movement
           * and make the model visible again
           */
          if( this->eventno == 0 )
          {
            this->entity_->mouseLook();
            this->entity_->setVisible(true);
          }
        }

        /* Get a variable that specifies how far along the trajectory
         * we are currently.
         */
        float dl = eventTime / currentEvent.duration; 

        /* Depending on command */
        /* Do some moving */
        if( this->processing )
        {
          // Abbreviation for "spiral" (rotation + translation)
          if (this->currentEvent.fctName == "spi") {

            // Need to know a perpendicular basis to the translation vector:
            // Given (a, b, c) we chose (b, -a, 0)norm and (0, c, -b)norm
            // Currently we set a fix rotational radius of 400
            // TODO: Add an option to adjust radius of spiral movement
            Vector3 direction = this->currentEvent.v1 - startpos;

            Vector3* ortho1 = new Vector3(direction.y, -direction.x, 0);
            float absOrtho1 = sqrt(direction.y * direction.y + direction.x * direction.x);
            *ortho1 = 400 * cos(2 * M_PI * dl) * (*ortho1)/absOrtho1;

            Vector3* ortho2 = new Vector3(0, direction.z, -direction.y);
            float absOrtho2 = sqrt(direction.y * direction.y + direction.z * direction.z);
            *ortho2 = 400 * sin(2 * M_PI * dl) * (*ortho2)/absOrtho2;

            this->entity_->setPosition( (1-dl)*startpos + dl * this->currentEvent.v1 + *ortho1 + *ortho2);

            delete ortho1;
            delete ortho2;
          }

          // Abbreviation for "rotate and look"
          if (this->currentEvent.fctName == "ral")
          { 
            // Specify the axis
            Vector3* a;
              switch ((int) currentEvent.d) {
                case 3:
                  a = new Vector3(this->currentEvent.v1.x + this->currentEvent.e *cos(2*M_PI*dl),
                                  this->currentEvent.v1.y + this->currentEvent.e*sin(2*M_PI*dl),
                                  this->currentEvent.v1.z);
                break;
                case 2:
                  a = new Vector3(this->currentEvent.v1.x + this->currentEvent.e*sin(2*M_PI*dl),
                                  this->currentEvent.v1.y,
                                  this->currentEvent.v1.z + this->currentEvent.e*cos(2*M_PI*dl));
                break;
                case 1:
                  a = new Vector3(this->currentEvent.v1.x,
                                  this->currentEvent.v1.y + this->currentEvent.e*sin(2*M_PI*dl),
                                  this->currentEvent.v1.z + this->currentEvent.e*cos(2*M_PI*dl));
                break;
              }

            this->entity_->setPosition(*a);

            /* Look at the specified position */
            this->entity_->lookAt(this->currentEvent.v1);
          }
          else if( this->currentEvent.fctName == "mal" )
          {
            /* Set the position to the correct place in the trajectory */
            this->entity_->setPosition( (1-dl)*startpos + dl * this->currentEvent.v1);

            /* Look at the specified position */
            this->entity_->lookAt(this->currentEvent.v2);
          }
          else if( this->currentEvent.fctName == "chl" )
          {
            /* Sweep the look from v1 to v2 */
            this->entity_->lookAt( (1-dl)*this->currentEvent.v1 + 
              dl * this->currentEvent.v2 );
          }


          /* Force mouse look */
          if( this->entity_->isInMouseLook() == false )
            this->entity_->mouseLook();
        }
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

      //tmp.x1 = x1; tmp.y1 = y1; tmp.z1 = z1;
      //tmp.x2 = x2; tmp.y2 = y2; tmp.z2 = z2;
      tmp.v1 = Vector3(x1,y1,z1);
      tmp.v2 = Vector3(x2,y2,z2);

      // the parameters are not required to be vector coordinates!
      // for convenience they are however stored twice if they have some kind of different meaning
      tmp.a = x1;
      tmp.b = y1;
      tmp.c = z1;
      tmp.d = x2;
      tmp.e = y2;
      tmp.f = z2;

      tmp.duration = duration;

      /* This is kind of a hack. If we hit the function idle all we want to do is
         advance event execution time, not schedule anything */
      if (instruction == "idle") {
        tmp.eventTime = this->prevEventTime;
        this->prevEventTime += x1;
        return;
      } else {
        tmp.eventTime = this->prevEventTime;
        this->prevEventTime += duration;
      }

      /* Add the created event to the event list */
      if(eventList.size()==0)
      { /* The list is still empty, just add it */
        orxout(verbose) << "eventList empty (01)" << endl;
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
          //orxout()<<"new event added"<<endl;
          return;
        }
      }

      /* If the event was still not added here, it belongs in the end of the list */
      eventList.insert(eventList.end(), tmp);
      this->eventno += 1;

    }
}
