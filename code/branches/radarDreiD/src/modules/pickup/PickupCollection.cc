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
#include "PickupSpawner.h"

#include "PickupCollection.h"

namespace orxonox
{

    RegisterClass(PickupCollection);

    /**
    @brief
        Default Constructor.
    @param creator
        The creator of the object.
    */
    PickupCollection::PickupCollection(Context* context) : BaseObject(context)
    {
        RegisterObject(PickupCollection);

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
            (*it)->destroy();
        }
        this->pickups_.clear();
    }

    /**
    @brief
        Creates an instance of this Class through XML.
    */
    void PickupCollection::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PickupCollection, XMLPort, xmlelement, mode);

        XMLPortParam(PickupCollection, "representation", setRepresentationName, getRepresentationName, xmlelement, mode);
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

        size_t numPickupsEnabled = 0;
        size_t numPickupsInUse = 0;
        for(std::list<CollectiblePickup*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); ++it)
        {
            if ((*it)->isEnabled())
                ++numPickupsEnabled;
            if ((*it)->isUsed())
                ++numPickupsInUse;
        }

        // If all the pickups are not in use but the PickupCollection is.
        if(numPickupsInUse == 0 && this->isUsed())
            this->setUsed(false);

        // If all the enabled pickups are in use but the PickupCollection is not.
        if(numPickupsInUse > 0 && numPickupsInUse == numPickupsEnabled && !this->isUsed())
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
        bool isOnePickupEnabledAndPickedUp = false;
        for(std::list<CollectiblePickup*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); ++it)
        {
            if ((*it)->isEnabled() && (*it)->isPickedUp())
            {
                isOnePickupEnabledAndPickedUp = true;
                break;
            }
        }
        if(!isOnePickupEnabledAndPickedUp && this->isPickedUp())
            this->Pickupable::destroy();
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
        this->pickupsChanged();
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
        if(this->pickups_.size() >= index)
            return NULL;

        std::list<CollectiblePickup*>::const_iterator it = this->pickups_.begin();
        std::advance(it, index);
        return *it;
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
                this->pickupsChanged();
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
        this->changedPickedUpAction();
    }

    /**
    @brief
        Informs the PickupCollection, that one of its pickups has been disabled.
        This is used internally by the CollectiblePickup class.
    */
    void PickupCollection::pickupDisabled(void)
    {
    }

    /**
    @brief
        Helpfer function if the number of pickups in this collection has changed.
    */
    void PickupCollection::pickupsChanged(void)
    {
        this->changedUsedAction();
        this->changedPickedUpAction();
    }

    /**
    @brief
        Facilitates the creation of a PickupSpawner upon dropping of the Pickupable.
    @return
        Returns true if a spawner was created, false if not.
    */
    bool PickupCollection::createSpawner(void)
    {
        PickupSpawner::createDroppedPickup(this->getContext(), this, this->getCarrier());
        return true;
    }

}
