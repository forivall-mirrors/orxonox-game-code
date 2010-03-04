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
        for(std::vector<Pickupable*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
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
        
        //TODO: Does this work? Problem could be, that Pickupable itself cannot be instantiated through XML, doubt that, though.
        XMLPortObject(PickupCollection, PickupCollection, "pickupables", addPickupable, getPickupable, xmlelement, mode);
        
        this->initializeIdentifier();
    }
    
    void PickupCollection::initializeIdentifier(void)
    {
        this->pickupCollectionIdentifier_.addClass(this->getIdentifier());
        
        for(std::vector<Pickupable*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            this->pickupCollectionIdentifier_.addPickup((*it)->getPickupIdentifier());
        }
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
        
        this->pickups_.push_back(pickup);
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
        return this->pickups_[index]; //TODO. Does this work?
    }
    
    //TODO: Steal description from Pickupable.
    void PickupCollection::changedUsed(void)
    {
        SUPER(PickupCollection, changedUsed);
        
        //! Change used for all Pickupables this PickupCollection consists of.
        for(std::vector<Pickupable*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            (*it)->setUsed(this->isUsed());
        }
    }
    
    void PickupCollection::changedCarrier()
    {
        SUPER(PickupCollection, changedCarrier);
        
        //! Change the carrier for all Pickupables this PickupCollection consists of.
        for(std::vector<Pickupable*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            (*it)->setCarrier(this->getCarrier());
        }
    }
    
    //TODO: Steal description from Pickupable.
    void PickupCollection::clone(OrxonoxClass* item)
    {
        if(item == NULL)
            item = new PickupCollection(this);
        
        SUPER(PickupCollection, clone, item);
        
        PickupCollection* pickup = dynamic_cast<PickupCollection*>(item);
        for(std::vector<Pickupable*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            Pickupable* newPickup = (*it)->clone();
            pickup->addPickupable(newPickup);
        }

        pickup->initializeIdentifier();
    }
    
}