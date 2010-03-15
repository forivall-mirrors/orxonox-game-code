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
    @file
    @brief Definition of the PickupManager class.
*/

#ifndef _PickupManager_H__
#define _PickupManager_H__

#include "PickupPrereqs.h"

#include <map>
#include "util/Singleton.h"
#include "pickup/PickupIdentifier.h"
#include "PickupRepresentation.h"

#include "core/OrxonoxClass.h"

namespace orxonox
{
    //TODO: Actually utilize this.
    struct PickupCarrierNode
    {
        Identifier* identifier;
        std::set<PickupCarrierNode*> children;
    };

    /**
    @brief
        Manages Pickupables.
        In essence has two tasks to fulfill. Firstly it must link Pickupables (through their PickupIdentifiers) and their PickupRepresentations. Secondly it manages the Pickup GUI.
        //TODO: Manage Pickup GUI.
    @author
        Damian 'Mozork' Frick
    */
    class _PickupExport PickupManager : public Singleton<PickupManager>, public OrxonoxClass
    {
        friend class Singleton<PickupManager>;
        
        public:
            PickupManager();
            virtual ~PickupManager();
            
            static PickupManager& getInstance() { return Singleton<PickupManager>::getInstance(); }
            
            bool registerRepresentation(const PickupIdentifier* identifier, PickupRepresentation* representation); //!< Registers a PickupRepresentation together with the PickupIdentifier of the Pickupable the PickupRepresentation represents.
            PickupRepresentation* getRepresentation(const PickupIdentifier* identifier); //!< Get the PickupRepresentation representing the Pickupable with the input PickupIdentifier.
            
            //TODO: Delete or utilitze this.
            //bool registerCarrier(Identifier* parent, )
            
        private:
            static PickupManager* singletonPtr_s;
            
            PickupRepresentation* defaultRepresentation_; //!< The default PickupRepresentation.
            std::map<const PickupIdentifier*, PickupRepresentation*, PickupIdentifierCompare> representations_; //!< Map linking PickupIdentifiers (representing types if Pickupables) and PickupRepresentations.
            
            //TODO: Delete or utilize this.
            PickupCarrierNode* pickupCarrierStructure_;
        
    };
    
}

#endif // _PickupManager_H__
