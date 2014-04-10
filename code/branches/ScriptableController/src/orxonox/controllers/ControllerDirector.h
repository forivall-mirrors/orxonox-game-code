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

#ifndef _ControllerDirector_H__
#define _ControllerDirector_H__

#include "OrxonoxPrereqs.h"
#include "ArtificialController.h"
#include "core/EventIncludes.h"


namespace orxonox
{
    class _OrxonoxExport ControllerDirector : public ArtificialController
    {
        public:
            ControllerDirector(Context* context);
            virtual ~ControllerDirector() { }

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            bool party(bool bTriggered, BaseObject* trigger);
	    virtual void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);
	
	
	
	    void takeControl(Controller * controller, BaseObject * trigger);
	    bool preparationToTakeControl(BaseObject * trigger);
	    void setNewController(Controller * controller);
	


        private:
	   
	   PlayerInfo* player_;

      	   ControllableEntity* entity_; 

	   PlayerTrigger * pTrigger_;
	

    };
}

#endif /* _ControllerDirector_H__ */
