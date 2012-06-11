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
    @file Pickupable.cc
    @brief Implementation of the Pickupable class.
*/

#include "Pickupable.h"

#include "core/Identifier.h"
#include "core/CoreIncludes.h"
#include "util/Convert.h"

#include "infos/PlayerInfo.h"
#include "pickup/PickupIdentifier.h"
#include "worldentities/pawns/Pawn.h"

#include "PickupCarrier.h"
#include "PickupListener.h"

namespace orxonox
{

    /**
    @brief
        Constructor. Registers the objects and initializes its member variables.
    */
    Pickupable::Pickupable() : pickupIdentifier_(NULL), used_(false), pickedUp_(false)
    {
        RegisterRootObject(Pickupable);

        this->carrier_ = NULL;

        this->pickupIdentifier_ = new PickupIdentifier(this);
        this->beingDestroyed_ = false;
        this->enabled_ = true;
    }

    /**
    @brief
        Destructor.
    */
    Pickupable::~Pickupable()
    {
        if(this->pickupIdentifier_ != NULL)
        {
            orxout(verbose, context::pickups) << "Pickupable (&" << this << ") destroyed." << endl;
            this->pickupIdentifier_->destroy();
        }
    }

    /**
    @brief
        A method that is called by OrxonoxClass::destroy() before the object is actually destroyed.
    */
    void Pickupable::preDestroy(void)
    {
        this->beingDestroyed_ = true;

        if(this->isPickedUp())
            this->drop(false); // Drops the pickup without creating a PickupSpawner.
    }

    /**
    @brief
        Is called internally within the pickup module to destroy pickups.
    */
    void Pickupable::destroy(void)
    {
        this->destroyPickup();
    }

    /**
    @brief
        Destroys a Pickupable.
        If the Pickupable is already in the process of being destroyed a warning is displayed and this method is skipped.
    */
    void Pickupable::destroyPickup(void)
    {
        if(!this->isBeingDestroyed())
            this->OrxonoxClass::destroy();
        else
            orxout(internal_warning, context::pickups) << this->getIdentifier()->getName() << " may be unsafe. " << endl;
    }

    /**
    @brief
        Sets the Pickupable to used or unused, depending on the input.
    @param used
        If used is true the Pickupable is set to used, it is set to unused, otherwise.
    @return
        Returns true if the used state was changed, false if not.
    */
    bool Pickupable::setUsed(bool used)
    {
        if(this->used_ == used || !this->isPickedUp()) // If either the used status of the Pickupable doesn't change or it isn't picked up.
            return false;

        if((!this->isUsable() && used) || (!this->isUnusable() && !used)) // If either the Pickupable is requested to be used but it is not usable or the Pickupable is requested to be unused, while it is not unusable.
            return false;

        orxout(verbose, context::pickups) << "Pickupable (&" << this << ") set to used " << used << "." << endl;

        this->used_ = used;

        // Notify all the PickupListeners of the change.
        PickupListener::broadcastPickupChangedUsed(this, used);

        this->changedUsed();


        return true;
    }

    /**
    @brief
        Get whether the given PickupCarrier is a target of this Pickupable.
    @param carrier
        The PickupCarrier of which it has to be determinde whether it is a target of this Pickupable.
    @return
        Returns true if the given PickupCarrier is a target.
    */
    bool Pickupable::isTarget(const PickupCarrier* carrier) const
    {
        if(carrier == NULL)
            return false;

        return this->isTarget(carrier->getIdentifier());
    }

    /**
    @brief
        Get whether the given Identififer is a target of this Pickupable.
    @param identifier
        The PickupCarrier of which it has to be determinde whether it is a target of this Pickupable.
    @return
        Returns true if the given PickupCarrier is a target.
    */
    bool Pickupable::isTarget(const Identifier* identifier) const
    {
        // Iterate through all targets of this Pickupable.
        for(std::list<Identifier*>::const_iterator it = this->targets_.begin(); it != this->targets_.end(); it++)
        {
            if(identifier->isA(*it))
                return true;
        }

        return false;
    }

    /**
    @brief
        Add a PickupCarrier as target of this Pickupable.
    @param target
        The PickupCarrier to be added.
    @return
        Returns true if the target was added, false if not.
    */
    bool Pickupable::addTarget(PickupCarrier* target)
    {
        return this->addTarget(target->getIdentifier());
    }

    /**
    @brief
        Add a class, representetd by the input Identifier, as target of this Pickupable.
    @param target
        The Identifier to be added.
    @return
        Returns true if the target was added, false if not.
    */
    bool Pickupable::addTarget(Identifier* target)
    {
        if(this->isTarget(target)) // If the input target is already present in the list of targets.
            return false;

        orxout(verbose, context::pickups) << "Target " << target->getName() << " added to Pickupable (" << this->getIdentifier()->getName() << ") (&" << this << ")." << endl;
        this->targets_.push_back(target);
        return true;
    }

