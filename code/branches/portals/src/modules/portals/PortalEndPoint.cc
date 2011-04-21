#include "PortalEndPoint.h"
#include "core/XMLPort.h"
#include "objects/triggers/MultiTriggerContainer.h"
#include "portals/PortalLink.h"
#include "worldentities/MobileEntity.h"


namespace orxonox
{
    CreateFactory(PortalEndPoint);
    
    /*static*/ const std::string PortalEndPoint::EVENTFUNCTIONNAME = "execute";

    std::map<unsigned int, PortalEndPoint *> PortalEndPoint::idMap_s;

    PortalEndPoint::PortalEndPoint(BaseObject* creator) : StaticEntity(creator), id_(0), trigger_(new DistanceMultiTrigger(this))
    {
        RegisterObject(PortalEndPoint);
        this->trigger_->setName("portal");
        this->attach(trigger_);
    }
    
    PortalEndPoint::~PortalEndPoint()
    {
    
    }

    void PortalEndPoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PortalEndPoint, XMLPort, xmlelement, mode);
        
        XMLPortParam(PortalEndPoint, "id", setID, getID, xmlelement, mode);
        XMLPortParam(PortalEndPoint, "design", setTemplate, getTemplate, xmlelement, mode);
        XMLPortParamExtern(PortalEndPoint, DistanceMultiTrigger, this->trigger_, "distance", setDistance, getDistance, xmlelement, mode);
        XMLPortParamLoadOnly(PortalEndPoint, "target", setTargets, xmlelement, mode).defaultValues("Pawn");
        
        // Add the DistanceMultiTrigger as event source.
        this->addEventSource(this->trigger_, EVENTFUNCTIONNAME);
        
        if(mode == XMLPort::LoadObject)
        {
            PortalEndPoint::idMap_s[this->id_] = this;
        }
    }

    void PortalEndPoint::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PortalEndPoint, XMLEventPort, xmlelement, mode);
        
        XMLPortEventSink(PortalEndPoint, BaseObject, EVENTFUNCTIONNAME, execute, xmlelement, mode);
    }

    bool PortalEndPoint::execute(bool bTriggered, BaseObject* trigger)
    {
        MultiTriggerContainer * cont = orxonox_cast<MultiTriggerContainer *>(trigger);
        if(cont == 0)
            return true;
        
        DistanceMultiTrigger * originatingTrigger = orxonox_cast<DistanceMultiTrigger *>(cont->getOriginator());
        if(originatingTrigger == 0)
        {
            COUT(1) << "originator no DistanceMultiTrigger\n" << std::endl;
            return true;
        }
        
        if(this->getAttachedObjects().find(orxonox_cast<WorldEntity *>(originatingTrigger)) == this->getAttachedObjects().end())  // only necessary if events have the same name
            return true;
        
        MobileEntity * entity = orxonox_cast<MobileEntity *>(cont->getData());
        if(entity == 0)
            return true;
        
        if(bTriggered)
        {
            if(this->recentlyJumpedOut_.find(entity) == this->recentlyJumpedOut_.end())  // only enter the portal if not recently jumped out of it
            {
                PortalLink::use(entity, this);
            }
        }
        else
        {
            this->recentlyJumpedOut_.erase(entity);
        }
        
        return true;
    }

    void PortalEndPoint::jumpOut(MobileEntity* entity)
    {
        this->recentlyJumpedOut_.insert(entity);
        entity->setPosition(this->getWorldPosition());
        entity->rotate(this->getWorldOrientation());
        entity->setVelocity(this->getWorldOrientation() * entity->getVelocity());
    }

}
