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
    @brief Definition of UsableItem.
*/

#ifndef _UsableItem_H__
#define _UsableItem_H__

#include "OrxonoxPrereqs.h"
#include "BaseItem.h"

namespace orxonox
{
    /**
        @brief Base class for all usable items (not usable yet).
        @author Daniel 'Huty' Haggenmueller
    */
    class _OrxonoxExport UsableItem : public BaseItem
    {
    public:
        UsableItem(BaseObject* creator);
        virtual ~UsableItem() { }

        /**
            @brief Method invoked when the item is being used.
            @param pawn Pawn which is using the item.
        */
        virtual void used(Pawn* pawn) { }
    };
}

#endif /* _UsableItem_H__ */
