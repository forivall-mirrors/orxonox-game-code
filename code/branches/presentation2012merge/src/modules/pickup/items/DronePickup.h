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
 *      Lukas Gasser
 *   Co-authors:
 *      ...
 *
 */

/**
    @file DronePickup.h
    @brief Declaration of the DronePickup class.
    @ingroup PickupItems
*/

#ifndef _DronePickup_H__
#define _DronePickup_H__

#include "pickup/PickupPrereqs.h"

#include <string>

#include "pickup/Pickup.h"
#include "tools/interfaces/Tickable.h"

namespace orxonox {

    /**
    @brief
        The DronePickup adds a Drone to the Pawn upon being picked up.

        It can be used in XML as follows:
        @code
        <DronePickup droneTemplate="myDroneTemplate" />
        @endcode
        Where <em>droneTemplate</em> specifies a @ref orxonox::Template "Template" based on which the Drone is created.

    @author
        Lukas Gassner

    @ingroup PickupItems
    */
    class _PickupExport DronePickup : public Pickup, public Tickable
    {
        public:

            DronePickup(BaseObject* creator); //!< Constructor.
            virtual ~DronePickup(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, orxonox::XMLPort::Mode mode); //!< Method for creating a DronePickup object through XML.

            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.
            virtual void clone(OrxonoxClass*& item); //!< Creates a duplicate of the input OrxonoxClass.

            const std::string& getDroneTemplate() const; //!< Get the name of the droneTemplate.

        protected:
            void initializeIdentifier(void); //!< Initializes the PickupIdentifier of this pickup.

            void setDroneTemplate(const std::string& templatename); //!< Set the droneTemplate.

        private:
            void initialize(void); //!< Initializes the member variables.
            std::string droneTemplate_; //!< The name of the template, based upon which the Drone is created.
            Pawn* carrierToPawnHelper(void); //!< Helper to transform the PickupCarrier to a Pawn, and throw an error message if the conversion fails.



    };
}

#endif // _DronePickup_H__
