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

#include "OrxonoxPrereqs.h"

#include "PickupSpawner.h"

namespace orxonox
{
    class _OrxonoxExport DroppedItem : public PickupSpawner
    {
        public:
            DroppedItem(BaseObject* creator);
            DroppedItem(BaseObject* creator, BaseItem* item, float triggerDistance, float respawnTime, int maxSpawnedItems);
            virtual ~DroppedItem();

            static DroppedItem* createDefaultDrop(BaseItem* item, const Vector3& position, const ColourValue& flareColour = ColourValue(0.5f, 1.0f, 0.3f), float timeToLive = 0);
            static DroppedItem* createDefaultDrop(BaseItem* item, Pawn* pawn, const ColourValue& flareColour = ColourValue(0.5f, 1.0f, 0.3f), float timeToLive = 0);

        protected:
            virtual BaseItem* getItem(void);

        private:

            BaseItem* item_; //!< The dropped item.

    };
}

#endif /* _DroppedItem_H__ */
