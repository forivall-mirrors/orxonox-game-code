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

#include "core/WeakPtr.h"

#include "pickup/PickupIdentifier.h"

#include "PickupRepresentation.h"

#include "util/Singleton.h"
#include "interfaces/PickupListener.h"

namespace orxonox // tolua_export
{ // tolua_export

    // tolua_begin
    struct PickupInventoryContainer
    {
        uint64_t pickup;
        bool inUse;
        bool pickedUp;
        bool usable;
        bool unusable;
        uint32_t representationObjectId;
    };
    // tolua_end

    /**
    @brief
        Manages Pickupables.
        In essence has two tasks to fulfill. Firstly it must link Pickupables (through their PickupIdentifiers) and their PickupRepresentations. Secondly it manages the PickupInventory.
    @author
        Damian 'Mozork' Frick
    */
    class _PickupExport PickupManager // tolua_export
        : public Singleton<PickupManager>, public PickupListener
    { // tolua_export
        friend class Singleton<PickupManager>;

        public:
            PickupManager();
            virtual ~PickupManager();

            static PickupManager& getInstance() { return Singleton<PickupManager>::getInstance(); } // tolua_export

            bool registerRepresentation(const PickupIdentifier* identifier, PickupRepresentation* representation); //!< Registers a PickupRepresentation together with the PickupIdentifier of the Pickupable the PickupRepresentation represents.
            bool registerRepresentation(PickupRepresentation* representation);
            bool unregisterRepresentation(const PickupIdentifier* identifier, PickupRepresentation* representation); //!< Unegisters a PickupRepresentation together with the PickupIdentifier of the Pickupable the PickupRepresentation represents.
            bool unregisterRepresentation(PickupRepresentation* representation);
            PickupRepresentation* getRepresentation(const PickupIdentifier* identifier); //!< Get the PickupRepresentation representing the Pickupable with the input PickupIdentifier.

            // tolua_begin
            orxonox::PickupRepresentation* getPickupRepresentation(uint64_t pickup); //!< Get the PickupRepresentation of an input Pickupable.

            int getNumPickups(void); //!< Update the list of picked up Pickupables and get the number of Pickupables in the list.
            /**
            @brief Get the next Pickupable in the list.
                   Use this, after having called getNumPickups() to access all the Pickupables individually and in succession.
            @return Returns the next Pickupable in the list.
            */
            orxonox::PickupInventoryContainer* popPickup(void) { return (this->pickupsIterator_++)->second; }

            void dropPickup(uint64_t pickup); //!< Drop the input Pickupable.
            void usePickup(uint64_t pickup, bool use); //!< Use (or unuse) the input Pickupable.
            bool isValidPickup(uint64_t pickup); //!< Check whether the input Pickupable is valid, meaning that it is in the PickupManager's list and still exists.
            // tolua_end

            static void dropPickupNetworked(uint64_t pickup);
            static void usePickupNetworked(uint64_t pickup, bool use);

            virtual void pickupChangedUsed(Pickupable* pickup, bool used);
            static void pickupChangedUsedNetwork(uint64_t pickup, bool inUse, bool usable, bool unusable);
            virtual void pickupChangedPickedUp(Pickupable* pickup, bool pickedUp);
            static void pickupChangedPickedUpNetwork(uint64_t pickup, bool usable, uint32_t representationObjectId, bool pickedUp);

        private:
            static PickupManager* singletonPtr_s;
            static const std::string guiName_s; //!< The name of the PickupInventory
            bool guiLoaded_;
            uint64_t pickupIndex_;

            PickupRepresentation* defaultRepresentation_; //!< The default PickupRepresentation.
            std::map<const PickupIdentifier*, PickupRepresentation*, PickupIdentifierCompare> representations_; //!< Map linking PickupIdentifiers (representing types if Pickupables) and PickupRepresentations.
            std::map<uint32_t, PickupRepresentation*> representationsNetworked_;

            std::map<uint64_t, PickupInventoryContainer*> pickupInventoryContainers_;
            std::map<uint64_t, PickupInventoryContainer*>::iterator pickupsIterator_; //!< An iterator pointing to the current Pickupable in pickupsList_.

            std::map<uint64_t, WeakPtr<Pickupable>*> pickups_;
            std::map<Pickupable*, uint64_t> indexes_;

            void updateGUI(void);
            uint64_t getPickupIndex(void);

    }; // tolua_export

} // tolua_export

#endif // _PickupManager_H__
