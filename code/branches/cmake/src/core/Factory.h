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
    @file Factory.h
    @brief Definition of the Factory and the BaseFactory class.

    The Factory is a singleton, containing two maps to map either the name or the network ID
    of a class with the corresponding Identifier.

    Usage:
    ID(classname) or ID(networkID) returns the corresponding Identifier.


    BaseObject is the parent of ClassFactory which is defined in ClassFactory.h.
    It can't be defined in ClassFactory.h, because of circular dependencies.
*/

#ifndef _Factory_H__
#define _Factory_H__

#include "CorePrereqs.h"

#include <map>
#include <string>

namespace orxonox
{
    // ###############################
    // ###         Factory         ###
    // ###############################
    //! The Factory is used to map the name or the network ID of a class with its Identifier.
    class _CoreExport Factory
    {
        public:
            static Identifier* getIdentifier(const std::string& name);
            static Identifier* getIdentifier(const unsigned int id);
            static void add(const std::string& name, Identifier* identifier);
            static void changeNetworkID(Identifier* identifier, const unsigned int oldID, const unsigned int newID);
            static void createClassHierarchy();

            static Factory* getFactoryPointer();    // avoid overriding order problem in the static intialisation process
            static std::map<std::string, Identifier*>::const_iterator getFactoryBegin();
            static std::map<std::string, Identifier*>::const_iterator getFactoryEnd();

        private:
            Factory() {}                            // don't create
            Factory(const Factory& factory);        // don't copy
            ~Factory() {}                           // don't delete

            std::map<std::string, Identifier*> identifierStringMap_;            //!< The map, mapping the name with the Identifier
            std::map<unsigned int, Identifier*> identifierNetworkIDMap_;        //!< The map, mapping the network ID with the Identifier
    };

    // ###############################
    // ###       BaseFactory       ###
    // ###############################
    //! Base-class of ClassFactory. Has to be defined separate because of circular dependencies.
    class _CoreExport BaseFactory
    {
        public:
            virtual BaseObject* fabricate() = 0;
            virtual ~BaseFactory() {};
    };
}

#endif /* _Factory_H__ */
