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
    @file PickupCollection.cc
    @brief Implementation of PickupCollection.
*/

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "interfaces/PickupCarrier.h"
#include "DroppedPickup.h"
#include "PickupCollectionIdentifier.h"

#include "PickupCollection.h"

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
        Destructor. Iterates through all Pickupables this PickupCollection consists of and destroys them if they haven't been already.
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

    /**
    @brief
        Initializes the PickupIdentifier for this pickup.
    */
    void PickupCollection::initializeIdentifier(void)
    {
        for(std::vector<WeakPtr<Pickupable> >::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            this->pickupCollectionIdentifier_->addPickup((*it).get()->getPickupIdentifier());
        }
    }

    /**
    @brief
        Is called when the pickup has transited from used to unused or the other way around.
        Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedUsed); to their changdeUsed method.
    */
    void PickupCollection::changedUsed(void)
    {
        SUPER(PickupCollection, changedUsed);

        //! Change used for all Pickupables this PickupCollection consists of.
        for(std::vector<WeakPtr<Pickupable> >::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            (*it).get()->setUsed(this->isUsed());
        }
    }

    /**
    @brief
        Is called when the pickup has changed its PickupCarrier.
        Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedCarrier); to their changedCarrier method.
    */
    void PickupCollection::changedCarrier(void)
    {
        SUPER(PickupCollection, changedCarrier);

        //! Change the PickupCarrier for all Pickupables this PickupCollection consists of.
        for(std::vector<WeakPtr<Pickupable> >::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            (*it).get()->setCarrier(this->getCarrier()->getTarget(*it), true);
        }
    }

    /**
    @brief
        Is called when the pickup has transited from picked up to dropped or the other way around.
        Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedPickedUp); to their changedPickedUp method.
    */
    void PickupCollection::changedPickedUp()
    {
        SUPER(PickupCollection, changedPickedUp);

        //! Change the pickedUp status for all Pickupables this PickupCollection consists of.
        for(std::vector<WeakPtr<Pickupable> >::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            (*it).get()->setPickedUp(this->isPickedUp());
        }
    }

    /**
    @brief
        Creates a duplicate of the input OrxonoxClass.
        This method needs to be implemented by any Class inheriting from Pickupable.
    @param item
        A reference to a pointer to the OrxonoxClass that is to be duplicated.
    */
    void PickupCollection::clone(OrxonoxClass*& item)
    {
        if(item == NULL)
            item = new PickupCollection(this);

        SUPER(PickupCollection, clone, item);

        PickupCollection* pickup = dynamic_cast<PickupCollection*>(item);
        //! Clone all Pickupables this PickupCollection consist of.
        for(std::vector<WeakPtr<Pickupable> >::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            Pickupable* newPickup = (*it).get()->clone();
            pickup->addPickupable(newPickup);
        }

        pickup->initializeIdentifier();
    }

    /**
    @brief
        Get whether a given class, represented by the input Identifier, is a target of this PickupCollection.
    @param identifier
        A pointer to the PickupIdentifier of the PickupCarrier we want to know of, whether it is a target of this PickupCollection.
    @return
        Returns true if the PickupCarrier identified by the input PickupIdentififer it is a target of this PickupCollection, false if not.
    */
    bool PickupCollection::isTarget(PickupCarrier* carrier) const
    {
        for(std::vector<WeakPtr<Pickupable> >::const_iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
        {
            if(!carrier->isTarget((*it).get()))
                return false;
        }

        return true;
    }

    /**
    @brief
        Get the PickupIdentifier of this PickupCollection.
        This is in fact the PickupCollectionIdentifier.
    @return
        Returns a pointer to the PickupIdentifier of this PickupCollection.
    */
    const PickupIdentifier* PickupCollection::getPickupIdentifier(void)
    {
        return this->pickupCollectionIdentifier_;
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

        WeakPtr<Pickupable> ptr = pickup; //!< Create a weak pointer to be able to test in the constructor if the Pointer is still valid.
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
        return this->pickups_[index].get();
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
    bool PickupCollection::createSpawner(void)
    {
        new DroppedPickup(this, this, this->getCarrier());
        return true;
    }

}