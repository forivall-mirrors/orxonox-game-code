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
    @brief Implementation of ModifierPickup (temporary(?) pickup for testing).
*/

#include "ModifierPickup.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateFactory(ModifierPickup);

    /**
        @brief Constructor. Registers the ModifierPickup.
        @param creator Pointer to the object which created this item.
    */
    ModifierPickup::ModifierPickup(BaseObject* creator) : PassiveItem(creator)
    {
        RegisterObject(ModifierPickup);

        this->duration_ = 0.0f;
    }
    //! Deconstructor.
    ModifierPickup::~ModifierPickup()
    {
    }
    /**
        @brief Method for loading information from a level file.
        @param element XMLElement from which to read the data.
        @param mode XMLPort mode to use.
    */
    void ModifierPickup::XMLPort(Element& element, XMLPort::Mode mode)
    {
        SUPER(ModifierPickup, XMLPort, element, mode);

        XMLPortParam(ModifierPickup, "duration", setDuration, getDuration, element, mode);

        XMLPortParamTemplate(ModifierPickup, "damageAdd", setAdditiveDamage, getAdditiveDamage, element, mode, float);
        XMLPortParamTemplate(ModifierPickup, "damageMulti", setMultiplicativeDamage, getMultiplicativeDamage, element, mode, float);

        XMLPortParamTemplate(ModifierPickup, "accelerationAdd", setAdditiveAcceleration, getAdditiveAcceleration, element, mode, float);
        XMLPortParamTemplate(ModifierPickup, "accelerationMulti", setMultiplicativeAcceleration, getMultiplicativeAcceleration, element, mode, float);
    }
    /**
        @brief
            Invoked when a pawn picks up the pickup.

            Adds the modifiers to the pawn and sets a timer (if effect is limited)
            if the pickup could be added to the pawn's PickupCollection.

        @param pawn Pawn which picked up the pickup.
        @return Returns whether the pickup was able to be added to the pawn.
    */
    bool ModifierPickup::pickedUp(Pawn* pawn)
    {
        if (this->addTo(pawn))
        {
            std::map<ModifierType::Value, float>::iterator it;

            for (it = this->additiveModifiers_.begin(); it != this->additiveModifiers_.end(); it++)
            {
                pawn->getPickups().addAdditiveModifier(it->first, it->second);
            }

            for (it = this->multiplicativeModifiers_.begin(); it != this->multiplicativeModifiers_.end(); it++)
            {
                pawn->getPickups().addMultiplicativeModifier(it->first, it->second);
            }

            if (this->duration_ > 0.0f)
            {
                Executor* executor = createExecutor(createFunctor(&ModifierPickup::timerCallback, this));
                executor->setDefaultValues(pawn);
                this->timer_.setTimer(this->duration_, false, executor);
            }

            return true;
        }
        return false;
    }
    /**
        @brief
            Invoked when a pawn drops the pickup.

            Removes the modifiers from the pawn if the pickup
            was successfully removed from it's PickupCollection.

        @param pawn Pawn which dropped the pickup.
        @return Returns whether the pickup could be removed.
    */
    bool ModifierPickup::dropped(Pawn* pawn)
    {
        if (this->removeFrom(pawn))
        {
            std::map<ModifierType::Value, float>::iterator it;

            for (it = this->additiveModifiers_.begin(); it != this->additiveModifiers_.end(); it++)
            {
                pawn->getPickups().removeAdditiveModifier(it->first, it->second);
            }

            for (it = this->multiplicativeModifiers_.begin(); it != this->multiplicativeModifiers_.end(); it++)
            {
                pawn->getPickups().removeMultiplicativeModifier(it->first, it->second);
            }

            if (this->timer_.getRemainingTime() > 0.0f)
                this->timer_.stopTimer();

            this->destroy();

            return true;
        }
        return false;
    }
    /**
        @brief Invoked when the timer finished, calls dropped().
    */
    void ModifierPickup::timerCallback(Pawn* pawn)
    {
        if (!this->dropped(pawn))
            COUT(2) << "Failed to remove modifier pickup after the timer ran out!" << std::endl;
    }
    /**
        @brief Gets the additive modifier of a given type.
        @param type ModifierType for which to return the modifier.
        @return Returns the additive modifier for type (or 0 if not exists).
    */
    float ModifierPickup::getAdditiveModifier(ModifierType::Value type) const
    {
        std::map<ModifierType::Value, float>::const_iterator it = this->additiveModifiers_.find(type);
        if (it != this->additiveModifiers_.end())
            return it->second;
        else
            return 0.0f;
    }
    /**
        @brief Gets the multiplicative modifier of a given type.
        @param type ModifierType for which to return the modifier.
        @return Returns the multiplicative modifier for type (or 1 if not exists).
    */
    float ModifierPickup::getMultiplicativeModifier(ModifierType::Value type) const
    {
        std::map<ModifierType::Value, float>::const_iterator it = this->multiplicativeModifiers_.find(type);
        if (it != this->multiplicativeModifiers_.end())
            return it->second;
        else
            return 1.0f;
    }
    /**
        @brief Gets the additive modifier of a given type.
        @param type ModifierType for which to return the modifier.
        @param value The new additive modifier for type.
    */
    void ModifierPickup::setAdditiveModifier(ModifierType::Value type, float value)
    {
        if (this->additiveModifiers_.find(type) == this->additiveModifiers_.end())
            this->additiveModifiers_.insert( std::pair<ModifierType::Value, float>(type, value) );
        else
            this->additiveModifiers_[type] = value;
    }
    /**
        @brief Gets the multiplicative modifier of a given type.
        @param type ModifierType for which to return the modifier.
        @param value The new multiplicative modifier for type.
    */
    void ModifierPickup::setMultiplicativeModifier(ModifierType::Value type, float value)
    {
        if (this->multiplicativeModifiers_.find(type) == this->multiplicativeModifiers_.end())
            this->multiplicativeModifiers_.insert( std::pair<ModifierType::Value, float>(type, value) );
        else
            this->multiplicativeModifiers_[type] = value;
    }
}
