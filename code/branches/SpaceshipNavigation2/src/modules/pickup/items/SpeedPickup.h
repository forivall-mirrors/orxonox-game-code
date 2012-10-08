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
    @file SpeedPickup.h
    @brief Declaration of the SpeedPickup class.
    @ingroup PickupItems
*/

#ifndef _SpeedPickup_H__
#define _SpeedPickup_H__

#include "pickup/PickupPrereqs.h"

#include <string>

#include "pickup/Pickup.h"

namespace orxonox {

    /**
    @brief
        A Pickup which can manipulate the Speed of a Pawn.

        There are 5 parameters that can be cosen:
        - The @b speedMultiply, specifies a factor by which the Spaceships speed is multiplied. The default is 1.
        - The @b speedAdd, specifies a value that is added to the speed of the Spaceship. The default is 0.
        - The @b activationType <em>immediate</em> or <em>onUse</em>, defines if the SpeedPickup is used when it's picked up or only after the player chooses to use it. The default is <em>immediate</em>.
        - The @b durationType Can be either <em>once</em> or <em>continuous</em>. For <em>once</em> the SpeedPickup is just active for as long as it is used, for <em>continuous</em> the SpeedPickup is active only for the specified duration. The default is <em>once</em>.
        - The @b duration The time in seconds the SpeedPickup is active at the most. The default is 0.

        An example, how a SpeedPickup could be defined in XML could be:
        @code
        <SpeedPickup
            speedMultiply = 2.0
            speedAdd = 10.0
            activationType = "immediate"
            durationType = "continuous"
            duration = 30.0
        />
        @endcode

    @author
        Eric Beier

    @ingroup PickupItems
    */
    class _PickupExport SpeedPickup : public Pickup
    {
        public:

            SpeedPickup(BaseObject* creator); //!< Constructor.
            virtual ~SpeedPickup(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode); //!< Method for creating a HealthPickup object through XML.

            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.

            /**
            @brief Get the duration, the time the SpeedPickup is active.
            @return Returns the duration in seconds.
            */
            inline float getDuration(void) const
                { return this->duration_; }
            /**
            @brief Get the value that is added to the speed of the Pawn.
            @return Returns the speedAdd.
            */
            inline float getSpeedAdd(void) const
                { return this->speedAdd_; }
            /**
            @brief Get the factor by wich the speed of the Pawn is multplied.
            @return Returns the speedMultiply.
            */
            inline float getSpeedMultiply(void) const
                { return this->speedMultiply_; }

        protected:
            void pickupTimerCallback(void); //!< Function that gets called when timer ends.

            void setDuration(float duration); //!< Sets the duration.
            void setSpeedAdd(float speedAdd); //!< Sets the SpeedAdd, the value that is added to the speed of the Pawn.
            void setSpeedMultiply(float speedMultiply); //!< Sets the speedMultiply, the factor by which the speed of the Pawn is multiplied.

        private:
            void initialize(void); //!< Initializes the member variables.
            SpaceShip* carrierToSpaceShipHelper(void); //!< Helper to transform the PickupCarrier to a SpaceShip, and throw an error message if the conversion fails.

            Timer durationTimer_; //!< Timer.

            float duration_; //!< The time in seconds for which the SpeedPickup stays active.
            float speedAdd_; //!< The value that is added to the speed of the Pawn.
            float speedMultiply_; //!< The factor by which the speed of the Pawn is multiplied.
    };
}

#endif // _SpeedPickup_H__
