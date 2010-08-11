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
    @file MetaPickup.h
    @brief Definition of the MetaPickup class.
*/

#ifndef _MetaPickup_H__
#define _MetaPickup_H__

#include "pickup/PickupPrereqs.h"

#include "pickup/Pickup.h"

namespace orxonox {

    //! The meta type, deciding what the pickup does exactly.
    namespace pickupMetaType
    {
        enum Value
        {
            none,
            use,
            drop,
            destroy,
            destroyCarrier
        };
    }

    /**
    @brief
        The MetaPickup is a pickup that can, depending on the parameter 'metaType', do different things. If the 'metaType' is set to
        1) 'use', all the pickups, the PickupCarrier has, are immediately set to used upon pickup of the MetaPickup.
        2) 'drop', all the pickups, the PickupCarrier has, are immediately dropped upon pickup of the MetaPickup.
        3) 'destroy', all the pickups, the PickupCarrier has, are immediately destroyed upon pickup of the MetaPickup.
        4) 'destroyCarrier', the PickupCarrier is immediately destroyed upon pickup of the MetaPickup.
    @author
        Damian 'Mozork' Frick
    */
    class _PickupExport MetaPickup : public Pickup
    {

        public:
            MetaPickup(BaseObject* creator); //!< Constructor. Registers and initializes the object.
            virtual ~MetaPickup(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode); //!< Method for creating a MetaPickup object through XML.

            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.
            virtual void clone(OrxonoxClass*& item); //!< Creates a duplicate of the input OrxonoxClass.

            /**
            @brief Returns the meta type of the MetaPickup.
            @return Returns an enum with the meta type of the MetaPickup.
            */
            inline pickupMetaType::Value getMetaTypeDirect(void)
                { return this->metaType_; }
            const std::string& getMetaType(void); //!< Get the meta type of this MetaPickup.

        protected:
            void initializeIdentifier(void); //!< Initializes the PickupIdentifier of this pickup.

            /**
            @brief Set the meta type of the MetaPickup.
            @param type The meta type as an enum.
            */
            inline void setMetaTypeDirect(pickupMetaType::Value type)
                { this->metaType_ =  type; }
            void setMetaType(const std::string& type); //!< Set the meta type of this MetaPickup.

        private:
            void initialize(void); //!< Initializes the member variables.

            pickupMetaType::Value metaType_; //!< The meta type of the MetaPickup, determines which actions are taken.

            //! Static strings for the meta types.
            static const std::string metaTypeNone_s;
            static const std::string metaTypeUse_s;
            static const std::string metaTypeDrop_s;
            static const std::string metaTypeDestroy_s;
            static const std::string metaTypeDestroyCarrier_s;


    };

}

#endif // _TestPickup_H__
