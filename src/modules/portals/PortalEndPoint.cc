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

#include "PortalEndPoint.h"
#include "portals/PortalLink.h"

#include <ctime>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "worldentities/MobileEntity.h"
#include "objects/triggers/MultiTriggerContainer.h"

#include "sound/WorldSound.h"

namespace orxonox
{
    CreateFactory(PortalEndPoint);

    /*static*/ const std::string PortalEndPoint::EVENTFUNCTIONNAME = "execute";

    std::map<unsigned int, PortalEndPoint *> PortalEndPoint::idMap_s;

    PortalEndPoint::PortalEndPoint(Context* context) : StaticEntity(context), RadarViewable(this, static_cast<WorldEntity*>(this)), id_(0), trigger_(NULL), reenterDelay_(0)
    {
        RegisterObject(PortalEndPoint);

        this->trigger_ = new DistanceMultiTrigger(this->getContext());
        this->trigger_->setName("portal");
        this->attach(this->trigger_);

        this->setRadarObjectColour(ColourValue::White);
        this->setRadarObjectShape(RadarViewable::Dot);
        this->setRadarVisibility(true);
        if( GameMode::isMaster() )
        {
            this->portalSound_ = new WorldSound(this->getContext());
            this->portalSound_->setLooping(false);
            this->attach(this->portalSound_);
            this->portalSound_->setSource("sounds/Weapon_HsW01.ogg"); //TODO: change sound file
        }
    }

    PortalEndPoint::~PortalEndPoint()
    {
        if(this->isInitialized() && this->trigger_ != NULL)
            delete this->trigger_;
    }

    void PortalEndPoint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PortalEndPoint, XMLPort, xmlelement, mode);

        XMLPortParam(PortalEndPoint, "id", setID, getID, xmlelement, mode);
        XMLPortParam(PortalEndPoint, "design", setTemplate, getTemplate, xmlelement, mode);
        XMLPortParam(PortalEndPoint, "reenterDelay", setReenterDelay, getReenterDelay, xmlelement, mode);
        XMLPortParamExtern(PortalEndPoint, DistanceMultiTrigger, this->trigger_, "distance", setDistance, getDistance, xmlelement, mode);
        XMLPortParamLoadOnly(PortalEndPoint, "target", setTarget, xmlelement, mode).defaultValues("Pawn");

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
        if(!this->isActive())
            return true;

        MultiTriggerContainer * cont = orxonox_cast<MultiTriggerContainer *>(trigger);
        if(cont == 0)
            return true;

        DistanceMultiTrigger * originatingTrigger = orxonox_cast<DistanceMultiTrigger *>(cont->getOriginator());
        if(originatingTrigger == 0)
        {
            return true;
        }

        MobileEntity * entity = orxonox_cast<MobileEntity *>(cont->getData());
        if(entity == 0)
            return true;

        if(bTriggered)
        {
            if(this->letsEnter(entity))  // only enter the portal if not just (this very moment) jumped out of it, or if the reenterDelay expired
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

    void PortalEndPoint::changedActivity(void)
    {
        SUPER(PortalEndPoint, changedActivity);

        this->setRadarVisibility(this->isActive());
    }

    bool PortalEndPoint::letsEnter(MobileEntity* entity)
    {
        // not allowed to enter if reenterDelay hasn't expired yet
        std::map<MobileEntity *, time_t>::const_iterator time = this->jumpOutTimes_.find(entity);
        if(time != this->jumpOutTimes_.end() && std::difftime(std::time(0),time->second) < this->reenterDelay_)
            return false;

        // not allowed to enter if jumped out of this portal and not left its activation radius yet
        std::set<MobileEntity *>::const_iterator recent = this->recentlyJumpedOut_.find(entity);
        if(recent != this->recentlyJumpedOut_.end())
            return false;
        
        return true;
    }

    void PortalEndPoint::jumpOut(MobileEntity* entity)
    {
        this->jumpOutTimes_[entity] = std::time(0);
        this->recentlyJumpedOut_.insert(entity);

        // adjust
        entity->setPosition(this->getWorldPosition());
        entity->rotate(this->getWorldOrientation());
        entity->setVelocity(this->getWorldOrientation() * entity->getVelocity());
        //play Sound
        if( this->portalSound_ && !(this->portalSound_->isPlaying()))
        {
            this->portalSound_->play();
        }
    }

}
