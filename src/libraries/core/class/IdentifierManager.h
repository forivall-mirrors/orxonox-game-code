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
    @ingroup Class Identifier
*/

#ifndef _IdentifierManager_H__
#define _IdentifierManager_H__

#include "core/CorePrereqs.h"

#include <map>
#include <set>
#include <string>

namespace orxonox
{
    class _CoreExport IdentifierManager
    {
        public:
            static IdentifierManager& getInstance();

            Identifier* getGloballyUniqueIdentifier(Identifier* proposal);
            void addIdentifierToLookupMaps(Identifier* identifier);

            unsigned int getUniqueClassId()
                { return this->classIDCounter_s++; }


            /////////////////////////////
            ////// Class Hierarchy //////
            /////////////////////////////
            void createClassHierarchy();
            void destroyAllIdentifiers();

            void createdObject(Identifiable* identifiable);

            /// Returns true, if a branch of the class-hierarchy is being created, causing all new objects to store their parents.
            inline bool isCreatingHierarchy()
                { return (hierarchyCreatingCounter_s > 0); }


            //////////////////////////
            ///// Identifier Map /////
            //////////////////////////
            Identifier* getIdentifierByString(const std::string& name);
            Identifier* getIdentifierByLowercaseString(const std::string& name);
            Identifier* getIdentifierByID(uint32_t id);

            void clearNetworkIDs();

            /// Returns the map that stores all Identifiers with their names.
            inline const std::map<std::string, Identifier*>& getIdentifierByStringMap()
                { return this->identifierByString_; }
            /// Returns the map that stores all Identifiers with their names in lowercase.
            inline const std::map<std::string, Identifier*>& getIdentifierByLowercaseStringMap()
                { return this->identifierByLowercaseString_; }
            /// Returns the map that stores all Identifiers with their IDs.
            inline const std::map<uint32_t, Identifier*>& getIdentifierByNetworkIdMap()
                { return this->identifierByNetworkId_; }

        private:
            IdentifierManager();
            IdentifierManager(const IdentifierManager&);
            ~IdentifierManager() {}

            /// Increases the hierarchyCreatingCounter_s variable, causing all new objects to store their parents.
            inline void startCreatingHierarchy()
                { hierarchyCreatingCounter_s++; }
            /// Decreases the hierarchyCreatingCounter_s variable, causing the objects to stop storing their parents.
            inline void stopCreatingHierarchy()
                { hierarchyCreatingCounter_s--; }

            std::map<std::string, Identifier*> identifierByTypeidName_;      //!< Map with the names as received by typeid(). This is only used internally.

            std::map<std::string, Identifier*> identifierByString_;          //!< Map that stores all Identifiers with their names.
            std::map<std::string, Identifier*> identifierByLowercaseString_; //!< Map that stores all Identifiers with their names in lowercase.
            std::map<uint32_t, Identifier*> identifierByNetworkId_;          //!< Returns the map that stores all Identifiers with their network IDs.

            int hierarchyCreatingCounter_s;                         //!< Bigger than zero if at least one Identifier stores its parents (its an int instead of a bool to avoid conflicts with multithreading)
            std::set<const Identifier*> identifiersOfNewObject_;    //!< Used while creating the object hierarchy to keep track of the identifiers of a newly created object
            unsigned int classIDCounter_s;                          //!< counter for the unique classIDs
    };
}

#endif /* _IdentifierManager_H__ */
