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
 *     Mauro Salomon
 *   Co-authors:
 *      ...
 *
 */

#include "SpaceRaceManager.h"


#include "core/XMLPort.h"

#include "core/CoreIncludes.h"

#include "util/Convert.h"
#include "util/Math.h"

namespace orxonox
{
    CreateUnloadableFactory(SpaceRaceManager);

    SpaceRaceManager::SpaceRaceManager(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(SpaceRaceManager);
        
        
    }

   
     void SpaceRaceManager::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceRaceManager, XMLPort, xmlelement, mode);

        //XMLPortParam(WaypointController, "accuracy", setAccuracy, getAccuracy, xmlelement, mode).defaultValues(100.0f);
        //XMLPortObject(WaypointController, WorldEntity, "waypoints", addWaypoint, getWaypoint,  xmlelement, mode);
    }
    
    void SpaceRaceManager::tick(float dt)
    {
    SUPER(SpaceRaceManager,tick,dt);
            
    }
     
    
    

    

}
