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
        //TODO: Manage Pickup GUI.
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
            int getNumPickups(void);
            orxonox::Pickupable* popPickup(void) { return (this->pickupsIterator_++)->first; }
            orxonox::PickupRepresentation* getPickupRepresentation(orxonox::Pickupable* pickup) { if(pickup != NULL) return this->getRepresentation(pickup->getPickupIdentifier()); return NULL; }

            void dropPickup(orxonox::Pickupable* pickup);
            void usePickup(orxonox::Pickupable* pickup, bool use);
            bool isValidPickup(orxonox::Pickupable* pickup) { std::map<Pickupable*, WeakPtr<Pickupable> >::iterator it = this->pickupsList_.find(pickup); if(it == this->pickupsList_.end()) return false; return it->second.get() != NULL; }
            // tolua_end
            
        private:
            static PickupManager* singletonPtr_s;
            static const std::string guiName_s;
            
            PickupRepresentation* defaultRepresentation_; //!< The default PickupRepresentation.
            std::map<const PickupIdentifier*, PickupRepresentation*, PickupIdentifierCompare> representations_; //!< Map linking PickupIdentifiers (representing types if Pickupables) and PickupRepresentations.

            std::map<Pickupable*, WeakPtr<Pickupable> > pickupsList_;
            std::map<Pickupable*, WeakPtr<Pickupable> >::iterator pickupsIterator_;

            std::vector<PickupCarrier*>* getAllCarriers(PickupCarrier* carrier);
        
    }; // tolua_export
    
} // tolua_export

#endif // _PickupManager_H__
