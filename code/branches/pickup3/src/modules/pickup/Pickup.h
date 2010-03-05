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

#ifndef _Pickup_H__
#define Pickup_H__

#include "pickup/PickupPrereqs.h"

#include "interfaces/Pickupable.h"
#include "core/BaseObject.h"
#include "core/XMLPort.h"

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
    
    class _PickupExport Pickup : public Pickupable, public BaseObject
    {
        
        public:
            
            Pickup(BaseObject* creator);
            virtual ~Pickup();
            
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
            
            inline bool isImmediate(void)
                { return this->getActivationTypeDirect() == pickupActivationType::immediate; }
            inline bool isOnUse(void)
                { return this->getActivationTypeDirect() == pickupActivationType::onUse; }
            inline bool isOnce(void)
                { return this->getDurationTypeDirect() == pickupDurationType::once; }
            inline bool isContinuous(void)
                { return this->getDurationTypeDirect() == pickupDurationType::continuous; }
                        
            virtual void clone(OrxonoxClass* item);
            
            virtual void changedCarrier(void);
                
        protected:
            void initializeIdentifier(void);
            
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
            pickupActivationType::Value activationType_; //!< The activation type of the Pickup.
            pickupDurationType::Value durationType_; //!< The duration type of the pickup.
            
            static const std::string activationTypeImmediate_s;
            static const std::string activationTypeOnUse_s;
            static const std::string durationTypeOnce_s;
            static const std::string durationTypeContinuous_s;
            static const std::string blankString_s; //TODO: Maybe already implemented somewhere?
        
    };
    
}
#endif // _Pickup_H__
