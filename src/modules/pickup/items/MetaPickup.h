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
    @ingroup PickupItems
*/

#ifndef _MetaPickup_H__
#define _MetaPickup_H__

#include "pickup/PickupPrereqs.h"

#include "pickup/Pickup.h"

namespace orxonox {

    /**
    @brief
        The meta type, specifying what the @ref orxonox::MetaPickup "MetaPickup" does exactly.

    @ingroup PickupItems
    */
    namespace pickupMetaType
    {
        enum Value
        {
            none, //!< The @ref orxonox::MetaPickup "MetaPickup" does nothing.
            use, //!< The @ref orxonox::MetaPickup "MetaPickup" uses all the @ref orxonox::PickupCarrier "PickupCarriers'" @ref orxonox::Pickupable "Pickupables".
            drop, //!< The @ref orxonox::MetaPickup "MetaPickup" drops all the @ref orxonox::PickupCarrier "PickupCarriers'" @ref orxonox::Pickupable "Pickupables".
            destroy, //!< The @ref orxonox::MetaPickup "MetaPickup" destroys all the @ref orxonox::PickupCarrier "PickupCarriers'" @ref orxonox::Pickupable "Pickupables".
            destroyCarrier //!< The @ref orxonox::MetaPickup "MetaPickup" destroys the @ref orxonox::PickupCarrier "PickupCarrier".
        };
    }

    /**
    @brief
        The MetaPickup is a Pickupable that can, depending on the parameter <em>metaTye</em>, do different things. If the <em>metaTye</em> is set to
        - @b use All the @ref orxonox::Pickupable "Pickupables", the PickupCarrier has, are immediately set to used upon using the MetaPickup.
        - @b drop All the @ref orxonox::Pickupable "Pickupables", the PickupCarrier has, are immediately dropped upon using the MetaPickup.
        - @b destroy All the @ref orxonox::Pickupable "Pickupables", the PickupCarrier has, are immediately destroyed upon using the MetaPickup.
        - @b destroyCarrier The PickupCarrier is immediately destroyed upon using the MetaPickup.

        The default value is <em>none</em>, which basically does nothing.

        The parameter <b>activation type</b> can be used to specify, whether the MetaPickup is used upon pickup (<em>immediate</em>) or not (<em>onUse</em>). With <em>immediate</em> being the default.

        An example of a XML implementation of a MetaPickup would be:
        @code
        <MetaPickup
            activationType = "immediate"
            metaType = "use"
        />
        @endcode

    @author
        Damian 'Mozork' Frick

    @ingroup PickupItems
    */
    class _PickupExport MetaPickup : public Pickup
    {

        public:
            MetaPickup(Context* context); //!< Constructor. Registers and initializes the object.
            virtual ~MetaPickup(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode); //!< Method for creating a MetaPickup object through XML.

            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.

            /**
            @brief Returns the meta type of the MetaPickup.
            @return Returns an enum with the meta type of the MetaPickup.
            */
            inline pickupMetaType::Value getMetaType(void) const
                { return this->metaType_; }
            const std::string& getMetaTypeAsString(void) const; //!< Get the meta type of this MetaPickup.

        protected:
            /**
            @brief Set the meta type of the MetaPickup.
            @param type The meta type as an enum.
            */
            inline void setMetaType(pickupMetaType::Value type)
                { this->metaType_ =  type; }
            void setMetaTypeAsString(const std::string& type); //!< Set the meta type of this MetaPickup.

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
