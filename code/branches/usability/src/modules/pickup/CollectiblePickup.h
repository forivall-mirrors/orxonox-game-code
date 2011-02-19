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
    @file CollectiblePickup.h
    @brief Definition of the CollectiblePickup class.
    @ingroup Pickup
*/

#ifndef _CollectiblePickup_H__
#define _CollectiblePickup_H__

#include "pickup/PickupPrereqs.h"

#include "interfaces/Pickupable.h"

namespace orxonox {

    /**
    @brief
        The CollectiblePickup class encompasses all @ref orxonox::Pickupable "Pickupables" that can be added to a @ref orxonox::PickupCollection "PickupCollection" and thus be part of such.

        All you need to do to make your @ref orxonox::Pickupable "Pickupable" a CollectiblePickup is to, in some way, inherit from it. (The @ref orxonox::Pickup "Pickup" class, for example, is already a CollectiblePickup).

    @author
        Damian 'Mozork' Frick

    @ingroup Pickup
    */
    class _PickupExport CollectiblePickup : public Pickupable
    {

        public:
            CollectiblePickup(); //!< Constructor.
            virtual ~CollectiblePickup(); //! Destructor.

            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.
            virtual void changedPickedUp(void); //!< Is called when the pickup has transited from picked up to dropped or the other way around.

            /**
            @brief Check whether the given CollectiblePickup is par of a PickupCollection.
            @return Returns true if the CollectiblePickup is part of a PickupCollection.
            */
            bool isInCollection(void) const
                { return this->isInCollection_; }

            bool addToCollection(PickupCollection* collection); //!< Adds this CollectiblePickup to the input PickupCollection.
            bool removeFromCollection(void); //!< Removes this CollectiblePickup from its PickupCollection.

            void carrierDestroyed(void); //!< Is called by the PickupCarrier when it is being destroyed.

        protected:
            virtual void preDestroy(void); //!< Is called by OrxonoxClass::destroy() before the object is actually destroyed.
            virtual void destroyPickup(void); //!< Destroys a Pickupable.

        private:
            bool isInCollection_; //!< True if the CollectiblePickup is in a PickupCollection.
            PickupCollection* collection_; //!< A pointer to the PickupCollection this CollectiblePickup is in.

    };
}

#endif // _CollectiblePickup_H__
