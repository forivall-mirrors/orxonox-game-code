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
    @brief Implementation of the OrxonoxClass Class.
*/

#include "OrxonoxClass.h"

#include "MetaObjectList.h"
#include "Identifier.h"

namespace orxonox
{
    /** @brief Constructor: Sets the default values. */
    OrxonoxClass::OrxonoxClass()
    {
        this->identifier_ = 0;
        this->parents_ = 0;
        this->metaList_ = new MetaObjectList();
    }

    /** @brief Destructor: Deletes, if existing, the list of the parents. */
    OrxonoxClass::~OrxonoxClass()
    {
        delete this->metaList_;

        // parents_ exists only if isCreatingHierarchy() of the associated Identifier returned true while creating the class
        if (this->parents_)
            delete this->parents_;
    }

    /** @brief Returns true if the objects class is of the given type or a derivative. */
    bool OrxonoxClass::isA(const Identifier* identifier)
        { return this->getIdentifier()->isA(identifier); }
    /** @brief Returns true if the objects class is exactly of the given type. */
    bool OrxonoxClass::isExactlyA(const Identifier* identifier)
        { return this->getIdentifier()->isExactlyA(identifier); }
    /** @brief Returns true if the objects class is a child of the given type. */
    bool OrxonoxClass::isChildOf(const Identifier* identifier)
        { return this->getIdentifier()->isChildOf(identifier); }
    /** @brief Returns true if the objects class is a direct child of the given type. */
    bool OrxonoxClass::isDirectChildOf(const Identifier* identifier)
        { return this->getIdentifier()->isDirectChildOf(identifier); }
    /** @brief Returns true if the objects class is a parent of the given type. */
    bool OrxonoxClass::isParentOf(const Identifier* identifier)
        { return this->getIdentifier()->isParentOf(identifier); }
    /** @brief Returns true if the objects class is a direct parent of the given type. */
    bool OrxonoxClass::isDirectParentOf(const Identifier* identifier)
        { return this->getIdentifier()->isDirectParentOf(identifier); }


    /** @brief Returns true if the objects class is of the given type or a derivative. */
    bool OrxonoxClass::isA(const SubclassIdentifier<class B>* identifier)
        { return this->getIdentifier()->isA(identifier->getIdentifier()); }
    /** @brief Returns true if the objects class is exactly of the given type. */
    bool OrxonoxClass::isExactlyA(const SubclassIdentifier<class B>* identifier)
        { return this->getIdentifier()->isExactlyA(identifier->getIdentifier()); }
    /** @brief Returns true if the objects class is a child of the given type. */
    bool OrxonoxClass::isChildOf(const SubclassIdentifier<class B>* identifier)
        { return this->getIdentifier()->isChildOf(identifier->getIdentifier()); }
    /** @brief Returns true if the objects class is a direct child of the given type. */
    bool OrxonoxClass::isDirectChildOf(const SubclassIdentifier<class B>* identifier)
        { return this->getIdentifier()->isDirectChildOf(identifier->getIdentifier()); }
    /** @brief Returns true if the objects class is a parent of the given type. */
    bool OrxonoxClass::isParentOf(const SubclassIdentifier<class B>* identifier)
        { return this->getIdentifier()->isParentOf(identifier->getIdentifier()); }
    /** @brief Returns true if the objects class is a direct parent of the given type. */
    bool OrxonoxClass::isDirectParentOf(const SubclassIdentifier<class B>* identifier)
        { return this->getIdentifier()->isDirectParentOf(identifier->getIdentifier()); }


    /** @brief Returns true if the objects class is of the given type or a derivative. */
    bool OrxonoxClass::isA(const SubclassIdentifier<class B> identifier)
        { return this->getIdentifier()->isA(identifier.getIdentifier()); }
    /** @brief Returns true if the objects class is exactly of the given type. */
    bool OrxonoxClass::isExactlyA(const SubclassIdentifier<class B> identifier)
        { return this->getIdentifier()->isExactlyA(identifier.getIdentifier()); }
    /** @brief Returns true if the objects class is a child of the given type. */
    bool OrxonoxClass::isChildOf(const SubclassIdentifier<class B> identifier)
        { return this->getIdentifier()->isChildOf(identifier.getIdentifier()); }
    /** @brief Returns true if the objects class is a direct child of the given type. */
    bool OrxonoxClass::isDirectChildOf(const SubclassIdentifier<class B> identifier)
        { return this->getIdentifier()->isDirectChildOf(identifier.getIdentifier()); }
    /** @brief Returns true if the objects class is a parent of the given type. */
    bool OrxonoxClass::isParentOf(const SubclassIdentifier<class B> identifier)
        { return this->getIdentifier()->isParentOf(identifier.getIdentifier()); }
    /** @brief Returns true if the objects class is a direct parent of the given type. */
    bool OrxonoxClass::isDirectParentOf(const SubclassIdentifier<class B> identifier)
        { return this->getIdentifier()->isDirectParentOf(identifier.getIdentifier()); }


    /** @brief Returns true if the objects class is of the given type or a derivative. */
    bool OrxonoxClass::isA(const OrxonoxClass* object)
        { return this->getIdentifier()->isA(object->getIdentifier()); }
    /** @brief Returns true if the objects class is exactly of the given type. */
    bool OrxonoxClass::isExactlyA(const OrxonoxClass* object)
        { return this->getIdentifier()->isExactlyA(object->getIdentifier()); }
    /** @brief Returns true if the objects class is a child of the given type. */
    bool OrxonoxClass::isChildOf(const OrxonoxClass* object)
        { return this->getIdentifier()->isChildOf(object->getIdentifier()); }
    /** @brief Returns true if the objects class is a direct child of the given type. */
    bool OrxonoxClass::isDirectChildOf(const OrxonoxClass* object)
        { return this->getIdentifier()->isDirectChildOf(object->getIdentifier()); }
    /** @brief Returns true if the objects class is a parent of the given type. */
    bool OrxonoxClass::isParentOf(const OrxonoxClass* object)
        { return this->getIdentifier()->isParentOf(object->getIdentifier()); }
    /** @brief Returns true if the objects class is a direct child of the given type. */
    bool OrxonoxClass::isDirectParentOf(const OrxonoxClass* object)
        { return this->getIdentifier()->isDirectParentOf(object->getIdentifier()); }
}
