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
    @file Pickupable.h
    @brief Definition of the Pickupable class.
    @ingroup Pickup
*/

#ifndef _Pickupable_H__
#define _Pickupable_H__

#include "OrxonoxPrereqs.h"

#include <list>
#include "core/class/Super.h"

#include "core/class/OrxonoxClass.h"
#include "Rewardable.h"

namespace orxonox
{

    /**
    @brief
        An Interface (or more precisely an abstract class) to model and represent different (all kinds of) pickups.

        Pickups (@ref orxonox::Pickupable "Pickupables") are objects that (quite unsurprisingly) can be picked up. Additionally they can be used and unused (transition from used to not used), and also dropped.

    @author
        Damian 'Mozork' Frick

    @ingroup Pickup
    */
    class _OrxonoxExport Pickupable : virtual public OrxonoxClass, public Rewardable
    {
        friend class PickupCarrier;

        protected:
            Pickupable(); //!< Default constructor.

        public:
            virtual ~Pickupable(); //!< Default destructor.

            //! @brief Returns the representation name which refers to the name of the PickupRepresentation that is used to represent this pickup.
            virtual const std::string& getRepresentationName() const = 0;

            /**
            @brief Get whether the Pickupable is currently in use or not.
            @return Returns true if the Pickupable is currently in use.
            */
            inline bool isUsed(void) const
                { return this->used_; }
            /**
            @brief  Should be called when the Pickupable has transited from used to unused or the other way around.
                    Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedUsed); to their changdeUsed method.
            */
            virtual void changedUsed(void) {}

            /**
            @brief Get the carrier of the Pickupable.
            @return Returns a pointer to the carrier of the Pickupable.
            */
            inline PickupCarrier* getCarrier(void)
                { return this->carrier_; }
            /**
            @brief Should be called when the Pickupable has changed its PickupCarrier.
                   Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedCarrier); to their changedCarrier method.
            */
            virtual void changedCarrier(void) {}

            /**
            @brief Returns whether the Pickupable is currently picked up.
            @return Returns true if the Pickupable is currently picked up, false if not.
            */
            inline bool isPickedUp(void) const
                { return this->pickedUp_; }
            /**
            @brief  Should be called when the Pickupable has transited from picked up to dropped or the other way around.
                    Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedPickedUp); to their changedPickedUp method.
            */
            virtual void changedPickedUp(void) {}

            /**
            @brief Returns whether the Pickupable can be used.
            @return Returns true if it can be used.
            */
            inline bool isUsable(void) const
                { return this->enabled_; }

            /**
            @brief Returns whether the Pickupable can be unused.
            @return Returns true if it can be unused.
            */
            inline bool isUnusable(void) const
                { return this->enabled_; }

            /**
            @brief Returns whether the Pickupable is enabled.
                   Once a Pickupable is disabled it cannot be enabled again. A Pickupable that is disabled can neither be used nor unused.
            @return Returns true if the Pickupable is enabled.
            */
            inline bool isEnabled(void) const
                { return this->enabled_; }

            bool pickup(PickupCarrier* carrier); //!< Can be called to pick up a Pickupable.
            bool drop(bool createSpawner = true); //!< Can be called to drop a Pickupable.

            virtual bool isTarget(const PickupCarrier* carrier) const; //!< Get whether the given PickupCarrier is a target of this Pickupable.
            bool isTarget(const Identifier* identifier) const; //!< Get whether a given class, represented by the input Identifier, is a target of this Pickupable.
            bool addTarget(PickupCarrier* target); //!< Add a PickupCarrier as target of this Pickupable.
            bool addTarget(Identifier* identifier); //!< Add a class, representetd by the input Identifier, as target of this Pickupable.

            bool setUsed(bool used); //!< Sets the Pickupable to used or unused, depending on the input.
            bool setPickedUp(bool pickedUp); //!< Helper method to set the Pickupable to either picked up or not picked up.
            bool setCarrier(PickupCarrier* carrier, bool tell = true); //!< Sets the carrier of the Pickupable.

            void destroy(void); //!< Is called internally within the Pickupable module to destroy pickups.

        protected:
            virtual void preDestroy(void); //!< A method that is called by Destroyable::destroy() before the object is actually destroyed.
            virtual void destroyPickup(void); //!< Destroys a Pickupable.
            virtual void carrierDestroyed(void); //!< Is called by the PickupCarrier when it is being destroyed.

            /**
            @brief Sets the Pickuapble to disabled.
            */
            inline void setDisabled(void)
                { this->enabled_ = false; }

            /**
            @brief Check whether the Pickupable is in the process of being destroyed.
            @return Returns true if so.
            */
            inline bool isBeingDestroyed(void)
                { return this->beingDestroyed_; }

            /**
            @brief Facilitates the creation of a PickupSpawner upon dropping of the Pickupable.
                   This method must be implemented by any class directly inheriting from Pickupable.
            @return Returns true if a spawner was created, false if not.
            */
            virtual bool createSpawner(void) = 0;

        private:

            bool used_; //!< Whether the Pickupable is currently in use or not.
            bool pickedUp_; //!< Whether the Pickupable is currently picked up or not.

            bool enabled_; //!< Whether the Pickupable is enabled or not.

            PickupCarrier* carrier_; //!< The PickupCarrier of the Pickupable.
            std::list<Identifier*> targets_; //!< The possible targets of this Pickupable.

            bool beingDestroyed_; //!< Is true if the Pickupable is in the process of being destroyed.

        // For implementing the Rewardable interface:
        public:
            virtual bool reward(PlayerInfo* player); //!< Method to transcribe a Pickupable as a Rewardable to the player.

    };

    //! SUPER functions.
    SUPER_FUNCTION(10, Pickupable, changedUsed, false);
    SUPER_FUNCTION(11, Pickupable, changedCarrier, false);
    SUPER_FUNCTION(12, Pickupable, changedPickedUp, false);
}

#endif /* _Pickupable_H__ */
