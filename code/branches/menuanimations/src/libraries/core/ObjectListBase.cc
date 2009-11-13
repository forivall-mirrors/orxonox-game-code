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
    @brief Implementation of the ObjectListBase class.

    The ObjectListBase is a double-linked list, used by Identifiers to store all objects of a given class.
    Newly created objects are added through the RegisterObject-macro in its constructor.
*/

#include "ObjectListBase.h"

#include <set>
#include "Identifier.h"
#include "Iterator.h"
#include "ObjectListIterator.h"

namespace orxonox
{
    /**
        @brief Constructor: Sets default values.
    */
    ObjectListBase::ObjectListBase(Identifier* identifier)
    {
        this->identifier_ = identifier;
        this->first_ = 0;
        this->last_ = 0;
    }

    /**
        @brief Destructor: Deletes all list-elements, but NOT THE OBJECTS.
    */
    ObjectListBase::~ObjectListBase()
    {
        ObjectListBaseElement* temp;
        while (this->first_)
        {
            temp = this->first_->next_;
            delete this->first_;
            this->first_ = temp;
        }
    }

    /**
        @brief Increases all Iterators that currently point on the given element (because it gets removed).
        @param element The element that gets removed
    */
    void ObjectListBase::notifyIterators(OrxonoxClass* object) const
    {
        for (std::vector<void*>::const_iterator it = this->iterators_.begin(); it != this->iterators_.end(); ++it)
            ((Iterator<OrxonoxClass>*)(*it))->incrementIfEqual(object);
        for (std::vector<void*>::const_iterator it = this->objectListIterators_.begin(); it != this->objectListIterators_.end(); ++it)
            ((ObjectListIterator<OrxonoxClass>*)(*it))->incrementIfEqual(object);
    }

    /**
        @brief Adds a new object to the end of the list.
        @param object The object to add
        @return The pointer to the new ObjectListBaseElement, needed by the MetaObjectList of the added object
    */
    ObjectListBaseElement* ObjectListBase::add(ObjectListBaseElement* element)
    {
        if (!this->last_)
        {
            // If the list is empty
            this->last_ = element;
            this->first_ = this->last_; // There's only one object in the list now
        }
        else
        {
            // If the list isn't empty
            ObjectListBaseElement* temp = this->last_;
            this->last_ = element;
            this->last_->prev_ = temp;
            temp->next_ = this->last_;
        }

        return this->last_;
    }
}
