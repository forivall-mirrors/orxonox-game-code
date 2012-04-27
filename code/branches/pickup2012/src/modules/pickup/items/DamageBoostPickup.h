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
    @file DamageBoostPickup.h
    @brief Declaration of the DamageBoostPickup class.
    @ingroup PickupItems
*/

#ifndef _DamageBoostPickup_H__
#define _DamageBoostPickup_H__

#include "pickup/PickupPrereqs.h"

#include <string>

#include "pickup/Pickup.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox {

    class _PickupExport DamageBoostPickup : public Pickup, public Tickable
    {
        public:

            DamageBoostPickup(BaseObject* creator); //!< Constructor.
            virtual ~DamageBoostPickup(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode); //!< Method for creating a DamageBoostPickup object through XML.
            // commented out cuz was not used
            // virtual void tick(float dt); //!< Is called every tick.

            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.
            virtual void clone(OrxonoxClass*& item); //!< Creates a duplicate of the input OrxonoxClass.

            /**
            @brief Get the damageboost that is transferred to the Pawn upon usage of this pickup.
            @return Returns the damageboost.
            */
            // Commentend, may not be used
            // inline float setDamageMultiplier(void) const
            //    { ship->setMultiplier(); }


    	    /**
            @brief Get the time the DamagePickup lasts.
            @return Returns the time in seconds the DamagePickup lasts.
            */
            inline float getDuration(void) const
                { return this->duration_; }
            inline void setDuration( float duration );


            // we need these methods to set the default damage multiplier from XML
            // not that beautiful yet
            inline void setDefaultDamageMultiplier(float multiplier)
                { this->damageMultiplier_ = multiplier; }
            inline float getDefaultDamageMultiplier()
                { return this->damageMultiplier_; }




        protected:
            void initializeIdentifier(void); //!< Initializes the PickupIdentifier of this pickup.

            // void setDamageBoost(float damageBoost);//!< Sets the damage boost.
            // void setDamageSave(float damageSave); //!< Saves the original damage.

        private:
            void initialize(void); //!< Initializes the member variables.
            Pawn* carrierToPawnHelper(void); //!< Helper to transform the PickupCarrier to a Pawn, and throw an error message if the conversion fails.
            SpaceShip* carrierToSpaceShipHelper(void);
            void pickupTimerCallback(void);



            float damageMultiplier_; //!< The damage boost that is transferred to the Pawn.
            float olddamageMultiplier_; //!< Helper to remember what the actual damage of the Pawn was before we changed it.

            Timer durationTimer_; //!< Timer.
	    float duration_; //! Duration of damage boost.

    };
}

#endif // _DamageBoostPickup_H__
