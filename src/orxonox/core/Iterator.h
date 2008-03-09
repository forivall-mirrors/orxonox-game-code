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

/**
    @file Iterator.h
    @brief Definition and implementation of the Iterator class.

    The Iterator of a given class allows to iterate through an ObjectList, containing all objects of that type.
    This is the only way to access the objects stored in an ObjectList.

    Usage:
    for (Iterator<class> it = ObjectList<class>::start(); it != 0; ++it)
    {
        it->someFunction(...);
        class* myObject = *it;
    }

    Warning: Don't delete objects directly through the iterator.
*/

#ifndef _Iterator_H__
#define _Iterator_H__

#include "CorePrereqs.h"

#include "ObjectList.h"
#include "Debug.h"

namespace orxonox
{
    //! The iterator allows to iterate through an ObjectList of a given class.
    template <class T>
    class Iterator
    {
        public:
            /**
                @brief Constructor: Sets the element, whereon the iterator points, to zero.
            */
            Iterator()
            {
                this->element_ = 0;
            }

            /**
                @brief Constructor: Sets the element, whereon the iterator points, to a given element.
                @param element The element to start with
            */
            Iterator(ObjectListElement<T>* element)
            {
                this->element_ = element;
            }

            /**
                @brief Assigns an element to the iterator.
                @param element The element
            */
            Iterator<T> operator=(ObjectListElement<T>* element)
            {
                this->element_ = element;
            }

            /**
                @brief Overloading of the ++it operator: Iterator points to the next object in the list.
                @return The Iterator itself
            */
            Iterator<T> operator++()
            {
                this->element_ = this->element_->next_;
                return *this;
            }

            /**
                @brief Overloading of the it++ operator: Iterator points to the next object in the list.
                @return The Iterator itself
            */
            Iterator<T> operator++(int i)
            {
                Iterator<T> copy = *this;
                this->element_ = this->element_->next_;
                return copy;
            }

            /**
                @brief Overloading of the --it operator: Iterator points to the previous object in the list.
                @return The Iterator itself
            */
            Iterator<T> operator--()
            {
                this->element_ = this->element_->prev_;
                return *this;
            }

            /**
                @brief Overloading of the it-- operator: Iterator points to the previous object in the list.
                @return The Iterator itself
            */
            Iterator<T> operator--(int i)
            {
                Iterator<T> copy = *this;
                this->element_ = this->element_->prev_;
                return copy;
            }

            /**
                @brief Overloading of the *it operator: returns the pointer to the object.
                @return The object the Iterator points at
            */
            T* operator*()
            {
                return this->element_->object_;
            }

            /**
                @brief Overloading of the it-> operator: returns the pointer to the object.
                @return The object the Iterator points at
            */
            T* operator->() const
            {
                return this->element_->object_;

            }

            /**
                @brief Overloading of the typecast-operator to bool: returns true if the iterator points to an existing object.
                @return True if the iterator points to an existing object.
            */
            operator bool()
            {
                return (this->element_ != 0);
            }

            /**
                @brief Overloading of the (it != int) operator: Used for (it != 0) instead of typecast-operator to bool.
                @param compare The integer (must be zero, everything else makes no sense).
                @return True if the iterator points to an existing object.
            */
            bool operator!=(ObjectListElement<T>* compare)
            {
                return (this->element_ != compare);
            }

        private:
            ObjectListElement<T>* element_;     //!< The element the Iterator points at
    };
}

#endif /* _Iterator_H__ */
