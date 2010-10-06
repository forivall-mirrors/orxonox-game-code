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

#include "util/Convert.h"
#include "util/ScopedSingletonManager.h"
#include "core/CoreIncludes.h"
#include "core/LuaState.h"
#include "core/GUIManager.h"
#include "core/Identifier.h"
#include "network/Host.h"
#include "network/NetworkFunction.h"
#include "interfaces/PickupCarrier.h"
#include "infos/PlayerInfo.h"
#include "worldentities/pawns/Pawn.h"
#include "CollectiblePickup.h"
#include "PickupRepresentation.h"

#include "ToluaBindPickup.h"

namespace orxonox
{
    // Register tolua_open function when loading the library
    DeclareToluaInterface(Pickup);

    ManageScopedSingleton(PickupManager, ScopeID::Root, false);

    /*static*/ const std::string PickupManager::guiName_s = "PickupInventory";

    registerStaticNetworkFunction(PickupManager::pickupChangedUsedNetwork);
    registerStaticNetworkFunction(PickupManager::pickupChangedPickedUpNetwork);
    registerStaticNetworkFunction(PickupManager::dropPickupNetworked);
    registerStaticNetworkFunction(PickupManager::usePickupNetworked);

    /**
    @brief
        Constructor. Registers the PickupManager and creates the default PickupRepresentation.
    */
    PickupManager::PickupManager() : guiLoaded_(false), pickupIndex_(0), defaultRepresentation_(NULL)
    {
        RegisterObject(PickupManager);

        this->defaultRepresentation_ = new PickupRepresentation();

        COUT(3) << "PickupManager created." << std::endl;
    }

    /**
    @brief
        Destructor.
        Destroys the default PickupRepresentation.
    */
    PickupManager::~PickupManager()
    {
        if(this->defaultRepresentation_ != NULL)
            this->defaultRepresentation_->destroy();

        this->representations_.clear();

        //TODO: Destroy properly...

        COUT(3) << "PickupManager destroyed." << std::endl;
    }

    /**
    @brief
        Registers a PickupRepresentation together with the PickupIdentifier of the Pickupable the PickupRepresentation represents.
        For every type of Pickupable (uniquely idnetified by a PickupIdentifier) there can be one (and just one) PickupRepresentation registered.
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

        if(!this->representations_.empty() && this->representations_.find(identifier) != this->representations_.end()) // If the Pickupable already has a Representation registered.
            return false;

        this->representations_[identifier] = representation;

        COUT(4) << "PickupRepresentation &" << representation << " registered with the PickupManager." << std::endl;
        return true;
    }

    bool PickupManager::registerRepresentation(PickupRepresentation* representation)
    {
        assert(representation);

        if(!this->representationsNetworked_.empty() && this->representationsNetworked_.find(representation->getObjectID()) != this->representationsNetworked_.end()) // If the PickupRepresentation is already registered.
            return false;

        this->representationsNetworked_[representation->getObjectID()] = representation;
        return true;
    }

    /**
    @brief
        Unegisters a PickupRepresentation together with the PickupIdentifier of the Pickupable the PickupRepresentation represents.
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
        if(it == this->representations_.end()) //!< If the Pickupable is not registered in the first place.
            return false;

        this->representations_.erase(it);

        COUT(4) << "PickupRepresentation &" << representation << " unregistered with the PickupManager." << std::endl;
        return true;
    }

    bool PickupManager::unregisterRepresentation(PickupRepresentation* representation)
    {
        assert(representation);

        std::map<uint32_t, PickupRepresentation*>::iterator it = this->representationsNetworked_.find(representation->getObjectID());
        if(it == this->representationsNetworked_.end()) //!< If the Pickupable is not registered in the first place.
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
    PickupRepresentation* PickupManager::getRepresentation(const PickupIdentifier* identifier)
    {
        std::map<const PickupIdentifier*, PickupRepresentation*, PickupIdentifierCompare>::iterator it = this->representations_.find(identifier);
        if(it == this->representations_.end())
        {
            COUT(4) << "PickupManager::getRepresentation() returned default representation." << std::endl;
            return this->defaultRepresentation_;
        }

        return it->second;
    }

    /**
    @brief
        Get the PickupRepresentation of an input Pickupable.
        This method spares us the hassle to export the PickupIdentifier class to lua.
    @param pickup
        The Pickupable whose PickupRepresentation should be returned.
    @return
        Returns the PickupRepresentation of the input Pickupable or NULL if an error occurred.
    */
    orxonox::PickupRepresentation* PickupManager::getPickupRepresentation(uint64_t pickup)
    {
        this->representationsNetworked_.clear();
        for(ObjectList<PickupRepresentation>::iterator it = ObjectList<PickupRepresentation>::begin(); it != ObjectList<PickupRepresentation>::end(); ++it)
            this->representationsNetworked_[it->getObjectID()] = *it;

        std::map<uint64_t, PickupInventoryContainer*>::iterator it = this->pickupInventoryContainers_.find(pickup);
        if(it == this->pickupInventoryContainers_.end())
            return this->defaultRepresentation_;

        std::map<uint32_t, PickupRepresentation*>::iterator it2 = this->representationsNetworked_.find(it->second->representationObjectId);
        if(it2 == this->representationsNetworked_.end())
            return this->defaultRepresentation_;

        return it2->second;
    }

