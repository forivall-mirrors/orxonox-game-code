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
    class _OrxonoxExport ScriptController // tolua_export 
       : public ArtificialController, public Tickable
    {  // tolua_export
        public:
            //ScriptController(Context* context, ControllableEntity* CE);
            ScriptController(Context* context);

            virtual ~ScriptController() { }

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            
            void takeControl(int ctrlid);
            void setPlayer(PlayerInfo* player) { this->player_ = player; }
           
            //void set_luasrc(std::string);
            //void set_controlled(ControllableEntity*);

            virtual void tick(float dt);

            // LUA interface
            // tolua_begin 
            void moveToPosition_beta(float x, float y, float z);

           
            static ScriptController* getScriptController();

            int getID() { return ctrlid_; }
            

            // tolua_end
            const Vector3& getPosition();

        private:
            // name of the LUA-sourcefile that shall be executed->see XMLPort-function
            std::string luasrc;		
            PlayerInfo* player_;
            ControllableEntity* entity_;
            int ctrlid_;


    };// tolua_export
} // tolua_export

#endif /* _ScriptController_H__ */
