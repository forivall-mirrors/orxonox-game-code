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
    @param maySpawnedItems
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
            COUT(2) << "A PickupSpawner was created without a valid Pickupable. This won't work." << std::endl;
            this->setActive(false);
        }
        else
        {
            PickupRepresentation* representation = PickupManager::getInstance().getRepresentation(this->pickup_->getPickupIdentifier());
            this->attach(representation->getSpawnerRepresentation(this));
        }
    }

    /**
    @brief
        Registers the object and sets some default values.
    */
    void PickupSpawner::initialize(void)
    {
        this->triggerDistance_ = 10;
        this->respawnTime_ = 0;
        this->maxSpawnedItems_ = INF;
        this->spawnsRemaining_ = INF;
    }

    /**
    @brief
        Destructor.
    */
    PickupSpawner::~PickupSpawner()
    {
        if(this->pickup_ != NULL)
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
            COUT(2) << "A PickupSpawner was created without a valid Pickupable. This won't work." << std::endl;
            this->setActive(false);
        }
        else
        {
            PickupRepresentation* representation = PickupManager::getInstance().getRepresentation(this->pickup_->getPickupIdentifier());
            representation->setVisible(this->isActive());
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

        this->setVisible(this->isActive());
    }

    /**
    @brief
        Tick, checks if any Pawn is close enough to trigger.
    @param dt
        Time since last tick.
    */
    //TODO: Replace with collisions.
    void PickupSpawner::tick(float dt)
    {
        SUPER(PickupSpawner, tick, dt);

        //! If the PickupSpawner is active.
        if (this->isActive())
        {
            SmartPtr<PickupSpawner> temp = this; // create a smart pointer to keep the PickupSpawner alive until we iterated through all Pawns (in case a Pawn takes the last pickup)

            //! Iterate trough all Pawns.
            for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
            {
                Vector3 distance = it->getWorldPosition() - this->getWorldPosition();
                PickupCarrier* carrier = dynamic_cast<PickupCarrier*>(*it);
                //! If a Pawn, that fits the target-range of the item spawned by this Pickup, is in trigger-distance.
                if (distance.length() < this->triggerDistance_ && carrier != NULL && carrier->isTarget(this->pickup_))
                {
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
        {
            this->spawnsRemaining_--;
        }
        if(this->spawnsRemaining_ != 0 && this->respawnTime_ > 0)
        {
            this->startRespawnTimer();

            this->setActive(false);
            this->fireEvent();
        }
        else
        {
            COUT(4) << "PickupSpawner (&" << this << ") empty, selfdestruct initialized." << std::endl;
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
            COUT(1) << "In PickupSpawner (&" << this << "): setPickupable called, with this->pickup_ already set." << std::endl;
            return;
        }
        if(pickup == NULL)
        {
            COUT(1) << "In PickupSpawner (&" << this << "): Argument of setPickupable is NULL." << std::endl;
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
    const Pickupable* PickupSpawner::getPickupable(void)
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
        if (this->isActive()) //!< Checks whether PickupSpawner is active.
        {
            COUT(4) << "PickupSpawner (&" << this << ") triggered and active." << std::endl;

            PickupCarrier* carrier = dynamic_cast<PickupCarrier*>(pawn);
            if(carrier == NULL)
            {
                COUT(1) << "This is bad. Pawn isn't PickupCarrier." << std::endl;
                return;
            }

            if(!carrier->isTarget(this->pickup_))
            {
                COUT(4) << "PickupSpawner (&" << this << ") triggered but Pawn wasn't a target of the Pickupable." << std::endl;
                return;
            }

            PickupCarrier* target = carrier->getTarget(this->pickup_);
            Pickupable* pickup = this->getPickup();

            if(target != NULL && pickup != NULL)
            {
                if(pickup->pickup(target))
                {
                    this->decrementSpawnsRemaining();
                }
                else
                {
                    pickup->destroy();
                }
            }
            else
            {
                if(target == NULL)
                    COUT(1) << "PickupSpawner (&" << this << "): Pickupable has no target." << std::endl;

                if(pickup == NULL)
                {
                    COUT(1) << "PickupSpawner (&" << this << "): getPickup produced an error, no Pickupable created." << std::endl;
                }
                else
                {
                    pickup->destroy();
                }
            }
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
            COUT(1) << "Massive Error: PickupSpawner still alive until having spawned last item." << std::endl;
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
        COUT(4) << "PickupSpawner (&" << this << ") reactivated." << std::endl;

        this->setActive(true);
    }
}
