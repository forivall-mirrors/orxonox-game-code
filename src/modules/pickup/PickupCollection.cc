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
#include "DroppedPickup.h"

#include "PickupCollectionIdentifier.h"

namespace orxonox
{
 
    CreateFactory(PickupCollection);

    /**
    @brief
        Default Constructor.
    */
    PickupCollection::PickupCollection(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(PickupCollection);
        
        this->pickupCollectionIdentifier_ = new PickupCollectionIdentifier(this);
    }
    
    /**
    @brief
        Destructor.
    */
    PickupCollection::~PickupCollection()
    {
        //! Destroy all Pickupables constructing this PickupCollection.
        for(std::vector<WeakPtr<Pickupable> >::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            if((*it).get() != NULL)
                (*it).get()->destroy();
        }
    }
    
    /**
    @brief
        Creates an instance of this Class through XML.
    */
    void PickupCollection::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PickupCollection, XMLPort, xmlelement, mode);
        
        XMLPortObject(PickupCollection, Pickupable, "pickupables", addPickupable, getPickupable, xmlelement, mode);
        
        this->initializeIdentifier();
    }
    
    void PickupCollection::initializeIdentifier(void)
    {
        for(std::vector<WeakPtr<Pickupable> >::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            this->pickupCollectionIdentifier_->addPickup((*it).get()->getPickupIdentifier());
        }
    }
    
    /**
    @brief
        Facilitates the creation of a PickupSpawner upon dropping of the Pickupable.
        This method must be implemented by any class directly inheriting from Pickupable. It is most easily done by just creating a new DroppedPickup, e.g.:
        DroppedPickup(BaseObject* creator, Pickupable* pickup, const Vector3& position);
    @param position
        The position at which the PickupSpawner should be placed.
    @return
        Returns true if a spawner was created, false if not.
    */
    bool PickupCollection::createSpawner(const Vector3& position)
    {
        new DroppedPickup(this, this, position);
        return true;
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
        
        WeakPtr<Pickupable> ptr = pickup;
        this->pickups_.push_back(ptr);
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
    const Pickupable* PickupCollection::getPickupable(unsigned int index)
    {
        return this->pickups_[index].get(); //TODO. Does this work?
    }
    
    void PickupCollection::changedUsed(void)
    {
        SUPER(PickupCollection, changedUsed);
        
        //! Change used for all Pickupables this PickupCollection consists of.
        for(std::vector<WeakPtr<Pickupable> >::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            (*it).get()->setUsed(this->isUsed());
        }
    }
    
    void PickupCollection::changedCarrier(void)
    {
        SUPER(PickupCollection, changedCarrier);
        
        //! Change used for all Pickupables this PickupCollection consists of.
        for(std::vector<WeakPtr<Pickupable> >::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            (*it).get()->setCarrier(this->getCarrier());
        }
    }
    
    void PickupCollection::changedPickedUp()
    {
        SUPER(PickupCollection, changedPickedUp);
        
        //! Change the carrier for all Pickupables this PickupCollection consists of.
        for(std::vector<WeakPtr<Pickupable> >::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            (*it).get()->setPickedUp(this->isPickedUp());
        }
    }
    
    void PickupCollection::clone(OrxonoxClass*& item)
    {
        if(item == NULL)
            item = new PickupCollection(this);
        
        SUPER(PickupCollection, clone, item);
        
        PickupCollection* pickup = dynamic_cast<PickupCollection*>(item);
        for(std::vector<WeakPtr<Pickupable> >::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            Pickupable* newPickup = (*it).get()->clone();
            pickup->addPickupable(newPickup);
        }

        pickup->initializeIdentifier();
    }
    
    bool PickupCollection::isTarget(Identifier* identifier) const
    {
        for(std::vector<WeakPtr<Pickupable> >::const_iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            if(!(*it).get()->isTarget(identifier))
                return false;
        }
        
        return true;
    }
    
    const PickupIdentifier* PickupCollection::getPickupIdentifier(void)
    {
        return this->pickupCollectionIdentifier_;
    }
    
}