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

#include "HealthPickup.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "worldentities/pawns/Pawn.h"

#include <sstream>

namespace orxonox
{
    
    /*static*/ const std::string HealthPickup::healthTypeLimited_s = "limited";
    /*static*/ const std::string HealthPickup::healthTypeTemporary_s = "temporary";
    /*static*/ const std::string HealthPickup::healthTypePermanent_s = "permanent";
    /*static*/ const std::string HealthPickup::blankString_s = "";
    
    CreateFactory(HealthPickup);
    
    HealthPickup::HealthPickup(BaseObject* creator) : Pickup(creator)
    {
        RegisterObject(HealthPickup);
        
        this->initialize();
    }
    
    HealthPickup::~HealthPickup()
    {
        
    }
    
    void HealthPickup::initialize(void)
    {
        RegisterObject(HealthPickup);
        
        this->health_ = 0;
        this->healthSpeed_ = 0;
        this->healthType_ = pickupHealthType::limited;
        
    }
    
    void HealthPickup::initializeIdentifier(void)
    {
        this->pickupIdentifier_.addClass(this->getIdentifier());
        
        std::stringstream stream;
        stream << this->getHealth();
        std::string type1 = "health";
        std::string val1 = stream.str();
        this->pickupIdentifier_.addParameter(type1, val1);
        
        //TODO: Does this work, is val valid outside the function scope?
        std::string val2 = this->getHealthType();
        std::string type2 = "healthType";
        this->pickupIdentifier_.addParameter(type2, val2);
    }
    
    void HealthPickup::HealthPickup::XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode)
    {
        SUPER(HealthPickup, XMLPort, xmlelement, mode);
        
        XMLPortParam(HealthPickup, "health", setHealth, getHealth, xmlelement, mode);
        XMLPortParam(HealthPickup, "healthSpeed", setHealthSpeed, getHealthSpeed, xmlelement, mode);
        XMLPortParam(HealthPickup, "healthType", setHealthType, getHealthType, xmlelement, mode);
        
        if(!this->isContinuous())
            this->healthSpeed_ = 0.0;
        
        this->initializeIdentifier();
    }
    
    void HealthPickup::setHealth(float health)
    {
        if(health > 0.0f)
        {
            this->health_ = health;
        }
        else
        {
            COUT(1) << "Invalid health in HealthPickup." << std::endl;
        }
    }
    
    void HealthPickup::setHealthSpeed(float speed)
    {
        if(speed >= 0)
        {
            this->healthSpeed_ = speed;
        }
        else
        {
            COUT(1) << "Invalid healthSpeed in HealthPickup." << std::endl; 
        }
    }
    
    void HealthPickup::setHealthType(std::string type)
    {
        if(type == HealthPickup::healthTypeLimited_s)
        {
            this->setHealthTypeDirect(pickupHealthType::limited);
        }
        else if(type == HealthPickup::healthTypeTemporary_s)
        {
            this->setHealthTypeDirect(pickupHealthType::temporary);
        }
        else if(type == HealthPickup::healthTypePermanent_s)
        {
            this->setHealthTypeDirect(pickupHealthType::permanent);
        }
        else
        {
            COUT(1) << "Invalid healthType in HealthPickup." << std::endl;
        }
    }
    
    void HealthPickup::tick(float dt)
    {
        if(this->isContinuous() && this->isUsed())
        {
            PickupCarrier* carrier = this->getCarrier();
            Pawn* pawn = dynamic_cast<Pawn*>(carrier);
            
            if(pawn == NULL)
            {
                COUT(1) << "Invalid PickupCarrier in HealthPickup." << std::endl;
                return;
            }
            
            float health = dt*this->getHealthSpeed();
            if(health > this->getHealth())
                health = this->getHealth();
            float fullHealth = pawn->getHealth() + health;
            this->setHealth(this->getHealth()-health);
                    
            switch(this->getHealthTypeDirect())
            {
                case pickupHealthType::permanent:
                    if(pawn->getMaxHealth() > fullHealth)
                        pawn->setMaxHealth(fullHealth);
                case pickupHealthType::limited:
                    pawn->addHealth(health);
                    break;
                case pickupHealthType::temporary:
                    //TODO: How?
                    break;
                default:
                    COUT(1) << "Invalid healthType in HealthPickup." << std::endl;
            }
            
            if(this->getHealth() == 0)
            {
                //TODO: Destroy
            }
        }
    }
    
    const std::string& HealthPickup::getHealthType(void)
    {
        switch(this->getHealthTypeDirect())
        {
            case pickupHealthType::limited:
                return HealthPickup::healthTypeLimited_s;
            case pickupHealthType::temporary:
                return HealthPickup::healthTypeTemporary_s;
            case pickupHealthType::permanent:
                return HealthPickup::healthTypePermanent_s;
            default:
                COUT(1) << "Invalid healthType in HealthPickup." << std::endl;
                return HealthPickup::blankString_s;
        }
    }
    
    void HealthPickup::clone(OrxonoxClass* item)
    {
        if(item == NULL)
            item = new HealthPickup(this);
        
        SUPER(HealthPickup, clone, item);
        
        //TODO: No temp needed?
        HealthPickup* pickup = dynamic_cast<HealthPickup*>(item);
        pickup->setHealth(this->getHealth());
        pickup->setHealthSpeed(this->getHealthSpeed());
        pickup->setHealthTypeDirect(this->getHealthTypeDirect());
        
        pickup->initializeIdentifier();
    }
    
    //TODO: Does this work even if Pickup doesn't implement it?
    void HealthPickup::changedUsed(void)
    {
        SUPER(HealthPickup, changedUsed);
        
        if(this->isUsed())
        {
            PickupCarrier* carrier = this->getCarrier();
            Pawn* pawn = dynamic_cast<Pawn*>(carrier);
            
            if(pawn == NULL)
            {
                COUT(1) << "Invalid PickupCarrier in HealthPickup." << std::endl;
                return;
            }
            
            if(this->isOnce())
            {
                float health = 0;
                switch(this->getHealthTypeDirect())
                {
                    case pickupHealthType::permanent:
                        health = pawn->getHealth()+this->getHealth();
                        if(pawn->getMaxHealth() < health)
                            pawn->setMaxHealth(health);
                    case pickupHealthType::limited:
                        pawn->addHealth(this->getHealth());
                        break;
                    case pickupHealthType::temporary:
                        //TODO: How?
                        break;
                    default:
                        COUT(1) << "Invalid healthType in HealthPickup." << std::endl;
                }
                
                //TODO: Destroy.
            }
        }
        else
        {
            //TODO: Destroy, but be careful to not destroy before being used.
        }
    }

}
