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
 *      Daniel 'Huty' Haggenmueller
 *   Co-authors:
 *      Damian 'Mozork' Frick
 *
 */

/**
    @file PickupSpawner.cc
    @brief Implementation of the PickupSpawner class.
*/

#include "PickupSpawner.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/Template.h"
#include "core/XMLPort.h"

#include "worldentities/pawns/Pawn.h"

#include "PickupManager.h"
#include "PickupRepresentation.h"

namespace orxonox
{

    CreateFactory(PickupSpawner);

    /**
    @brief
        Constructor. Creates a blank PickupSpawner.
    @param creator
        Pointer to the object which created this item.
    */
    PickupSpawner::PickupSpawner(BaseObject* creator) : StaticEntity(creator), pickup_(NULL)
    {
        RegisterObject(PickupSpawner);

        this->initialize();
    }

    /**
    @brief
        Constructor, Creates a fully functional PickupSpawner.
    @param creator
        The creator of this PickupSpawner.
    @param pickup
        The Pickupable to be spawned by this PickupSpawner.
    @param triggerDistance
        The distance at which the PickupSpawner will trigger.
    @param respawnTime
        The minimum time between two spawns.
    @param maxSpawnedItems
        The maximum number of items spawned by this PickupSpawner.
    */
    PickupSpawner::PickupSpawner(BaseObject* creator, Pickupable* pickup, float triggerDistance, float respawnTime, int maxSpawnedItems) : StaticEntity(creator), pickup_(NULL)
    {
        RegisterObject(PickupSpawner);

        this->initialize();

        this->pickup_ = pickup;

        this->triggerDistance_ = triggerDistance;
        this->respawnTime_ = respawnTime;
        this->setMaxSpawnedItems(maxSpawnedItems);

        if(this->pickup_ == NULL)
        {
            orxout(internal_warning, context::pickups) << "A PickupSpawner was created without a valid Pickupable. This won't work." << endl;
            this->setActive(false);
        }
        else
        {
            PickupRepresentation* representation = PickupManager::getInstance().getRepresentation(this->pickup_->getPickupIdentifier());
            this->attach(representation->getSpawnerRepresentation(this));
            this->setActive(true);
        }
    }

    /**
    @brief
        Registers the object and sets some default values.
    */
    void PickupSpawner::initialize(void)
    {
        this->triggerDistance_ = 10;
        this->respawnTime_ = 5.0f;
        this->maxSpawnedItems_ = INF;
        this->spawnsRemaining_ = INF;
        this->selfDestruct_ = false;
    }

    /**
    @brief
        Destructor.
    */
    PickupSpawner::~PickupSpawner()
    {
        if(this->isInitialized() && this->selfDestruct_ && this->pickup_ != NULL)
            this->pickup_->destroy();
    }

