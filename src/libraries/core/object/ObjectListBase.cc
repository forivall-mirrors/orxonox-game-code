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
*/

#include "ObjectListBase.h"

#include <set>
#include "Iterator.h"
#include "Listable.h"
#include "ObjectListIterator.h"

namespace orxonox
{
    ObjectListBaseElement::~ObjectListBaseElement()
    {
        if (this->list_)
            this->list_->removeElement(this);
    }

    /**
        @brief Constructor: Sets default values.
    */
    ObjectListBase::ObjectListBase()
    {
        this->first_ = 0;
        this->last_ = 0;
        this->size_ = 0;
    }

    /**
        @brief Destructor: Deletes all list-elements, but NOT THE OBJECTS.
    */
    ObjectListBase::~ObjectListBase()
    {
        while (this->first_)
            delete this->first_;
    }

    /**
        @brief Notifies all listeners that the given element is about to get removed.
        @param element The element that gets removed
        This is mainly used for iterators which point at the removed element
    */
    void ObjectListBase::notifyRemovalListeners(ObjectListBaseElement* element) const
    {
        for (std::vector<ObjectListElementRemovalListener*>::const_iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
            (*it)->removedElement(element);
    }

    /**
        @brief Adds a new object to the end of the list.
        @param element The element to add
    */
    void ObjectListBase::addElement(ObjectListBaseElement* element)
    {
        if (element->list_)
        {
            orxout(internal_error) << "Element is already registered in another list" << endl;
            return;
        }

        if (element->objectBase_)
            orxout(verbose, context::object_list) << "Added object to " << element->objectBase_->getIdentifier()->getName() << "-list." << endl;

        if (!this->last_)
        {
            // If the list is empty
            this->last_ = element;
            this->first_ = element; // There's only one object in the list now
        }
        else
        {
            // If the list isn't empty
            ObjectListBaseElement* temp = this->last_;
            this->last_ = element;
            element->prev_ = temp;
            temp->next_ = element;
        }

        element->list_ = this;
        ++this->size_;
    }

    /**
     * @brief Removes the element from the list
     */
    void ObjectListBase::removeElement(ObjectListBaseElement* element)
    {
        if (element->list_ != this)
        {
            orxout(internal_error) << "Element is not registered in this list" << endl;
            return;
        }

        if (element->objectBase_)
            orxout(verbose, context::object_list) << "Removing Object from " << element->objectBase_->getIdentifier()->getName() << "-list." << endl;
        this->notifyRemovalListeners(element);

        if (element->next_)
            element->next_->prev_ = element->prev_;
        else
            this->last_ = element->prev_; // If there is no next_, we deleted the last object and have to update the last_ pointer of the list

        if (element->prev_)
            element->prev_->next_ = element->next_;
        else
            this->first_ = element->next_; // If there is no prev_, we deleted the first object and have to update the first_ pointer of the list

        element->list_ = 0;
        element->next_ = 0;
        element->prev_ = 0;
        --this->size_;
    }
}
