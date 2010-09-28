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
    @file CollectiblePickup.cc
    @brief Implementation of the CollectiblePickup class.
*/

#include "CollectiblePickup.h"

#include "core/CoreIncludes.h"

#include "PickupCollection.h"

namespace orxonox {

    /**
    @brief
        Constructor.
        Registers the object and initializes variables.
    */
    CollectiblePickup::CollectiblePickup() : isInCollection_(false)
    {
        RegisterObject(CollectiblePickup);

        this->collection_ = NULL;
    }

    /**
    @brief
        Destructor.
    */
    CollectiblePickup::~CollectiblePickup()
    {

    }

    /**
    @brief
        Is called by OrxonoxClass::destroy() before the object is actually destroyed.
    */
    void CollectiblePickup::preDestroy(void)
    {
        this->Pickupable::preDestroy();

        // The PickupCollection has to be destroyed as well.
        if(this->isInCollection())
            this->collection_->Pickupable::destroy();
    }

    /**
    @brief
        Destroys a Pickupable.
    */
    void CollectiblePickup::destroyPickup(void)
    {
        if(!this->isInCollection()) // If the CollectiblePickup is not in a PickupCollection the destroyPickup method of Pickupable is called.
            this->Pickupable::destroyPickup();
        else // Else the ColectiblePickup is dropped and disabled,
        {
            this->drop(false);
            if(this->isInCollection() && this->isEnabled()) // It is only disabled if it is enabled and still ina PickupCollection after having been dropped.
            {
                this->setDisabled();
                this->collection_->pickupDisabled();
            }
        }
    }

    /**
    @brief
        Is called by the PickupCarrier when it is being destroyed.
    */
    void CollectiblePickup::carrierDestroyed(void)
    {
        if(!this->isInCollection())
            this->Pickupable::destroy();
        else // If the CollectiblePickup is part of a PickupCollection it is just dropped instead of destroyed.
            this->drop(false);
    }

    /**
    @brief
        Is called when the pickup has transited from used to unused or the other way around.
    */
    void CollectiblePickup::changedUsed(void)
    {
        SUPER(CollectiblePickup, changedUsed);

        if(this->isInCollection())
            this->collection_->pickupChangedUsed(this->isUsed());
    }

    /**
    @brief
        Is called when the pickup has transited from picked up to dropped or the other way around.
    */
    void CollectiblePickup::changedPickedUp(void)
    {
        SUPER(CollectiblePickup, changedPickedUp);

        if(this->isInCollection())
            this->collection_->pickupChangedPickedUp(this->isPickedUp());
    }

    /**
    @brief
        Adds this CollectiblePickup to the input PickupCollection.
    @param collection
        A pointer to the PickupCollection to which the CollectiblePickup should be added.
    @return
        Returns true if the CollectiblePickup was successfully added to the PickupCollection.
    */
    bool CollectiblePickup::addToCollection(PickupCollection* collection)
    {
        if(this->isInCollection() || collection == NULL) //If the CollectiblePickup already is in a PickupCollection or if the input pointer is NULL.
            return false;

        this->isInCollection_ = true;
        this->collection_ = collection;
        return true;
    }

    /**
    @brief
        Removes this CollectiblePickup from its PickupCollection.
    @return
        Returns true if the CollectiblePickup was succcessfully removed.
    */
    bool CollectiblePickup::removeFromCollection(void)
    {
        if(!this->isInCollection()) //If the CollectiblePickup is not in a PickupCollection.
            return false;

        this->isInCollection_ = false;
        this->collection_ = NULL;
        return true;
    }

}
