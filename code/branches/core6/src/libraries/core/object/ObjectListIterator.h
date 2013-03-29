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
    @brief Definition of the ObjectListIterator class, used to iterate through object-lists.

    @anchor ObjectListIteratorExample

    @ref orxonox::ObjectListIterator "ObjectListIterator<T>" allows to iterate through
    @ref orxonox::ObjectList "ObjectList<T>", containing all objects of type @a T. In contrast to
    @ref orxonox::Iterator "Iterator<T>", this iterator is limited to the object-list of type @a T.
    It is, however, much faster as it doesn't need a @c dynamic_cast.

    Usage:
    @code
    for (ObjectListIterator<myClass> it = ObjectList<myClass>::begin(); it != ObjectList<myClass>::end(); ++it)
    {
        it->someFunction(...);
        myClass* myObject = *it;
    }
    @endcode

    @note @ref orxonox::ObjectList::iterator "ObjectList<T>::iterator" is identical to
          @ref orxonox::ObjectListIterator "ObjectListIterator<T>" (it's just a typedef).
*/

#ifndef _ObjectListIterator_H__
#define _ObjectListIterator_H__

#include "core/CorePrereqs.h"
#include "core/class/Identifier.h"
#include "ObjectList.h"
#include "IteratorBase.h"

namespace orxonox
{
    /**
        @brief ObjectListIterator<T> allows to iterate through the ObjectList of class @a T.

        @see See @ref ObjectListIteratorExample "ObjectListIterator.h" for more information an example.
    */
    template <class T>
    class ObjectListIterator : public IteratorBase<T, ObjectListIterator<T> >
    {
        template <class I>
        friend class Iterator;

        public:
            /**
                @brief Constructor: Sets the element, whereon the ObjectListIterator points, to zero.
            */
            inline ObjectListIterator() : IteratorBase<T, ObjectListIterator<T> >(NULL) {}

            /**
                @brief Constructor: Sets this element to a given element.
                @param element The element to start with
            */
            inline ObjectListIterator(ObjectListElement<T>* element) : IteratorBase<T, ObjectListIterator<T> >(element) {}

            /**
                @brief Constructor: Sets this element to the element of another ObjectListIterator.
                @param other The other ObjectListIterator
            */
            inline ObjectListIterator(const IteratorBase<T, ObjectListIterator<T> >& other) : IteratorBase<T, ObjectListIterator<T> >(other) {}

            /**
                @brief Overloading of the *it operator: returns the pointer to the object.
                @return The object the ObjectListIterator points at
            */
            inline T* operator*() const
            {
                return static_cast<ObjectListElement<T>*>(this->element_)->object_;
            }

            /**
                @brief Overloading of the it-> operator: returns the pointer to the object.
                @return The object the ObjectListIterator points at
            */
            inline T* operator->() const
            {
                return static_cast<ObjectListElement<T>*>(this->element_)->object_;
            }
    };
}

#endif /* _ObjectListIterator_H__ */
