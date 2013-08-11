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
            static IdentifierManager& getInstance();

            /////////////////////////////
            ////// Class Hierarchy //////
            /////////////////////////////
            void createClassHierarchy();

            /// Returns true, if a branch of the class-hierarchy is being created, causing all new objects to store their parents.
            inline bool isCreatingHierarchy()
                { return (hierarchyCreatingCounter_s > 0); }


            //////////////////////////
            ///// Identifier Map /////
            //////////////////////////
            void destroyAllIdentifiers();

            Identifier* getIdentifierByString(const std::string& name);
            Identifier* getIdentifierByLowercaseString(const std::string& name);
            Identifier* getIdentifierByID(uint32_t id);

            void clearNetworkIDs();

            /// Returns the map that stores all Identifiers with their names.
            inline const std::map<std::string, Identifier*>& getStringIdentifierMap()
                { return this->identifierByString_; }
            /// Returns a const_iterator to the beginning of the map that stores all Identifiers with their names.
            inline std::map<std::string, Identifier*>::const_iterator getStringIdentifierMapBegin()
                { return this->identifierByString_.begin(); }
            /// Returns a const_iterator to the end of the map that stores all Identifiers with their names.
            inline std::map<std::string, Identifier*>::const_iterator getStringIdentifierMapEnd()
                { return this->identifierByString_.end(); }

            /// Returns the map that stores all Identifiers with their names in lowercase.
            inline const std::map<std::string, Identifier*>& getLowercaseStringIdentifierMap()
                { return this->identifierByLowercaseString_; }
            /// Returns a const_iterator to the beginning of the map that stores all Identifiers with their names in lowercase.
            inline std::map<std::string, Identifier*>::const_iterator getLowercaseStringIdentifierMapBegin()
                { return this->identifierByLowercaseString_.begin(); }
            /// Returns a const_iterator to the end of the map that stores all Identifiers with their names in lowercase.
            inline std::map<std::string, Identifier*>::const_iterator getLowercaseStringIdentifierMapEnd()
                { return this->identifierByLowercaseString_.end(); }

            /// Returns the map that stores all Identifiers with their IDs.
            inline const std::map<uint32_t, Identifier*>& getIDIdentifierMap()
                { return this->identifierByNetworkId_; }
            /// Returns a const_iterator to the beginning of the map that stores all Identifiers with their IDs.
            inline std::map<uint32_t, Identifier*>::const_iterator getIDIdentifierMapBegin()
                { return this->identifierByNetworkId_.begin(); }
            /// Returns a const_iterator to the end of the map that stores all Identifiers with their IDs.
            inline std::map<uint32_t, Identifier*>::const_iterator getIDIdentifierMapEnd()
                { return this->identifierByNetworkId_.end(); }

        protected:
            Identifier* getIdentifierSingleton(const std::string& name, Identifier* proposal);

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

            std::map<std::string, Identifier*> identifierByTypeId_;          //!< Map with the names as received by typeid(). This is only used internally.

            std::map<std::string, Identifier*> identifierByString_;          //!< Map that stores all Identifiers with their names.
            std::map<std::string, Identifier*> identifierByLowercaseString_; //!< Map that stores all Identifiers with their names in lowercase.
            std::map<uint32_t, Identifier*> identifierByNetworkId_;          //!< Returns the map that stores all Identifiers with their network IDs.

            int hierarchyCreatingCounter_s;                         //!< Bigger than zero if at least one Identifier stores its parents (its an int instead of a bool to avoid conflicts with multithreading)
            unsigned int classIDCounter_s;                          //!< counter for the unique classIDs
    };
}

#endif /* _IdentifierManager_H__ */
