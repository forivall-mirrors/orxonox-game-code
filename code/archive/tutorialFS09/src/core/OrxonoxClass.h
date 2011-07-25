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
    @brief Declaration of the OrxonoxClass Class.

    All objects and interfaces of the game-logic (not the engine) are derived from OrxonoxClass.
    It stores the Identifier and the MetaObjectList and has all needed functions to create and use the class-hierarchy.
*/

#ifndef _OrxonoxClass_H__
#define _OrxonoxClass_H__

#include "CorePrereqs.h"

#include <set>
#include <string>

namespace orxonox
{
    //! The class all objects and interfaces of the game-logic (not the engine) are derived from.
    /**
        The BaseObject and Interfaces are derived with 'virtual public OrxonoxClass' from OrxonoxClass.
        OrxonoxClass is needed to create the class-hierarchy at startup and to store the Identifier and the MetaObjectList.
    */
    class _CoreExport OrxonoxClass
    {
        public:
            OrxonoxClass();
            virtual ~OrxonoxClass();

            /** @brief Function to collect the SetConfigValue-macro calls. */
            void setConfigValues() {};

            /** @brief Returns the Identifier of the object. @return The Identifier */
            inline Identifier* getIdentifier() const { return this->identifier_; }

            /** @brief Sets the Identifier of the object. Used by the RegisterObject-macro. */
            inline void setIdentifier(Identifier* identifier) { this->identifier_ = identifier; }

            /** @brief Returns the list of all parents of the object. @return The list */
            inline std::set<const Identifier*>* getParents() const { return this->parents_; }

            /** @brief Creates the parents-list. */
            inline void createParents() { this->parents_ = new std::set<const Identifier*>(); }

            /** @brief Returns the MetaObjectList of the object, containing a link to all ObjectLists and ObjectListElements the object is registered in. @return The list */
            inline MetaObjectList& getMetaList() { return (*this->metaList_); }


            bool isA(const Identifier* identifier);
            bool isExactlyA(const Identifier* identifier);
            bool isChildOf(const Identifier* identifier);
            bool isDirectChildOf(const Identifier* identifier);
            bool isParentOf(const Identifier* identifier);
            bool isDirectParentOf(const Identifier* identifier);

            bool isA(const SubclassIdentifier<class B>* identifier);
            bool isExactlyA(const SubclassIdentifier<class B>* identifier);
            bool isChildOf(const SubclassIdentifier<class B>* identifier);
            bool isDirectChildOf(const SubclassIdentifier<class B>* identifier);
            bool isParentOf(const SubclassIdentifier<class B>* identifier);
            bool isDirectParentOf(const SubclassIdentifier<class B>* identifier);

            bool isA(const SubclassIdentifier<class B> identifier);
            bool isExactlyA(const SubclassIdentifier<class B> identifier);
            bool isChildOf(const SubclassIdentifier<class B> identifier);
            bool isDirectChildOf(const SubclassIdentifier<class B> identifier);
            bool isParentOf(const SubclassIdentifier<class B> identifier);
            bool isDirectParentOf(const SubclassIdentifier<class B> identifier);

            bool isA(const OrxonoxClass* object);
            bool isExactlyA(const OrxonoxClass* object);
            bool isChildOf(const OrxonoxClass* object);
            bool isDirectChildOf(const OrxonoxClass* object);
            bool isParentOf(const OrxonoxClass* object);
            bool isDirectParentOf(const OrxonoxClass* object);

        private:
            Identifier* identifier_;                   //!< The Identifier of the object
            std::set<const Identifier*>* parents_;     //!< List of all parents of the object
            MetaObjectList* metaList_;                 //!< MetaObjectList, containing all ObjectLists and ObjectListElements the object is registered in
    };
}

#endif /* _OrxonoxClass_H__ */
