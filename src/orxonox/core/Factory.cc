/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
    @file Factory.cc
    @brief Implementation of the Factory class.
*/

#include "Identifier.h"
#include "Debug.h"
#include "BaseObject.h"
#include "Factory.h"

namespace orxonox
{
    /**
        @brief Returns the Identifier with a given name.
        @param name The name of the wanted Identifier
        @return The Identifier
    */
    Identifier* Factory::getIdentifier(const std::string& name)
    {
        return getFactoryPointer()->identifierStringMap_[name];
    }

    /**
        @brief Returns the Identifier with a given network ID.
        @param id The network ID of the wanted Identifier
        @return The Identifier
    */
    Identifier* Factory::getIdentifier(const unsigned int id)
    {
        return getFactoryPointer()->identifierNetworkIDMap_[id];
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
            BaseObject* temp = (*it).second->fabricate();
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

    /**
        @brief Returns the begin-iterator of the factory-map.
        @return The begin-iterator
    */
    std::map<std::string, Identifier*>::const_iterator Factory::getFactoryBegin()
    {
        return Factory::getFactoryPointer()->identifierStringMap_.begin();
    }

    /**
        @brief Returns the end-iterator of the factory-map.
        @return The end-iterator
    */
    std::map<std::string, Identifier*>::const_iterator Factory::getFactoryEnd()
    {
        return Factory::getFactoryPointer()->identifierStringMap_.end();
    }
}
