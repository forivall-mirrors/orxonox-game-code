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
    @brief Implementation of BaseItem (base-class for items/pickups).
*/

#include "BaseItem.h"

#include "PickupCollection.h"
#include "objects/worldentities/pawns/Pawn.h"

namespace orxonox
{
    /**
        @brief Constructor. Registers the BaseItem.
        @param creator Pointer to the object which created this item.
    */
    BaseItem::BaseItem(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(BaseItem);

        this->setOwner(0);
        this->setPickupIdentifier(this->getName());
    }
    //! Deconstructor.
    BaseItem::~BaseItem()
    {
    }
    /**
        @brief Method to add the item to a pawn.
        @param pawn Pawn to which the item should get added.
        @return Returns whether the pawn's PickupCollection accepted the item.
    */
    bool BaseItem::addTo(Pawn* pawn)
    {
        this->setOwner(pawn);

        COUT(3) << "Adding '" << this->getPickupIdentifier() << "' item." << std::endl;

        return pawn->getPickups().add(this);
    }
    /**
        @brief Removes the item from a pawn.
        @param pawn Pawn from which to remove the item.
        @return Returns whether the pawn's PickupCollection was able to locate and remove the item.
    */
    bool BaseItem::removeFrom(Pawn* pawn)
    {
        this->setOwner(0);

        COUT(3) << "Removing '" << this->getPickupIdentifier() << "' item." << std::endl;

        pawn->getPickups().remove(this, false);

        return true;
    }
}
