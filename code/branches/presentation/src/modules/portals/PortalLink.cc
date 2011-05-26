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
 *      Andreas Büchel
 *   Co-authors:
 *      ...
 *
 */

#include "PortalLink.h"
#include "core/XMLPort.h"
#include "objects/triggers/MultiTriggerContainer.h"
#include "worldentities/MobileEntity.h"

namespace orxonox
{
    CreateFactory(PortalLink);

    std::map<PortalEndPoint *, PortalEndPoint *> PortalLink::links_s;
    
    PortalLink::PortalLink(BaseObject* creator) : BaseObject(creator), fromID_(0), toID_(0), from_(0), to_(0)
    {
        RegisterObject(PortalLink);
    }
    
    PortalLink::~PortalLink()
    {
        
    }
    
    void PortalLink::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PortalLink, XMLPort, xmlelement, mode);
        XMLPortParam(PortalLink, "fromID", setFromID, getFromID, xmlelement, mode);
        XMLPortParam(PortalLink, "toID", setToID, getToID, xmlelement, mode);

        if(mode == XMLPort::LoadObject)
        {
            PortalEndPoint * from = PortalEndPoint::idMap_s[this->fromID_];
            PortalEndPoint * to   = PortalEndPoint::idMap_s[this->toID_];
            PortalLink::links_s[from] = to;
        }
    }

    void PortalLink::use(MobileEntity* entity, PortalEndPoint * entrance)
    {
        if(entrance == 0)
        {
            return;
        }
        
        std::map<PortalEndPoint *, PortalEndPoint *>::iterator endpoints = PortalLink::links_s.find(entrance);
        
        if(endpoints == PortalLink::links_s.end())  // entrance has no corresponding exit
            return;

        endpoints->second->jumpOut(entity);
    }
}
