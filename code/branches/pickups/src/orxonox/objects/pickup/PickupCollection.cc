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
    @brief Implementation of PickupCollection.
*/

#include "PickupCollection.h"

#include "BaseItem.h"
#include "EquipmentItem.h"
#include "PassiveItem.h"
#include "UsableItem.h"

#include "objects/worldentities/pawns/Pawn.h"

namespace orxonox
{
    //! Constructor
    PickupCollection::PickupCollection()
    {
        this->bBlockRemovals_ = false;
    }

    /**
        @brief
            Add an item to the collection.
            
            Only adds the item if there's a free slot for it.

        @param item Item to add to the collection.
        @return Returns whether the item has been added to the collection.
    */
    bool PickupCollection::add(BaseItem* item)
    {
        if (this->checkSlot(item))
        {
            this->items_.insert( std::pair<std::string, BaseItem*> (item->getPickupIdentifier(), item) );
            return true;
        }
        else
            return false;
    }
    /**
        @brief
            Check if there's a free slot for an item.

            Compares the amount of the item-type in the collection
            against the maximal amount of the item that can be carried.

        @param item Item to check for a slot.
        @return Returns if there's a free slot for the item.
    */
    bool PickupCollection::checkSlot(BaseItem* item)
    {
        return (this->items_.count(item->getPickupIdentifier()) < item->getMaxCarryAmount());
    }
    /**
        @brief
            Empty the collection.

            Calls dropped() on all the items in the collection,
            then clears the collection.
    */
    void PickupCollection::clear()
    {
        this->bBlockRemovals_ = true;
        for (std::multimap<std::string, BaseItem*>::iterator it = this->items_.begin(); it != this->items_.end(); it++)
        {
            if((*it).second && (*it).second->getOwner())
                (*it).second->dropped((*it).second->getOwner());
        }
        this->items_.clear();
        this->bBlockRemovals_ = false;
    }
    /**
        @brief Check if an item/type of item is in the collection.
        @param item Item to check.
        @param anyOfType If it should look for any item of the item's type (default: false).
        @return Whether the collection contains the item/type of item.
    */
    bool PickupCollection::contains(BaseItem* item, bool anyOfType)
    {
        if (anyOfType)
        {
            return (this->items_.count(item->getPickupIdentifier()) > 0);
        }
        else
        {
            std::multimap<std::string, BaseItem*>::_Pairii bounds = this->items_.equal_range(item->getPickupIdentifier());
            for (std::multimap<std::string, BaseItem*>::iterator it = bounds.first; it != bounds.second && it != this->items_.end(); it++)
            {
                if ((*it).second == item)
                {
                    return true;
                }
            }
            return false;
        }
    }
    //! Uses the first usable item in the collection on the owner.
    void PickupCollection::useItem()
    {
        Identifier* ident = Class(UsableItem);
        for (std::multimap<std::string, BaseItem*>::iterator it = this->items_.begin(); it != this->items_.end(); it++)
        {
            if ((*it).second->isA(ident))
            {
                UsableItem* asUsable = dynamic_cast<UsableItem*>((*it).second);
                asUsable->used(this->owner_);
                return;
            }
        }
    }
    /**
        @brief Uses a usable item on the owner of the collection.
        @param item Item to use.
    */
    void PickupCollection::useItem(UsableItem* item)
    {
        if (item && this->owner_)
            item->used(this->owner_);
    }
    /**
        @brief Remove an item/all of a type from the collection.
        @param item Item to remove.
        @param removeAllOfType Whether to remove all the items with the item's type (default: false).
    */
    void PickupCollection::remove(BaseItem* item, bool removeAllOfType)
    {
        if (!item || !this->contains(item, removeAllOfType) || this->bBlockRemovals_)
            return;

        if (removeAllOfType)
        {
            std::multimap<std::string, BaseItem*>::iterator it;
            while ((it = this->items_.find(item->getPickupIdentifier())) != this->items_.end())
            {
                this->items_.erase(it);
            }
        }
        else
        {
            std::multimap<std::string, BaseItem*>::_Pairii bounds = this->items_.equal_range(item->getPickupIdentifier());
            for (std::multimap<std::string, BaseItem*>::iterator it = bounds.first; it != bounds.second && it != this->items_.end(); it++)
            {
                if ((*it).second == item)
                {
                    this->items_.erase(it);
                    return;
                }
            }
        }
    }
    /**
        @brief Add an additive modifier.
        @param type ModifierType to add.
        @param value Value for the modifier.
    */
    void PickupCollection::addAdditiveModifier(ModifierType::Enum type, float value)
    {
        this->additiveModifiers_.insert( std::pair<ModifierType::Enum, float>(type, value) );
    }
    /**
        @brief Get the total amount of an additive modifier.
        @param type Type for which to get the total.
        @return Returns the sum of the additive modifiers of the type.
    */
    float PickupCollection::getAdditiveModifier(ModifierType::Enum type)
    {
        float v = 0.0f;

        std::multimap<ModifierType::Enum, float>::_Pairii range = this->additiveModifiers_.equal_range(type);

        for (std::multimap<ModifierType::Enum, float>::iterator it = range.first; it != range.second && it != this->additiveModifiers_.end(); it++)
        {
            v += (*it).second;
        }

        return v;
    }
    /**
        @brief Remove an additive modifier.
        @param type Type of modifier.
        @param value Value which is to be removed.
    */
    void PickupCollection::removeAdditiveModifier(ModifierType::Enum type, float value)
    {
        std::multimap<ModifierType::Enum, float>::_Pairii range = this->additiveModifiers_.equal_range(type);
        for (std::multimap<ModifierType::Enum, float>::iterator it = range.first; it != range.second && it != this->additiveModifiers_.end(); it++)
        {
            if ((*it).second == value)
            {
                this->additiveModifiers_.erase(it);
                return;
            }
        }
    }
    /**
        @brief Add a multiplicative modifier.
        @param type ModifierType to add.
        @param value Value for the modifier.
    */
    void PickupCollection::addMultiplicativeModifier(ModifierType::Enum type, float value)
    {
        this->multiplicativeModifiers_.insert( std::pair<ModifierType::Enum, float>(type, value) );
    }
    /**
        @brief Get the total amount of a multiplicative modifier.
        @param type Type for which to get the total.
        @return Returns the product of the multiplicative modifiers of the type.
    */
    float PickupCollection::getMultiplicativeModifier(ModifierType::Enum type)
    {
        float v = 1.0f;

        std::multimap<ModifierType::Enum, float>::_Pairii range = this->multiplicativeModifiers_.equal_range(type);
        for (std::multimap<ModifierType::Enum, float>::iterator it = range.first; it != range.second && it != this->multiplicativeModifiers_.end(); it++)
        {
            v *= (*it).second;
        }

        return v;
    }
    /**
        @brief Remove a multiplicative modifier.
        @param type Type of modifier.
        @param value Value which is to be removed.
    */
    void PickupCollection::removeMultiplicativeModifier(ModifierType::Enum type, float value)
    {
        std::multimap<ModifierType::Enum, float>::_Pairii range = this->multiplicativeModifiers_.equal_range(type);
        for (std::multimap<ModifierType::Enum, float>::iterator it = range.first; it != range.second && it != this->multiplicativeModifiers_.end(); it++)
        {
            if ((*it).second == value)
            {
                this->multiplicativeModifiers_.erase(it);
                return;
            }
        }
    }
    /**
        @brief Applies modifiers to a float.
        @param type Type of modifier tp apply.
        @param inputValue Value which is to be processed.
        @param addBeforeMultiplication Whether to apply the additive modifier before the multiplicative one (default: false).
        @return Returns the value after being processed.
    */
    float PickupCollection::processModifiers(ModifierType::Enum type, float inputValue, bool addBeforeMultiplication)
    {
        float outputValue = inputValue;

        if (addBeforeMultiplication)
            outputValue += this->getAdditiveModifier(type);

        outputValue *= this->getMultiplicativeModifier(type);

        if (!addBeforeMultiplication)
            outputValue += this->getAdditiveModifier(type);

        return outputValue;
    }
    /**
        @brief Applies modifiers to a Vector3.
        @param type Type of modifier tp apply.
        @param inputValue Value which is to be processed.
        @param addBeforeMultiplication Whether to apply the additive modifier before the multiplicative one (default: false).
        @return Returns the value after being processed.
    */
    Vector3 PickupCollection::processModifiers(ModifierType::Enum type, Vector3 inputValue, bool addBeforeMultiplication)
    {
        Vector3 outputValue = inputValue;

        if (addBeforeMultiplication)
            outputValue += Vector3(this->getAdditiveModifier(type));

        outputValue *= this->getMultiplicativeModifier(type);

        if (!addBeforeMultiplication)
            outputValue += Vector3(this->getAdditiveModifier(type));

        return outputValue;
    }
    /**
        @brief Get a list of equipment-type items.
        @return Returns a list of all the equipment-type items in the collection.
    */
    std::set<BaseItem*> PickupCollection::getEquipmentItems()
    {
        std::set<BaseItem*> ret;
        Identifier* ident = Class(EquipmentItem);

        for (std::multimap<std::string, BaseItem*>::iterator it = this->items_.begin(); it != this->items_.end(); it++)
        {
            if ((*it).second->isA(ident))
                ret.insert((*it).second);
        }

        return ret;
    }
    /**
        @brief Get a list of passive items.
        @return Returns a list of all the passive items in the collection.
    */
    std::set<BaseItem*> PickupCollection::getPassiveItems()
    {
        std::set<BaseItem*> ret;
        Identifier* ident = Class(PassiveItem);

        for (std::multimap<std::string, BaseItem*>::iterator it = this->items_.begin(); it != this->items_.end(); it++)
        {
            if ((*it).second->isA(ident))
                ret.insert((*it).second);
        }

        return ret;
    }
    /**
        @brief Get a list of usable items.
        @return Returns a list of all the usable items in the collection.
    */
    std::set<BaseItem*> PickupCollection::getUsableItems()
    {
        std::set<BaseItem*> ret;
        Identifier* ident = Class(UsableItem);

        for (std::multimap<std::string, BaseItem*>::iterator it = this->items_.begin(); it != this->items_.end(); it++)
        {
            if ((*it).second->isA(ident))
                ret.insert((*it).second);
        }

        return ret;
    }
}
