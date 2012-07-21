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
    @file PickupCollection.h
    @brief Declaration of PickupCollection.
    @ingroup Pickup
*/

#ifndef _PickupCollection_H__
#define _PickupCollection_H__

#include "PickupPrereqs.h"

#include <list>

#include "CollectiblePickup.h"
#include "core/BaseObject.h"

namespace orxonox
{

    /**
    @brief
        The PickupCollection combines different @ref orxonox::Pickupable "Pickupables" (more precisely @ref orxonox::CollectiblePickup "CollectiblePickups") to a coherent, single pickup and makes them seem (from the outside looking in) just as if they were just one @ref orxonox::Pickupable "Pickupable".

        A PickupCollection can be created in XML as follows:
        @code
        <PickupCollection>
            <pickupables>
                <CollectiblePickup ... />
                ...
                <CollectiblePickup ... />
            </pickupables>
        </PickupCollection>
        @endcode

    @author
        Damian 'Mozork' Frick

    @ingroup Pickup
    */
    class _PickupExport PickupCollection : public CollectiblePickup, public BaseObject
    {

        public:
            PickupCollection(BaseObject* creator); //!< Default Constructor.
            virtual ~PickupCollection(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Creates an instance of this Class through XML.

            virtual void changedUsed(void); //!< Is called when the pickup has transited from used to unused or the other way around.
            virtual void changedCarrier(void); //!< Is called when the pickup has changed its PickupCarrier.
            virtual void changedPickedUp(void); //!< Is called when the pickup has transited from picked up to dropped or the other way around.

            virtual bool isTarget(const PickupCarrier* carrier) const; //!< Get whether a given class, represented by the input Identifier, is a target of this PickupCollection.

            inline void setRepresentationName(const std::string& name)
                { this->representationName_ = name; }
            virtual const std::string& getRepresentationName() const
                { return this->representationName_; }

            bool addPickupable(CollectiblePickup* pickup); //!< Add the input Pickupable to list of Pickupables combined by this PickupCollection.
            const Pickupable* getPickupable(unsigned int index) const; //!< Get the Pickupable at the given index.
            bool removePickupable(CollectiblePickup* pickup); //!< Removes the input Pickupable from the list of Pickupables in this PickupCollection.

            inline const std::list<CollectiblePickup*>& getPickups() const
                { return this->pickups_; }

            void pickupChangedUsed(bool changed); //!< Informs the PickupCollection, that one of its pickups has changed its used status to the input value.
            void pickupChangedPickedUp(bool changed); //!< Informs the PickupCollection, that one of its pickups has changed its picked up status to the input value.
            void pickupDisabled(void); //!< Informs the PickupCollection, that one of its pickups has been disabled.

        protected:
            virtual bool createSpawner(void); //!< Facilitates the creation of a PickupSpawner upon dropping of the Pickupable.

        private:
            void changedUsedAction(void); //!< Helper method.
            void changedPickedUpAction(void); //!< Helper method.
            void pickupsChanged(void); //!< Helper method.

            std::string representationName_; //!< The name of the associated PickupRepresentation.
            std::list<CollectiblePickup*> pickups_; //!< The list of the pointers of all the Pickupables this PickupCollection consists of. They are weak pointers to facilitate testing, whether the pointers are still valid.

            bool processingUsed_; //!< Boolean to ensure, that the PickupCollection doesn't update its used status while its internal state is inconsistent.
            bool processingPickedUp_; //!< Boolean to ensure, that the PickupCollection doesn't update its picked upp status while its internal state is inconsistent.

    };

}

#endif /* _PickupCollection_H__ */
