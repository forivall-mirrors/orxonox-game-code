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
    @file PickupManager.cc
    @brief Implementation of the PickupManager class.
*/

#include "PickupManager.h"

#include "core/CoreIncludes.h"
#include "core/LuaState.h"
#include "core/GUIManager.h"
#include "core/Identifier.h"
#include "network/Host.h"
#include "network/NetworkFunction.h"
#include "util/ScopedSingletonManager.h"

#include "infos/PlayerInfo.h"
#include "interfaces/PickupCarrier.h"
#include "worldentities/pawns/Pawn.h"

#include "CollectiblePickup.h"
#include "PickupRepresentation.h"

#include "ToluaBindPickup.h"

namespace orxonox
{
    // Register tolua_open function when loading the library
    DeclareToluaInterface(Pickup);

    ManageScopedSingleton(PickupManager, ScopeID::Root, false);

    // Initialization of the name of the PickupInventory GUI.
    /*static*/ const std::string PickupManager::guiName_s = "PickupInventory";

    // Register static newtork functions that are used to communicate changes to pickups over the network, such that the PickupInventory can display the information about the pickups properly.
    registerStaticNetworkFunction(PickupManager::pickupChangedUsedNetwork);
    registerStaticNetworkFunction(PickupManager::pickupChangedPickedUpNetwork);
    registerStaticNetworkFunction(PickupManager::dropPickupNetworked);
    registerStaticNetworkFunction(PickupManager::usePickupNetworked);

    /**
    @brief
        Constructor. Registers the PickupManager and creates the default PickupRepresentation.
    */
    PickupManager::PickupManager() : guiLoaded_(false), pickupHighestIndex_(0), defaultRepresentation_(NULL)
    {
        RegisterObject(PickupManager);

        //TODO: Only create if isMaster().
        this->defaultRepresentation_ = new PickupRepresentation();

        COUT(3) << "PickupManager created." << std::endl;
    }

    /**
    @brief
        Destructor.
        Destroys the default PickupRepresentation and does some cleanup.
    */
    PickupManager::~PickupManager()
    {
        // Destroying the default representation.
        if(this->defaultRepresentation_ != NULL)
            this->defaultRepresentation_->destroy();

        this->representations_.clear();
        this->representationsNetworked_.clear();

        // Destroying all the PickupInventoryContainers that are still there.
        for(std::map<uint32_t, PickupInventoryContainer*>::iterator it = this->pickupInventoryContainers_.begin(); it != this->pickupInventoryContainers_.end(); it++)
            delete it->second;
        this->pickupInventoryContainers_.clear();

        // Destroying all the WeakPointers that are still there.
        for(std::map<uint32_t, WeakPtr<Pickupable>*>::iterator it = this->pickups_.begin(); it != this->pickups_.end(); it++)
            delete it->second;
        this->pickups_.clear();

        this->indexes_.clear();

        COUT(3) << "PickupManager destroyed." << std::endl;
    }

    /**
    @brief
        Registers a PickupRepresentation together with the PickupIdentifier of the Pickupable the PickupRepresentation represents, on the server (or in standalone mode).
        For every type of Pickupable (uniquely identified by a PickupIdentifier) there can be one (and just one) PickupRepresentation registered.
    @param identifier
        The PickupIdentifier identifying the Pickupable.
    @param representation
        A pointer to the PickupRepresentation.
    @return
        Returns true if successful and false if not.
    */
    bool PickupManager::registerRepresentation(const PickupIdentifier* identifier, PickupRepresentation* representation)
    {
        assert(identifier);
        assert(representation);

        // If the list is not empty and Pickupable already has a Representation registered.
        if(!this->representations_.empty() && this->representations_.find(identifier) != this->representations_.end())
            return false;

        this->representations_[identifier] = representation;

        COUT(4) << "PickupRepresentation &" << representation << " registered with the PickupManager." << std::endl;
        return true;
    }

    /**
    @brief
        Unegisters a PickupRepresentation together with the PickupIdentifier of the Pickupable the PickupRepresentation represents, on the server (or in standalone mode).
    @param identifier
        The PickupIdentifier identifying the Pickupable.
    @param representation
        A pointer to the PickupRepresentation.
    @return
        Returns true if successful and false if not.
    */
    bool PickupManager::unregisterRepresentation(const PickupIdentifier* identifier, PickupRepresentation* representation)
    {
        assert(identifier);
        assert(representation);

        std::map<const PickupIdentifier*, PickupRepresentation*, PickupIdentifierCompare>::iterator it = this->representations_.find(identifier);
        if(it == this->representations_.end()) // If the Pickupable is not registered in the first place.
            return false;

        this->representations_.erase(it);

        COUT(4) << "PickupRepresentation &" << representation << " unregistered with the PickupManager." << std::endl;
        return true;
    }

