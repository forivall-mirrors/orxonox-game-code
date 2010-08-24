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
*/

#ifndef _InvisiblePickup_H__
#define _InvisiblePickup_H__

#include "pickup/PickupPrereqs.h"

#include <string>

#include <worldentities/pawns/Pawn.h>
#include "worldentities/StaticEntity.h"
#include "pickup/Pickup.h"

namespace orxonox {

    /**
    @brief
        A pickup that makes the Pawn invisible.
        There are 2 parameters that can be chosen:
        1) The activation type: It can be chosen to be either 'immediate' or 'onUse'. The activation type essentially (as indicated by the name) defines when the Pawn will be invisible, either immediately after being picked up or only after the player uses it.
        2) The duration type: It can be chosen how long the Pawn will be invisibel.
    @author
        Benedict Simlinger
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
            inline bool getInvisibility(bool)
                { return this->invisible_; }
            inline float getDuration()
                { return this->duration_; }

        protected:
            bool setInvisible(bool invisibility); //!< Set the Pawn to be invisible or visible again.
            void setDuration(float duration);
            void initializeIdentifier(void);
            void pickupTimerCallback(void); //!< Function that gets called when the timer ends.

        private:
            void initialize(void); //!< Initializes the member variables.
            Pawn* carrierToPawnHelper(void); //!< Helper to transform the PickupCarrier to a Pawn, and throw an error message if the conversion fails.

            Timer durationTimer_; //!< Timer.

            bool invisible_; //!< Helper to remember wether the Pawn is invisible.
            float duration_; //! Duration of invisibility.
    };
}

#endif // _InvisiblePickup_H__
