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
    int IdentifierManager::hierarchyCreatingCounter_s = 0;
    unsigned int IdentifierManager::classIDCounter_s = 0;

    /**
        @brief Returns the identifier map with the names as received by typeid(). This is only used internally.
    */
    std::map<std::string, Identifier*>& IdentifierManager::getTypeIDIdentifierMap()
    {
        static std::map<std::string, Identifier*> identifiers;    //!< The map to store all Identifiers.
        return identifiers;
    }

    /**
        @brief Returns an identifier by name and adds it if not available
        @param name The name of the identifier as typeid().name() suggests
        @param proposal A pointer to a newly created identifier for the case of non existence in the map
        @return The identifier (unique instance)
    */
    Identifier* IdentifierManager::getIdentifierSingleton(const std::string& name, Identifier* proposal)
    {
        std::map<std::string, Identifier*>::const_iterator it = getTypeIDIdentifierMap().find(name);

        if (it != getTypeIDIdentifierMap().end())
        {
            // There is already an entry: return it and delete the proposal
            delete proposal;
            return it->second;
        }
        else
        {
            // There is no entry: put the proposal into the map and return it
            getTypeIDIdentifierMap()[name] = proposal;
            return proposal;
        }
    }

    /**
        @brief Creates the class-hierarchy by creating and destroying one object of each type.
    */
    void IdentifierManager::createClassHierarchy()
    {
        orxout(internal_status) << "Create class-hierarchy" << endl;
        IdentifierManager::startCreatingHierarchy();
        for (std::map<std::string, Identifier*>::const_iterator it = IdentifierManager::getStringIdentifierMap().begin(); it != IdentifierManager::getStringIdentifierMap().end(); ++it)
        {
            // To create the new branch of the class-hierarchy, we create a new object and delete it afterwards.
            if (it->second->hasFactory())
            {
                Identifiable* temp = it->second->fabricate(0);
                delete temp;
            }
        }
        IdentifierManager::stopCreatingHierarchy();
        orxout(internal_status) << "Finished class-hierarchy creation" << endl;
    }

    /**
        @brief Destroys all Identifiers. Called when exiting the program.
    */
    void IdentifierManager::destroyAllIdentifiers()
    {
        for (std::map<std::string, Identifier*>::iterator it = IdentifierManager::getTypeIDIdentifierMap().begin(); it != IdentifierManager::getTypeIDIdentifierMap().end(); ++it)
            delete (it->second);
    }

    /**
        @brief Returns the map that stores all Identifiers with their names.
        @return The map
    */
    std::map<std::string, Identifier*>& IdentifierManager::getStringIdentifierMapIntern()
    {
        static std::map<std::string, Identifier*> identifierMap;
        return identifierMap;
    }

    /**
        @brief Returns the map that stores all Identifiers with their names in lowercase.
        @return The map
    */
    std::map<std::string, Identifier*>& IdentifierManager::getLowercaseStringIdentifierMapIntern()
    {
        static std::map<std::string, Identifier*> lowercaseIdentifierMap;
        return lowercaseIdentifierMap;
    }

    /**
        @brief Returns the map that stores all Identifiers with their network IDs.
        @return The map
    */
    std::map<uint32_t, Identifier*>& IdentifierManager::getIDIdentifierMapIntern()
    {
        static std::map<uint32_t, Identifier*> identifierMap;
        return identifierMap;
    }

    /**
        @brief Returns the Identifier with a given name.
        @param name The name of the wanted Identifier
        @return The Identifier
    */
    Identifier* IdentifierManager::getIdentifierByString(const std::string& name)
    {
        std::map<std::string, Identifier*>::const_iterator it = IdentifierManager::getStringIdentifierMapIntern().find(name);
        if (it != IdentifierManager::getStringIdentifierMapIntern().end())
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
        std::map<std::string, Identifier*>::const_iterator it = IdentifierManager::getLowercaseStringIdentifierMapIntern().find(name);
        if (it != IdentifierManager::getLowercaseStringIdentifierMapIntern().end())
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
        std::map<uint32_t, Identifier*>::const_iterator it = IdentifierManager::getIDIdentifierMapIntern().find(id);
        if (it != IdentifierManager::getIDIdentifierMapIntern().end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Cleans the NetworkID map (needed on clients for correct initialization)
    */
    void IdentifierManager::clearNetworkIDs()
    {
        IdentifierManager::getIDIdentifierMapIntern().clear();
    }
}
