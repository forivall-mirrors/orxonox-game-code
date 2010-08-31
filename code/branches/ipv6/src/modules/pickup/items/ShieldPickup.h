
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
 *      Eric Beier
 *   Co-authors:
 *      ...
 *
 */

/**
    @file ShieldPickup.h
    @brief Declaration of the ShieldPickup class.
*/

#ifndef _ShieldPickup_H__
#define _ShieldPickup_H__

#include "pickup/PickupPrereqs.h"

#include <string>
#include "worldentities/pawns/Pawn.h"
#include "worldentities/StaticEntity.h"

#include "pickup/Pickup.h"

namespace orxonox {

    /**
    @brief
        A Pickup which can add a Shield to the Pawn.

        1) The percentage: The percentage the shield takes from the damage dealt to a Pawn
        2) The hit points: The amount of damage points a shield can teake before collapsing
        3) The activation type: 'immediate' or 'onUse'. defines if the item is used when it's picked up or only after the player chooses to use it.
        4) The duration: the activation time of the pickup.

    @author
        Eric Beier
    */
    class _PickupExport ShieldPickup : public Pickup
    {
        public:

            ShieldPickup(BaseObject* creator); //!< Constructor.
            virtual ~ShieldPickup(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode); //!< Method for creating a HealthPickup object through XML.

            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.
            virtual void clone(OrxonoxClass*& item); //!< Creates a duplicate of the input OrxonoxClass.

            inline float getDuration(void)
                { return this->duration_; }
            inline float getShieldHealth()
                { return this->shieldHealth_; }
            inline float getShieldAbsorption()
                { return this->shieldAbsorption_; }

        protected:
            void initializeIdentifier(void); //!< Initializes the PickupIdentifier of this pickup.

            void pickupTimerCallback(void); //!< Function that gets called when timer ends.

            void setDuration(float duration);
            void setShieldHealth(float shieldHealth);
            void setShieldAbsorption(float shieldAbsorption);

        private:
            void initialize(void); //!< Initializes the member variables.
            Pawn* carrierToPawnHelper(void); //!< Helper to transform the PickupCarrier to a Pawn, and throw an error message if the conversion fails.

            Timer durationTimer_; //!< Timer.

            float duration_; //!< The health that is transferred to the Pawn.
            float shieldHealth_;
            float shieldAbsorption_; // Has to be between 0 and 1

    };
}

#endif // _ShieldPickup_H__
