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
    @file Pickup.h
    @brief Declaration of the Pickup class.
    @ingroup Pickup
*/

#ifndef _Pickup_H__
#define _Pickup_H__

#include "pickup/PickupPrereqs.h"

#include "core/BaseObject.h"
#include "core/XMLPort.h"
#include "tools/Timer.h"

#include "CollectiblePickup.h"

namespace orxonox
{

    /**
    @brief
        Enum for the @ref orxonox::Pickup "Pickup" activation type.

    @ingroup Pickup
    */
    namespace pickupActivationType
    {
        enum Value
        {
            immediate, //!< Means that the @ref orxonox::Pickup "Pickup" will be used immediately after pickup.
            onUse, //!< Means that the @ref orxonox::Pickup "Pickup" will be used at a later point trough some external influence.
        };
    }

    /**
    @brief
        Enum for the @ref orxonox::Pickup "Pickup" duration type.

    @ingroup Pickup
    */
    namespace pickupDurationType
    {
        enum Value
        {
            once, //!< Means that the @ref orxonox::Pickup "Pickup" will be used only once at a singular time instant.
            continuous, //!< Means that the @ref orxonox::Pickup "Pickup" will be used over a continuous timespan.
        };
    }

    /**
    @brief
        The Pickup class offers (useful) base functionality for a wide range of pickups.

        Pickups inheriting from this class can choose an activation type and a duration type.
        - The <b>activationType</b> deals with what happens to the Pickup as soon as it is picked up. It can either be set to <em>immediate</em>, which means that the Pickup is activated/used immediately upon being picked up. Or to <em>onUse</em>, which means, that the Pickup will be activated/used if some outside entity (most commonly the player through the PickupInventory) decides to use it. Default is <em>immediate</em>.
        - The <b>durationType</b> deals with whether the Pickup has a continuous effect or whether its effect is focused on a singular instant. It can either be set to <em>once</em>, which means, that the Pickup just has an effect (at a singular instant in time) and is done once that effect has been applied. Or to <em>continuous</em>, which means that the effect of the Pickup unfolds over some timespan. Default is <em>once</em>.

        If it were not an abstract class it could for example be used as follows in XML.
        @code
        <Pickup activationType="onUse" durationType="continuous" />
        @endcode
        In reality you can (naturally) use the parameters <b>activation type</b> and <b>duration type</b> in any pickup inheriting from Pickup, unless the pickup already specifies one (or both) of the parameters.

    @author
        Damian 'Mozork' Frick

    @ingroup Pickup
    */
    class _PickupExport Pickup : public CollectiblePickup, public BaseObject
    {

        public:
            Pickup(BaseObject* creator); //!< Constructor.
            virtual ~Pickup(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            virtual const std::string& getRepresentationName() const
                { return this->representationName_; }

            /**
            @brief Get the activation type of the Pickup.
            @return Returns the activation type of the Pickup.
            */
            inline pickupActivationType::Value getActivationType(void) const
                { return this->activationType_; }
            /**
            @brief Get the duration type of the Pickup.
            @return Returns the duration type of the Pickup.
            */
            inline pickupDurationType::Value getDurationType(void) const
                { return this->durationType_; }

            const std::string& getActivationTypeAsString(void) const; //!< Get the activation type of the Pickup.
            const std::string& getDurationTypeAsString(void) const; //!< Get the duration type of the Pickup.

            /**
            @brief Get whether the activation type is 'immediate'.
            @return Returns true if the activation type is 'immediate'.
            */
            inline bool isImmediate(void) const
                { return this->getActivationType() == pickupActivationType::immediate; }
            /**
            @brief Get whether the activation type is 'onUse'.
            @return Returns true if the activation type is 'onUse'.
            */
            inline bool isOnUse(void) const
                { return this->getActivationType() == pickupActivationType::onUse; }
            /**
            @brief Get whether the duration type is 'once'.
            @return Returns true if the duration type is 'once'.
            */
            inline bool isOnce(void) const
                { return this->getDurationType() == pickupDurationType::once; }
            /**
            @brief Get whether the duration type is 'continuous'.
            @return Returns true if the duration type is 'continuous'.
            */
            inline bool isContinuous(void) const
                { return this->getDurationType() == pickupDurationType::continuous; }

            virtual void changedPickedUp(void); //!< Should be called when the pickup has transited from picked up to dropped or the other way around.
            virtual void clone(OrxonoxClass*& item); //!< Creates a duplicate of the OrxonoxClass.

        protected:
            virtual bool createSpawner(void); //!< Facilitates the creation of a PickupSpawner upon dropping of the Pickupable.

            /**
            @brief Sets the representation name which refers to the name of the PickupRepresentation that is used to represent this pickup.
            */
            inline void setRepresentationName(const std::string& name)
                { this->representationName_ = name; }

            /**
            @brief Set the activation type of the Pickup.
            @param type The activation type of the Pickup.
            */
            inline void setActivationType(pickupActivationType::Value type)
                { this->activationType_ = type; }
            /**
            @brief Set the duration type of the Pickup.
            @param type The duration type of the Pickup.
            */
            inline void setDurationType(pickupDurationType::Value type)
                { this->durationType_ = type; }

            void setActivationTypeAsString(const std::string& type); //!< Set the activation type of the Pickup.
            void setDurationTypeAsString(const std::string& type); //!< Set the duration type of the Pickup.

        private:
            void initialize(void); //!< Initializes the member variables.

            std::string representationName_; //!< The name of the associated PickupRepresentation.
            pickupActivationType::Value activationType_; //!< The activation type of the Pickup.
            pickupDurationType::Value durationType_; //!< The duration type of the Pickup.

            //! Strings for the activation and duration types.
            static const std::string activationTypeImmediate_s;
            static const std::string activationTypeOnUse_s;
            static const std::string durationTypeOnce_s;
            static const std::string durationTypeContinuous_s;
    };

}
#endif // _Pickup_H__
