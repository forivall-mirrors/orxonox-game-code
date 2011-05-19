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
            // TODO COUT
            return;
        }
        
        std::map<PortalEndPoint *, PortalEndPoint *>::iterator endpoints = PortalLink::links_s.find(entrance);
        
        if(endpoints == PortalLink::links_s.end())  // entrance has no corresponding exit
            return;
        
        endpoints->second->jumpOut(entity);
    }


}
