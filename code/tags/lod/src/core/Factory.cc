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
    @brief Implementation of the Factory class.
*/

#include "Factory.h"
#include "Identifier.h"
#include "BaseObject.h"
#include "util/Debug.h"

namespace orxonox
{
    /**
        @brief Returns the Identifier with a given name.
        @param name The name of the wanted Identifier
        @return The Identifier
    */
    Identifier* Factory::getIdentifier(const std::string& name)
    {
        std::map<std::string, Identifier*>::const_iterator it = getFactoryPointer()->identifierStringMap_.find(name);
        if (it != getFactoryPointer()->identifierStringMap_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Returns the Identifier with a given network ID.
        @param id The network ID of the wanted Identifier
        @return The Identifier
    */
    Identifier* Factory::getIdentifier(const unsigned int id)
    {
        std::map<unsigned int, Identifier*>::const_iterator it = getFactoryPointer()->identifierNetworkIDMap_.find(id);
        if (it != getFactoryPointer()->identifierNetworkIDMap_.end())
            return it->second;
        else
            return 0;
    }

    /**
        @brief Adds a new Identifier to both maps.
        @param name The name of the identifier
        @param identifier The identifier to add
    */
    void Factory::add(const std::string& name, Identifier* identifier)
    {
        getFactoryPointer()->identifierStringMap_[name] = identifier;
        getFactoryPointer()->identifierNetworkIDMap_[identifier->getNetworkID()] = identifier;
//std::cout << identifier->getName() << ": " << identifier->getNetworkID() << std::endl;
    }

    /**
        @brief Removes the entry with the old network ID and adds a new one.
        @param identifier The identifier to change
        @param oldID The old networkID
        @param newID The new networkID
    */
    void Factory::changeNetworkID(Identifier* identifier, const unsigned int oldID, const unsigned int newID)
    {
        getFactoryPointer()->identifierNetworkIDMap_.erase(oldID);
        getFactoryPointer()->identifierNetworkIDMap_[newID] = identifier;
//std::cout << identifier->getName() << ": " << oldID << " -> " << newID << std::endl;
    }

    /**
        @brief Creates the class-hierarchy by creating and destroying one object of each type.
    */
    void Factory::createClassHierarchy()
    {
        COUT(3) << "*** Factory: Create class-hierarchy" << std::endl;
        std::map<std::string, Identifier*>::iterator it;
        it = getFactoryPointer()->identifierStringMap_.begin();
        (*getFactoryPointer()->identifierStringMap_.begin()).second->startCreatingHierarchy();
        for (it = getFactoryPointer()->identifierStringMap_.begin(); it != getFactoryPointer()->identifierStringMap_.end(); ++it)
        {
            // To create the new branch of the class-hierarchy, we create a new object and delete it afterwards.
            BaseObject* temp = (*it).second->fabricate(0);
            delete temp;
        }
        (*getFactoryPointer()->identifierStringMap_.begin()).second->stopCreatingHierarchy();
        COUT(3) << "*** Factory: Finished class-hierarchy creation" << std::endl;
    }

    /**
        @brief Ensures the Factory gets created in the right moment.
        @return The Factory.
    */
    Factory* Factory::getFactoryPointer()
    {
      static Factory theOneAndOnlyFactoryInstance = Factory();
      return &theOneAndOnlyFactoryInstance;
    }
}
