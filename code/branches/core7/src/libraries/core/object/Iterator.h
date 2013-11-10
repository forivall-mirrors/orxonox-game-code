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
    @ingroup Object ObjectList
    @brief Definition of the Iterator class, used to iterate through object-lists.

    @anchor IteratorExample

    @ref orxonox::Iterator "Iterator" allows to iterate through an @ref orxonox::ObjectListBase
    "ObjectListBase". Objects in this list are cast to the template argument @a T of Iterator<T> using
    @c dynamic_cast. In contrast to @ref orxonox::ObjectListIterator "ObjectListIterator<T>",
    @ref orxonox::Iterator "Iterator<T>" can iterate through every object-list. In practice though it
    is limited to objects of type @a T and its subclasses. Because of the @c dynamic_cast, this iterator
    is much slower than ObjectListIterator.

    Usage:
    @code
    for (Iterator<myClass> it = anyidentifier->getObjects()->begin(); it != anyidentifier->getObjects()->end(); ++it)
    {
        it->someFunction(...);
        myClass* myObject = *it;
    }
    @endcode
*/

#ifndef _Iterator_H__
#define _Iterator_H__

#include "core/CorePrereqs.h"

#include "ObjectListBase.h"
#include "IteratorBase.h"

namespace orxonox
{
    /**
        @brief The Iterator allows to iterate through a given ObjectList.

        Independent of the object-list's type, the objects in the list are always casted
        to @a T using @c dynamic_cast.

        @see See @ref IteratorExample "Iterator.h" for more information an example.
    */
    template <class T>
    class Iterator : public IteratorBase<T, Iterator<T> >
    {
        public:
            /**
                @brief Constructor: Sets the element, whereon the iterator points, to zero.
            */
            inline Iterator() : IteratorBase<T, Iterator<T> >(NULL) {}

            /**
                @brief Constructor: Sets this element to a given element
                @param element The element
            */
            inline Iterator(ObjectListBaseElement* element) : IteratorBase<T, Iterator<T> >(element) {}

            /**
                @brief Constructor: Sets this element to the element of another Iterator.
                @param other The other Iterator
            */
            inline Iterator(const Iterator<T>& other) : IteratorBase<T, Iterator<T> >(other) {}

            /**
                @brief Assigns a given element.
                @param element The element
            */
            inline Iterator<T>& operator=(ObjectListBaseElement* element)
            {
                this->setElement(element);
                return (*this);
            }

            /**
                @brief Overloading of the *it operator: returns the pointer to the object.
                @return The object the Iterator points at
            */
            inline T* operator*() const
            {
                return orxonox_cast<T*>(this->element_->objectBase_);
            }

            /**
                @brief Overloading of the it-> operator: returns the pointer to the object.
                @return The object the Iterator points at
            */
            inline T* operator->() const
            {
                return orxonox_cast<T*>(this->element_->objectBase_);
            }
    };
}

#endif /* _Iterator_H__ */
