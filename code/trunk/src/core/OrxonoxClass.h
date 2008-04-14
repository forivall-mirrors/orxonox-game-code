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
    @file OrxonoxClass.h
    @brief Definition of the OrxonoxClass Class.

    All objects and interfaces of the game-logic (not the engine) are derived from OrxonoxClass.
    It stores the Identifier and the MetaObjectList and has all needed functions to create and use the class-hierarchy.
*/

#ifndef _OrxonoxClass_H__
#define _OrxonoxClass_H__

#include <set>
#include <string>

#include "CorePrereqs.h"
#include "MetaObjectList.h"
#include "Iterator.h"

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
            inline MetaObjectList& getMetaList() { return this->metaList_; }


            /** @brief Returns true if the objects class is of the given type or a derivative. */
            inline bool isA(const Identifier* identifier)
                { return this->getIdentifier()->isA(identifier); }
            /** @brief Returns true if the objects class is exactly of the given type. */
            inline bool isExactlyA(const Identifier* identifier)
                { return this->getIdentifier()->isExactlyA(identifier); }
            /** @brief Returns true if the objects class is a child of the given type. */
            inline bool isChildOf(const Identifier* identifier)
                { return this->getIdentifier()->isChildOf(identifier); }
            /** @brief Returns true if the objects class is a direct child of the given type. */
            inline bool isDirectChildOf(const Identifier* identifier)
                { return this->getIdentifier()->isDirectChildOf(identifier); }
            /** @brief Returns true if the objects class is a parent of the given type. */
            inline bool isParentOf(const Identifier* identifier)
                { return this->getIdentifier()->isParentOf(identifier); }
            /** @brief Returns true if the objects class is a direct parent of the given type. */
            inline bool isDirectParentOf(const Identifier* identifier)
                { return this->getIdentifier()->isDirectParentOf(identifier); }


            /** @brief Returns true if the objects class is of the given type or a derivative. */
            inline bool isA(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isA(identifier->getIdentifier()); }
            /** @brief Returns true if the objects class is exactly of the given type. */
            inline bool isExactlyA(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isExactlyA(identifier->getIdentifier()); }
            /** @brief Returns true if the objects class is a child of the given type. */
            inline bool isChildOf(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isChildOf(identifier->getIdentifier()); }
            /** @brief Returns true if the objects class is a direct child of the given type. */
            inline bool isDirectChildOf(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isDirectChildOf(identifier->getIdentifier()); }
            /** @brief Returns true if the objects class is a parent of the given type. */
            inline bool isParentOf(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isParentOf(identifier->getIdentifier()); }
            /** @brief Returns true if the objects class is a direct parent of the given type. */
            inline bool isDirectParentOf(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isDirectParentOf(identifier->getIdentifier()); }


            /** @brief Returns true if the objects class is of the given type or a derivative. */
            inline bool isA(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isA(identifier.getIdentifier()); }
            /** @brief Returns true if the objects class is exactly of the given type. */
            inline bool isExactlyA(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isExactlyA(identifier.getIdentifier()); }
            /** @brief Returns true if the objects class is a child of the given type. */
            inline bool isChildOf(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isChildOf(identifier.getIdentifier()); }
            /** @brief Returns true if the objects class is a direct child of the given type. */
            inline bool isDirectChildOf(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isDirectChildOf(identifier.getIdentifier()); }
            /** @brief Returns true if the objects class is a parent of the given type. */
            inline bool isParentOf(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isParentOf(identifier.getIdentifier()); }
            /** @brief Returns true if the objects class is a direct parent of the given type. */
            inline bool isDirectParentOf(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isDirectParentOf(identifier.getIdentifier()); }


            /** @brief Returns true if the objects class is of the given type or a derivative. */
            inline bool isA(const OrxonoxClass* object)
                { return this->getIdentifier()->isA(object->getIdentifier()); }
            /** @brief Returns true if the objects class is exactly of the given type. */
            inline bool isExactlyA(const OrxonoxClass* object)
                { return this->getIdentifier()->isExactlyA(object->getIdentifier()); }
            /** @brief Returns true if the objects class is a child of the given type. */
            inline bool isChildOf(const OrxonoxClass* object)
                { return this->getIdentifier()->isChildOf(object->getIdentifier()); }
            /** @brief Returns true if the objects class is a direct child of the given type. */
            inline bool isDirectChildOf(const OrxonoxClass* object)
                { return this->getIdentifier()->isDirectChildOf(object->getIdentifier()); }
            /** @brief Returns true if the objects class is a parent of the given type. */
            inline bool isParentOf(const OrxonoxClass* object)
                { return this->getIdentifier()->isParentOf(object->getIdentifier()); }
            /** @brief Returns true if the objects class is a direct child of the given type. */
            inline bool isDirectParentOf(const OrxonoxClass* object)
                { return this->getIdentifier()->isDirectParentOf(object->getIdentifier()); }

        private:
            Identifier* identifier_;                    //!< The Identifier of the object
            std::set<const Identifier*>* parents_;     //!< List of all parents of the object
            MetaObjectList metaList_;                   //!< MetaObjectList, containing all ObjectLists and ObjectListElements the object is registered in
    };
}

#endif /* _OrxonoxClass_H__ */
