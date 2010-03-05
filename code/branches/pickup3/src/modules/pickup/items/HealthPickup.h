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

#ifndef _HealthPickup_H__
#define _HealthPickup_H__

#include "pickup/PickupPrereqs.h"

#include "pickup/Pickup.h"
#include "tools/interfaces/Tickable.h"
#include "worldentities/StaticEntity.h"

#include <string>

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
    
    class _PickupExport HealthPickup : public Pickup, public Tickable
    {
        public:
        
            HealthPickup(BaseObject* creator);
            virtual ~HealthPickup();
            
            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode);
            virtual void tick(float dt);
            
            virtual void clone(OrxonoxClass* item);
                        
            virtual void changedUsed(void);
            
        protected:
            void initializeIdentifier(void);

            void setHealth(float health);
            void setHealthSpeed(float speed);
            void setHealthType(std::string type);
            inline void setHealthTypeDirect(pickupHealthType::Value type)
                { this->healthType_ = type; }
            
            inline float getHealth(void)
                { return this->health_; }
            inline float getHealthSpeed(void)
                { return this->healthSpeed_; }
            const std::string& getHealthType(void);
            inline pickupHealthType::Value getHealthTypeDirect(void)
                { return this->healthType_; }
        
        private:
            void initialize(void);
            
            float health_;
            float healthSpeed_;
            pickupHealthType::Value healthType_;
            
            static const std::string healthTypeLimited_s;
            static const std::string healthTypeTemporary_s;
            static const std::string healthTypePermanent_s;
            static const std::string blankString_s; //TODO: Maybe already implemented somewhere?
        
    };
}

#endif // _HealthPickup_H__
