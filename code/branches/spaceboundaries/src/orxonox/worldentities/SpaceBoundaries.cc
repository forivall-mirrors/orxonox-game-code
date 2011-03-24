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
 *      Maurus Kaufmann
 *   Co-authors:
 *      ...
 *
 */

#include "SpaceBoundaries.h"

/* Folgender Block ist Copy-Paste und somit teilweise wohl unnoetig */
#include "core/Template.h"
#include "core/XMLPort.h"
#include "gametypes/Gametype.h"
#include "worldentities/pawns/Pawn.h"

/* Eigene, spezifische include-Statements*/
#include "worldentities/MobileEntity.h"
#include "core/ObjectListIterator.h"

namespace orxonox
{
    CreateFactory(SpaceBoundaries);

    SpaceBoundaries::SpaceBoundaries(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(SpaceBoundaries);
        
        // Show Boundaries on the radar.
    }
    SpaceBoundaries::~SpaceBoundaries()
    {
    
    }
    
    void SpaceBoundaries::setCenter(Vector3 r)
    {
        this->center = r;
    }
    Vector3 SpaceBoundaries::getCenter()
    {
        return this->center;
    }
    
    void SpaceBoundaries::setMaxDistance(float r)
    {
        this->maxDistance = r;
    }
    float SpaceBoundaries::getMaxDistance()
    {
        return this->maxDistance;
    }
    
    void SpaceBoundaries::setWarnDistance(float r)
    {
        this->warnDistance = r;
    }
    float SpaceBoundaries::getWarnDistance()
    {
        return this->warnDistance;
    }

    void SpaceBoundaries::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceBoundaries, XMLPort, xmlelement, mode);

        XMLPortParam(SpaceBoundaries, "center", setCenter, getCenter, xmlelement, mode);
        XMLPortParam(SpaceBoundaries, "maxDistance", setMaxDistance, getMaxDistance, xmlelement, mode);
        XMLPortParam(SpaceBoundaries, "warnDistance", setWarnDistance, getWarnDistance, xmlelement, mode);
    }
    
    void SpaceBoundaries::tick(float dt)
    {
        for(ObjectListIterator<MobileEntity> item = ObjectList<MobileEntity>::begin(); item != ObjectList<MobileEntity>::end(); ++item)
        {
            float distance = computeDistance((WorldEntity *)&item); // fuer casts gibt's scheinbar andere, neuere Variante --> vgl. Coding-Guide und andere Files
            if(distance > this->warnDistance && distance < this->maxDistance)
            {
                COUT(0) << "You are leaving the area" << std::endl; //!< message for debugging
                // Display Warning on Screen if the humanPlayer (infos/PlayerInfo.h) is leaving the area
            } else if(distance > maxDistance)
            {
                // Decrease Health
            }
        }
    }
    
    float SpaceBoundaries::computeDistance(WorldEntity *item)
    {
        Vector3 itemPosition = item->getPosition();
        return (itemPosition.distance(center));
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}
