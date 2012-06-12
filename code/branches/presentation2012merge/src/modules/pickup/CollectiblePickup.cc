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
    CollectiblePickup::CollectiblePickup() : collection_(NULL)
    {
        RegisterObject(CollectiblePickup);
    }

    /**
    @brief
        Destructor.
    */
    CollectiblePickup::~CollectiblePickup()
    {
        if (this->isInCollection())
            this->collection_->removePickupable(this);
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
        {
            if (!this->isPickedUp())
                this->collection_->removePickupable(this);
            else
                this->collection_->pickupChangedPickedUp(this->isPickedUp());
        }
    }

    /**
    @brief
        Notifies this CollectiblePickup that it was added to a PickupCollection.
    @param collection
        A pointer to the PickupCollection to which the CollectiblePickup should be added.
    */
    void CollectiblePickup::wasAddedToCollection(PickupCollection* collection)
    {
        this->collection_ = collection;
    }

    /**
    @brief
        Notifies this CollectiblePickup that it was removed from its PickupCollection.
    */
    void CollectiblePickup::wasRemovedFromCollection(void)
    {
        this->collection_ = NULL;
    }
}
