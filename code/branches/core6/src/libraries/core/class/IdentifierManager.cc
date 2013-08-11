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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

/**
    @file
    @brief Implementation of the Identifier class.
*/

#include "IdentifierManager.h"

#include <ostream>

#include "util/StringUtils.h"
#include "core/config/ConfigValueContainer.h"
#include "core/XMLPort.h"
#include "core/object/ClassFactory.h"

namespace orxonox
{
    /* static */ IdentifierManager& IdentifierManager::getInstance()
    {
        static IdentifierManager instance;
        return instance;
    }

    IdentifierManager::IdentifierManager()
    {
        this->hierarchyCreatingCounter_s = 0;
        this->classIDCounter_s = 0;
    }

    /**
        @brief Returns an identifier by name and adds it if not available
        @param proposal A pointer to a newly created identifier for the case of non existence in the map
        @return The identifier (unique instance)
    */
    Identifier* IdentifierManager::getIdentifierSingleton(Identifier* proposal)
    {
        const std::string& typeidName = proposal->getTypeidName();
        std::map<std::string, Identifier*>::const_iterator it = this->identifierByTypeidName_.find(typeidName);

        if (it != this->identifierByTypeidName_.end())
        {
            // There is already an entry: return it
            return it->second;
        }
        else
        {
            // There is no entry: put the proposal into the map and return it
            this->identifierByTypeidName_[typeidName] = proposal;
            return proposal;
        }
    }

    /**
     * Registers the identifier in all maps of the IdentifierManager.
     */
    void IdentifierManager::registerIdentifier(Identifier* identifier)
    {
        IdentifierManager::getInstance().identifierByString_[identifier->getName()] = identifier;
        IdentifierManager::getInstance().identifierByLowercaseString_[getLowercase(identifier->getName())] = identifier;
        IdentifierManager::getInstance().identifierByNetworkId_[identifier->getNetworkID()] = identifier;
    }

    /**
        @brief Creates the class-hierarchy by creating and destroying one object of each type.
    */
    void IdentifierManager::createClassHierarchy()
    {
        orxout(internal_status) << "Create class-hierarchy" << endl;
        this->startCreatingHierarchy();
        for (std::map<std::string, Identifier*>::const_iterator it = this->identifierByTypeidName_.begin(); it != this->identifierByTypeidName_.end(); ++it)
        {
            // To create the new branch of the class-hierarchy, we create a new object and delete it afterwards.
            if (it->second->hasFactory())
            {
                Identifiable* temp = it->second->fabricate(0);
                delete temp;
            }
        }
        this->stopCreatingHierarchy();
        orxout(internal_status) << "Finished class-hierarchy creation" << endl;
    }

    /**
        @brief Destroys all Identifiers. Called when exiting the program.
    */
    void IdentifierManager::destroyAllIdentifiers()
    {
        for (std::map<std::string, Identifier*>::iterator it = this->identifierByTypeidName_.begin(); it != this->identifierByTypeidName_.end(); ++it)
            delete (it->second);

        this->identifierByTypeidName_.clear();
        this->identifierByString_.clear();
        this->identifierByLowercaseString_.clear();
        this->identifierByNetworkId_.clear();
    }

    /**
        @brief Returns the Identifier with a given name.
        @param name The name of the wanted Identifier
        @return The Identifier
    */
    Identifier* IdentifierManager::getIdentifierByString(const std::string& name)
    {
        std::map<std::string, Identifier*>::const_iterator it = this->identifierByString_.find(name);
        if (it != this->identifierByString_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Returns the Identifier with a given name in lowercase.
        @param name The name of the wanted Identifier
        @return The Identifier
    */
    Identifier* IdentifierManager::getIdentifierByLowercaseString(const std::string& name)
    {
        std::map<std::string, Identifier*>::const_iterator it = this->identifierByLowercaseString_.find(name);
        if (it != this->identifierByLowercaseString_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Returns the Identifier with a given network ID.
        @param id The network ID of the wanted Identifier
        @return The Identifier
    */
    Identifier* IdentifierManager::getIdentifierByID(const uint32_t id)
    {
        std::map<uint32_t, Identifier*>::const_iterator it = this->identifierByNetworkId_.find(id);
        if (it != this->identifierByNetworkId_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Cleans the NetworkID map (needed on clients for correct initialization)
    */
    void IdentifierManager::clearNetworkIDs()
    {
        this->identifierByNetworkId_.clear();
    }
}
