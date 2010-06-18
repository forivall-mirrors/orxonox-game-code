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

    int PickupManager::getNumPickups(void)
    {
        this->pickupsList_.clear();

        PlayerInfo* player = GUIManager::getInstance().getPlayer(PickupManager::guiName_s);
        PickupCarrier* carrier = NULL;
        if (player != NULL)
            carrier =  dynamic_cast<PickupCarrier*>(player->getControllableEntity());
        else
            return 0;

        std::vector<PickupCarrier*>* carriers = this->getAllCarriers(carrier);
        for(std::vector<PickupCarrier*>::iterator it = carriers->begin(); it != carriers->end(); it++)
        {
            std::set<Pickupable*> pickups = (*it)->getPickups();
            for(std::set<Pickupable*>::iterator pickup = pickups.begin(); pickup != pickups.end(); pickup++)
            {
                this->pickupsList_.insert(std::pair<Pickupable*, WeakPtr<Pickupable> >(*pickup, WeakPtr<Pickupable>(*pickup)));
            }
        }
        delete carriers;

        this->pickupsIterator_ = this->pickupsList_.begin();
        return this->pickupsList_.size();
    }

    std::vector<PickupCarrier*>* PickupManager::getAllCarriers(PickupCarrier* carrier)
    {
        //TODO: More efficiently.
        std::vector<PickupCarrier*>* carriers = new std::vector<PickupCarrier*>();
        carriers->insert(carriers->end(), carrier);
        std::vector<PickupCarrier*>* children = carrier->getCarrierChildren();
        for(std::vector<PickupCarrier*>::iterator it = children->begin(); it != children->end(); it++)
        {
            std::vector<PickupCarrier*>* childrensChildren = this->getAllCarriers(*it);
            for(std::vector<PickupCarrier*>::iterator it2 = childrensChildren->begin(); it2 != childrensChildren->end(); it2++)
            {
                carriers->insert(carriers->end(), *it2);
            }
            delete childrensChildren;
        }
        delete children;
        return carriers;
    }

    void PickupManager::dropPickup(orxonox::Pickupable* pickup)
    {
        std::map<Pickupable*, WeakPtr<Pickupable> >::iterator it = this->pickupsList_.find(pickup);
        if(pickup == NULL || it == this->pickupsList_.end() || it->second.get() == NULL)
            return;

        if(!pickup->isPickedUp())
            return;

        PickupCarrier* carrier = pickup->getCarrier();
        if(pickup != NULL && carrier != NULL)
        {
            pickup->drop(carrier);
        }
    }

    void PickupManager::usePickup(orxonox::Pickupable* pickup, bool use)
    {
        std::map<Pickupable*, WeakPtr<Pickupable> >::iterator it = this->pickupsList_.find(pickup);
        if(pickup == NULL || it == this->pickupsList_.end() || it->second.get() == NULL)
            return;

        if(!pickup->isPickedUp())
            return;

        PickupCarrier* carrier = pickup->getCarrier();
        if(pickup != NULL && carrier != NULL)
            pickup->setUsed(use);
    }

}
