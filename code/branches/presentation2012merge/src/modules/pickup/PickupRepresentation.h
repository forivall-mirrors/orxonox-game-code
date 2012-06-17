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
    @file PickupRepresentation.h
    @brief Definition of the PickupRepresentation class.
    @ingroup Pickup
*/

#ifndef _PickupRepresentation_H__
#define _PickupRepresentation_H__

#include "PickupPrereqs.h"

#include "core/XMLPort.h"

#include "interfaces/Pickupable.h"
#include "pickup/PickupIdentifier.h"
#include "worldentities/StaticEntity.h"

#include "PickupSpawner.h"

#include "core/BaseObject.h"
#include "network/synchronisable/Synchronisable.h"

namespace orxonox // tolua_export
{ // tolua_export

    /**
    @brief
        The PickupRepresentation class represents a specific pickup type (identified by its @ref orxonox::PickupIdentifier "PickupIdentifier"). It defines the information displayed in the GUI (PickupInventory) and how @ref orxonox::PickupSpawner "PickupSpawners" that spawn the pickup type look like.
        They are created through XML and are registered with the @ref orxonox::PickupManager "PickupManager".

        Creating a PickupRepresentation in XML could look as follows:
        @code
        <PickupRepresentation
            name = "My awesome Pickup"
            description = "This is the most awesome Pickup ever to exist."
            spawnerTemplate = "awesomePickupRepresentation"
            inventoryRepresentation = "AwesomePickup"
        >
            <pickup>
                <SomePickup />
            </pickup>
        </PickupRepresentation>
        @endcode
        As you might have noticed, there is a parameter called <em>spawnerTemplate</em> and also another parameter called <em>inventoryRepresentation</em>. Let's first explain the second one, <em>inventoryRepresentation</em>.
        - The <b>inventoryRepresentation</b> specifies the image that is displayed in the PickupInventory for the specific type of @ref orxonox::Pickupable "Pickupable". More technically, it is the name of an image located in the <code>PickupInventory.imageset</code>, which in turn is located in <code>data_extern/gui/imagesets/</code>.
        - The <b>spawnerTemplate</b> specifies how the type of @ref orxonox::Pickupable "Pickupable" (or more precisely the @ref orxonox::PickupSpawner "PickupSpawner", that spawns that type of @ref orxonox::Pickupable "Pickupable") is displayed ingame. It is a @ref orxonox::Template "Template" defined in XML. The <em>spawnerTemplate</em> can be specified as follows (keep in mind, that the template needs to have been specified before the definition of the PickupRepresentation that uses it).
        @code
        <Template name="awesomePickupRepresentation">
            <PickupRepresentation>
                <spawner-representation>
                    <StaticEntity>
                        <attached>
                            <!-- Here you can put all the objects which define the look of the spawner. -->
                        </attached>
                    </StaticEntity>
                </spawner-representation>
            </PickupRepresentation>
        </Template>
        @endcode

        For the purpose of them working over the network, they are synchronised.

    @author
        Damian 'Mozork' Frick

    @ingroup Pickup
    */
    class _PickupExport PickupRepresentation // tolua_export
        : public BaseObject, public Synchronisable
    { // tolua_export

        public:
            PickupRepresentation(); //!< Constructor
            PickupRepresentation(BaseObject* creator); //!< Default constructor.
            virtual ~PickupRepresentation(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a PickupRepresentation object through XML.

            /**
            @brief Get the name of the Pickupable represented by this PickupRepresentation.
            @return Returns the name.
            */
            inline const std::string& getPickupName(void) const { return this->name_; } // tolua_export
            /**
            @brief Get the description of the Pickupable represented by this PickupRepresentation.
            @return Returns the description.
            */
            inline const std::string& getPickupDescription(void) const { return this->description_; } // tolua_export
            /**
            @brief Get the name of spawnerTemplate the Pickupable represented by this PickupRepresentation.
            @return Returns the name of the spawnerTemplate.
            */
            inline const std::string& getSpawnerTemplate(void) const
                { return this->spawnerTemplate_; }
            /**
            @brief Get the StaticEntity that defines how the PickupSpawner of the Pickupable represented by this PickupRepresentation looks like.
            @param index The index.
            @return Returns (for index = 0) a pointer to the StaticEntity. For index > 0 it returns NULL.
            */
            inline const StaticEntity* getSpawnerRepresentationIndex(unsigned int index) const
                { if(index == 0) return this->spawnerRepresentation_; return NULL; }
            /**
            @brief Get the name of the image representing the pickup in the PickupInventory.
            @return Returns the name of the image as a string.
            */
            inline const std::string& getInventoryRepresentation(void) const { return this->inventoryRepresentation_; } // tolua_export
            /**
            @brief Get the Pickupable represented by this PickupRepresentation.
            @param index The index.
            @return Returns (for index = 0) a pointer to the Pickupable. For index > 0 it returns NULL.
            */
            inline const Pickupable* getPickup(unsigned int index) const
                { if(index == 0) return this->pickup_; return NULL; }

            StaticEntity* createSpawnerRepresentation(PickupSpawner* spawner); //!< Create a spawnerRepresentation for a specific PickupSpawner.

        protected:
            /**
            @brief Set the name of the Pickupable represented by this PickupRepresentation.
            @param name The name.
            */
            inline void setPickupName(const std::string& name)
                { this->name_ = name; }
            /**
            @brief Set the description of the Pickupable represented by this PickupRepresentation.
            @param description The Description.
            */
            inline void setPickupDescription(const std::string& description)
                { this->description_ = description; }
            /**
            @brief Set the spawnerTemplate of the Pickupable represented by this PickupRepresentation.
                   The spawnerTemplate is a name of a template defined in XML that defines the StaticEntity that is the spawnerRepresentation of this PickupRepresentation.
            @param spawnerTemplate The name of the template.
            */
            inline void setSpawnerTemplate(const std::string& spawnerTemplate)
                { this->spawnerTemplate_ = spawnerTemplate; }
            /**
            @brief Set the StaticEntity that defines how the PickupSpawner of the Pickupable represented by this PickupRepresentation looks like.
                   This will be set by the spawnerTemplate. Setting it when creating the PickupRepresentation without creating a template and specifying its name will be futile, because through the course of the game many PickupSpawners for one specific pickup type may have to be created, thus the StaticEntity that is the spawnerRepresentation has to be generated (with the template) for every new PickupSpawner spawning the Pickupable represented by this PickupRepresentation. The spawnerRepresentation that is set here, however can only be used once.
            @param representation A pointer to the StaticEntity that is the spawnerRepresentation of this PickupRepresentation.
            */
            inline void setSpawnerRepresentation(StaticEntity* representation)
                { this->spawnerRepresentation_ = representation; }
            /**
            @brief Set the image representing the pickup in the PickupInventory.
            @param image A string with the name of the image representing the pickup.
            */
            inline void setInventoryRepresentation(const std::string& image)
                { this->inventoryRepresentation_ = image; }
            /**
            @brief Set the Pickupable that is represented by this PickupRepresentation.
            @param pickup A pointer to the Pickupable.
            */
            inline void setPickup(Pickupable* pickup)
                { this->pickup_ = pickup; }

        private:
            void initialize(void); //!< Initializes the member variables of this PickupRepresentation.
            StaticEntity* getDefaultSpawnerRepresentation(PickupSpawner* spawner); //!< Get the default spawnerRepresentation for a specific PickupSpawner.

            void registerVariables(void); //!< Registers the variables that need to be synchronised.

            std::string name_; //!< The name of the Pickupable represented by this PickupRepresentation.
            std::string description_; //!< The description of the Pickupable represented by this PickupRepresentation.
            std::string spawnerTemplate_; //!<  The name of the template of this PickupRepresentation.
            StaticEntity* spawnerRepresentation_; //!< The spawnerRepresentation of this PickupRepresentation.
            std::string inventoryRepresentation_; //!< The name of an image representing the pickup in the PickupInventory.

            Pickupable* pickup_; //!< The Pickupable that is represented by this PickupRepresentation.

    }; // tolua_export

} // tolua_export

#endif // _PickupRepresentation_H__
