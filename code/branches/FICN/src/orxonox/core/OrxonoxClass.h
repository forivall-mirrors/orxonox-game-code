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

/*!
    @file OrxonoxClass.h
    @brief Definition of the OrxonoxClass Class.

    All objects and interfaces of the game-logic (not the engine) are derived from OrxonoxClass.
    It stores the Identifier and the MetaObjectList and has all needed functions to create and use the class-hierarchy.
*/

#ifndef _OrxonoxClass_H__
#define _OrxonoxClass_H__

#include <string>

#include "CorePrereqs.h"

#include "MetaObjectList.h"
#include "Identifier.h"

namespace orxonox
{
    //! The class all objects and interfaces of the game-logic (not the engine) are derived from.
    /**
        The BaseObject and Interaces are derived with 'virtual public OrxonoxClass' from OrxonoxClass.
        OrxonoxClass is needed to create the class-hierarchy at startup and to store the Identifier and the MetaObjectList.
    */
    class _CoreExport OrxonoxClass
    {
        public:
            OrxonoxClass();
            virtual ~OrxonoxClass();

            /** @brief Function to collect the SetConfigValue-macro calls. */
            void setConfigValues() {};

            /** @returns the Identifier of the object */
            inline Identifier* getIdentifier() const { return this->identifier_; }

            /** @brief Sets the Identifier of the object. Used by the RegisterObject-macro. */
            inline void setIdentifier(Identifier* identifier) { this->identifier_ = identifier; }

            /** @returns the list of all parents of the object */
            inline IdentifierList* getParents() const { return this->parents_; }

            /** @brief Sets the Parents of the object. Used by the RegisterObject-macro. */
            inline void setParents(IdentifierList* parents) { this->parents_ = parents; }

            /** @returns the MetaObjectList of the object, containing a link to all ObjectLists and ObjectListElements the object is registered in. */
            inline MetaObjectList& getMetaList() { return this->metaList_; }


            /** @returns true if the objects class is of the given type or a derivative. */
            inline bool isA(const Identifier* identifier)
                { return this->getIdentifier()->isA(identifier); }
            /** @returns true if the objects class is exactly of the given type. */
            inline bool isDirectlyA(const Identifier* identifier)
                { return this->getIdentifier()->isDirectlyA(identifier); }
            /** @returns true if the objects class is a child of the given type. */
            inline bool isChildOf(const Identifier* identifier)
                { return this->getIdentifier()->isChildOf(identifier); }
            /** @returns true if the objects class is a parent of the given type. */
            inline bool isParentOf(const Identifier* identifier)
                { return this->getIdentifier()->isParentOf(identifier); }


            /** @returns true if the objects class is of the given type or a derivative. */
            inline bool isA(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isA(identifier->getIdentifier()); }
            /** @returns true if the objects class is exactly of the given type. */
            inline bool isDirectlyA(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isDirectlyA(identifier->getIdentifier()); }
            /** @returns true if the objects class is a child of the given type. */
            inline bool isChildOf(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isChildOf(identifier->getIdentifier()); }
            /** @returns true if the objects class is a parent of the given type. */
            inline bool isParentOf(const SubclassIdentifier<class B>* identifier)
                { return this->getIdentifier()->isParentOf(identifier->getIdentifier()); }


            /** @returns true if the objects class is of the given type or a derivative. */
            inline bool isA(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isA(identifier.getIdentifier()); }
            /** @returns true if the objects class is exactly of the given type. */
            inline bool isDirectlyA(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isDirectlyA(identifier.getIdentifier()); }
            /** @returns true if the objects class is a child of the given type. */
            inline bool isChildOf(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isChildOf(identifier.getIdentifier()); }
            /** @returns true if the objects class is a parent of the given type. */
            inline bool isParentOf(const SubclassIdentifier<class B> identifier)
                { return this->getIdentifier()->isParentOf(identifier.getIdentifier()); }


            /** @returns true if the objects class is of the given type or a derivative. */
            inline bool isA(const OrxonoxClass* object)
                { return this->getIdentifier()->isA(object->getIdentifier()); }
            /** @returns true if the objects class is exactly of the given type. */
            inline bool isDirectlyA(const OrxonoxClass* object)
                { return this->getIdentifier()->isDirectlyA(object->getIdentifier()); }
            /** @returns true if the objects class is a child of the given type. */
            inline bool isChildOf(const OrxonoxClass* object)
                { return this->getIdentifier()->isChildOf(object->getIdentifier()); }
            /** @returns true if the objects class is a parent of the given type. */
            inline bool isParentOf(const OrxonoxClass* object)
                { return this->getIdentifier()->isParentOf(object->getIdentifier()); }


            /** @brief Sets the name of the object. @param name The name */
            inline virtual void setName(const std::string& name) { this->name_ = name; }

            /** @returns the name of the object. */
            inline const std::string& getName() const { return this->name_; }

            /** @brief Sets the state of the objects activity. @param bActive True = active */
            inline virtual void setActive(bool bActive) { this->bActive_ = bActive; }

            /** @returns the state of the objects activity. */
            inline const bool isActive() const { return this->bActive_; }

            /** @brief Sets the state of the objects visibility. @param bVisible True = visible */
            inline virtual void setVisible(bool bVisible) { this->bVisible_ = bVisible; }

            /** @returns the state of the objects visibility. */
            inline const bool isVisible() const { return this->bVisible_; }

        private:
            Identifier* identifier_;        //!< The Identifier of the object
            IdentifierList* parents_;       //!< List of all parents of the object
            MetaObjectList metaList_;       //!< MetaObjectList, containing all ObjectLists and ObjectListElements the object is registered in

            std::string name_;              //!< The name of the object
            bool bActive_;                  //!< True = the object is active
            bool bVisible_;                 //!< True = the object is visible
    };
}

#endif /* _OrxonoxClass_H__ */
