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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
*/

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "interfaces/PickupCarrier.h"
#include "pickup/PickupIdentifier.h"

#include "MetaPickup.h"

namespace orxonox {
 
    CreateFactory(MetaPickup);
    
    /*static*/ const std::string MetaPickup::metaTypeNone_s = "none";
    /*static*/ const std::string MetaPickup::metaTypeUse_s = "use";
    /*static*/ const std::string MetaPickup::metaTypeDrop_s = "drop";
    
    MetaPickup::MetaPickup(BaseObject* creator) : Pickup(creator)
    {
        RegisterObject(MetaPickup);
        
        this->addTarget(ClassIdentifier<PickupCarrier>::getIdentifier());
        this->setActivationTypeDirect(pickupActivationType::immediate);
        this->setDurationTypeDirect(pickupDurationType::once);
        this->metaType_ = pickupMetaType::none;
    }
    
    MetaPickup::~MetaPickup()
    {
        
    }
    
    void MetaPickup::initializeIdentifier(void)
    {
        std::string val = this->getMetaType();
        std::string type = "metaType";
        this->pickupIdentifier_->addParameter(type, val);
    }
    
    void MetaPickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(MetaPickup, XMLPort, xmlelement, mode);
        
        XMLPortParam(MetaPickup, "metaType", setMetaType, getMetaType, xmlelement, mode);
        
        this->initializeIdentifier();
    }
    
    void MetaPickup::changedUsed(void)
    {
        SUPER(MetaPickup, changedUsed);
        
        if(this->isUsed())
        {
            PickupCarrier* carrier = this->getCarrier();
            if(this->getMetaTypeDirect() != pickupMetaType::none && carrier != NULL)
            {
                std::set<Pickupable*> pickups = carrier->getPickups();
                for(std::set<Pickupable*>::iterator it = pickups.begin(); it != pickups.end(); it++)
                {
                    Pickup* pickup = dynamic_cast<Pickup*>(*it);
                    if(this->getMetaTypeDirect() == pickupMetaType::use)
                    {
                        if(pickup != NULL && pickup != this && pickup->isOnUse() && !pickup->isUsed())
                        {
                            pickup->setUsed(true);
                        }
                    }
                    if(this->getMetaTypeDirect() == pickupMetaType::drop)
                    {
                        if(pickup != NULL && pickup != this)
                        {
                            carrier->drop(pickup);
                        }
                    }
                }
            }
            this->destroy();
        }
    }
    
    const std::string& MetaPickup::getMetaType(void)
    {
        switch(this->getMetaTypeDirect())
        {
            case pickupMetaType::none:
                return MetaPickup::metaTypeNone_s;
            case pickupMetaType::use:
                return MetaPickup::metaTypeUse_s;
            case pickupMetaType::drop:
                return MetaPickup::metaTypeDrop_s;
            default:
                return BLANKSTRING;
        }
    }
    
    void MetaPickup::setMetaType(const std::string& type)
    {
        if(type == MetaPickup::metaTypeNone_s)
        {
            this->setMetaTypeDirect(pickupMetaType::none);
        }
        else if(type == MetaPickup::metaTypeUse_s)
        {
            this->setMetaTypeDirect(pickupMetaType::use);
        }
        else if(type == MetaPickup::metaTypeDrop_s)
        {
            this->setMetaTypeDirect(pickupMetaType::drop);
        }
    }
    
    void MetaPickup::clone(OrxonoxClass*& item)
    {
        if(item == NULL)
            item = new MetaPickup(this);
        
        SUPER(MetaPickup, clone, item);
        
        MetaPickup* pickup = dynamic_cast<MetaPickup*>(item);
        pickup->setMetaTypeDirect(this->getMetaTypeDirect());
        
        pickup->initializeIdentifier();
    }
    
}
