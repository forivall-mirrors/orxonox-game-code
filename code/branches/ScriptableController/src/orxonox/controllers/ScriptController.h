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

#ifndef _ScriptController_H__
#define _ScriptController_H__

#include "OrxonoxPrereqs.h"                 /* die ganzen tolua, kopiert aus Dock.h*/
#include "ArtificialController.h"
#include "core/EventIncludes.h"



namespace orxonox  // tolua_export
{  // tolua_export

    /** Structure to describe a single event */
    struct event 
    {   
        /** Instruction for this event */
        std::string fctName;

        Vector3 v1;
        Vector3 v2;

        /** Time span of the event */
        float duration;

        /** Start point in time of the event */
        float eventTime;
    }; 

    class _OrxonoxExport ScriptController // tolua_export 
       : public ArtificialController, public Tickable
    {  // tolua_export
        public:
            ScriptController(Context* context);
            virtual ~ScriptController() { }

            void takeControl(int ctrlid);
            void setPlayer(PlayerInfo* player) { this->player_ = player; }
           
            virtual void tick(float dt);

            // LUA interface
            // tolua_begin 
            void eventScheduler(std::string instruction, 
              float x1, float y1, float z1, 
              float x2, float y2, float z2, 
              float duration, float executionTime);

            static ScriptController* getScriptController();

            int getID() { return ctrlid_; }

            // tolua_end
            const Vector3& getPosition();

            void execute(event ev);

        private:
            /* Information about the player that this ScriptController will
             * control */
            /* - Player pointer */
            PlayerInfo* player_;

            /* - Entity pointer, this is for convenience and will be the same as 
             *   player_->getControllableEntity() 
             */
            ControllableEntity* entity_;

            /* Controller ID, defaults to 0 and is set using takeControl() */
            int ctrlid_;

            /* List of events to walk through */
            std::vector<event> eventList;
            unsigned int eventno;

            /* Time since the creation of this ScriptController object */
            float scTime;  

            /* Boolean that specifies whether we're processing an event right
             * now */
            bool processing;

            /* Data about the event currently being processed */
            /* - The event itself */
            event currentEvent;

            /* - Time this event has been going on for */
            float eventTime;
            Vector3 startpos;

            /* - Position to look at during that transition */
            //Vector3 lookAtPosition;

    };// tolua_export
} // tolua_export

#endif /* _ScriptController_H__ */
