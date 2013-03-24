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
#include <string>

namespace orxonox
{
    class _CoreExport IdentifierManager
    {
        friend class Identifier;
        template <class T> friend class ClassIdentifier;

        public:
            /////////////////////////////
            ////// Class Hierarchy //////
            /////////////////////////////
            static void createClassHierarchy();

            /// Returns true, if a branch of the class-hierarchy is being created, causing all new objects to store their parents.
            inline static bool isCreatingHierarchy()
                { return (hierarchyCreatingCounter_s > 0); }


            //////////////////////////
            ///// Identifier Map /////
            //////////////////////////
            static void destroyAllIdentifiers();

            static Identifier* getIdentifierByString(const std::string& name);
            static Identifier* getIdentifierByLowercaseString(const std::string& name);
            static Identifier* getIdentifierByID(uint32_t id);

            static void clearNetworkIDs();

            /// Returns the map that stores all Identifiers with their names.
            static inline const std::map<std::string, Identifier*>& getStringIdentifierMap()
                { return IdentifierManager::getStringIdentifierMapIntern(); }
            /// Returns a const_iterator to the beginning of the map that stores all Identifiers with their names.
            static inline std::map<std::string, Identifier*>::const_iterator getStringIdentifierMapBegin()
                { return IdentifierManager::getStringIdentifierMap().begin(); }
            /// Returns a const_iterator to the end of the map that stores all Identifiers with their names.
            static inline std::map<std::string, Identifier*>::const_iterator getStringIdentifierMapEnd()
                { return IdentifierManager::getStringIdentifierMap().end(); }

            /// Returns the map that stores all Identifiers with their names in lowercase.
            static inline const std::map<std::string, Identifier*>& getLowercaseStringIdentifierMap()
                { return IdentifierManager::getLowercaseStringIdentifierMapIntern(); }
            /// Returns a const_iterator to the beginning of the map that stores all Identifiers with their names in lowercase.
            static inline std::map<std::string, Identifier*>::const_iterator getLowercaseStringIdentifierMapBegin()
                { return IdentifierManager::getLowercaseStringIdentifierMap().begin(); }
            /// Returns a const_iterator to the end of the map that stores all Identifiers with their names in lowercase.
            static inline std::map<std::string, Identifier*>::const_iterator getLowercaseStringIdentifierMapEnd()
                { return IdentifierManager::getLowercaseStringIdentifierMap().end(); }

            /// Returns the map that stores all Identifiers with their IDs.
            static inline const std::map<uint32_t, Identifier*>& getIDIdentifierMap()
                { return IdentifierManager::getIDIdentifierMapIntern(); }
            /// Returns a const_iterator to the beginning of the map that stores all Identifiers with their IDs.
            static inline std::map<uint32_t, Identifier*>::const_iterator getIDIdentifierMapBegin()
                { return IdentifierManager::getIDIdentifierMap().begin(); }
            /// Returns a const_iterator to the end of the map that stores all Identifiers with their IDs.
            static inline std::map<uint32_t, Identifier*>::const_iterator getIDIdentifierMapEnd()
                { return IdentifierManager::getIDIdentifierMap().end(); }

        protected:
            static Identifier* getIdentifierSingleton(const std::string& name, Identifier* proposal);

            /// Returns the map that stores all Identifiers with their names.
            static std::map<std::string, Identifier*>& getStringIdentifierMapIntern();
            /// Returns the map that stores all Identifiers with their names in lowercase.
            static std::map<std::string, Identifier*>& getLowercaseStringIdentifierMapIntern();
            /// Returns the map that stores all Identifiers with their network IDs.
            static std::map<uint32_t, Identifier*>& getIDIdentifierMapIntern();

        private:
            /// Increases the hierarchyCreatingCounter_s variable, causing all new objects to store their parents.
            inline static void startCreatingHierarchy()
                { hierarchyCreatingCounter_s++; }
            /// Decreases the hierarchyCreatingCounter_s variable, causing the objects to stop storing their parents.
            inline static void stopCreatingHierarchy()
                { hierarchyCreatingCounter_s--; }

            static std::map<std::string, Identifier*>& getTypeIDIdentifierMap();

            static int hierarchyCreatingCounter_s;                         //!< Bigger than zero if at least one Identifier stores its parents (its an int instead of a bool to avoid conflicts with multithreading)
            static unsigned int classIDCounter_s;                          //!< Static counter for the unique classIDs
    };
}

#endif /* _IdentifierManager_H__ */
