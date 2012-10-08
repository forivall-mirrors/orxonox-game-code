
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
    @ingroup PickupItems
*/

#ifndef _ShieldPickup_H__
#define _ShieldPickup_H__

#include "pickup/PickupPrereqs.h"

#include <string>

#include "pickup/Pickup.h"

namespace orxonox {

    /**
    @brief
        A Pickup which can add a Shield to the Pawn.

        There are 5 parameters that can be cosen.
        - The @b shieldhealth< The amount of damage points a shield can take before collapsing. The default is 0.
        - The @b shieldabsorption The percentage the shield takes from the damage dealt to a Pawn. The default is 0.
        - The @b activationType, <em>immediate</em> or <em>onUse</em>. defines if the ShiedlPickup is used when it's picked up or only after the player chooses to use it. The default is <em>immediate</em>.
        - The @b durationType, <em>once</em> means, that the shield will stay until it collapses, <em>continuous</em> means, that the shield only stays for a limited amount of time, specifiey by the duration. The default is <em>once</em>.
        - The @b duration the time in seconds the shield is active at the most. The default is 0.

        An example of a XML implementation of a ShieldPickup would be:
        @code
        <ShieldPickup
            shieldhealth = 30
            shieldabsorption = 0.8
            activationType = "immediate"
            durationtype = "continuous"
            duration = 20.0
        />
        @endcode

    @author
        Eric Beier

    @ingroup PickupItems
    */
    class _PickupExport ShieldPickup : public Pickup
    {
        public:

            ShieldPickup(BaseObject* creator); //!< Constructor.
            virtual ~ShieldPickup(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode); //!< Method for creating a HealthPickup object through XML.

            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.

            /**
            @brief Get the duration, the time the shield is actvie at the most.
            @return Returns the duration in seconds.
            */
            inline float getDuration(void) const
                { return this->duration_; }
            /**
            @brief Get the shield health, the amount of damage the shield can sustain.
            @return Returns the shield health.
            */
            inline float getShieldHealth() const
                { return this->shieldHealth_; }
            /**
            @brief Get the shield absorption, the percentage of damage that is absorbed by the shield.
            @return Returns the shield absorption.
            */
            inline float getShieldAbsorption() const
                { return this->shieldAbsorption_; }

        protected:
            void pickupTimerCallback(void); //!< Helper method. Is called by the Timer as soon as it expires.

            void setDuration(float duration); //!< Sets the duration.
            void setShieldHealth(float shieldHealth); //!< Sets the health of the shield.
            void setShieldAbsorption(float shieldAbsorption); //!< Sets the percentage the shield absorbs of the dealt damage.

        private:
            void initialize(void); //!< Initializes the member variables.
            Pawn* carrierToPawnHelper(void); //!< Helper to transform the PickupCarrier to a Pawn, and throw an error message if the conversion fails.

            Timer durationTimer_; //!< Timer.

            float duration_; //!< The health that is transferred to the Pawn.
            float shieldHealth_; //!< The amount of damage the shield can sustain.
            float shieldAbsorption_; //!< The percentage of damage that is absorbed by the shield.

    };
}

#endif // _ShieldPickup_H__
