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
    @file PickupRepresentation.cc
    @brief Implementation of the PickupRepresentation class.
*/

#include "PickupRepresentation.h"

#include "core/CoreIncludes.h"
#include "graphics/Billboard.h"
#include "util/StringUtils.h"
#include "PickupManager.h"

namespace orxonox
{
    
    CreateFactory(PickupRepresentation);
    
    /**
    @brief
        Constructor. Registers the object and initializes its member variables.
        This is primarily for use of the PickupManager in creating a default PickupRepresentation.
    */
    PickupRepresentation::PickupRepresentation() : BaseObject(NULL), spawnerRepresentation_(NULL), pickup_(NULL)
    {
        RegisterObject(PickupRepresentation);
        
        this->initialize();
    }
    
    /**
    @brief
        Default Constructor. Registers the object and initializes its member variables.
    */
    PickupRepresentation::PickupRepresentation(BaseObject* creator) : BaseObject(creator), spawnerRepresentation_(NULL), pickup_(NULL)
    {
        RegisterObject(PickupRepresentation);
        
        this->initialize();
    }
    
    /**
    @brief
        Destructor.
    */
    PickupRepresentation::~PickupRepresentation()
    {
        if(this->spawnerRepresentation_ != NULL)
            this->spawnerRepresentation_->destroy();
        
        if(this->pickup_ != NULL)
            PickupManager::getInstance().unregisterRepresentation(this->pickup_->getPickupIdentifier(), this);
    }
    
    /**
    @brief
        Initializes the member variables of this PickupRepresentation.
    */
    void PickupRepresentation::initialize(void)
    {
        this->description_ = "This is a pickup.";
        this->name_ = "Pickup";
        this->spawnerTemplate_ = "";
        this->inventoryRepresentation_ = "Default";
    }
    
    /**
    @brief
        Method for creating a PickupRepresentation object through XML.
    */
    void PickupRepresentation::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PickupRepresentation, XMLPort, xmlelement, mode);
        
        XMLPortParam(PickupRepresentation, "pickupName", setPickupName, getPickupName, xmlelement, mode);
        XMLPortParam(PickupRepresentation, "pickupDescription", setPickupDescription, getPickupDescription, xmlelement, mode);
        XMLPortParam(PickupRepresentation, "spawnerTemplate", setSpawnerTemplate, getSpawnerTemplate, xmlelement, mode);
        XMLPortParam(PickupRepresentation, "inventoryRepresentation", setInventoryRepresentation, getInventoryRepresentation, xmlelement, mode);
        XMLPortObject(PickupRepresentation, Pickupable, "pickup", setPickup, getPickup, xmlelement, mode);
        XMLPortObject(PickupRepresentation, StaticEntity, "spawner-representation", setSpawnerRepresentation, getSpawnerRepresentationIndex, xmlelement, mode);
        
        PickupManager::getInstance().registerRepresentation(this->pickup_->getPickupIdentifier(), this); //!< Registers the PickupRepresentation with the PickupManager through the PickupIdentifier of the Pickupable it represents.
        
        if(this->spawnerRepresentation_ != NULL)
            this->spawnerRepresentation_->setVisible(false);
        
        COUT(4) << "PickupRepresentation created: name: '" << this->name_ << "', description: '" << this->description_ << "', spawnerTemplate: '" << this->spawnerTemplate_ << "'." << std::endl;
    }
    
    /**
    @brief
        Get a spawnerRepresentation for a specific PickupSpawner.
    @param spawner
        A pointer to the PickupSpawner.
    @return
        Returns a pointer to the StaticEntity.
    */
    StaticEntity* PickupRepresentation::getSpawnerRepresentation(PickupSpawner* spawner)
    {
        if(this->spawnerRepresentation_ == NULL)
        {
            COUT(4) << "PickupRepresentation: No spawner representation found." << std::endl;
            if(this->spawnerTemplate_ == BLANKSTRING)
            {
                COUT(4) << "PickupRepresentation: Spawner template is empty." << std::endl;
                //!< If neither spawnerRepresentation nor spawnerTemplate was specified
                return this->getDefaultSpawnerRepresentation(spawner);
            }
            this->addTemplate(this->spawnerTemplate_);
        }
        
        StaticEntity* representation = this->spawnerRepresentation_;
        representation->setVisible(true);
        
        this->addTemplate(this->spawnerTemplate_);
        this->spawnerRepresentation_->setVisible(false);
        
        return representation;
    }
    
    /**
    @brief
        Get the default spawnerRepresentation for a specific PickupSpawner.
        Helper method of internal use.
    @param spawner
        A pointer to the PickupSpawner.
    @return
        Returns a pointer to the StaticEntity.
    */
    //TODO: Possibility to define default representation through XML.
    StaticEntity* PickupRepresentation::getDefaultSpawnerRepresentation(PickupSpawner* spawner)
    {
        StaticEntity* representation = new StaticEntity(spawner);
        Billboard* sphere = new Billboard(spawner);
        sphere->setColour(ColourValue(0.95f, 0.85f, 0.27f));
        sphere->setMaterial("Sphere2");
        sphere->setScale(0.1f);
        Billboard* icon = new Billboard(spawner);
        icon->setColour(ColourValue(0.89f, 0.79f, 0.08f));
        icon->setMaterial("Asterix");
        icon->setScale(0.5);
        sphere->attach(icon);
        representation->attach(sphere);
        return representation;
    }
    
}
