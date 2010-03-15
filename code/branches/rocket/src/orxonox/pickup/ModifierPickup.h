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
    @brief Definition of ModifierPickup (temporary(?) pickup for testing).
*/

#ifndef _ModifierPickup_H__
#define _ModifierPickup_H__

#include "OrxonoxPrereqs.h"

#include <climits>
#include <map>

#include "tools/Timer.h"
#include "ModifierType.h"
#include "PassiveItem.h"

namespace orxonox
{
    /**
        @brief Class for a (temporary) modifier effect.
        @author Daniel 'Huty' Haggenmueller
    */
    class _OrxonoxExport ModifierPickup : public PassiveItem
    {
    public:
        ModifierPickup(BaseObject* creator);
        virtual ~ModifierPickup();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);  //!< To create a ModifierPickup through the level file.

        virtual bool pickedUp(Pawn* pawn);                              //!< Override of the BaseItem::pickedUp() method.
        virtual bool dropped(Pawn* pawn);                               //!< Override of the BaseItem::dropped() method

        virtual int getMaxCarryAmount(){ return INT_MAX; }              //!< Allow the player to carry infinite ModPickups

        /**
            @brief Get the duration of this pickup.
            @return Returns how long the effect holds on.
        */
        inline float getDuration() const
            { return this->duration_; }
        /**
            @brief Set the duration of this pickup.
            @param duration How long the effect should hold.
        */
        inline void setDuration(float duration)
            { this->duration_ = duration; }

        /**
            @brief Get the amount of damage this pickup adds.
            @return Returns how much damage this pickup adds.
        */
        inline float getAdditiveDamage() const
            { return this->getAdditiveModifier(ModifierType::Damage); }
        /**
            @brief Get the factor by which this pickup multiplies the damage.
            @return Returns the factor by which to multiply damage.
        */
        inline float getMultiplicativeDamage() const
            { return this->getMultiplicativeModifier(ModifierType::Damage); }

        /**
            @brief Set the amount of damage this pickup adds.
            @param value How much damage this pickup adds.
        */
        inline void setAdditiveDamage(float value)
            { this->setAdditiveModifier(ModifierType::Damage, value); }
        /**
            @brief Set the factor by which this pickup multiplies the damage.
            @param value Factor by which to multiply damage.
        */
        inline void setMultiplicativeDamage(float value)
            { this->setMultiplicativeModifier(ModifierType::Damage, value); }

        /**
            @brief Get the amount of acceleration this pickup adds.
            @return Returns how much acceleration this pickup adds.
        */
        inline float getAdditiveAcceleration() const
            { return this->getAdditiveModifier(ModifierType::Acceleration); }
        /**
            @brief Get the factor by which this pickup multiplies the acceleration.
            @return Returns the factor by which to multiply acceleration.
        */
        inline float getMultiplicativeAcceleration() const
            { return this->getMultiplicativeModifier(ModifierType::Acceleration); }

        /**
            @brief Set the amount of acceleration this pickup adds.
            @param value How much acceleration this pickup adds.
        */
        inline void setAdditiveAcceleration(float value)
            { this->setAdditiveModifier(ModifierType::Acceleration, value); }
        /**
            @brief Set the factor by which this pickup multiplies the acceleration.
            @param value Factor by which to multiply acceleration.
        */
        inline void setMultiplicativeAcceleration(float value)
            { this->setMultiplicativeModifier(ModifierType::Acceleration, value); }

        void timerCallback(Pawn* pawn);     //!< Method called when the timer runs out.

    private:
        float getAdditiveModifier(ModifierType::Value type) const;               //!< Get the additive modifier for a given ModifierType.
        float getMultiplicativeModifier(ModifierType::Value type) const;         //!< Get the multiplicative modifier for a given ModifierType.
        void setAdditiveModifier(ModifierType::Value type, float value);         //!< Set the additive modifier for a given ModifierType.
        void setMultiplicativeModifier(ModifierType::Value type, float value);   //!< Set the multiplicative modifier for a given ModifierType

        std::map<ModifierType::Value, float> additiveModifiers_;                 //!< Map of additive modifiers, indexed by ModifierType.
        std::map<ModifierType::Value, float> multiplicativeModifiers_;           //!< Map of multiplicative modifiers, indexed by ModifierType.

        float duration_;                                                         //!< Duration of this pickup's effect (0 for unlimited).
        Timer timer_;                                                            //!< Timer used if the pickup's effect has a time limit.
    };
}

#endif /* _ModifierPickup_H__ */
