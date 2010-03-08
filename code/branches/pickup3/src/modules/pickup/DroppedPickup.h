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
    @file
    @brief Definition of the DroppedPickup class.
*/

#ifndef _DroppedPickup_H__
#define _DroppedPickup_H__

#include "PickupPrereqs.h"

#include "PickupSpawner.h"

namespace orxonox
{
    
    /**
    @brief
        Special PickupSpawner that is created whe a Pickupable is dropped. It just spawns one pickup, the one that was dropped.
    @author
        Daniel 'Huty' Haggenmueller
        Damian 'Mozork' Frick
    */
    class _PickupExport DroppedPickup : public PickupSpawner
    {
        public:
            DroppedPickup(BaseObject* creator); //!< Default constructor.
            DroppedPickup(BaseObject* creator, Pickupable* pickup, const Vector3& position, float triggerDistance = 10.0); //!< Constructor.
            virtual ~DroppedPickup(); //!< Destructor.

        protected:
            virtual Pickupable* getPickup(void); //!< Creates the Pickupable that is going to get picked up.
            
        private:
            void initialize(void); //!< Initializes the member variables of the object.
            
            bool gotPickedUp_; //!< Whether the pickup got picked up or not.

    };
}

#endif /* _DroppedPickup_H__ */
