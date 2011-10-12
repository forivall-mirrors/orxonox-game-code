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
 *      Sandro Sgier
 *   Co-authors:
 *      ...
 *
 */

/**
    @file ShrinkPickup.h
    @brief Declaration of the ShrinkPickup class.
    @ingroup PickupItems
*/


#ifndef _ShrinkPickup_H__
#define _ShrinkPickup_H__

#include "pickup/PickupPrereqs.h"

#include <string>
#include <vector>

#include "pickup/Pickup.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox {

    /**
    @brief
        The ShrinkPickup is a Pickupable that causes the pawn to shrink to a certain size for a certain time with a certain speed, all of them specified in the following variables:
        - The @b shrinkFactor It determines how much the ship is going to shrink (e.g. the factor 2 would make the ship shrinking to half its size).
        - The @b duration Specifies how long the ship will stay small.
        - The @b shrinkDuration Defines how fast the ship shrinks and grows in seconds.

        An example of a XML implementation of a ShrinkPickup would be:
        @code
        <ShrinkPickup
            shrinkFactor = "5.0"
            duration = "5.0"
            shrinkSpeed = "5.0"
        />
        @endcode

    @author
        Sandro Sgier

    @ingroup PickupItems
    */

    class _PickupExport ShrinkPickup : public Pickup, public Tickable
    {
        public:
            ShrinkPickup(BaseObject* creator); // Constructor.
            virtual ~ShrinkPickup(); // Destructor.

            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode);
            virtual void tick(float dt);

            virtual void changedUsed(void); // Is called when the pickup has transited from used to unused or the other way around.
            virtual void changedPickedUp(void);
            virtual void clone(OrxonoxClass*& item); // Creates a duplicate of the input OrxonoxClass.

            /**
            @brief Get the shrinking factor.
            @return Returns the shrinking factor,
            */
            inline float getShrinkFactor(void) const
                { return this->shrinkFactor_; }
            /**
            @brief Sets the shrinking factor.
            @param factor The factor, needs to greater than 1.
            */
            inline void setShrinkFactor(float factor)
                { if(factor <= 1.0f) { orxout(internal_warning, context::pickups) << "Invalid shrinking factor in ShrinkPickup. Ignoring.." << endl; return; } this->shrinkFactor_ = factor; }
            /**
            @brief Get the duration for which the ship remains shrunken.
            @return Returns the duration.
            */
            inline float getDuration(void) const
                { return this->duration_; }
            /**
            @brief Set the duration for which the ship remains shrunken.
            @param duration The duration, needs to be non-negative.
            */
            inline void setDuration(float duration)
                { if(duration < 0.0f) { orxout(internal_warning, context::pickups) << "Invalid duration in ShrinkPickup. Ignoring.." << endl; return; } this->duration_ = duration; }
            /**
            @brief Get the shrink speed.
            @return Returns the shrink speed.
            */
            inline float getShrinkDuration(void) const
                { return this->shrinkDuration_; }
            /**
            @brief Set the shrink duration.
            @param speed The shrink duration, needs to be positive.
            */
            inline void setShrinkDuration(float speed)
                { if(speed <= 0.0f) { orxout(internal_warning, context::pickups) << "Invalid shrink duration in ShrinkPickup. Ignoring.." << endl; return; } this->shrinkDuration_ = speed; }

        protected:
            void initializeIdentifier(void);

        private:
            void initialize(void);

            float duration_;            //!< Determines how long the pickup will be active
            float shrinkFactor_;        //!< Shrink factor of the space ship
            float shrinkDuration_;      //!< Duration of shrinking

            bool isActive_;             //!< True if ship is shrinking, small, or growing back.
            bool isShrinking_;          //!< True if ship is shrinking
            bool isTerminating_;        //!< True if ship is growing back to the original size

            float currentFactor_;       //!< The shrink factor that is currently applied.
            float timeRemainig_;        //!< The remaining shrink time.
            
            Pawn* carrierToPawnHelper(void);
            Timer durationTimer_;
            void terminate(void);
    };
}

#endif
