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

#include "CollectiblePickup.h"
#include "DroppedPickup.h"
#include "PickupCollectionIdentifier.h"

#include "PickupCollection.h"

namespace orxonox
{

    CreateFactory(PickupCollection);

    /**
    @brief
        Default Constructor.
    @param creator
        The creator of the object.
    */
    PickupCollection::PickupCollection(BaseObject* creator) : BaseObject(creator), pickupCollectionIdentifier_(NULL)
    {
        RegisterObject(PickupCollection);

        this->pickupCollectionIdentifier_ = new PickupCollectionIdentifier(this);
        this->usedCounter_ = 0;
        this->pickedUpCounter_ = 0;
        this->disabledCounter_ = 0;
        this->processingUsed_ = false;
        this->processingPickedUp_ = false;
    }

    /**
    @brief
        Destructor. Iterates through all Pickupables this PickupCollection consists of and destroys them if they haven't been already.
    */
    PickupCollection::~PickupCollection()
    {
        // Destroy all Pickupables constructing this PickupCollection.
        for(std::list<CollectiblePickup*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); ++it)
        {
            (*it)->wasRemovedFromCollection();
            (*it)->destroyPickup();
        }
        this->pickups_.clear();

        if(this->pickupCollectionIdentifier_ != NULL)
            delete this->pickupCollectionIdentifier_;
    }

    /**
    @brief
        Creates an instance of this Class through XML.
    */
    void PickupCollection::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PickupCollection, XMLPort, xmlelement, mode);

        XMLPortObject(PickupCollection, CollectiblePickup, "pickupables", addPickupable, getPickupable, xmlelement, mode);
    }

    /**
    @brief
        Is called when the pickup has transited from used to unused or the other way around.
        Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedUsed); to their changdeUsed method.
    */
    void PickupCollection::changedUsed(void)
    {
        SUPER(PickupCollection, changedUsed);

        this->processingUsed_ = true;
        // Change used for all Pickupables this PickupCollection consists of.
        for(std::list<CollectiblePickup*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); ++it)
            (*it)->setUsed(this->isUsed());

        this->processingUsed_ = false;

        this->changedUsedAction();
    }

    /**
    @brief
        Helper method.
        Checks whether due to changes in the used status of the pickups of this PickupCollection the used status of this PickupCollection has to change as well.
    */
    void PickupCollection::changedUsedAction(void)
    {
        if(this->processingUsed_)
            return;

        // If all the pickups are not in use but the PickupCollection is.
        if(this->usedCounter_ == 0 && this->isUsed())
            this->setUsed(false);

        // If all the enabled pickups are in use but the PickupCollection is not.
        if(this->usedCounter_ != 0 && this->usedCounter_ == this->pickups_.size()-this->disabledCounter_ && !this->isUsed())
            this->setUsed(true);
    }

    /**
    @brief
        Is called when the pickup has changed its PickupCarrier.
        Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedCarrier); to their changedCarrier method.
    */
    void PickupCollection::changedCarrier(void)
    {
        SUPER(PickupCollection, changedCarrier);

        // Change the PickupCarrier for all Pickupables this PickupCollection consists of.
        for(std::list<CollectiblePickup*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); ++it)
        {
            if(this->getCarrier() == NULL)
                (*it)->setCarrier(NULL);
            else
                (*it)->setCarrier(this->getCarrier()->getTarget(*it));
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

        this->processingPickedUp_ = true;
        // Change the pickedUp status for all Pickupables this PickupCollection consists of.
        for(std::list<CollectiblePickup*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); )
            (*(it++))->setPickedUp(this->isPickedUp());

        this->processingPickedUp_ = false;

        this->changedPickedUpAction();
    }

    /**
    @brief
        Helper method.
        Checks whether due to changes in the picked up status of the pickups of this PickupCollection the picked up status of this PickupCollection has to change as well.
    */
    void PickupCollection::changedPickedUpAction(void)
    {
        if(this->processingPickedUp_)
            return;

        // If at least all the enabled pickups of this PickupCollection are no longer picked up.
        if(this->pickedUpCounter_ <= this->disabledCounter_ && this->isPickedUp())
            this->Pickupable::destroy();

        // If the PickupCollection is no longer picked up.
        if(!this->isPickedUp())
            this->pickedUpCounter_ = 0;
    }

    /**
    @brief
        Creates a duplicate of the input Pickupable.
        This method needs to be implemented by any Class inheriting from Pickupable.
    @param item
        A reference to a pointer to the OrxonoxClass that is to be duplicated.
    */
    void PickupCollection::clone(OrxonoxClass*& item)
    {
        if(item == NULL)
            item = new PickupCollection(this);

        SUPER(PickupCollection, clone, item);

        PickupCollection* pickup = orxonox_cast<PickupCollection*>(item);
        // Clone all Pickupables this PickupCollection consist of.
        for(std::list<CollectiblePickup*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); ++it)
        {
            Pickupable* newPickup = (*it)->clone();
            CollectiblePickup* collectible = static_cast<CollectiblePickup*>(newPickup);
            pickup->addPickupable(collectible);
        }
    }

    /**
    @brief
        Get whether a given class, represented by the input Identifier, is a target of this PickupCollection.
    @param carrier
        A pointer to the PickupCarrier we want to know of, whether it is a target of this PickupCollection.
    @return
        Returns true if the PickupCarrier identified by the input PickupIdentififer it is a target of this PickupCollection, false if not.
    */
    bool PickupCollection::isTarget(const PickupCarrier* carrier) const
    {
        for(std::list<CollectiblePickup*>::const_iterator it = this->pickups_.begin(); it != this->pickups_.end(); ++it)
        {
            if(!carrier->isTarget(*it))
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
    const PickupIdentifier* PickupCollection::getPickupIdentifier(void) const
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
    bool PickupCollection::addPickupable(CollectiblePickup* pickup)
    {
        if(pickup == NULL)
            return false;

        this->pickups_.push_back(pickup);
        pickup->wasAddedToCollection(this);
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
    const Pickupable* PickupCollection::getPickupable(unsigned int index) const
    {
        unsigned int count = 0;
        for (std::list<CollectiblePickup*>::const_iterator it = this->pickups_.begin(); it != this->pickups_.end(); ++it)
        {
            if (count == index)
                return *it;
            else
                ++count;
        }
        return NULL;
    }

    /**
    @brief
        Removes the Pickup from the Collection.
    @param pickup
        The Pickup to be removed.
    @return
        Returns true if the pickup was in the collection.
    */
    bool PickupCollection::removePickupable(CollectiblePickup* pickup)
    {
        for(std::list<CollectiblePickup*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); ++it)
        {
            if (*it == pickup)
            {
                this->pickups_.erase(it);
                pickup->wasRemovedFromCollection();
                return true;
            }
        }
        return false;
    }

    /**
    @brief
        Informs the PickupCollection, that one of its pickups has changed its used status to the input value.
        This is used internally by the CollectiblePickup class.
    @param changed
        The value the used status has changed to.
    */
    void PickupCollection::pickupChangedUsed(bool changed)
    {
        if(changed)
            this->usedCounter_++;
        else
            this->usedCounter_--;

        this->changedUsedAction();
    }

    /**
    @brief
        Informs the PickupCollection, that one of its pickups has changed its picked up status to the input value.
        This is used internally by the CollectiblePickup class.
    @param changed
        The value the picked up status has changed to.
    */
    void PickupCollection::pickupChangedPickedUp(bool changed)
    {
        if(changed)
            this->pickedUpCounter_++;
        else
            this->pickedUpCounter_--;

        this->changedPickedUpAction();
    }

    /**
    @brief
        Informs the PickupCollection, that one of its pickups has been disabled.
        This is used internally by the CollectiblePickup class.
    */
    void PickupCollection::pickupDisabled(void)
    {
        this->disabledCounter_++;
    }

    /**
    @brief
        Facilitates the creation of a PickupSpawner upon dropping of the Pickupable.
        This method must be implemented by any class directly inheriting from Pickupable. It is most easily done by just creating a new DroppedPickup, e.g.:
        DroppedPickup(BaseObject* creator, Pickupable* pickup, const Vector3& position);
    @return
        Returns true if a spawner was created, false if not.
    */
    bool PickupCollection::createSpawner(void)
    {
        new DroppedPickup(this, this, this->getCarrier());
        return true;
    }

}
