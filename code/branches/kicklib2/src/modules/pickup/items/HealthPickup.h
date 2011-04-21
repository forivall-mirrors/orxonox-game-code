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

#include "pickup/Pickup.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox {

    /**
    @brief
        Enum for the type of the @ref orxonox::HealthPickup "HealthPickup".

    @ingroup PickupItems
    */
    namespace pickupHealthType
    {
        enum Value
        {
            limited, //!< Means that the @ref orxonox::HealthPickup "HealthPickup" only increases the users health to its maximum health.
            temporary, //!< Means that the @ref orxonox::HealthPickup "HealthPickup" temporarily increases the users health even above its maximum health, but only as long as it is in use.
            permanent //!< Means that the @ref orxonox::HealthPickup "HealthPickup" increases the users health even above its maximum health and increases the maximum health permanently such that it matches the new health.
        };
    }

    /**
    @brief
        The HealthPickup is a Pickupable that can do (dependent upon the parameters) lots of different things to the health of a Pawn.
        There are 5 parameters that can be chosen:
        - The @b activationType It can be chosen to be either <em>immediate</em> or <em>onUse</em>. The activation type essentially (as indicated by the name) defines when the health is transferred, either immediately after being picked up or only after the player uses it. The default is <em>immediate</em>.
        - The @b durationType It can be chosen to be either <em>once</em> or <em>continuous</em>. For <em>once</em> the specified health is transferred once to the Pawn, for <em>continuous</em> the set health is transferred over a span of time at a rate defined by the health rate parameter. The default is <em>once</em>.
        - The @b health The amount of health that (in a way dependent on the other parameters) is transferred to the Pawn. Default is 0.
        - The @b healthType The health type can be chosen to be <em>limited</em>, <em>temporary</em> or <em>permanent</em>. <em>limited</em> means that the health is increased only to the maximum health of the Pawn. 'temporary' means that the maximum health is temporarily elevated but will be set back as soon as the pickup is no longer in use. <em>permanent</em> means that the maximum health of the Pawn is increased such that the health provided by the pickup will fit in and the maximum health stays that way. The default is <em>limited</em>.
        - The @b healthRate If set to continuous, the health that is transferred per second is the <em>healthRate</em>. There is health transferred until the health set by <em>health</em> is depleted. The default is 0.

        An example of a XML implementation of a HealthPickup would be:
        @code
        <HealthPickup
            health = 50
            healthType = "limited"
            activationType = "immediate"
            durationType = "once"
        />
        @endcode

    @author
        Damian 'Mozork' Frick

    @ingroup PickupItems
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
            @brief Get the health that is transferred to the Pawn upon usage of this pickup.
            @return Returns the health.
            */
            inline float getHealth(void) const
                { return this->health_; }
            /**
            @brief Get the rate at which the health is transferred to the Pawn, if this pickup has duration type 'continuous'.
            @return Returns the rate.
            */
            inline float getHealthRate(void) const
                { return this->healthRate_; }

            /**
            @brief Get the type of HealthPickup, this pickup is.
            @return Returns the health type as an enum.
            */
            inline pickupHealthType::Value getHealthTypeDirect(void) const
                { return this->healthType_; }
            const std::string& getHealthType(void) const; //!< Get the health type of this pickup.

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
