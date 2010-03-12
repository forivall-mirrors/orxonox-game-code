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

#ifndef _MetaPickup_H__
#define _MetaPickup_H__

#include "pickup/PickupPrereqs.h"

#include "pickup/Pickup.h"

namespace orxonox {

    namespace pickupMetaType
    {
        enum Value
        {
            none,
            use,
            drop
        };
    }
    
    class _PickupExport MetaPickup : public Pickup
    {
        friend class PickupCarrier;
        
        public:
            MetaPickup(BaseObject* creator);
            virtual ~MetaPickup();
            
            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode); //!< Method for creating a HealthPickup object through XML.
            
            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.
            virtual void clone(OrxonoxClass*& item); //!< Creates a duplicate of the input OrxonoxClass.
            
            inline pickupMetaType::Value getMetaTypeDirect(void)
                { return this->metaType_; }
            const std::string& getMetaType(void);
            
        protected:
            void initializeIdentifier(void); //!< Initializes the PickupIdentifier of this pickup.
            
            inline void setMetaTypeDirect(pickupMetaType::Value type)
                { this->metaType_ =  type; }
            void setMetaType(const std::string& type);
            
        private:
            void initialize(void); //!< Initializes the member variables.
            
            pickupMetaType::Value metaType_;
            static const std::string metaTypeNone_s;
            static const std::string metaTypeUse_s;
            static const std::string metaTypeDrop_s;
            
        
    };

}

#endif // _TestPickup_H__