    /**
    @brief
        Update the list of picked up Pickupables and get the number of Pickupables in the list.
        This method is used in lua to populate the PickupInventory. The intended usage is to call this method to update the list of Pickupables and then use popPickup() to get the individual Pickupables.
    @return
        Returns the number of the players picked up Pickupables.
    */
    int PickupManager::getNumPickups(void)
    {
        this->pickupsIterator_ = this->pickupInventoryContainers_.begin();
        return this->pickupInventoryContainers_.size();
    }

    /**
    @brief
        Drop the input Pickupable.
        This method checks whether the inout Pickupable still exists and drops it, if so.
    @param pickup
        The Pickupable to be dropped.
    */
    void PickupManager::dropPickup(uint64_t pickup)
    {
        if(GameMode::isMaster() && !this->pickups_.empty())
        {
            Pickupable* pickupable = this->pickups_.find(pickup)->second->get();
            if(pickupable != NULL)
                pickupable->drop();
        }
        else
        {
            callStaticNetworkFunction(PickupManager::dropPickupNetworked, 0, pickup);
        }
    }

    /*static*/ void PickupManager::dropPickupNetworked(uint64_t pickup)
    {
        if(GameMode::isServer())
        {
            PickupManager& manager = PickupManager::getInstance();
            if(manager.pickups_.empty())
                return;
            Pickupable* pickupable = manager.pickups_.find(pickup)->second->get();
            if(pickupable != NULL)
                pickupable->drop();
        }
    }

    /**
    @brief
        Use (or unuse) the input Pickupable.
        This method checks whether the input Pickupable still exists and uses (or unuses) it, if so,
    @param pickup
        The Pickupable to be used (or unused).
    @param use
        If true the input Pickupable is used, if false it is unused.
    */
    void PickupManager::usePickup(uint64_t pickup, bool use)
    {
        if(GameMode::isMaster() && !this->pickups_.empty())
        {
            Pickupable* pickupable = this->pickups_.find(pickup)->second->get();
            if(pickupable != NULL)
                pickupable->setUsed(use);
        }
        else
        {
            callStaticNetworkFunction(PickupManager::usePickupNetworked, 0, pickup, use);
        }
    }

    /*static*/ void PickupManager::usePickupNetworked(uint64_t pickup, bool use)
    {
        if(GameMode::isServer())
        {
            PickupManager& manager = PickupManager::getInstance();
            if(manager.pickups_.empty())
                return;
            Pickupable* pickupable = manager.pickups_.find(pickup)->second->get();
            if(pickupable != NULL)
                pickupable->setUsed(use);
        }
    }

    /**
    @brief
        Check whether the input Pickupable is valid, meaning that it is in the PickupManager's list and still exists.
    @param pickup
        The Pickupable.
    @return
        Returns true if the input Pickupable is still valid, false if not.
    */
    bool PickupManager::isValidPickup(uint64_t pickup)
    {
        return this->pickups_.find(pickup) != this->pickups_.end();
    }

    void PickupManager::pickupChangedUsed(Pickupable* pickup, bool used)
    {
        assert(pickup);

        if(!GameMode::isMaster()) // If this is neither standalone nor the server.
            return;

        CollectiblePickup* collectible = orxonox_cast<CollectiblePickup*>(pickup);
        // If the Picupable is part of a PickupCollection it isn't displayed in the PickupInventory, just the PickupCollection is.
        if(collectible != NULL && collectible->isInCollection())
            return;

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

        std::map<Pickupable*, uint64_t>::iterator it = this->indexes_.find(pickup);
        assert(it != this->indexes_.end());

        uint64_t index = it->second;
        if(GameMode::isStandalone() || Host::getPlayerID() == clientId)
        {
            PickupManager::pickupChangedUsedNetwork(index, used, pickup->isUsable(), pickup->isUnusable());
        }
        else
        {
            callStaticNetworkFunction(PickupManager::pickupChangedUsedNetwork, clientId, index, used, pickup->isUsable(), pickup->isUnusable());
        }
    }

