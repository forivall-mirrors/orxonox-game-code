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
    @file
    @brief Implementation of PickupSpawner.
*/

#include "PickupSpawner.h"

#include "core/CoreIncludes.h"
//#include "core/GUIManager.h"     // HACK; see below
#include "core/Template.h"
#include "core/XMLPort.h"
#include "worldentities/pawns/Pawn.h"
//#include "PickupInventory.h"    // HACK; Only for hack, remove later


namespace orxonox
{

//     const float PickupSpawner::bounceSpeed_s = 6.0f;
//     const float PickupSpawner::rotationSpeed_s = 1.0f;
//     const float PickupSpawner::bounceDistance_s = 4.0f;

    CreateFactory(PickupSpawner);

    /**
    @brief
        Constructor. Registers the PickupSpawner.
    @param creator
        Pointer to the object which created this item.
    */
    PickupSpawner::PickupSpawner(BaseObject* creator) : StaticEntity(creator)
    {
        this->initialize();
    }

    PickupSpawner::PickupSpawner(BaseObject* creator, Pickupable* pickup, float triggerDistance, float respawnTime, int maxSpawnedItems) : StaticEntity(creator)
    {
        this->initialize();
  
        this->pickup_ = pickup;

        this->triggerDistance_ = triggerDistance;
        this->respawnTime_ = respawnTime;
        this->setMaxSpawnedItems(maxSpawnedItems);
    }

    void PickupSpawner::initialize(void)
    {
        RegisterObject(PickupSpawner);

        this->pickup_ = NULL;
        
        this->triggerDistance_ = 20;
        this->respawnTime_ = 0;
        this->tickSum_ = 0;
        this->maxSpawnedItems_ = INF;
        this->spawnsRemaining_ = INF;
    }

    /**
    @brief
        Destructor.
    */
    PickupSpawner::~PickupSpawner()
    {
        
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

        XMLPortObject(PickupSpawner, Pickupable, "pickup", addPickupable, getPickupable, xmlelement, mode);
        
        XMLPortParam(PickupSpawner, "triggerDistance", setTriggerDistance, getTriggerDistance, xmlelement, mode);
        XMLPortParam(PickupSpawner, "respawnTime", setRespawnTime, getRespawnTime, xmlelement, mode);
        XMLPortParam(PickupSpawner, "maxSpawnedItems", setMaxSpawnedItems, getMaxSpawnedItems, xmlelement, mode);

        //TODO: Kill hack.
        // HACKs
        // Load the GUI image as soon as the PickupSpawner gets loaded
        //  = less delays while running
//         BaseObject* newObject = this->itemTemplate_->getBaseclassIdentifier()->fabricate(this);
//         BaseItem* asItem = orxonox_cast<BaseItem*>(newObject);
//         if (asItem)
//         {
//             asItem->addTemplate(this->itemTemplate_);
//             PickupInventory::getImageForItem(asItem);
//             newObject->destroy();
//         }

        //  & load the GUI itself too, along with some empty windows
        //   = even less delays
//         GUIManager::showGUI("PickupInventory");
//         GUIManager::hideGUI("PickupInventory");
//         PickupInventory::getSingleton();
    }
    
    void PickupSpawner::addPickupable(Pickupable* pickup)
    {
        if(this->pickup_ != NULL)
        {
            COUT(1) << "addPickupable called, with this->pickup_ already set." << std::endl;
            return;
        }
        if(pickup == NULL)
        {
            COUT(1) << "Argument of addPickupable is NULL." << std::endl;
            return;
        }
        
        this->pickup_ = pickup;
    }
    
    Pickupable* PickupSpawner::getPickupable(void)
    {
        return this->pickup_;
    }

    /**
    @brief
        Invoked when the activity has changed. Sets visibility of attached objects.
    */
//     void PickupSpawner::changedActivity()
//     {
//         SUPER(PickupSpawner, changedActivity);
// 
//         for (std::set<WorldEntity*>::const_iterator it = this->getAttachedObjects().begin(); it != this->getAttachedObjects().end(); it++)
//             (*it)->setVisible(this->isActive());
//     }

    void PickupSpawner::setMaxSpawnedItems(int items)
    {
        this->maxSpawnedItems_ = items;
        this->spawnsRemaining_ = items;
    }

    /**
    @brief
        Tick, checks if any Pawn is close enough to trigger.
    @param dt
        Time since last tick.
    */
    //TODO: Replace this with a real DistanceTrigger?
    void PickupSpawner::tick(float dt)
    {
        //! If the PickupSpawner is active.
        if (this->isActive())
        {
            //! Iterate trough all Pawns.
            for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
            {
                Vector3 distance = it->getWorldPosition() - this->getWorldPosition();
                //! If a Pawn, that fits the target-range of the item spawned by this Pickup, is in trigger-distance.
                if (distance.length() < this->triggerDistance_ && this->pickup_->isTarget(*it))
                {
                    this->trigger(*it);
                }
            }

            //! Animation.
//             this->yaw(Radian(rotationSpeed_s*dt));
//             this->tickSum_ += bounceSpeed_s*dt;
//             this->translate(Vector3(0,bounceDistance_s*dt*sin(this->tickSum_),0));
//             if (this->tickSum_ > 2*Ogre::Math::PI)
//                 this->tickSum_ -= 2*Ogre::Math::PI;
        }
    }

    /**
    @brief
        Trigger the PickupSpawner.

        Adds the pickup to the Pawn that triggered,
        sets the timer to re-activate and deactives the PickupSpawner.

    @param pawn
        Pawn which triggered the PickupSpawner.
    */
    void PickupSpawner::trigger(Pawn* pawn)
    {
        //TODO: If private, isActive doesn't need to be tested anymore.
        if (this->isActive()) //!< Checks whether PickupItem is active.
        {
            Pickupable* pickup = this->getPickup();
            if (pickup != NULL) //!< If everything went ok, and pickup is not NULL.
            {
                PickupCarrier* carrier = dynamic_cast<PickupCarrier*>(pawn);
                if(carrier == NULL)
                {
                    COUT(1) << "This is bad. Pawn isn't PickupCarrier." << std::endl;
                    return;
                }
                
                if(pickup->pickup(carrier))
                {
                    COUT(3) << "Pickup got picked up." << std::endl;

                    this->decrementSpawnsRemaining();
                }
                else
                {
                    pickup->destroy();
                }
            }
        }
    }
    
    void PickupSpawner::decrementSpawnsRemaining(void)
    {
        if(this->spawnsRemaining_ != INF)
        {
            this->spawnsRemaining_--;
        }
        if(this->spawnsRemaining_ != 0 && this->respawnTime_ > 0)
        {
            //TODO: Nicer?
            this->respawnTimer_.setTimer(this->respawnTime_, false, createExecutor(createFunctor(&PickupSpawner::respawnTimerCallback, this)));

            this->setActive(false);
            this->fireEvent();
        }
        else
        {
            COUT(3) << "PickupSpawner empty, selfdistruct initialized." << std::endl;
            this->setActive(false);
            this->destroy(); //TODO: Implement destroy().
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
        COUT(3) << "PickupSpawner reactivated." << std::endl;

        this->setActive(true);
    }
}
