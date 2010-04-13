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
*/

#ifndef _Pickup_H__
#define _Pickup_H__

#include "pickup/PickupPrereqs.h"

#include "core/BaseObject.h"
#include "core/XMLPort.h"

#include "interfaces/Pickupable.h"

#include "tools/Timer.h"

namespace orxonox
{

    //! Enum for the activation type.
    namespace pickupActivationType
    {
        enum Value
        {
            immediate,
            onUse,
        };
    }

    //! Enum for the duration tyoe.
    namespace pickupDurationType
    {
        enum Value
        {
            once,
            continuous,
        };
    }

    /**
    @brief
        Pickup class. Offers base functionality for a wide range of pickups.
        Pickups ingeriting from this class cann choose an activation type and a duration type.
    @author
        Damian 'Mozork' Frick
    */
    class _PickupExport Pickup : public Pickupable, public BaseObject
    {

        protected:
            Pickup(BaseObject* creator); //!< Constructor.

        public:
            virtual ~Pickup(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            /**
            @brief Get the activation type of the pickup.
            @return Returns the activation type of the pickup.
            */
            inline pickupActivationType::Value getActivationTypeDirect(void)
                { return this->activationType_; }
            /**
            @brief Get the duration type of the pickup.
            @return Returns the duration type of the pickup.
            */
            inline pickupDurationType::Value getDurationTypeDirect(void)
                { return this->durationType_; }

            const std::string& getActivationType(void); //!< Get the activation type of the pickup.
            const std::string& getDurationType(void); //!< Get the duration type of the pickup.

            /**
            @brief Get whether the activation type is 'immediate'.
            @return Returns true if the activation type is 'immediate'.
            */
            inline bool isImmediate(void)
                { return this->getActivationTypeDirect() == pickupActivationType::immediate; }
            /**
            @brief Get whether the activation type is 'onUse'.
            @return Returns true if the activation type is 'onUse'.
            */
            inline bool isOnUse(void)
                { return this->getActivationTypeDirect() == pickupActivationType::onUse; }
            /**
            @brief Get whether the duration type is 'once'.
            @return Returns true if the duration type is 'once'.
            */
            inline bool isOnce(void)
                { return this->getDurationTypeDirect() == pickupDurationType::once; }
            /**
            @brief Get whether the duration type is 'continuous'.
            @return Returns true if the duration type is 'continuous'.
            */
            inline bool isContinuous(void)
                { return this->getDurationTypeDirect() == pickupDurationType::continuous; }

            virtual void changedPickedUp(void); //!< Should be called when the pickup has transited from picked up to dropped or the other way around.

            virtual void clone(OrxonoxClass*& item); //!< Creates a duplicate of the Pickup.

        protected:
            void initializeIdentifier(void);

            virtual bool createSpawner(void); //!< Facilitates the creation of a PickupSpawner upon dropping of the Pickupable.

            bool startPickupTimer(float durationTime);

            virtual void pickupTimerCallback(void) {}

            /**
            @brief Set the activation type of the pickup.
            @param type The activation type of the pickup.
            */
            inline void setActivationTypeDirect(pickupActivationType::Value type)
                { this->activationType_ = type; }
            /**
            @brief Set the duration type of the pickup.
            @param type The duration type of the pickup.
            */
            inline void setDurationTypeDirect(pickupDurationType::Value type)
                { this->durationType_ = type; }

            void setActivationType(const std::string& type); //!< Set the activation type of the pickup.
            void setDurationType(const std::string& type); //!< Set the duration type of the pickup

        private:
            void initialize(void); //!< Initializes the member variables.

            pickupActivationType::Value activationType_; //!< The activation type of the Pickup.
            pickupDurationType::Value durationType_; //!< The duration type of the pickup.

            static const std::string activationTypeImmediate_s;
            static const std::string activationTypeOnUse_s;
            static const std::string durationTypeOnce_s;
            static const std::string durationTypeContinuous_s;

            float durationTime_;
            Timer durationTimer_;
    };

}
#endif // _Pickup_H__
