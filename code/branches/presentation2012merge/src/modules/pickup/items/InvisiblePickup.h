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
 *      Benedict Simlinger
 *   Co-authors:
 *      ...
 *
 */

/**
    @file InvisiblePickup.h
    @brief Declaration of the InvisiblePickup class.
    @ingroup PickupItems
*/

#ifndef _InvisiblePickup_H__
#define _InvisiblePickup_H__

#include "pickup/PickupPrereqs.h"

#include <string>

#include "pickup/Pickup.h"

namespace orxonox {

    /**
    @brief
        A pickup that makes the Pawn invisible.
        There are 3 parameters that can be chosen:
        - The @b activationType It can be chosen to be either <em>immediate</em> or <em>onUse</em>. The activation type essentially (as indicated by the name) defines when the Pawn will be invisible, either immediately after being picked up or only after the player uses it. The default is <em>immediate</em>.
        - The @b durationType< It can be chosen to be either <em>once</em> or <em>continuous</em>. For <em>once</em> the InvisiblePickup just makes the Pawn invisible for as long as it is used, for <em>continuous</em> the Pawn is invisible for the specified duration. The default is <em>once</em>.
        - The @b duration Specifies how long (in seconds) the invisibility lasts. The default is 0.

        An example of a XML implementation of a InvisiblePickup would be:
        @code
        <InvisiblePickup
            activationType = "immediate"
            durationType = "continuous"
            duration = 30.0
        />
        @endcode

    @author
        Benedict Simlinger

    @ingroup PickupItems
    */
    class _PickupExport InvisiblePickup : public Pickup
    {
        public:

            InvisiblePickup(BaseObject* creator); //!< Constructor.
            virtual ~InvisiblePickup(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode); //!< Method for creating a HealthPickup object through XML.
            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.
            virtual void clone(OrxonoxClass*& item); //!< Creates a duplicate of the input OrxonoxClass.

            /**
            @brief Checks whether the Pawn is invisible.
            @return Returns if the Pawn is invisible.
            */
            inline bool getInvisibility(bool) const
                { return this->invisible_; }
            /**
            @brief Get the time the InvisibilityPickup lasts.
            @return Returns the time in seconds the InvisibiltyPickup lasts.
            */
            inline float getDuration(void) const
                { return this->duration_; }

        protected:
            bool setInvisible(bool invisibility); //!< Set the Pawn to be invisible or visible again.
            void setDuration(float duration); //!< Sets the time the InvisibilityPickup will last.

            void pickupTimerCallback(void); //!< Helper method. Is called by the Timer as soon as it expires.

        private:
            void initialize(void); //!< Initializes the member variables.
            Pawn* carrierToPawnHelper(void); //!< Helper to transform the PickupCarrier to a Pawn, and throw an error message if the conversion fails.

            Timer durationTimer_; //!< Timer.

            bool invisible_; //!< Helper to remember wether the Pawn is invisible.
            float duration_; //! Duration of invisibility.
    };
}

#endif // _InvisiblePickup_H__
