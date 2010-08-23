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
 *      ...
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
#include "core/ScopedSingletonManager.h"
#include "core/Identifier.h"
#include "util/Convert.h"
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

    /**
    @brief
        Constructor. Registers the PickupManager and creates the default PickupRepresentation.
    */
    PickupManager::PickupManager() : defaultRepresentation_(NULL)
    {
        RegisterRootObject(PickupManager);

        //TODO: This doesn't work, yet.
        if( GameMode::showsGraphics() )
        {
            GUIManager::getInstance().loadGUI(PickupManager::guiName_s);
        }
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
        if(identifier == NULL || representation == NULL || this->representations_.find(identifier) != this->representations_.end()) //!< If the Pickupable already has a Representation registered.
            return false;

        this->representations_[identifier] = representation;

        COUT(4) << "PickupRepresentation " << representation << " registered with the PickupManager." << std::endl;
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
        if(identifier == NULL || representation == NULL)
            return false;

        std::map<const PickupIdentifier*, PickupRepresentation*, PickupIdentifierCompare>::iterator it = this->representations_.find(identifier);
        if(it == this->representations_.end()) //!< If the Pickupable is not registered in the first place.
            return false;

        this->representations_.erase(it);

        COUT(4) << "PickupRepresentation " << representation << " unregistered with the PickupManager." << std::endl;
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
    orxonox::PickupRepresentation* PickupManager::getPickupRepresentation(orxonox::Pickupable* pickup)
    {
        if(pickup != NULL)
            return this->getRepresentation(pickup->getPickupIdentifier());
        
        return NULL;
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
        this->pickupsList_.clear(); // Clear the list if Pickupables.

        PlayerInfo* player = GUIManager::getInstance().getPlayer(PickupManager::guiName_s);
        PickupCarrier* carrier = NULL;
        if (player != NULL)
            carrier =  orxonox_cast<PickupCarrier*>(player->getControllableEntity());
        else
            return 0;

        std::vector<PickupCarrier*>* carriers = this->getAllCarriers(carrier); // Get a list of all the entities which carry the players Pickupables.
        // Iterate through all the carriers and add their Pickupable to the list.
        for(std::vector<PickupCarrier*>::iterator it = carriers->begin(); it != carriers->end(); it++)
        {
            std::set<Pickupable*> pickups = (*it)->getPickups();
            for(std::set<Pickupable*>::iterator pickup = pickups.begin(); pickup != pickups.end(); pickup++)
            {
                CollectiblePickup* collectible = orxonox_cast<CollectiblePickup*>(*pickup);
                // If the Pickupable is in a PickupCollection it is not added to the list and thus not displayed in the PickupInventory.
                if(collectible == NULL || !collectible->isInCollection())
                    this->pickupsList_.insert(std::pair<Pickupable*, WeakPtr<Pickupable> >(*pickup, WeakPtr<Pickupable>(*pickup)));
            }
        }
        delete carriers;

        this->pickupsIterator_ = this->pickupsList_.begin(); //Set the iterator to the start of the list.
        return this->pickupsList_.size();
    }

    /**
    @brief
        Helper method. Get all the PickupCarriers that carry Pickupables, recursively.
    @param carrier
        The PickupCarrier whose children should be added to the list of PickupCarriers.
    @param carriers
        The list of PickupCarriers, used in "recursive-mode".
        For the first instance this is just NULL (which is default and can be omitted) upon which a new list is allocated.
    @return
        Returns the list of PickupCarriers.
    */
    std::vector<PickupCarrier*>* PickupManager::getAllCarriers(PickupCarrier* carrier, std::vector<PickupCarrier*>* carriers)
    {
        if(carriers == NULL) // Create a new list if no list was passed.
            carriers = new std::vector<PickupCarrier*>();

        carriers->insert(carriers->end(), carrier); // Add the input PickupCarrier to the list.

        std::vector<PickupCarrier*>* children = carrier->getCarrierChildren(); // Get the children of the input PickupCarrier.
        // Iterate through the children and add them (and their children) to the list by recursively executing getAllCarriers().
        for(std::vector<PickupCarrier*>::iterator it = children->begin(); it != children->end(); it++)
            this->getAllCarriers(*it, carriers);
        delete children;

        return carriers;
    }

    /**
    @brief
        Drop the input Pickupable.
        This method checks whether the inout Pickupable still exists and drops it, if so.
    @param pickup
        The Pickupable to be dropped.
    */
    void PickupManager::dropPickup(orxonox::Pickupable* pickup)
    {
        if(pickup == NULL)
            return;

        std::map<Pickupable*, WeakPtr<Pickupable> >::iterator it = this->pickupsList_.find(pickup); // Get the WeakPointer of the Pickupable.
        // If either the input Pickupable is not in the PickupManagers list or it no longer exists, the method returns.
        if(it == this->pickupsList_.end() || it->second.get() == NULL)
            return;

        pickup->drop(); // The Pickupable is dropped.
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
    void PickupManager::usePickup(orxonox::Pickupable* pickup, bool use)
    {
        if(pickup == NULL)
            return;
        
        std::map<Pickupable*, WeakPtr<Pickupable> >::iterator it = this->pickupsList_.find(pickup); // Get the WeakPointer of the Pickupable.
        // If either the input Pickupable is not in the PickupManagers list or it no longer exists, the method returns.
        if(it == this->pickupsList_.end() || it->second.get() == NULL)
            return;

        pickup->setUsed(use); // The Pickupable is used (or unused).
    }

    /**
    @brief
        Check whether the input Pickupable is valid, meaning that it is in the PickupManager's list and still exists.
    @param pickup
        The Pickupable.
    @return
        Returns true if the input Pickupable is still valid, false if not.
    */
    bool PickupManager::isValidPickup(orxonox::Pickupable* pickup)
    {
        std::map<Pickupable*, WeakPtr<Pickupable> >::iterator it = this->pickupsList_.find(pickup); // Get the WeakPointer of the Pickupable.
        if(it == this->pickupsList_.end()) // If the Pickupable is not in the PickupManager's list.
            return false;
        return it->second.get() != NULL; // Returns whether the Pickupable still exists.
    }

}
