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
#include "core/OrxonoxClass.h"

#include "core/Super.h"
#include "pickup/PickupIdentifier.h"
#include <list>

namespace orxonox
{
    
    /**
    @brief
        An Interface (or more precisely an abstract Class) to model and represent different (all kinds of) pickups.
    @author
        Damian 'Mozork' Frick
    */
    //TODO: Add stuff like weight/space ?
    class _OrxonoxExport Pickupable : virtual public OrxonoxClass
    {
        
        public:
            Pickupable(); //!< Default constructor.
            virtual ~Pickupable() {} //!< Default destructor.
                
            /**
            @brief Get the carrier of the pickup.
            @return Returns a pointer to the carrier of the pickup.
            */
            inline PickupCarrier* getCarrier(void)
                { return this->carrier_; }
                
            /**
            @brief Get whether the pickup is currently in use or not.
            @return Returns true if the pickup is currently in use.
            */
            inline bool isUsed(void)
                { return this->used_; }
                
            bool isTarget(PickupCarrier* carrier);
            bool addTarget(PickupCarrier* target);

            bool setUsed(bool used);
            
            bool pickedUp(PickupCarrier* carrier);
            bool dropped(void);
            
            inline bool isPickedUp(void)
                { return this->pickedUp_; }
            
            Pickupable* clone(void);
            
            virtual const PickupIdentifier* getPickupIdentifier(void)
                { return &this->pickupIdentifier_; }
                
            virtual void clone(OrxonoxClass* item);
            
            /**
            @brief  Should be called when the pickup has transited from used to unused or the other way around.
                    Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedUsed); to their changdeUsed method.
            */
            virtual void changedUsed(void) {}
            
            /**
            @brief  Should be called when the pickup has transited from picked up to dropped or the other way around.
                    Any Class overwriting this method must call its SUPER function by adding SUPER(Classname, changedCarrier); to their changedCarrier method.
            */
            virtual void changedCarrier(void) {}
            
            bool setCarrier(PickupCarrier* carrier);
            
        protected:
            void initializeIdentifier(void) {}
            
            PickupIdentifier pickupIdentifier_;
            
        private:
            inline void setPickedUp(bool pickedUp)
                { this->pickedUp_ = pickedUp; }
            
            bool used_; //!< Whether the pickup is currently in use or not.
            bool pickedUp_; //!< Whether the pickup is currently picked up or not.
            
            PickupCarrier* carrier_; //!< The owner of the pickup.
            std::list<Identifier*> targets_; //!< The possible targets of this pickup.

    };
    
    SUPER_FUNCTION(10, Pickupable, changedUsed, false);
    SUPER_FUNCTION(12, Pickupable, changedCarrier, false);
}

#endif /* _Pickupable_H__ */