    /**
    @brief
        Registers a PickupRepresentation on the host it was created.
    @param representation
        A pointer to the PickupRepresentation.
    @return
        Returns true if successful, false if not.
    */
    bool PickupManager::registerRepresentation(PickupRepresentation* representation)
    {
        assert(representation);

        // If the list is not empty and PickupRepresentation is already registered.
        if(!this->representationsNetworked_.empty() && this->representationsNetworked_.find(representation->getObjectID()) != this->representationsNetworked_.end())
            return false;

        this->representationsNetworked_[representation->getObjectID()] = representation;
        return true;
    }

    /**
    @brief
        Unregisters a PickupRepresentation on the host it is being destroyed (which is the same host on which it was created).
    @param representation
        A pointer to the Pickuprepresentation.
    @return
        Returns true if successful, false if not.
    */
    bool PickupManager::unregisterRepresentation(PickupRepresentation* representation)
    {
        assert(representation);

        std::map<uint32_t, PickupRepresentation*>::iterator it = this->representationsNetworked_.find(representation->getObjectID());
        if(it == this->representationsNetworked_.end()) // If the Pickupable is not registered in the first place.
            return false;

        this->representationsNetworked_.erase(it);
        return true;
    }

    /**
    @brief
        Get the PickupRepresentation representing the Pickupable with the input PickupIdentifier.
    @param identifier
        The PickupIdentifier.
    @return
        Returns a pointer to the PickupRepresentation.
    */
    //TODO: Why not return a const?
    PickupRepresentation* PickupManager::getRepresentation(const PickupIdentifier* identifier)
    {
        std::map<const PickupIdentifier*, PickupRepresentation*, PickupIdentifierCompare>::iterator it = this->representations_.find(identifier);
        if(it == this->representations_.end()) // If there is no PickupRepresentation associated with the input PickupIdentifier.
        {
            COUT(4) << "PickupManager::getRepresentation() returned default representation." << std::endl;
            return this->defaultRepresentation_;
        }

        return it->second;
    }

    /**
    @brief
        Is called by the PickupListener to notify the PickupManager, that the input Pickupable has transited to the input used state.
    @param pickup
        The Pickupable whose used status changed.
    @param used
        The used status the Pickupable changed to.
    */
    void PickupManager::pickupChangedUsed(Pickupable* pickup, bool used)
    {
        assert(pickup);

        if(!GameMode::isMaster()) // If this is neither standalone nor the server.
            return;

        CollectiblePickup* collectible = orxonox_cast<CollectiblePickup*>(pickup);
        // If the Pickupable is part of a PickupCollection it isn't displayed in the PickupInventory, just the PickupCollection is.
        if(collectible != NULL && collectible->isInCollection())
            return;

        // Getting clientId of the host this change of the pickup's used status concerns.
        PickupCarrier* carrier = pickup->getCarrier();
        while(carrier->getCarrierParent() != NULL)
            carrier = carrier->getCarrierParent();
        Pawn* pawn = orxonox_cast<Pawn*>(carrier);
        if(pawn == NULL)
            return;
        PlayerInfo* info = pawn->getPlayer();
        if(info == NULL)
            return;
        unsigned int clientId = info->getClientID();

        // Get the number identifying the pickup.
        std::map<Pickupable*, uint32_t>::iterator it = this->indexes_.find(pickup);
        assert(it != this->indexes_.end());
        uint32_t index = it->second;

        // If we're either in standalone mode or this is the host whom the change of the pickup's status concerns.
        if(GameMode::isStandalone() || Host::getPlayerID() == clientId)
        {
            PickupManager::pickupChangedUsedNetwork(index, used, pickup->isUsable(), pickup->isUnusable());
        }
        // If the concerned host is somewhere in the network, we call pickupChangedUsedNetwork() on its PickupManager.
        else
        {
            callStaticNetworkFunction(PickupManager::pickupChangedUsedNetwork, clientId, index, used, pickup->isUsable(), pickup->isUnusable());
        }
    }

