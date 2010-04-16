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
#include "interfaces/PickupCarrier.h"
#include "infos/PlayerInfo.h"
#include "worldentities/pawns/Pawn.h"
#include "PickupRepresentation.h"

#include "ToluaBindPickup.h"

namespace orxonox
{
    // Register tolua_open function when loading the library
    DeclareToluaInterface(Pickup);
    
    ManageScopedSingleton(PickupManager, ScopeID::Graphics, false);
    
    /*static*/ const std::string PickupManager::guiName_s = "PickupInventory";
    
    /**
    @brief
        Constructor. Registers the PickupManager and creates the default PickupRepresentation.
    */
    PickupManager::PickupManager() : defaultRepresentation_(NULL)
    {
        RegisterRootObject(PickupManager);
        
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
    
    PickupCarrier* PickupManager::getPawn(void)
    {
        Pawn* pawn = dynamic_cast<Pawn*>(GUIManager::getInstance().getPlayer(PickupManager::guiName_s)->getControllableEntity());
        if(pawn == NULL)
            return NULL;
        return dynamic_cast<PickupCarrier*>(pawn);
    }
    
    int PickupManager::getNumCarrierChildren(PickupCarrier* carrier)
    {
        if(carrier == NULL)
            return 0;
        return carrier->getNumCarrierChildren();
    }
            
    PickupCarrier* PickupManager::getCarrierChild(int index, PickupCarrier* carrier)
    {
        if(carrier == NULL)
            return NULL;
        return carrier->getCarrierChild(index);
    }
    
    const std::string& PickupManager::getCarrierName(orxonox::PickupCarrier* carrier)
    {
        if(carrier == NULL)
            return BLANKSTRING;
        return carrier->getCarrierName();
    }
    
    PickupRepresentation* PickupManager::getPickupRepresentation(int index, PickupCarrier* carrier)
    {
        Pickupable* pickup = carrier->getPickup(index);
        if(pickup == NULL)
            return NULL;
        
        return this->getRepresentation(pickup->getPickupIdentifier());
    }
    
    int PickupManager::getNumPickups(PickupCarrier* carrier)
    {
        if(carrier == NULL)
            return 0;
        return carrier->getNumPickups();
    }
    
    void PickupManager::dropPickup(int index, PickupCarrier* carrier)
    {
        Pickupable* pickup = carrier->getPickup(index);
        if(pickup != NULL)
            carrier->drop(pickup);
    }
    
    void PickupManager::usePickup(int index, PickupCarrier* carrier, bool use)
    {
        Pickupable* pickup = carrier->getPickup(index);
        if(pickup != NULL)
            pickup->setUsed(use);
    }
    
}
