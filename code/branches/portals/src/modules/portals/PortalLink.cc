#include "PortalLink.h"
#include "core/XMLPort.h"
#include "objects/triggers/MultiTriggerContainer.h"

namespace orxonox
{
    CreateFactory(PortalLink);
    
    PortalLink::PortalLink(BaseObject* creator) : BaseObject(creator), fromID_(0), toID_(0), from_(0), to_(0), activationRadius_(20)
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
            this->from_ = PortalEndPoint::idMap_s[this->fromID_];
            this->to_   = PortalEndPoint::idMap_s[this->toID_];
            recentlyPorted.clear();
        }
    }
    
    void PortalLink::tick(float dt)
    {
        SUPER(PortalLink, tick)
    }
    
    void PortalLink::processEvent(Event& event)
    {
        SUPER(PortalLink, processEvent);
        if(!event.activate_)
        {
            return;
        }
        MultiTriggerContainer * origin = dynamic_cast<MultiTriggerContainer *>(event.originator_);
        if(!origin)
        {
            return;
        }
        PortalEndPoint * eventFrom = dynamic_cast<PortalEndPoint *>(origin->getOriginator());
        WorldEntity * eventEntity = dynamic_cast<WorldEntity *>(origin->getData());
        if(eventFrom != this->from_ || !eventEntity || eventFrom->hasRecentlyJumpedOut(eventEntity) == true)
        {
            return;
        }
        to_->jumpOut(entity);
    }

}