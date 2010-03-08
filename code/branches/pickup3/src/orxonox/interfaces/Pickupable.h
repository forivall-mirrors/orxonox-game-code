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
    @file
    @brief Definition of the Pickupable class.
*/

#ifndef _Pickupable_H__
#define _Pickupable_H__

#include "OrxonoxPrereqs.h"

#include <list>
#include "core/Super.h"

#include "core/OrxonoxClass.h"

namespace orxonox
{
    
    /**
    @brief
        An Interface (or more precisely an abstract class) to model and represent different (all kinds of) pickups.
    @author
        Damian 'Mozork' Frick
    */
    //TODO: Add stuff like weight/space ?
    class _OrxonoxExport Pickupable : virtual public OrxonoxClass
    {
        
        public:
            Pickupable(); //!< Default constructor.
            virtual ~Pickupable(); //!< Default destructor.
            
            /**
            @brief Get whether the pickup is currently in use or not.
            @return Returns true if the pickup is currently in use.
            */
            inline bool isUsed(void)
                { return this->used_; }
            /**
            @brief  Should be called when the pickup has transited from used to unused or the other way around.
                    Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedUsed); to their changdeUsed method.
            */
            virtual void changedUsed(void) {}
            bool setUsed(bool used); //!< Sets the Pickupable to used or unused, depending on the input.
            
            /**
            @brief Returns whether the Pickupable is currently picked up.
            @return Returns true if the Pickupable is currently picked up, false if not.
            */
            inline bool isPickedUp(void)
                { return this->pickedUp_; }
            //TODO: Better private, or protected?
            bool pickedUp(PickupCarrier* carrier); //!< Sets the Pickupable to picked up.
            bool dropped(void); //!< Sets the Pickupable to not picked up or dropped.
            
            bool isTarget(const PickupCarrier* carrier) const; //!< Get whether the given PickupCarrier is a target of this pickup.
            bool isTarget(Identifier* identifier) const; //!< Get whether a given class, represented by the input Identifier, is a target of this pickup.
            bool addTarget(PickupCarrier* target); //!< Add a PickupCarrier as target of this pickup.
            bool addTarget(Identifier* identifier); //!< Add a class, representetd by the input Identifier, as target of this pickup.
            
            /**
            @brief Get the carrier of the pickup.
            @return Returns a pointer to the carrier of the pickup.
            */
            inline PickupCarrier* getCarrier(void)
                { return this->carrier_; }
            /**
            @brief  Should be called when the pickup has transited from picked up to dropped or the other way around.
                    Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedCarrier); to their changedCarrier method.
            */
            virtual void changedCarrier(void) {}
            //TODO: Maybe private?
            bool setCarrier(PickupCarrier* carrier); //!< Sets the carrier of the pickup.
            
            Pickupable* clone(void); //!< Creates a duplicate of the Pickupable.
            virtual void clone(OrxonoxClass* item); //!< Creates a duplicate of the input OrxonoxClass.
            
            /**
            @brief Get the PickupIdentifier of this Pickupable.
            @return Returns a pointer to the PickupIdentifier of this Pickupable.
            */
            virtual const PickupIdentifier* getPickupIdentifier(void)
                { return this->pickupIdentifier_; }
                
            virtual void destroy(void)
                { delete this; }
            
        protected:
            /**
            @brief Helper method to initialize the PickupIdentifier.
            */
            //TODO: Really needed?
            void initializeIdentifier(void) {}
            
            /**
            @brief Facilitates the creation of a PickupSpawner upon dropping of the Pickupable.
                   This method must be implemented by any class directly inheriting from Pickupable. It is most easily done by just creating a new DroppedPickup, e.g.:
                   DroppedPickup(BaseObject* creator, Pickupable* pickup, const Vector3& position, float triggerDistance);
            @param position The position at which the PickupSpawner should be placed.
            @return Returns true if a spawner was created, false if not.
            */
            virtual bool createSpawner(const Vector3& position) = 0;
            
            //TODO: Move to private and create get method in protected.
            PickupIdentifier* pickupIdentifier_; //!< The PickupIdentifier of this Pickupable.
            
        private:
            /**
            @brief Helper method to set the Pickupable to either picked up or not picked up.
            @param pickedUp The value this->pickedUp_ should be set to.
            */
            inline void setPickedUp(bool pickedUp)
                { this->pickedUp_ = pickedUp; }
            
            bool used_; //!< Whether the pickup is currently in use or not.
            bool pickedUp_; //!< Whether the pickup is currently picked up or not.
            
            PickupCarrier* carrier_; //!< The carrier of the pickup.
            std::list<Identifier*> targets_; //!< The possible targets of this pickup.

    };
    
    SUPER_FUNCTION(10, Pickupable, changedUsed, false);
    SUPER_FUNCTION(12, Pickupable, changedCarrier, false);
}

#endif /* _Pickupable_H__ */