    /*static*/ void PickupManager::pickupChangedUsedNetwork(uint64_t pickup, bool inUse, bool usable, bool unusable)
    {
        PickupManager& manager = PickupManager::getInstance();
        if(manager.pickupInventoryContainers_.find(pickup) == manager.pickupInventoryContainers_.end())
        {
            COUT(1) << "Error: Pickupable &(" << pickup << ") was not registered with PickupManager for the PickupInventory, when it changed used." << std::endl;
            return;
        }

        manager.pickupInventoryContainers_[pickup]->inUse = inUse;
        manager.pickupInventoryContainers_[pickup]->usable = usable;
        manager.pickupInventoryContainers_[pickup]->unusable = unusable;

        manager.updateGUI();
    }

    void PickupManager::pickupChangedPickedUp(Pickupable* pickup, bool pickedUp)
    {
        assert(pickup);

        if(!GameMode::isMaster())
            return;

        CollectiblePickup* collectible = orxonox_cast<CollectiblePickup*>(pickup);
        // If the Picupable is part of a PickupCollection it isn't displayed in the PickupInventory, just the PickupCollection is.
        if(collectible != NULL && collectible->isInCollection())
            return;

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

        uint64_t index = 0;
        if(pickedUp)
        {
            index = this->getPickupIndex();
            this->indexes_[pickup] = index;
            this->pickups_[index] = new WeakPtr<Pickupable>(pickup);
        }
        else
        {
            std::map<Pickupable*, uint64_t>::iterator it = this->indexes_.find(pickup);
            index = it->second;
            WeakPtr<Pickupable>* ptr = this->pickups_[index];
            this->indexes_.erase(it);
            this->pickups_.erase(index);
            delete ptr;
        }

        if(GameMode::isStandalone() || Host::getPlayerID() == clientId)
        {
            PickupManager::pickupChangedPickedUpNetwork(index, pickup->isUsable(), this->representations_[pickup->getPickupIdentifier()]->getObjectID(), pickedUp);
        }
        else
        {
            callStaticNetworkFunction(PickupManager::pickupChangedPickedUpNetwork, clientId, index, pickup->isUsable(), this->representations_[pickup->getPickupIdentifier()]->getObjectID(), pickedUp);
        }

    }

    /*static*/ void PickupManager::pickupChangedPickedUpNetwork(uint64_t pickup, bool usable, uint32_t representationObjectId, bool pickedUp)
    {
        PickupManager& manager = PickupManager::getInstance();
        if(pickedUp)
        {
            PickupInventoryContainer* container = new PickupInventoryContainer;
            container->pickup = pickup;
            container->inUse = false;
            container->pickedUp = pickedUp;
            container->usable = usable;
            container->unusable = false;
            container->representationObjectId = representationObjectId;
            manager.pickupInventoryContainers_.insert(std::pair<uint64_t, PickupInventoryContainer*>(pickup, container));

            if(GameMode::showsGraphics())
            {
                if(!manager.guiLoaded_)
                {
                    GUIManager::getInstance().loadGUI(PickupManager::guiName_s);
                    manager.guiLoaded_ = true;
                }
                GUIManager::getInstance().getLuaState()->doString(PickupManager::guiName_s + ".update()");
            }
        }
        else
        {
            std::map<uint64_t, PickupInventoryContainer*>::iterator it = manager.pickupInventoryContainers_.find(pickup);
            if(it != manager.pickupInventoryContainers_.end())
                delete it->second;
            manager.pickupInventoryContainers_.erase(pickup);

            manager.updateGUI();
        }
    }

    inline void PickupManager::updateGUI(void)
    {
        if(GameMode::showsGraphics())
        {
            if(!this->guiLoaded_)
            {
                GUIManager::getInstance().loadGUI(PickupManager::guiName_s);
                this->guiLoaded_ = true;
            }
            GUIManager::getInstance().getLuaState()->doString(PickupManager::guiName_s + ".update()");
        }
    }

    uint64_t PickupManager::getPickupIndex(void)
    {
        if(this->pickupIndex_ == uint64_t(~0x0)-1)
            this->pickupIndex_ = 0;
        return this->pickupIndex_++;
    }

}
