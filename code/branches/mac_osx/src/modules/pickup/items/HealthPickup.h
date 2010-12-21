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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

/**
    @file HealthPickup.h
    @brief Declaration of the HealthPickup class.
    @ingroup PickupItems
*/

#ifndef _HealthPickup_H__
#define _HealthPickup_H__

#include "pickup/PickupPrereqs.h"

#include <string>
#include <worldentities/pawns/Pawn.h>
#include "worldentities/StaticEntity.h"

#include "pickup/Pickup.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox {

    //! Enum for the type of the HealthPickup
    namespace pickupHealthType
    {
        enum Value
        {
            limited,
            temporary,
            permanent
        };
    }

    /**
    @brief
        A pickup that can do (dependent upon the parameters) lots of different things to the health of a Pawn.
        There are 4 parameters that can be chosen:
        - The @b health The amount of health that (in a way dependent on the other parameters) is transfered to the Pawn.
        - The @b activation @b type It can be chosen to be either 'immediate' or 'onUse'. The activation type essentially (as indicated by the name) defines when the health is transfered, either immediately after being picked up or only after the player uses it.
        - The @b duration @b type It can be chosen to be either 'once' or 'continuous'. For 'once' the specified health is transfered once to the Pawn, for 'continuous' the set health is transfered over a span of time at a rate defined by the health rate parameter.
        - The @b health @b type The health type can be chosen to be 'limited', 'temporary' or 'permanent'. 'limited' means that the health is increased only to the maximum health of the Pawn. 'temporary' means that the maximum health is temporarily elevated but will be set back as soon as the pickup is no longer in use. 'permanent' means that the maximum health of the Pawn is increased such that the health provided by the pickup will fit in and the maximum health stays that way.
    @author
        Damian 'Mozork' Frick
    */
    class _PickupExport HealthPickup : public Pickup, public Tickable
    {
        public:

            HealthPickup(BaseObject* creator); //!< Constructor.
            virtual ~HealthPickup(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode); //!< Method for creating a HealthPickup object through XML.
            virtual void tick(float dt); //!< Is called every tick.

            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.
            virtual void clone(OrxonoxClass*& item); //!< Creates a duplicate of the input OrxonoxClass.

            /**
            @brief Get the health that is transfered to the Pawn upon usage of this pickup.
            @return Returns the health.
            */
            inline float getHealth(void)
                { return this->health_; }
            /**
            @brief Get the rate at which the health is transferred to the Pawn, if this pickup has duration type 'continuous'.
            @return Returns the rate.
            */
            inline float getHealthRate(void)
                { return this->healthRate_; }

            /**
            @brief Get the type of HealthPickup, this pickup is.
            @return Returns the health type as an enum.
            */
            inline pickupHealthType::Value getHealthTypeDirect(void)
                { return this->healthType_; }
            const std::string& getHealthType(void); //!< Get the health type of this pickup.

        protected:
            void initializeIdentifier(void); //!< Initializes the PickupIdentifier of this pickup.

            void setHealth(float health); //!< Sets the health.
            void setHealthRate(float speed); //!< Set the rate at which health is transferred if the pickup is continuous.

            /**
            @brief Set the health type of this pickup.
            @param type The type of this pickup as an enum.
            */
            inline void setHealthTypeDirect(pickupHealthType::Value type)
                { this->healthType_ = type; }
            void setHealthType(std::string type); //!< Set the type of the HealthPickup.

        private:
            void initialize(void); //!< Initializes the member variables.
            Pawn* carrierToPawnHelper(void); //!< Helper to transform the PickupCarrier to a Pawn, and throw an error message if the conversion fails.

            float health_; //!< The health that is transferred to the Pawn.
            float healthRate_; //!< The rate at which the health is transferred.
            float maxHealthSave_; //!< Helper to remember what the actual maxHealth of the Pawn was before we changed it.
            float maxHealthOverwrite_; //!< Helper to remember with which value we overwrote the maxHealh, to detect if someone else changed it as well.
            pickupHealthType::Value healthType_; //!< The type of the HealthPickup.

            //! Strings for the health types.
            static const std::string healthTypeLimited_s;
            static const std::string healthTypeTemporary_s;
            static const std::string healthTypePermanent_s;

    };
}

#endif // _HealthPickup_H__