    /**
    @brief
        Helper method to react to the change in the used status of a Pickupable.
        Static method that is used by the server to inform the client it concerns about the status change.
        The parameters that are given are used to update the information (i.e. the PickupInventoryContainer) the concerning PickupManager has about the Pickupable that changed.
    @param pickup
        A number identifying the Pickupable that changed its used status.
    @param inUse
        The used status the Pickupable changed to. (i.e. whether the Pickupable is in use or not).
    @param usable
        Whether the Pickupable's used status can be changed used in the PickupInventory.
    @param unusable
        Whether the Pickupable's used status can be changed to unused in the PickupInventory.
    */
    /*static*/ void PickupManager::pickupChangedUsedNetwork(uint32_t pickup, bool inUse, bool usable, bool unusable)
    {
        PickupManager& manager = PickupManager::getInstance(); // Get the PickupManager singleton on this host.
        // If the input Pickupable (i.e its identifier) is not present in the list the PickupManager has.
        if(manager.pickupInventoryContainers_.find(pickup) == manager.pickupInventoryContainers_.end())
        {
            COUT(1) << "Error: Pickupable &(" << pickup << ") was not registered with PickupManager for the PickupInventory, when it changed used." << std::endl;
            return;
        }

        // Update the Pickupable's container with the information transferred.
        manager.pickupInventoryContainers_[pickup]->inUse = inUse;
        manager.pickupInventoryContainers_[pickup]->usable = usable;
        manager.pickupInventoryContainers_[pickup]->unusable = unusable;

        manager.updateGUI(); // Tell the PickupInventory that something has changed.
    }

    /**
    @brief
        Is called by the PickupListener to notify the PickupManager, that the input Pickupable has transited to the input pickedUp state.
    @param pickup
        The Pickupable whose pickedUp status changed.
    @param pickedUp
        The pickedUp status the Pickupable changed to.
    */
    void PickupManager::pickupChangedPickedUp(Pickupable* pickup, bool pickedUp)
    {
        assert(pickup);

        if(!GameMode::isMaster()) // If this is neither standalone nor the server.
            return;

        CollectiblePickup* collectible = orxonox_cast<CollectiblePickup*>(pickup);
        // If the Pickupable is part of a PickupCollection it isn't displayed in the PickupInventory, just the PickupCollection is.
        if(collectible != NULL && collectible->isInCollection())
            return;

        // Getting clientId of the host this change of the pickup's pickedUp status concerns.
        PickupCarrier* carrier = pickup->getCarrier();
        while(carrier->getCarrierParent() != NULL)
            carrier = carrier->getCarrierParent();
        Pawn* pawn = orxonox_cast<Pawn*>(carrier);
        if(pawn == NULL)
            return;
        PlayerInfo* info = pawn->getFormerPlayer();
        if(info == NULL)
            return;
        unsigned int clientId = info->getClientID();

        uint32_t index = 0;
        if(pickedUp) // If the Pickupable has changed to picked up, it is added to the required lists.
        {
            index = this->getPickupIndex(); // Ge a new identifier (index) for the Pickupable.
            // Add the Pickupable to the indexes_ and pickups_ lists.
            this->indexes_[pickup] = index;
            this->pickups_[index] = new WeakPtr<Pickupable>(pickup);
        }
        else // If it was dropped, it is removed from the required lists.
        {
            // Get the indentifier (index) that identifies the input Pickupable.
            std::map<Pickupable*, uint32_t>::iterator it = this->indexes_.find(pickup);
            index = it->second;

            // Remove the Pickupable form the indexes_ and pickups_ list.
            WeakPtr<Pickupable>* ptr = this->pickups_[index];
            this->indexes_.erase(it);
            this->pickups_.erase(index);
            delete ptr;
        }

        // If we're either in standalone mode or this is the host whom the change of the pickup's status concerns.
        //TODO: Needs to be added to server even if is was not picked up by it?
        if(GameMode::isStandalone() || Host::getPlayerID() == clientId)
        {
            // If there is no PickupRepresentation registered the default representation is used.
            if(this->representations_.find(pickup->getPickupIdentifier()) == this->representations_.end())
                PickupManager::pickupChangedPickedUpNetwork(index, pickup->isUsable(), this->defaultRepresentation_->getObjectID(), pickedUp);
            else
                PickupManager::pickupChangedPickedUpNetwork(index, pickup->isUsable(), this->representations_[pickup->getPickupIdentifier()]->getObjectID(), pickedUp);
        }
        // If the concerned host is somewhere in the network, we call pickupChangedPickedUpNetwork() on its PickupManager.
        else
        {
            // If there is no PickupRepresentation registered the default representation is used.
            if(this->representations_.find(pickup->getPickupIdentifier()) == this->representations_.end())
            {
                callStaticNetworkFunction(PickupManager::pickupChangedPickedUpNetwork, clientId, index, pickup->isUsable(), this->defaultRepresentation_->getObjectID(), pickedUp);
            }
            else
            {
                callStaticNetworkFunction(PickupManager::pickupChangedPickedUpNetwork, clientId, index, pickup->isUsable(), this->representations_[pickup->getPickupIdentifier()]->getObjectID(), pickedUp);
            }
        }

    }

