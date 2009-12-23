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

/**
    @file
    @brief Implementation of PickupCollection.
*/

#include "PickupCollection.h"

#include "core/CoreIncludes.h"
#include "core/Template.h"
#include "core/XMLPort.h"
#include "interfaces/PickupCarrier.h"

namespace orxonox
{
 
    /**
    @brief
        Default Constructor.
    */
    PickupCollection::PickupCollection(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(PickupCollection);
    }
    
    /**
    @brief
        Destructor.
    */
    PickupCollection::~PickupCollection()
    {
        //! Destroy all Pickupables constructing this PickupCollection.
        for(std::list<Pickupable*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            delete *it;
        }
    }
    
    /**
    @brief
        Creates an instance of this Class through XML.
    */
    void PickupCollection::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PickupCollection, XMLPort, xmlelement, mode);
        
        //TODO: Does this work? Problem could be, that Pickupable itself cannot be instantiated through XML...
        XMLPortObject(PickupCollection, PickupCollection, "pickupables", addPickupable, getPickupable, xmlelement, mode);
    }
    
    /**
    @brief
        Add the input Pickupable to list of Pickupables combined by this PickupCollection.
    @param pickup
        The Pickupable to be added.
    @return
        Returns true if successful,
    */
    bool PickupCollection::addPickupable(Pickupable* pickup)
    {
        if(pickup == NULL)
            return false;
        
        this->pickups_.insert(pickup);
        return true;
    }
    
    /**
    @brief
        Get the Pickupable at the given index.
    @param index
        The index the Pickupable is fetched from.
    @return
        Returns a pointer to the Pickupable at the index given by index.
    */
    Pickupable* PickupCollection::getPickupable(unsigned int index)
    {
        return this->pickups_[index]; //TODO. Does this work?
    }
    
    //TODO: Steal description from Pickupable.
    void PickupCollection::changedUsed(void)
    {
        SUPER(PickupCollection, changedUsed);
        
        //! Change used for all Pickupables this PickupCollection consists of.
        for(std::list<Pickupable*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            (*it)->changedUsed();
        }
    }
    
    /**
    @brief
        Puts the PickupCollection in use.
    @return
        Returns true if successful.
    */
    //TODO: Revert if one fails? (same for unused)
    bool PickupCollection::use(void)
    {
        if(this->isUsed())
            return false;
        
        bool success = true;
        //! Set all Pickupables to used.
        for(std::list<Pickupable*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            if(!(*it)->use())
            {
                success = false;
            }
        }
        
        this->changedUsed();
        
        return success;
    }
    
    /**
    @brief
        Puts the PickupCollection out of use.
    @return
        Returns true if successful.
    */
    bool PickupCollection::unuse(void)
    {
        if(!this->isUsed())
            return false;
        
        bool success = true;
        //! Set all Pickupables to unused.
        for(std::list<Pickupable*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            if(!(*it)->unuse())
            {
                success = false;
            }
        }
        
        this->changedUsed();
        
        return success;
    }

    /**
    @brief
        Is invoked when the pickup is picked up.
    @param carrier
        The PickupCarrier that is picking up this pickup.
    @return
        Returns true if successful.
    */
    //TODO: Something should happen in the carrier as well, maybe just in the carrier. Owner might not be correct if the carrier hands the pickup down or up. Maybe even a Pawn as input instead fo a carrier. Or do this in Spawner?
    bool PickupCollection::pickup(PickupCarrier* carrier)
    {
        if(this->getOwner() != NULL)
        {
            COUT(2) << "Pickup wanted to get picked up by a new carrier, but it already has a carrier." << std::endl;
            return false;
        }
        for(std::list<Pickupable*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            (*it)->setOwner(carrier);
        }
        
        this->setOwner(carrier);
        
        return true;
    }
    
    /**
    @brief
        Drop the pickup.
    @return
        Return true if successful.
    */
    bool PickupCollection::drop(void)
    {
        this->unuse();
        this->setOwner(NULL);
        
        //TODO: Create new Pickupspawner/DroppedPickup
        return true;
    }
    
    //TODO: Steal description from Pickupable.
    Pickupable* PickupCollection::clone()
    {
        Template* collectionTemplate = Template::getTemplate(this->getIdentifier()->getName());
        BaseObject* newObject = collectionTemplate->getBaseclassIdentifier()->fabricate(this);
        
        PickupCollection* newCollection = dynamic_cast<PickupCollection*>(newObject);
        for(std::list<Pickupable*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            Pickupable* newPickup = (*it)->clone();
            newCollection->pickups_.push_back(newPickup);
        }
        
        Pickupable* pickup = dynamic_cast<Pickupable*>(newCollection);
        return pickup;
    }
    
}