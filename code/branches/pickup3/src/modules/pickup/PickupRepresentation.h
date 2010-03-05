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

#ifndef _PickupRepresentation_H__
#define _PickupRepresentation_H__

#include "PickupPrereqs.h"

#include "core/BaseObject.h"

#include "core/XMLPort.h"
#include "pickup/PickupIdentifier.h"
#include "interfaces/Pickupable.h"
#include "pickup/PickupSpawner.h"
#include "worldentities/StaticEntity.h"

namespace orxonox
{

    class _PickupExport PickupRepresentation : public BaseObject
    {
        
        public:
            PickupRepresentation();
            PickupRepresentation(BaseObject* creator);
            virtual ~PickupRepresentation();
            
            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            
            inline void setName(const std::string& name)
                { this->name_ = name; }
            inline void setDescription(const std::string& description)
                { this->description_ = description; }
            inline void setSpawnerTemplate(const std::string& spawnerTemplate)
                { this->spawnerTemplate_ = spawnerTemplate; }
            inline void setSpawnerRepresentation(StaticEntity* representation)
                { this->spawnerRepresentation_ = representation; }
            inline void setPickup(Pickupable* pickup)
                { this->pickup_ = pickup; }
                
            inline const std::string& getName(void)
                { return this->name_; }
            inline const std::string& getDescription(void)
                { return this->description_; }
            inline const std::string& getSpawnerTemplate(void)
                { return this->spawnerTemplate_; }
            inline const StaticEntity* getSpawnerRepresentationIndex(unsigned int index)
                { if(index == 0) return this->spawnerRepresentation_; return NULL; }
            inline const Pickupable* getPickup(unsigned int index)
                { if(index == 0) return this->pickup_; return NULL; }
                
            StaticEntity* getSpawnerRepresentation(PickupSpawner* spawner);
        
        private:
            void initialize(void);
            StaticEntity* getDefaultSpawnerRepresentation(PickupSpawner* spawner);
            
            std::string name_;
            std::string description_;
            std::string spawnerTemplate_;
            StaticEntity* spawnerRepresentation_;
            
            Pickupable* pickup_;
            
            
    };

}
    
#endif // _PickupRepresentation_H__
