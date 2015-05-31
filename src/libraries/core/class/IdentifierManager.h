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
#include <list>
#include <string>

namespace orxonox
{
    class _CoreExport IdentifierManager
    {
        public:
            static IdentifierManager& getInstance();

            void addIdentifier(Identifier* identifier);
            void removeIdentifier(Identifier* identifier);


            /////////////////////////////
            ////// Class Hierarchy //////
            /////////////////////////////
            void createClassHierarchy();
            void verifyClassHierarchy(const std::set<Identifier*>& initializedIdentifiers);
            void destroyClassHierarchy();

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
            Identifier* getIdentifierByTypeInfo(const std::type_info& typeInfo);

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

            void destroyAllIdentifiers();

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

            std::set<Identifier*> identifiers_;                              //!< All identifiers. This is only used internally.
            std::map<std::string, Identifier*> identifierByString_;          //!< Map that stores all Identifiers with their names.
            std::map<std::string, Identifier*> identifierByLowercaseString_; //!< Map that stores all Identifiers with their names in lowercase.
            std::map<uint32_t, Identifier*> identifierByNetworkId_;          //!< Returns the map that stores all Identifiers with their network IDs.

            int hierarchyCreatingCounter_s;                         //!< Bigger than zero if at least one Identifier stores its parents (its an int instead of a bool to avoid conflicts with multithreading)

            /// Used while creating the object hierarchy to keep track of the identifiers of a newly created object (and all other objects that get created as
            /// a consequence of this, e.g. nested member objects).
            std::map<Identifiable*, std::list<const Identifier*> > identifierTraceOfNewObject_;
            Identifier* recordTraceForIdentifier_; //!< The identifier for which we want to record the trace of identifiers during object creation. If null, no trace is recorded.
    };
}

#endif /* _IdentifierManager_H__ */
