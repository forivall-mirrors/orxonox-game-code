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
    /**
    @brief
        Data structure to store collected data for one specific @ref orxonox::Pickupable "Pickupable".
        This is used to not have to synchronise @ref orxonox::Pickupable "Pickupable" just to have the needed information for the PickupInventory available on all clients. Instead the information is sent over the network and stored in a PickupInventoryContainer.

    @ingroup Pickup
    */
    struct PickupInventoryContainer
    {
        uint32_t pickup; //!< An indentifier for the @ref orxonox::Pickupable "Pickupable" that is associated with the information stored here.
        bool inUse; //!< Whether the @ref orxonox::Pickupable "Pickupable" is currently in use.
        bool pickedUp; //!< Whether the @ref orxonox::Pickupable "Pickupable" is currently picked up.
        bool usable; //!< Whether the @ref orxonox::Pickupable "Pickupable" is usable.
        bool unusable; //!< Whether the @ref orxonox::Pickupable "Pickupable" is droppable.
        uint32_t representationObjectId; //!< The objectId of the @ref orxonox::PickupRepresentation "PickupRepresentation" that represents the @ref orxonox::Pickupable "Pickupable".
    };
    // tolua_end

    /**
    @brief
        The PickupManager class manages @ref orxonox::Pickupable "Pickupables".

        It has in essence two tasks to fulfill. Firstly it must link @ref orxonox::Pickupable "Pickupables" (through their @ref orxonox::PickupIdentifier "PickupIdentifiers") to their respective @ref orxonox::PickupRepresentation "PickupRepresentations". Secondly it manages the PickupInventory. (The PickupInventory is the GUI that displays @ref orxonox::Pickupable "Pickupables" for the covenience of the user.)

        @section PickupManagerTechnicalDetails Technical details
        Unfortunately <em>under the hood</em> it isn't just as easy. At least the PickupInventory part isn't. To grasp why this is we need to have a brief look at how the pickups module works over the network:

        The pickups module essentially just exists on the @ref orxonox::Server "Server", since it's all game logic. That means, a @ref orxonox::PickupSpawner "PickupSpawner" is always triggered on the server, the @ref orxonox::Pickupable "Pickupable" is then picked up (and maybe used already) on the server. The effects the pickup has are synchronised over the network, if they have to be, but this is the responsibility of the entities that are affected, not the pickups module, and normally this is already implemented. The only two things that need to be communicated over the network are the graphical component of the @ref orxonox::PickupSpawner "PickupSpawner", since it needs to be displayed on the @ref orxonox::Client "Clients" as well, and anything that is needed for the PickupInventory, since it exists for each @ref orxonox::Host "Host" seperatly.

        Fortunately synchronising the @ref orxonox::PickupSpawner "PickupSpawner" is already being taken care of by the synchronisation of the @ref orxonox::StaticEntity "StaticEntity" (or parents thereof).

        This leaves us with the PickupInventory component (and this is really the source of all the complexity).

        Firstly there are a number of lists (where by list I really mean any kind of ordered data structure) kept.
        - The @ref orxonox::PickupManager::representations_ "representations_" list links @ref orxonox::PickupRepresentation "PickupRepresentations" with @ref orxonox::PickupIdentifier "PickupIdentifiers" and can be used to get the @ref orxonox::PickupRepresentation "PickupRepresentation" for a given @ref orxonox::Pickupable "Pickupable". It is only populated on the server (or in standalone mode). Each @ref orxonox::PickupRepresentation "PickupRepresentation" that is generated through XML registers itself with the PickupManager and is thereby added to the list.
        - The @ref orxonox::PickupManager::representationsNetworked_ "representationsNetworked_" list is the networked (hence the name) equivalent to the @ref orxonox::PickupManager::representations_ "representations_" list. It links an objectId of a @ref orxonox::PickupRepresentation "PickupRepresentation" to a @ref orxonox::PickupRepresentation "PickupRepresentation". This list is maintained on all hosts, each representation registers itself (in its constructor) with the PickupManager. Since the representations are synchronised they are created on each host. The "same" instance on each host is identified by the same objectId and that is why we store the representations with the objectId as key. We can then use this list to get a @ref orxonox::PickupRepresentation "PickupRepresentation" for a specific @ref orxonox::Pickupable "Pickupable" (or more precisely a number identifiying that particular pickup, but we'll see that, when we look at the next list) on a client to be used in the PickupInventory.
        - The @ref orxonox::PickupManager::pickupInventoryContainers_ "pickupInventoryContainers_" list links a number identifying a @ref orxonox::Pickupable "Pickupable" to a data structure (the @ref orxonox::PickupInventoryContainer "PickupInventoryContainer"), which contains all necessary information about that @ref orxonox::Pickupable "Pickupable". This list is maintained on all hosts, a new container is inserted when a @ref orxonox::Pickupable "Pickupable" is picked up, but only if it has been picked up by the repsective host. This list is then used by the PickupInventory to access the required information and to get the correct @ref orxonox::PickupRepresentation "PickupRepresentation".
        - The @ref orxonox::PickupManager::pickups_ "pickups_" list links a number identifiying a @ref orxonox::Pickupable "Pickupable" to a (weak pointer to a) @ref orxonox::Pickupable "Pickupable". It is only maintained by the server (or in standalone mode), to be able to use, unuse and drop @ref orxonox::Pickupable "Pickupables" through the PickupInventory. Since @ref orxonox::Pickupable "Pickupables" only exist on the server a client that wants to change a pickups status has so send a request over the network (with the number identifying the pickup) to the server and then the server facilitates the change, using this list to map from the identifyer to the actual @ref orxonox::Pickupable "Pickupable".
        - The @ref orxonox::PickupManager::indexes_ "indexes_" list links a @ref orxonox::Pickupable "Pickupable" to the number identifying it. This is only maintained on the server (or in standalone mode), and is used for the inverse mapping of the previous list, which means the server uses it identify pickups on clients when it communicates changes in pickups to clients.

        There is communication in both directions. From server to client, when the server informs the client that the state of some @ref orxonox::Pickupable "Pickupable" has changed, during which all necessary information to create or update the PickupInventoryContainer for that pickup on the client is sent as well. Or from client to server, when the client wants the server to change the state of some @ref orxonox::Pickupable "Pickupable".

    @author
        Damian 'Mozork' Frick

    @ingroup Pickup
    */
    class _PickupExport PickupManager // tolua_export
        : public Singleton<PickupManager>, public PickupListener
    { // tolua_export
        friend class Singleton<PickupManager>;

        public:
            PickupManager(); //!< Constructor.
            virtual ~PickupManager(); //!< Destructor.

            /**
            @brief Get the instance of the PickupManager singleton.
            @return Returns the instance of the PickupManager singleton.
            */
            static PickupManager& getInstance() { return Singleton<PickupManager>::getInstance(); } // tolua_export

            bool registerRepresentation(const PickupIdentifier* identifier, PickupRepresentation* representation); //!< Registers a PickupRepresentation together with the PickupIdentifier of the Pickupable the PickupRepresentation represents.
            bool unregisterRepresentation(const PickupIdentifier* identifier, PickupRepresentation* representation); //!< Unegisters a PickupRepresentation together with the PickupIdentifier of the Pickupable the PickupRepresentation represents.

            bool registerRepresentation(PickupRepresentation* representation); //!< Registers a PickupRepresentation on the host it was created.
            bool unregisterRepresentation(PickupRepresentation* representation); //!< Unregisters a PickupRepresentation on the host it is being destroyed.

            PickupRepresentation* getRepresentation(const PickupIdentifier* identifier); //!< Get the PickupRepresentation representing the Pickupable with the input PickupIdentifier.

            virtual void pickupChangedUsed(Pickupable* pickup, bool used); //!< Is called by the PickupListener to notify the PickupManager, that the input Pickupable has transited to the input used state.
            static void pickupChangedUsedNetwork(uint32_t pickup, bool inUse, bool usable, bool unusable); //!< Helper method to react to the change in the used status of a Pickupable.
            virtual void pickupChangedPickedUp(Pickupable* pickup, bool pickedUp); //!< Is called by the PickupListener to notify the PickupManager, that the input Pickupable has transited to the input pickedUp state.
            static void pickupChangedPickedUpNetwork(uint32_t pickup, bool usable, uint32_t representationObjectId, bool pickedUp); //!< Helper method to react to the change in the pickedUp status of a Pickupable.

        // Methods to be used by the PickupInventory.
        public:
            // tolua_begin
            orxonox::PickupRepresentation* getPickupRepresentation(uint32_t pickup); //!< Get the PickupRepresentation of an input indentifier for Pickupable.

            int getNumPickups(void); //!< Get the number of pickups currently picked up by the player.
            /**
            @brief Get the next PickupInventoryContainer in the list.
                   Use this, after having called getNumPickups() to access all the PickupInventoryContainers individually and in succession.
            @return Returns the next PickupInventoryContainer in the list.
            */
            const orxonox::PickupInventoryContainer* popPickup(void)
                { return (this->pickupsIterator_++)->second; }

            void dropPickup(uint32_t pickup); //!< Drop the input Pickupable.
            void usePickup(uint32_t pickup, bool use); //!< Use (or unuse) the input Pickupable.
             /**
            @brief Check whether the input Pickupable is valid, meaning that it is in the PickupManager's list and still exists.
            @param pickup The Pickupable.
            @return Returns true if the input Pickupable is still valid, false if not.
            */
            bool isValidPickup(uint32_t pickup) const
                { return this->pickups_.find(pickup) != this->pickups_.end(); }
            // tolua_end

            static void dropPickupNetworked(uint32_t pickup); //!< Helper method to drop the input pickup on the server.
            static void usePickupNetworked(uint32_t pickup, bool use); //!< Helper method to use (or unuse) the input Pickupable on the server.

        private:
            static PickupManager* singletonPtr_s;
            static const std::string guiName_s; //!< The name of the PickupInventory
            bool guiLoaded_; //!< Whether the PickupInventory GUI has been loaded, yet.
            uint32_t pickupHighestIndex_; //!< The highest pickup index currently in use. (not taking wrap-around into account)

            PickupRepresentation* defaultRepresentation_; //!< The default PickupRepresentation.

            std::map<const PickupIdentifier*, PickupRepresentation*, PickupIdentifierCompare> representations_; //!< Map linking PickupIdentifiers (representing types of Pickupables) and PickupRepresentations.
            std::map<uint32_t, PickupRepresentation*> representationsNetworked_; //!< Map linking the PickupRepresentation's objectId to the PickupRepresentation itself. It is used for networking purposes.

            std::map<uint32_t, PickupInventoryContainer*> pickupInventoryContainers_; //!< Map linking a number identifying a Pickupable to a PickupInventoryContainer, which contains all necessary information about that Pickupable.
            std::map<uint32_t, PickupInventoryContainer*>::iterator pickupsIterator_; //!< An iterator pointing to the current Pickupable in pickupsList_.

            std::map<uint32_t, WeakPtr<Pickupable>*> pickups_; //!< Map linking a number identifying a Pickupable to a weak pointer of a Pickupable.
            std::map<Pickupable*, uint32_t> indexes_;//!< Map linking Pickupable to the number identifying it.

            void updateGUI(void); //!< Updates the PickupInventory GUI.
            uint32_t getPickupIndex(void); //!< Get a new index for a Pickupable.

    }; // tolua_export

} // tolua_export

#endif // _PickupManager_H__
