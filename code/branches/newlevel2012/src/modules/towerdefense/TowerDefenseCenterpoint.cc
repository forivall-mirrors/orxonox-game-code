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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

/**
    @file TowerDefenseCenterpoint.cc
    @brief Implementation of the TowerDefenseCenterpoint class.
*/

#include "TowerDefenseCenterpoint.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "TowerDefense.h"

namespace orxonox
{
    CreateFactory(TowerDefenseCenterpoint);

    /**
    @brief
        Constructor. Registers and initializes the object and checks whether the gametype is actually TowerDefense.
    */
    TowerDefenseCenterpoint::TowerDefenseCenterpoint(BaseObject* creator) : MobileEntity(creator)
    {
        RegisterObject(TowerDefenseCenterpoint);
 
 	   	this->width_ = 15;
        this->height_ = 15;
        this->towerTemplate_ = "";
        
        //this->setCollisionType(Static);

		this->checkGametype();

    }

    /**
    @brief
        Method to create a TowerDefenseCenterpoint through XML.
    */
    void TowerDefenseCenterpoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(TowerDefenseCenterpoint, XMLPort, xmlelement, mode);

        XMLPortParam(TowerDefenseCenterpoint, "width", setWidth, getWidth, xmlelement, mode);
        XMLPortParam(TowerDefenseCenterpoint, "height", setHeight, getHeight, xmlelement, mode);
		XMLPortParam(TowerDefenseCenterpoint, "tileScale", setTileScale, getTileScale, xmlelement, mode);
        XMLPortParam(TowerDefenseCenterpoint, "towerTemplate", setTowerTemplate, getTowerTemplate, xmlelement, mode);
		
        //TODO: add XMLPortObject(TowerDefenseCenterpoint, WorldEntity, "waypoints", addWaypoint, getWaypoint,  xmlelement, mode);
        // This was copied and shightly modified from WaypointController.cc ; there are no getters and setters and no membervariable yet
        // The centerpoint should have all waypoints for "attached" to it. By using this function you can seperately add waypoints
        // When you spawn a new enemy you should let it be controlled by a WaypointController. And that controller should get the waypoints.

        // Alternatively you can manage the waypoints directly in TowerDefense.cc
    }

    /**
    @brief
        Is called when the gametype has changed.
    */
    void TowerDefenseCenterpoint::changedGametype()
    {
        SUPER(TowerDefenseCenterpoint, changedGametype);

        // Check, whether it's still TowerDefense.
        this->checkGametype();
    }

    /**
    @brief
        Checks whether the gametype is TowerDefense and if it is, sets its centerpoint.
    */
    void TowerDefenseCenterpoint::checkGametype()
    {
		if (this->getGametype() != NULL && this->getGametype()->isA(Class(TowerDefense)))
        {
			// Sets the centerpoint of the gametype. The gametype uses this to later spawn in towers, he needs the tower template stored in the center point
            TowerDefense* towerDefenseGametype = orxonox_cast<TowerDefense*>(this->getGametype().get());
            towerDefenseGametype->setCenterpoint(this);
        }
    }
}
