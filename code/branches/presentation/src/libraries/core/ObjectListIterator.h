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

#include "CorePrereqs.h"
#include "Identifier.h"
#include "ObjectList.h"

namespace orxonox
{
    /**
        @brief ObjectListIterator<T> allows to iterate through the ObjectList of class @a T.

        @see See @ref ObjectListIteratorExample "ObjectListIterator.h" for more information an example.
    */
    template <class T>
    class ObjectListIterator
    {
        template <class I>
        friend class Iterator;

        public:
            /**
                @brief Constructor: Sets the element, whereon the ObjectListIterator points, to zero.
            */
            inline ObjectListIterator()
            {
                this->element_ = 0;
                ClassIdentifier<T>::getIdentifier()->getObjects()->registerObjectListIterator(this);
            }

            /**
                @brief Constructor: Sets this element to a given element.
                @param element The element to start with
            */
            inline ObjectListIterator(ObjectListElement<T>* element)
            {
                this->element_ = element;
                ClassIdentifier<T>::getIdentifier()->getObjects()->registerObjectListIterator(this);
            }

            /**
                @brief Constructor: Sets this element to the element of another ObjectListIterator.
                @param other The other ObjectListIterator
            */
            inline ObjectListIterator(const ObjectListIterator<T>& other)
            {
                this->element_ = other.element_;
                ClassIdentifier<T>::getIdentifier()->getObjects()->registerObjectListIterator(this);
            }

            /**
                @brief Unregisters the ObjectListIterator from the ObjectList.
            */
            inline ~ObjectListIterator()
            {
                ClassIdentifier<T>::getIdentifier()->getObjects()->unregisterObjectListIterator(this);
            }

            /**
                @brief Assigns an ObjectListElement.
                @param element The ObjectListElement
            */
            inline ObjectListIterator<T>& operator=(ObjectListElement<T>* element)
            {
                this->element_ = element;
                return (*this);
            }

            /**
                @brief Assigns the element of another ObjectListIterator.
                @param other The other ObjectListIterator
            */
            inline ObjectListIterator<T>& operator=(const ObjectListIterator<T>& other)
            {
                this->element_ = other.element_;
                return (*this);
            }

            /**
                @brief Overloading of the ++it operator: ObjectListIterator points to the next object in the list.
                @return The ObjectListIterator itself
            */
            inline const ObjectListIterator<T>& operator++()
            {
                this->element_ = static_cast<ObjectListElement<T>*>(this->element_->next_);
                return *this;
            }

            /**
                @brief Overloading of the it++ operator: ObjectListIterator points to the next object in the list.
                @return The ObjectListIterator itself
            */
            inline ObjectListIterator<T> operator++(int)
            {
                ObjectListIterator<T> copy = *this;
                this->element_ = static_cast<ObjectListElement<T>*>(this->element_->next_);
                return copy;
            }

            /**
                @brief Overloading of the --it operator: ObjectListIterator points to the previous object in the list.
                @return The ObjectListIterator itself
            */
            inline const ObjectListIterator<T>& operator--()
            {
                this->element_ = static_cast<ObjectListElement<T>*>(this->element_->prev_);
                return *this;
            }

            /**
                @brief Overloading of the it-- operator: ObjectListIterator points to the previous object in the list.
                @return The ObjectListIterator itself
            */
            inline ObjectListIterator<T> operator--(int i)
            {
                ObjectListIterator<T> copy = *this;
                this->element_ = static_cast<ObjectListElement<T>*>(this->element_->prev_);
                return copy;
            }

            /**
                @brief Overloading of the *it operator: returns the pointer to the object.
                @return The object the ObjectListIterator points at
            */
            inline T* operator*() const
            {
                return this->element_->object_;
            }

            /**
                @brief Overloading of the it-> operator: returns the pointer to the object.
                @return The object the ObjectListIterator points at
            */
            inline T* operator->() const
            {
                return this->element_->object_;
            }

            /**
                @brief Overloading of the typecast-operator to bool: returns true if the ObjectListIterator points to an existing object.
                @return True if the ObjectListIterator points to an existing object.
            */
            inline operator bool() const
            {
                return (this->element_ != 0);
            }

            /**
                @brief Overloading of the == operator to compare with another ObjectListIterator.
                @param compare The other ObjectListIterator
                @return True if the ObjectListIterator point to the same element
            */
            inline bool operator==(const ObjectListIterator<T>& compare) const
            {
                return (this->element_ == compare.element_);
            }

            /**
                @brief Overloading of the != operator to compare with another ObjectListIterator.
                @param compare The other ObjectListIterator
                @return True if the ObjectListIterator point to different elements
            */
            inline bool operator!=(const ObjectListIterator<T>& compare) const
            {
                return (this->element_ != compare.element_);
            }

            /**
                @brief Increments the ObjectListIterator if it points at the given object.
                @param object The object to compare with
            */
            inline void incrementIfEqual(OrxonoxClass* object)
            {
                if (this->element_ && this->element_->objectBase_ == object)
                    this->operator++();
            }

        private:
            ObjectListElement<T>* element_;        //!< The element the iterator points at
    };
}

#endif /* _ObjectListIterator_H__ */