    /**
    @brief
        Helper method to react to the change in the pickedUp status of a Pickupable.
        Static method that is used by the server to inform the client it concerns about the status change.
        The parameters that are given are used to update the information (i.e. the PickupInventoryContainer) the concerning PickupManager has about the Pickupable that changed.
    @param pickup
        A number identifying the Pickupable that changed its pickedUp status.
    @param usable
        Whether the Pickupable's used status can be changed to used in the PickupInventory.
    @param representationObjectId
        The objectId identifying (over the network) the PickupRepresentation that represents this Pickupable.
    @param pickedUp
        The pickedUp status the Pickupable changed to.
    */
    /*static*/ void PickupManager::pickupChangedPickedUpNetwork(uint32_t pickup, bool usable, uint32_t representationObjectId, bool pickedUp)
    {
        PickupManager& manager = PickupManager::getInstance(); // Get the PickupManager singleton on this host.
        // If the Pickupable has been picked up, we create a new PickupInventoryContainer for it.
        if(pickedUp)
        {
            // Create a new PickupInventoryContainer for the Pickupable and set all the necessary information.
            PickupInventoryContainer* container = new PickupInventoryContainer;
            container->pickup = pickup;
            container->inUse = false;
            container->pickedUp = pickedUp;
            container->usable = usable;
            container->unusable = false;
            container->representationObjectId = representationObjectId;
            // Insert the container into the pickupInventoryContainers_ list.
            manager.pickupInventoryContainers_.insert(std::pair<uint32_t, PickupInventoryContainer*>(pickup, container));

            manager.updateGUI(); // Tell the PickupInventory that something has changed.
        }
        // If the Pickupable has been dropped, we remove it from the pickupInventoryContainers_ list.
        else
        {
            std::map<uint32_t, PickupInventoryContainer*>::iterator it = manager.pickupInventoryContainers_.find(pickup);
            if(it != manager.pickupInventoryContainers_.end())
                delete it->second;
            manager.pickupInventoryContainers_.erase(pickup);

            manager.updateGUI(); // Tell the PickupInventory that something has changed.
        }
    }

    /**
    @brief
        Get the PickupRepresentation of an input Pickupable.
        This method spares us the hassle to export the PickupIdentifier class to lua.
    @param pickup
        The number identifying the Pickupable whose PickupRepresentation should be returned.
    @return
        Returns the PickupRepresentation of the input Pickupable or NULL if an error occurred.
    */
    orxonox::PickupRepresentation* PickupManager::getPickupRepresentation(uint32_t pickup)
    {
        // Clear and rebuild the representationsNetworked_ list.
        //TODO: Better solution?
        this->representationsNetworked_.clear();
        for(ObjectList<PickupRepresentation>::iterator it = ObjectList<PickupRepresentation>::begin(); it != ObjectList<PickupRepresentation>::end(); ++it)
            this->representationsNetworked_[it->getObjectID()] = *it;

        // Get the container belonging to the input pickup, if not found return the default representation.
        std::map<uint32_t, PickupInventoryContainer*>::iterator it = this->pickupInventoryContainers_.find(pickup);
        if(it == this->pickupInventoryContainers_.end())
            return this->defaultRepresentation_;

        // Get the PickupRepresentation of the input pickup (through the objecId of the representation stored in the PickupInventoryContainer belonging to the pickup), if not found return the default representation.
        std::map<uint32_t, PickupRepresentation*>::iterator it2 = this->representationsNetworked_.find(it->second->representationObjectId);
        if(it2 == this->representationsNetworked_.end())
            return this->defaultRepresentation_;

        return it2->second;
    }

