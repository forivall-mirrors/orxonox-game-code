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
    PickupSpawner::PickupSpawner(Context* context) : StaticEntity(context), pickup_(NULL), representation_(NULL), pickupTemplate_(NULL)
    {
        RegisterObject(PickupSpawner);

        this->initialize();
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

        this->setPickupable(NULL);
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
        Factory method, Creates a fully functional PickupSpawner.
    @param creator
        The creator of this PickupSpawner.
    @param pickup
        The Pickupable to be spawned by this PickupSpawner.
    @param carrier
        The PickupCarrier that carried the input pickup before it was dropped.
    @param triggerDistance
        The distance at which the PickupSpawner will trigger.
    */
    /*static*/ PickupSpawner* PickupSpawner::createDroppedPickup(Context* context, Pickupable* pickup, PickupCarrier* carrier, float triggerDistance)
    {
        PickupSpawner* spawner = new PickupSpawner(context);

        spawner->setPickupable(pickup);
        spawner->setTriggerDistance(triggerDistance);
        spawner->setMaxSpawnedItems(1);

        spawner->setPosition(carrier->getCarrierPosition());
        spawner->block(carrier);

        return spawner;
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

        XMLPortParam(PickupSpawner, "pickup", setPickupTemplateName, getPickupTemplateName, xmlelement, mode);
        XMLPortParam(PickupSpawner, "triggerDistance", setTriggerDistance, getTriggerDistance, xmlelement, mode);
        XMLPortParam(PickupSpawner, "respawnTime", setRespawnTime, getRespawnTime, xmlelement, mode);
        XMLPortParam(PickupSpawner, "maxSpawnedItems", setMaxSpawnedItems, getMaxSpawnedItems, xmlelement, mode);
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
            WeakPtr<PickupSpawner> spawner = this; // Create a smart pointer to keep the PickupSpawner alive until we iterated through all Pawns (in case a Pawn takes the last pickup)

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
                if(spawner == NULL) // Stop if the PickupSpawner has been deleted (e.g. because it has run out of pickups to distribute).
                    break;

                Vector3 distance = it->getWorldPosition() - this->getWorldPosition();
                PickupCarrier* carrier = static_cast<PickupCarrier*>(*it);
                // If a PickupCarrier, that fits the target-range of the Pickupable spawned by this PickupSpawner, is in trigger-distance and the carrier is not blocked.
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
        Trigger the PickupSpawner.
        Adds the pickup to the Pawn that triggered, sets the timer to re-activate and deactives the PickupSpawner.
    @param carrier
        Carrier which triggered the PickupSpawner.
    */
    void PickupSpawner::trigger(PickupCarrier* carrier)
    {
        orxout(verbose, context::pickups) << "PickupSpawner (&" << this << ") triggered and active." << endl;

        PickupCarrier* target = carrier->getTarget(this->pickup_);

        this->block(carrier);

        assert(target);
        bool pickedUp = this->pickup_->pickup(target);
        assert(pickedUp);
        pickedUp = false; // To avoid compiler warning.

        this->setPickupable(NULL);
        this->decrementSpawnsRemaining();
    }

    void PickupSpawner::setPickupTemplateName(const std::string& name)
    {
        Template* temp = Template::getTemplate(name);
        if (temp)
            this->setPickupTemplate(temp);
    }

    void PickupSpawner::setPickupTemplate(Template* temp)
    {
        this->pickupTemplate_ = temp;
        this->pickupTemplateName_ = temp->getName();

        this->setPickupable(this->createPickup());
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

        this->setActive(false);

        if(this->spawnsRemaining_ != 0 && this->respawnTime_ > 0)
        {
            this->startRespawnTimer();
            this->fireEvent();
        }
        else
        {
            orxout(verbose, context::pickups) << "PickupSpawner (&" << this << ") empty, selfdestruct initialized." << endl;
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
        Invoked by the timer, re-activates the PickupSpawner.
    */
    void PickupSpawner::respawnTimerCallback()
    {
        orxout(verbose, context::pickups) << "PickupSpawner (&" << this << ") reactivated." << endl;

        this->setPickupable(this->createPickup());
    }

    /**
    @brief
        Creates a new Pickupable.
    @return
        The Pickupable created.
    */
    Pickupable* PickupSpawner::createPickup(void)
    {
        if(this->spawnsRemaining_ == 0)
        {
            orxout(internal_error, context::pickups) << "Massive Error: PickupSpawner still alive until having spawned last item." << endl;
            return NULL;
        }

        if (this->pickupTemplate_ != NULL)
        {
            Identifier* identifier = this->pickupTemplate_->getBaseclassIdentifier();
            if (identifier != NULL)
            {
                Pickupable* pickup = orxonox_cast<Pickupable*>(identifier->fabricate(this->getContext()));
                orxonox_cast<BaseObject*>(pickup)->addTemplate(this->pickupTemplate_);
                return pickup;
            }
            else
                orxout(internal_error, context::pickups) << "No base class defined in pickup-template " << this->pickupTemplateName_ << endl;
        }

        return NULL;
    }

    /**
    @brief
        Sets a Pickupable for the PickupSpawner to spawn.
    @param pickup
        The Pickupable to be set.
    */
    void PickupSpawner::setPickupable(Pickupable* pickup)
    {
        if (this->representation_ != NULL)
        {
            this->representation_->destroy();
            this->representation_ = NULL;
        }

        if (pickup != NULL)
        {
            if (this->pickup_ != NULL)
                this->pickup_->destroy();

            PickupRepresentation* representation = PickupManager::getInstance().getRepresentation(pickup->getRepresentationName());
            this->representation_ = representation->createSpawnerRepresentation(this);
            this->attach(this->representation_);
            this->setActive(true);
        }
        else
        {
            this->setActive(false);
        }

        this->pickup_ = pickup;
    }
}
