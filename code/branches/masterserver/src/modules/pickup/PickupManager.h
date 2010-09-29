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
    @file PickupManager.h
    @brief Definition of the PickupManager class.
    @ingroup Pickup
*/

#ifndef _PickupManager_H__
#define _PickupManager_H__

#include "PickupPrereqs.h"

#include <map>
#include "util/Singleton.h"
#include "core/WeakPtr.h"
#include "pickup/PickupIdentifier.h"
#include "PickupRepresentation.h"

#include "core/OrxonoxClass.h"

namespace orxonox // tolua_export
{ // tolua_export

    /**
    @brief
        Manages Pickupables.
        In essence has two tasks to fulfill. Firstly it must link Pickupables (through their PickupIdentifiers) and their PickupRepresentations. Secondly it manages the PickupInventory.
    @author
        Damian 'Mozork' Frick
    */
    class _PickupExport PickupManager // tolua_export
        : public Singleton<PickupManager>, public OrxonoxClass
    { // tolua_export
        friend class Singleton<PickupManager>;

        public:
            PickupManager();
            virtual ~PickupManager();

            static PickupManager& getInstance() { return Singleton<PickupManager>::getInstance(); } // tolua_export

            bool registerRepresentation(const PickupIdentifier* identifier, PickupRepresentation* representation); //!< Registers a PickupRepresentation together with the PickupIdentifier of the Pickupable the PickupRepresentation represents.
            bool unregisterRepresentation(const PickupIdentifier* identifier, PickupRepresentation* representation); //!< Unegisters a PickupRepresentation together with the PickupIdentifier of the Pickupable the PickupRepresentation represents.
            PickupRepresentation* getRepresentation(const PickupIdentifier* identifier); //!< Get the PickupRepresentation representing the Pickupable with the input PickupIdentifier.

            // tolua_begin
            orxonox::PickupRepresentation* getPickupRepresentation(orxonox::Pickupable* pickup); //!< Get the PickupRepresentation of an input Pickupable.

            int getNumPickups(void); //!< Update the list of picked up Pickupables and get the number of Pickupables in the list.
            /**
            @brief Get the next Pickupable in the list.
                   Use this, after having called getNumPickups() to access all the Pickupables individually and in succession.
            @return Returns the next Pickupable in the list.
            */
            orxonox::Pickupable* popPickup(void) { return (this->pickupsIterator_++)->first; }

            void dropPickup(orxonox::Pickupable* pickup); //!< Drop the input Pickupable.
            void usePickup(orxonox::Pickupable* pickup, bool use); //!< Use (or unuse) the input Pickupable.
            bool isValidPickup(orxonox::Pickupable* pickup); //!< Check whether the input Pickupable is valid, meaning that it is in the PickupManager's list and still exists.
            // tolua_end

        private:
            static PickupManager* singletonPtr_s;
            static const std::string guiName_s; //!< The name of the PickupInventory

            PickupRepresentation* defaultRepresentation_; //!< The default PickupRepresentation.
            std::map<const PickupIdentifier*, PickupRepresentation*, PickupIdentifierCompare> representations_; //!< Map linking PickupIdentifiers (representing types if Pickupables) and PickupRepresentations.

            std::map<Pickupable*, WeakPtr<Pickupable> > pickupsList_; //!< A list of all the picked up Pickupables.
            std::map<Pickupable*, WeakPtr<Pickupable> >::iterator pickupsIterator_; //!< An iterator pointing to the current Pickupable in pickupsList_.

            std::vector<PickupCarrier*>* getAllCarriers(PickupCarrier* carrier, std::vector<PickupCarrier*>* carriers = NULL); //!< Helper method. Get all the PickupCarriers that carry Pickupables, recursively.

    }; // tolua_export

} // tolua_export

#endif // _PickupManager_H__