    /**
    @brief
        Can be called to pick up a Pickupable.
    @param carrier
        A pointer to the PickupCarrier that picks up the Pickupable.
    @return
        Returns true if the Pickupable was picked up, false if not.
    */
    bool Pickupable::pickup(PickupCarrier* carrier)
    {
        if(carrier == NULL || this->isPickedUp()) // If carrier is NULL or the Pickupable is already picked up.
            return false;

        if(!this->setCarrier(carrier))
        {
            orxout(internal_warning, context::pickups) << "A Pickupable (&" << this << ") was trying to be added to a PickupCarrier, but was already present." << endl;
            return false;
        }

        this->setPickedUp(true);
        orxout(verbose, context::pickups) << "Pickupable (&" << this << ") got picked up by a PickupCarrier (&" << carrier << ")." << endl;
        return true;
    }

    /**
    @brief
        Can be called to drop a Pickupable.
    @param createSpawner
        If true a spawner is to be created for the dropped Pickupable. True is default.
    @return
        Returns true if the Pickupable has been dropped, false if not.
    */
    bool Pickupable::drop(bool createSpawner)
    {
        if(!this->isPickedUp()) // If the Pickupable is not picked up.
            return false;

        assert(this->getCarrier()); // The Carrier cannot be NULL at this point.
        if(!this->getCarrier()->removePickup(this)) //TODO Shouldn't this be a little later?
            orxout(internal_warning, context::pickups) << "Pickupable (&" << this << ", " << this->getIdentifier()->getName() << ") is being dropped, but it was not present in the PickupCarriers list of pickups." << endl;

        orxout(verbose, context::pickups) << "Pickupable (&" << this << ") got dropped up by a PickupCarrier (&" << this->getCarrier() << ")." << endl;
        this->setUsed(false);
        this->setPickedUp(false);

        bool created = false;
        if(createSpawner)
            created = this->createSpawner();

        this->setCarrier(NULL);

        if(!created && createSpawner) // If a PickupSpawner should have been created but wasn't.
            this->destroy();

        return true;
    }

    /**
    @brief
        Helper method to set the Pickupable to either picked up or not picked up.
    @param pickedUp
        The value this->pickedUp_ should be set to.
    @return
        Returns true if the pickedUp status was changed, false if not.
    */
    bool Pickupable::setPickedUp(bool pickedUp)
    {
        if(this->pickedUp_ == pickedUp) // If the picked up status has not changed.
            return false;

        orxout(verbose, context::pickups) << "Pickupable (&" << this << ") set to pickedUp " << pickedUp << "." << endl;

        this->pickedUp_ = pickedUp;

        // Notify all the PickupListeners of the change.
        PickupListener::broadcastPickupChangedPickedUp(this, pickedUp);

        if(!pickedUp) // if the Pickupable has been dropped it unregisters itself with its PickupCarrier.
            this->getCarrier()->removePickup(this);
        this->changedPickedUp();

        return true;
    }

    /**
    @brief
        Sets the carrier of the Pickupable.
    @param carrier
        Sets the input PickupCarrier as the carrier of the pickup.
    @param tell
        If true (default) the pickup is added to the list of pickups in the PickupCarrier.
    @return
        Returns true if successful, false if not.
    */
    bool Pickupable::setCarrier(orxonox::PickupCarrier* carrier, bool tell)
    {
        if(this->carrier_ == carrier) // If the PickupCarrier doesn't change.
            return false;

        orxout(verbose, context::pickups) << "Pickupable (&" << this << ") changed Carrier (& " << carrier << ")." << endl;

        if(carrier != NULL && tell)
        {
            if(!carrier->addPickup(this))
                return false;
        }

        this->carrier_ = carrier;
        this->changedCarrier();
        return true;
    }

    /**
    @brief
        Is called by the PickupCarrier when it is being destroyed.
    */
    void Pickupable::carrierDestroyed(void)
    {
        this->destroy();
    }

    /**
    @brief
        Creates a duplicate of the Pickupable.
    @return
        Returns the clone of this pickup as a pointer to a Pickupable.
    */
    Pickupable* Pickupable::clone(void)
    {
        OrxonoxClass* item = NULL;
        this->clone(item);

        Pickupable* pickup = orxonox_cast<Pickupable*>(item);

        orxout(verbose, context::pickups) << "Pickupable (&" << this << ") cloned. Clone is new Pickupable (&" << pickup << ")." << endl;
        return pickup;
    }

    /**
    @brief
        Method to transcribe a Pickupable as a Rewardable to the player.
    @param player
        A pointer to the PlayerInfo, do whatever you want with it.
    @return
        Return true if successful.
    */
    bool Pickupable::reward(PlayerInfo* player)
    {
        ControllableEntity* entity = player->getControllableEntity();
        Pawn* pawn = static_cast<Pawn*>(entity);
        PickupCarrier* carrier = static_cast<PickupCarrier*>(pawn);
        return this->pickup(carrier);
    }

}
