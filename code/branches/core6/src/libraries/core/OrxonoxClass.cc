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
    @brief Implementation of OrxonoxClass.
*/

#include "OrxonoxClass.h"

#include <cassert>
#include "object/MetaObjectList.h"
#include "Identifier.h"

namespace orxonox
{
    /**
        @brief Constructor: Sets the default values.
    */
    OrxonoxClass::OrxonoxClass()
    {
        this->identifier_ = 0;
        this->parents_ = 0;
        this->metaList_ = new MetaObjectList();
        this->referenceCount_ = 0;
        this->requestedDestruction_ = false;
        // Optimisation
        this->objectPointers_.reserve(6);
    }

    /**
        @brief Destructor: Removes the object from the object-lists, notifies all DestructionListener (for example @ref WeakPtr "weak pointers") that this object is being deleted.
    */
    OrxonoxClass::~OrxonoxClass()
    {
//        if (!this->requestedDestruction_)
//            orxout(internal_warning) << "Destroyed object without destroy() (" << this->getIdentifier()->getName() << ')' << endl;

        assert(this->referenceCount_ <= 0);

        this->unregisterObject();

        // parents_ exists only if isCreatingHierarchy() of the associated Identifier returned true while creating the class
        if (this->parents_)
            delete this->parents_;

        // notify all destruction listeners
        for (std::set<DestructionListener*>::iterator it = this->destructionListeners_.begin(); it != this->destructionListeners_.end(); )
            (*(it++))->objectDeleted();
    }

    /**
        @brief Deletes the object if no @ref orxonox::SmartPtr "smart pointers" point to this object. Otherwise schedules the object to be deleted as soon as possible.
    */
    void OrxonoxClass::destroy()
    {
        assert(this); // Just in case someone tries to delete a NULL pointer
        this->requestedDestruction_ = true;
        if (this->referenceCount_ == 0)
        {
            this->preDestroy();
            if (this->referenceCount_ == 0)
                delete this;
        }
    }

    /**
        @brief Removes this object from the object-lists.
    */
    void OrxonoxClass::unregisterObject()
    {
        if (this->metaList_)
            delete this->metaList_;
        this->metaList_ = 0;
    }

    /// Returns true if the object's class is of the given type or a derivative.
    bool OrxonoxClass::isA(const Identifier* identifier)
        { return this->getIdentifier()->isA(identifier); }
    /// Returns true if the object's class is exactly of the given type.
    bool OrxonoxClass::isExactlyA(const Identifier* identifier)
        { return this->getIdentifier()->isExactlyA(identifier); }
    /// Returns true if the object's class is a child of the given type.
    bool OrxonoxClass::isChildOf(const Identifier* identifier)
        { return this->getIdentifier()->isChildOf(identifier); }
    /// Returns true if the object's class is a direct child of the given type.
    bool OrxonoxClass::isDirectChildOf(const Identifier* identifier)
        { return this->getIdentifier()->isDirectChildOf(identifier); }
    /// Returns true if the object's class is a parent of the given type.
    bool OrxonoxClass::isParentOf(const Identifier* identifier)
        { return this->getIdentifier()->isParentOf(identifier); }
    /// Returns true if the object's class is a direct parent of the given type.
    bool OrxonoxClass::isDirectParentOf(const Identifier* identifier)
        { return this->getIdentifier()->isDirectParentOf(identifier); }


    /// Returns true if the object's class is of the given type or a derivative.
    bool OrxonoxClass::isA(const OrxonoxClass* object)
        { return this->getIdentifier()->isA(object->getIdentifier()); }
    /// Returns true if the object's class is exactly of the given type.
    bool OrxonoxClass::isExactlyA(const OrxonoxClass* object)
        { return this->getIdentifier()->isExactlyA(object->getIdentifier()); }
    /// Returns true if the object's class is a child of the given type.
    bool OrxonoxClass::isChildOf(const OrxonoxClass* object)
        { return this->getIdentifier()->isChildOf(object->getIdentifier()); }
    /// Returns true if the object's class is a direct child of the given type.
    bool OrxonoxClass::isDirectChildOf(const OrxonoxClass* object)
        { return this->getIdentifier()->isDirectChildOf(object->getIdentifier()); }
    /// Returns true if the object's class is a parent of the given type.
    bool OrxonoxClass::isParentOf(const OrxonoxClass* object)
        { return this->getIdentifier()->isParentOf(object->getIdentifier()); }
    /// Returns true if the object's class is a direct child of the given type.
    bool OrxonoxClass::isDirectParentOf(const OrxonoxClass* object)
        { return this->getIdentifier()->isDirectParentOf(object->getIdentifier()); }
}
