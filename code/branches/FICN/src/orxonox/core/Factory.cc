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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

/*!
    @file Factory.cc
    @brief Implementation of the Factory class.
*/

#include "Factory.h"
#include "Identifier.h"
#include "Debug.h"
#include "../objects/BaseObject.h"

namespace orxonox
{
    Factory* Factory::pointer_s = NULL; // Set the static member variable pointer_s to zero

    /**
        @returns the Identifier with a given name.
        @param name The name of the wanted Identifier
    */
    Identifier* Factory::getIdentifier(const std::string& name)
    {
        if (!pointer_s)
            pointer_s = new Factory;

        return pointer_s->identifierStringMap_[name];
    }

    /**
        @returns the Identifier with a given network ID.
        @param id The network ID of the wanted Identifier
    */
    Identifier* Factory::getIdentifier(const unsigned int id)
    {
        if (!pointer_s)
            pointer_s = new Factory;

        return pointer_s->identifierNetworkIDMap_[id];
    }

    /**
        @brief Adds a new Identifier to both maps.
        @param name The name of the identifier
        @param identifier The identifier to add
    */
    void Factory::add(const std::string& name, Identifier* identifier)
    {
        if (!pointer_s)
            pointer_s = new Factory;

        pointer_s->identifierStringMap_[name] = identifier;
        pointer_s->identifierNetworkIDMap_[identifier->getNetworkID()] = identifier;
    }

    /**
        @brief Removes the entry with the old network ID and adds a new one.
        @param identifier The identifier to change
        @param oldID The old networkID
        @param newID The new networkID
    */
    void Factory::changeNetworkID(Identifier* identifier, const unsigned int oldID, const unsigned int newID)
    {
        pointer_s->identifierNetworkIDMap_.erase(oldID);
        pointer_s->identifierNetworkIDMap_[newID] = identifier;
    }

    /**
        @brief Creates the class-hierarchy by creating and destroying one object of each type.
    */
    void Factory::createClassHierarchy()
    {
        if (!pointer_s)
            pointer_s = new Factory;

        COUT(4) << "*** Factory -> Create class-hierarchy\n";
        std::map<std::string, Identifier*>::iterator it;
        it = pointer_s->identifierStringMap_.begin();
        (*pointer_s->identifierStringMap_.begin()).second->startCreatingHierarchy();
        for (it = pointer_s->identifierStringMap_.begin(); it != pointer_s->identifierStringMap_.end(); ++it)
        {
            // To create the new branch of the class-hierarchy, we create a new object and delete it afterwards.
            BaseObject* temp = (*it).second->fabricate();
            delete temp;
        }
        (*pointer_s->identifierStringMap_.begin()).second->stopCreatingHierarchy();
        COUT(4) << "*** Factory -> Finished class-hierarchy creation\n";
    }
}
