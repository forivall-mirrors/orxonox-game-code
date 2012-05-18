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
 *      Nino Weingart
 *   Co-authors:
 *      ...
 *
 */

/**
    @file FlagPickup.h
    @brief Declaration of the FlagPickup class.
    @ingroup PickupItems
*/

#ifndef _FlagPickup_H__
#define _FlagPickup_H__

#include "pickup/PickupPrereqs.h"

#include <string>

#include "pickup/Pickup.h"
#include "tools/Timer.h"


namespace orxonox {
/*
    @ingroup PickupItems
    */
    namespace pickupFlagType
    {
        enum Value
        {
            blue, //!< Means that the @ref orxonox::FlagPickup "FlagPickup" is the flag of the blue team.
            red, //!< Means that the @ref orxonox::FlagPickup "FlagPickup" is the flag of the red team.
	        neutral //!< Means that the @ref orxonox::FlagPickup "FlagPickup" is a neutral flag.
        };
    }

    /**

    @author
        Nino Weingart

    @ingroup PickupItems
    */
    class _PickupExport FlagPickup : public Pickup, public Tickable
    {
        public:

            FlagPickup(BaseObject* creator); //!< Constructor.
            virtual ~FlagPickup(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode); //!< Method for creating a FlagPickup object through XML.

            virtual void tick(float dt);

	    const std::string& getFlagType(void) const; //!< Get the flag type of this pickup.

        int teamScore_;

	    virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.

        protected:
            void initializeIdentifier(void); //!< Initializes the PickupIdentifier of this pickup.

            /**
            @brief Set the flag type of this pickup.
            @param type The type of this pickup as an enum.
            */
            inline void setFlagTypeDirect(pickupFlagType::Value type)
                { this->flagType_ = type; }
            void setFlagType(std::string type); //!< Set the type of the FlagPickup.

        private:
            Pawn* carrierToPawnHelper(void); //!< Helper to transform the PickupCarrier to a Pawn, and throw an error message if the conversion fails.
            int getTeam(PlayerInfo* player);
            void initialize(void); //!< Initializes the member variables.

            pickupFlagType::Value flagType_; //!< The type of the FlagPickup.


            //! Strings for the flag types.
            static const std::string flagTypeBlue_s;
            static const std::string flagTypeRed_s;
	        static const std::string flagTypeNeutral_s;

    };
}

#endif // _FlagPickup_H__
