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
    @brief Definition of PickupCollection.
*/

#ifndef _PickupCollection_H__
#define _PickupCollection_H__

#include "OrxonoxPrereqs.h"

#include <deque>
#include <map>
#include <string>

#include "util/Math.h"
#include "ModifierType.h"

namespace orxonox
{
    /**
        @brief PickupCollection for organising items.
        @author Daniel 'Huty' Haggenmueller
    */
    class _OrxonoxExport PickupCollection
    {
    public:
        PickupCollection();

        bool add(BaseItem* item);       //!< Add an item to the collection.

        bool checkSlot(BaseItem* item); //!< Check if there's a free slot in the collection for an item.

        void clear();                   //!< Empty the collection
        bool contains(BaseItem* item, bool anyOfType = false);                      //!< Check if the collection contains an item.

        void remove(BaseItem* item, bool removeAllOfType = false);                  //!< Remove an item from the collection.

        void useItem();                                                             //!< Use the first usable item.
        void useItem(UsableItem* item);                                             //!< Use a usable item.

        void addAdditiveModifier(ModifierType::Value type, float value);             //!< Add an additive modifier.
        void addMultiplicativeModifier(ModifierType::Value type, float value);       //!< Add a multiplicative modifier.

        float getAdditiveModifier(ModifierType::Value type);                         //!< Get total additive modifier.
        float getMultiplicativeModifier(ModifierType::Value type);                   //!< Get total multiplicative modifier.

        void removeAdditiveModifier(ModifierType::Value type, float value);          //!< Remove an additive modifier.
        void removeMultiplicativeModifier(ModifierType::Value type, float value);    //!< Remove a multiplicative modifier.

        float processModifiers(ModifierType::Value type, float inputValue, bool addBeforeMultiplication = false);        //!< Apply the modifiers to a float.
        Vector3 processModifiers(ModifierType::Value type, Vector3 inputValue, bool addBeforeMultiplication = false);    //!< Apply the modifiers to a Vector3.

        /**
            @brief Get the map of contained items.
            @return The map of items.
        */
        std::multimap<std::string, BaseItem*> getItems() const
            { return this->items_; }

        /**
            @brief Get the owner of the PickupCollection.
            @return Returns the pawn which owns the PickupCollection.
        */
        inline Pawn* getOwner() const
            { return this->owner_; }
        /**
            @brief Set the owner of the PickupCollection.
            @param owner The new Pawn which owns the PickupCollection.
        */
        inline void setOwner(Pawn* owner)
            { this->owner_ = owner; }

        inline UsableItem* getCurrentUsable()
            { return this->currentUsable_; };
        inline void setCurrentUsable(UsableItem* usable)
            { this->currentUsable_ = usable; }

        std::deque<EquipmentItem*> getEquipmentItems();   //!< Get a list of equipment-type items.
        std::deque<PassiveItem*> getPassiveItems();     //!< Get a list of passive items.
        std::deque<UsableItem*> getUsableItems();      //!< Get a list of usable items.
    private:
        Pawn* owner_;           //!< The owner of the PickupCollection.
        UsableItem* currentUsable_;

        bool bBlockRemovals_;   //!< Whether to block direct removals through remove().

        std::multimap<ModifierType::Value, float> additiveModifiers_;        //!< Contains additive modifiers (indexed by ModifierType).
        std::multimap<ModifierType::Value, float> multiplicativeModifiers_;  //!< Contains multiplicative modifiers (indexed by ModifierType).

        std::multimap<std::string, BaseItem*> items_;                       //!< Map of items in the collection (indexed by pickupIdentifier of the items).
    };
}

#endif /* _PickupCollection_H__ */