    /**
    @brief
        Get the number of pickups currently picked up by the player.
        This method is used in lua to populate the PickupInventory. The intended usage is to call this method to reset the iterator of the list of PickupInventoryContainers and then use popPickup() to get the individual PickupInventoryContainers.
    @return
        Returns the number of the players picked up Pickupables.
    */
    int PickupManager::getNumPickups(void)
    {
        this->pickupsIterator_ = this->pickupInventoryContainers_.begin(); // Reset iterator.

        return this->pickupInventoryContainers_.size();
    }

    /**
    @brief
        Drop the input Pickupable.
        This method checks whether the input Pickupable still exists and drops it, if so.
    @param pickup
        The identifier of the Pickupable to be dropped.
    */
    void PickupManager::dropPickup(uint32_t pickup)
    {
        // If we're either server or standalone and the list of pickups is not empty, we find and drop the input pickup.
        if(GameMode::isMaster())
        {
            if(this->pickups_.empty())
                return;
            Pickupable* pickupable = this->pickups_.find(pickup)->second->get();
            if(pickupable != NULL)
                pickupable->drop();
        }
        // If we're neither server nor standalone we drop the pickup by calling dropPickupNetworked() of the PickupManager on the server.
        else
        {
            callStaticNetworkFunction(PickupManager::dropPickupNetworked, 0, pickup);
        }
    }

    /**
    @brief
        Helper method to drop the input pickup on the server.
        Static method that is used by clients to instruct the server to drop the input pickup.
    @param pickup
        The identifier of the Pickupable to be dropped.
    */
    /*static*/ void PickupManager::dropPickupNetworked(uint32_t pickup)
    {
        if(GameMode::isServer()) // Obviously we only want to do this on the server.
        {
            PickupManager& manager = PickupManager::getInstance();
            manager.dropPickup(pickup);
        }
    }

    /**
    @brief
        Use (or unuse) the input Pickupable.
        This method checks whether the input Pickupable still exists and uses (or unuses) it, if so,
    @param pickup
        The identifier of the Pickupable to be used (or unused).
    @param use
        If true the input Pickupable is used, if false it is unused.
    */
    void PickupManager::usePickup(uint32_t pickup, bool use)
    {
        // If we're either server or standalone and the list of pickups is not empty, we find and change the used status of the input pickup.
        if(GameMode::isMaster())
        {
            if(this->pickups_.empty())
                return;
            Pickupable* pickupable = this->pickups_.find(pickup)->second->get();
            if(pickupable != NULL)
                pickupable->setUsed(use);
        }
        // If we're neither server nor standalone we change the used status of the pickup by calling usePickupNetworked() of the PickupManager on the server.
        else
        {
            callStaticNetworkFunction(PickupManager::usePickupNetworked, 0, pickup, use);
        }
    }

    /**
    @brief
        Helper method to use (or unuse) the input Pickupable on the server.
        Static method that is used by clients to instruct the server to use (or unuse) the input pickup.
    @param pickup
        The identifier of the Pickupable to be used (or unused).
    @param use
        If true the input Pickupable is used, if false it is unused.
    */
    /*static*/ void PickupManager::usePickupNetworked(uint32_t pickup, bool use)
    {
        if(GameMode::isServer())
        {
            PickupManager& manager = PickupManager::getInstance();
            manager.usePickup(pickup, use);
        }
    }

    /**
    @brief
        Updates the PickupInventory GUI.
        Also loads the PickupInventory GUI if is hasn't been done already.
    */
    inline void PickupManager::updateGUI(void)
    {
        // We only need to update (and load) the GUI if this host shows graphics.
        if(GameMode::showsGraphics())
        {
            if(!this->guiLoaded_) // If the GUI hasn't been loaded, yet, we load it.
            {
                GUIManager::getInstance().loadGUI(PickupManager::guiName_s);
                this->guiLoaded_ = true;
            }

            // Update the GUI.
            GUIManager::getInstance().getLuaState()->doString(PickupManager::guiName_s + ".update()");
        }
    }

    /**
    @brief
        Get a new index for a Pickupable.
        This will work as long as the number of Pickupables that are picked up is sufficiently small and as long as they don't exist forever.
    @return
        Returns the new index.
    */
    uint32_t PickupManager::getPickupIndex(void)
    {
        if(this->pickupHighestIndex_ == uint32_t(~0x0)-1) // If we've reached the highest possible number, we wrap around.
            this->pickupHighestIndex_ = 0;
        return this->pickupHighestIndex_++;
    }

}
