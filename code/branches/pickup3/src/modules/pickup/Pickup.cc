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

#include "Pickup.h"

#include "core/CoreIncludes.h"

namespace orxonox
{
    
    /*static*/ const std::string Pickup::activationTypeImmediate_s = "immediate";
    /*static*/ const std::string Pickup::activationTypeOnUse_s = "onUse";
    /*static*/ const std::string Pickup::durationTypeOnce_s = "once";
    /*static*/ const std::string Pickup::durationTypeContinuous_s = "continuous";
    /*static*/ const std::string Pickup::blankString_s = "";
    
    //TODO: Should this bee here? Does it work without?
    CreateFactory(Pickup);
    
    Pickup::Pickup(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(Pickup);
        
    }
    
    Pickup::~Pickup()
    {
        
    }
    
    void Pickup::initializeIdentifier(void)
    {
        this->pickupIdentifier_.addClass(this->getIdentifier());
        
        //TODO: Works?
        std::string val1 = this->getActivationType();
        std::string type1 = "activationType";
        this->pickupIdentifier_.addParameter(type1, val1);
        
        std::string val2 = this->getDurationType();
        std::string type2 = "durationType";
        this->pickupIdentifier_.addParameter(type2, val2);
    }
    
    void Pickup::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Pickup, XMLPort, xmlelement, mode);

        XMLPortParam(Pickup, "activationType", setActivationType, getActivationType, xmlelement, mode);
        XMLPortParam(Pickup, "durationType", setDurationType, getDurationType, xmlelement, mode);
        
        this->initializeIdentifier();
    }
    
    /**
    @brief
        Get the activation type of the pickup.
    @param buffer
        The buffer to store the activation type as string in.
    */
    const std::string& Pickup::getActivationType(void)
    {
        switch(this->activationType_)
        {
            case pickupActivationType::immediate:
                return activationTypeImmediate_s;
            case pickupActivationType::onUse:
                return activationTypeOnUse_s;
            default:
                return blankString_s;
        }
    }
        
    /**
    @brief
        Get the duration type of the pickup.
    @param buffer
        The buffer to store the duration type as string in.
    */
    const std::string& Pickup::getDurationType(void)
    {
        switch(this->durationType_)
        {
            case pickupDurationType::once:
                return durationTypeOnce_s;
            case pickupDurationType::continuous:
                return durationTypeContinuous_s;
            default:
                return blankString_s;
        }
    }
    
    /**
    @brief
        Set the activation type of the Pickup.
    @param type
        The activation type of the Pickup as a string.
    */
    void Pickup::setActivationType(const std::string& type)
    {
        if(type == activationTypeImmediate_s)
        {
            this->activationType_ = pickupActivationType::immediate;
        }
        else if(type == activationTypeOnUse_s)
        {
            this->activationType_ = pickupActivationType::onUse;
        }
        else
        {
            COUT(1) << "Invalid activationType in pickup." << std::endl;
        }
    }
        
    /**
    @brief
        Set the duration type of the Pickup.
    @param type
        The duration type of the Pickup as a string.
    */
    void Pickup::setDurationType(const std::string& type)
    {
        if(type == durationTypeOnce_s)
        {
            this->durationType_ = pickupDurationType::once;
        }
        else if(type == durationTypeContinuous_s)
        {
            this->durationType_ = pickupDurationType::continuous;
        }
        else
        {
            COUT(1) << "Invalid durationType in pickup." << std::endl;
        }
    }
    
    /**
    @brief
        Creates a duplicate of the pickup.
    @return
        Returns the clone of this pickup as a pointer to a Pickupable.
    */
    void Pickup::clone(OrxonoxClass* item)
    {
        if(item == NULL)
            item = new Pickup(this);
        
        SUPER(Pickup, clone, item);
        
        Pickup* pickup = dynamic_cast<Pickup*>(item);
        pickup->setActivationTypeDirect(this->getActivationTypeDirect());
        pickup->setDurationTypeDirect(this->getDurationTypeDirect());
        
        pickup->initializeIdentifier();
    }
    
    void Pickup::changedCarrier(void)
    {
        SUPER(Pickup, changedCarrier);
        
        if(this->isPickedUp() && this->isImmediate())
        {
            this->setUsed(true);
        }
    }
    
}
