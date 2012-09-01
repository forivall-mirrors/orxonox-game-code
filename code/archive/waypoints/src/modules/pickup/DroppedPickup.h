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
 *      Daniel 'Huty' Haggenmueller
 *   Co-authors:
 *      Damian 'Mozork' Frick
 *
 */

/**
    @file DroppedPickup.h
    @brief Definition of the DroppedPickup class.
    @ingroup Pickup
*/

#ifndef _DroppedPickup_H__
#define _DroppedPickup_H__

#include "PickupPrereqs.h"

#include "PickupSpawner.h"

namespace orxonox
{

    /**
    @brief
        Special PickupSpawner that is created when a @ref orxonox::Pickupable "Pickupable" is dropped. It just spawns one pickup, the one that was dropped.

    @author
        Daniel 'Huty' Haggenmueller
    @author
        Damian 'Mozork' Frick

    @ingroup Pickup
    */
    class _PickupExport DroppedPickup : public PickupSpawner
    {
        public:
            DroppedPickup(BaseObject* creator);
            DroppedPickup(BaseObject* creator, Pickupable* pickup, PickupCarrier* carrier, float triggerDistance = 10.0); //!< Constructor.
            virtual ~DroppedPickup();

        protected:
            virtual Pickupable* getPickup(void); //!< Creates the Pickupable that is going to get picked up.

        private:
            static const unsigned int DEFAULT_BLOCKED_TIME = 10; //!< The default time a PickupCarrier is blocked from picking up the pickupable again, after it has dropped it.

    };
}

#endif /* _DroppedPickup_H__ */
