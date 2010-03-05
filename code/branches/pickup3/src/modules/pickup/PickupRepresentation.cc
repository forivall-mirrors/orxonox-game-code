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

#include "PickupRepresentation.h"

#include "core/CoreIncludes.h"
#include "PickupManager.h"
#include "graphics/Billboard.h"

namespace orxonox
{
    
    CreateFactory(PickupRepresentation);
    
    PickupRepresentation::PickupRepresentation() : BaseObject(this)
    {
        RegisterObject(PickupRepresentation);
        
        this->initialize();
    }
    
    PickupRepresentation::PickupRepresentation(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(PickupRepresentation);
        
        this->initialize();
    }
    
    PickupRepresentation::~PickupRepresentation()
    {
        
    }
    
    void PickupRepresentation::initialize(void)
    {
        this->description_ = "This is a pickup.";
        this->name_ = "Pickup";
        this->spawnerTemplate_ = "";
        this->spawnerRepresentation_ = NULL;
        this->pickup_ = NULL;
    }
    
    void PickupRepresentation::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PickupRepresentation, XMLPort, xmlelement, mode);
        
        XMLPortParam(PickupRepresentation, "name", setName, getName, xmlelement, mode);
        XMLPortParam(PickupRepresentation, "description", setDescription, getDescription, xmlelement, mode);
        XMLPortParam(PickupRepresentation, "spawnerTemplate", setSpawnerTemplate, getSpawnerTemplate, xmlelement, mode);
        XMLPortObject(PickupRepresentation, Pickupable, "pickup", setPickup, getPickup, xmlelement, mode);
        XMLPortObject(PickupRepresentation, StaticEntity, "spawner-representation", setSpawnerRepresentation, getSpawnerRepresentationIndex, xmlelement, mode);
        
        PickupManager::getInstance().registerRepresentation(*this->pickup_->getPickupIdentifier(), this);
    }
    
    StaticEntity* PickupRepresentation::getSpawnerRepresentation(PickupSpawner* spawner)
    {
        if(this->spawnerRepresentation_ == NULL)
        {
            COUT(4) << "PickupRepresentation: No spawner representation found." << std::endl;
            if(this->spawnerTemplate_ == "")
            {
                COUT(4) << "PickupRepresentation: Spawner template is empty." << std::endl;
                return this->getDefaultSpawnerRepresentation(spawner);
            }
            this->addTemplate(this->spawnerTemplate_);
        }
        StaticEntity* representation = this->spawnerRepresentation_;
        
        this->addTemplate(this->spawnerTemplate_);
        
        return representation;
    }
    
    StaticEntity* PickupRepresentation::getDefaultSpawnerRepresentation(PickupSpawner* spawner)
    {
        StaticEntity* representation = new StaticEntity(spawner);
        //TODO: Nicer...
        Billboard* billboard = new Billboard(spawner);
        billboard->setColour(ColourValue(1.0, 0.0, 0.0));
        billboard->setMaterial("Examples/Flare");
        representation->attach(billboard);
        return representation;
    }
    
}