    /**
    @brief
        Method for creating a PickupSpawner through XML.
    @param xmlelement
        XML element which contains the PickupSpawner.
    @param mode
        XMLPort mode.
    */
    void PickupSpawner::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PickupSpawner, XMLPort, xmlelement, mode);

        XMLPortObject(PickupSpawner, Pickupable, "pickup", setPickupable, getPickupable, xmlelement, mode);

        XMLPortParam(PickupSpawner, "triggerDistance", setTriggerDistance, getTriggerDistance, xmlelement, mode);
        XMLPortParam(PickupSpawner, "respawnTime", setRespawnTime, getRespawnTime, xmlelement, mode);
        XMLPortParam(PickupSpawner, "maxSpawnedItems", setMaxSpawnedItems, getMaxSpawnedItems, xmlelement, mode);

        if(this->pickup_ == NULL)
        {
            orxout(internal_warning, context::pickups) << "A PickupSpawner was created without a valid Pickupable. This won't work." << endl;
            this->setActive(false);
        }
        else
        {
            PickupRepresentation* representation = PickupManager::getInstance().getRepresentation(this->pickup_->getPickupIdentifier());
            this->attach(representation->getSpawnerRepresentation(this));
            this->setActive(true);
        }
    }

    /**
    @brief
        Invoked when the activity has changed. Sets visibility of attached objects.
    */
    void PickupSpawner::changedActivity()
    {
        SUPER(PickupSpawner, changedActivity);

        if(GameMode::isMaster())
            this->setVisible(this->isActive());
    }

    /**
    @brief
        Tick, checks if any Pawn is close enough to trigger.
    @param dt
        Time since last tick.
    */
    //TODO: Replace with collisions?
    void PickupSpawner::tick(float dt)
    {
        SUPER(PickupSpawner, tick, dt);

        // If the PickupSpawner is active.
        if(GameMode::isMaster() && this->isActive())
        {
            SmartPtr<PickupSpawner> temp = this; //Create a smart pointer to keep the PickupSpawner alive until we iterated through all Pawns (in case a Pawn takes the last pickup)

            // Remove PickupCarriers from the blocked list if they have exceeded their time.
            for(std::map<PickupCarrier*, std::time_t>::iterator it = this->blocked_.begin(); it != this->blocked_.end(); )
            {
                std::map<PickupCarrier*, std::time_t>::iterator temp = it;
                it++;
                if(temp->second < std::time(0))
                    this->blocked_.erase(temp);
            }

            // Iterate trough all Pawns.
            for(ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
            {
                Vector3 distance = it->getWorldPosition() - this->getWorldPosition();
                PickupCarrier* carrier = dynamic_cast<PickupCarrier*>(*it);
                // If a PickupCarrier, that fits the target-range of the Pickupable spawned by this PickupSpawnder, is in trigger-distance and the carrier is not blocked.
                if(distance.length() < this->triggerDistance_ && carrier != NULL && this->blocked_.find(carrier) == this->blocked_.end())
                {
                    if(carrier->isTarget(this->pickup_))
                        this->trigger(*it);
                }
            }
        }
    }

    /**
    @brief
        Sets the maximum number of spawned items.
    @param items
        The maximum number of spawned items to be set.
    */
    void PickupSpawner::setMaxSpawnedItems(int items)
    {
        this->maxSpawnedItems_ = items;
        this->spawnsRemaining_ = items;
    }

    /**
    @brief
        Decrements the number of remaining spawns.
        Sets the PickupSpawner to inactive for the duration of the respawnTime.
        Destroys the PickupSpawner if the number of remaining spawns has reached zero.
    */
    void PickupSpawner::decrementSpawnsRemaining(void)
    {
        if(this->spawnsRemaining_ != INF)
            this->spawnsRemaining_--;

        if(this->spawnsRemaining_ != 0 && this->respawnTime_ > 0)
        {
            this->startRespawnTimer();

            this->setActive(false);
            this->fireEvent();
        }
        else
        {
            orxout(verbose, context::pickups) << "PickupSpawner (&" << this << ") empty, selfdestruct initialized." << endl;
            this->setActive(false);
            this->destroy();
        }
    }

    /**
    @brief
        Starts the respawn timer.
    */
    void PickupSpawner::startRespawnTimer(void)
    {
        this->respawnTimer_.setTimer(this->respawnTime_, false, createExecutor(createFunctor(&PickupSpawner::respawnTimerCallback, this)));
    }

    /**
    @brief
        Sets a Pickupable for the PickupSpawner to spawn.
    @param pickup
        The Pickupable to be set.
    */
    void PickupSpawner::setPickupable(Pickupable* pickup)
    {
        if(this->pickup_ != NULL)
        {
            orxout(internal_error, context::pickups) << "In PickupSpawner (&" << this << "): setPickupable called, with this->pickup_ already set." << endl;
            return;
        }
        if(pickup == NULL)
        {
            orxout(internal_error, context::pickups) << "In PickupSpawner (&" << this << "): Argument of setPickupable is NULL." << endl;
            return;
        }

        this->pickup_ = pickup;
    }

    /**
    @brief
        Get the Pickupable that is spawned by this PickupSpawner.
    @return
        Returns the Pickupable that is spawned by this PickupSpawner.
    */
    const Pickupable* PickupSpawner::getPickupable(void) const
    {
        return this->pickup_;
    }

    /**
    @brief
        Trigger the PickupSpawner.
        Adds the pickup to the Pawn that triggered, sets the timer to re-activate and deactives the PickupSpawner.
    @param pawn
        Pawn which triggered the PickupSpawner.
    */
    void PickupSpawner::trigger(Pawn* pawn)
    {
        if(this->isActive()) // Checks whether PickupSpawner is active.
        {
            orxout(verbose, context::pickups) << "PickupSpawner (&" << this << ") triggered and active." << endl;

            PickupCarrier* carrier = dynamic_cast<PickupCarrier*>(pawn);
            assert(carrier);

            // If the Pawn isn't a target of the Pickupable.
            if(!carrier->isTarget(this->pickup_))
            {
                orxout(verbose, context::pickups) << "PickupSpawner (&" << this << ") triggered but Pawn wasn't a target of the Pickupable." << endl;
                return;
            }

            PickupCarrier* target = carrier->getTarget(this->pickup_);
            Pickupable* pickup = this->getPickup();

            this->block(carrier);

            assert(pickup);
            assert(target);
            bool pickedUp = pickup->pickup(target);
            assert(pickedUp);
            pickedUp = false; // To avoid compiler warning.

            this->decrementSpawnsRemaining();
        }
    }

    /**
    @brief
        Creates a new Pickupable.
    @return
        The Pickupable created.
    */
    Pickupable* PickupSpawner::getPickup(void)
    {
        if(this->spawnsRemaining_ == 0)
        {
            orxout(internal_error, context::pickups) << "Massive Error: PickupSpawner still alive until having spawned last item." << endl;
            return NULL;
        }

        Pickupable* pickup = this->pickup_->clone();
        return pickup;
    }

    /**
    @brief
        Invoked by the timer, re-activates the PickupSpawner.
    */
    void PickupSpawner::respawnTimerCallback()
    {
        orxout(verbose, context::pickups) << "PickupSpawner (&" << this << ") reactivated." << endl;

        this->setActive(true);
    }
}
