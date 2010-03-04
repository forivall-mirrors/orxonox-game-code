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
 *      ...
 *
 */

/**
    @file
    @brief Definition of DroppedItem
*/

#ifndef _DroppedItem_H__
#define _DroppedItem_H__

#include "PickupPrereqs.h"

#include "PickupSpawner.h"

namespace orxonox
{
    class _OrxonoxExport DroppedItem : public PickupSpawner
    {
        public:
            DroppedItem(BaseObject* creator);
            DroppedItem(BaseObject* creator, Pickupable* item, const Vector3& position, float triggerDistance);
            virtual ~DroppedItem();

        protected:
            virtual Pickupable* getPickup(void);
            
        private:
            void initialize(void);
            void createDrop(const Vector3& position);
            
            bool gotPickedUp_;

    };
}

#endif /* _DroppedItem_H__ */
