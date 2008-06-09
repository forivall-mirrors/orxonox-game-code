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
    @file Iterator.h
    @brief Definition and implementation of the Iterator class.

    The Iterator of a given class allows to iterate through an ObjectList, containing all objects of that type.
    This is the only way to access the objects stored in an ObjectList.

    Usage:
    for (Iterator<myClass> it = ObjectList<myClass>::begin(); it != ObjectList<myClass>::end(); ++it)
    {
        it->someFunction(...);
        myClass* myObject = *it;
    }
*/

#ifndef _Iterator_H__
#define _Iterator_H__

#include "CorePrereqs.h"

#include "IteratorBase.h"

namespace orxonox
{
    //! The Iterator allows to iterate through the ObjectList of a given class.
    template <class T>
    class Iterator : public IteratorBase
    {
        public:
            /**
                @brief Constructor: Sets the element, whereon the iterator points, to zero.
            */
            inline Iterator()
            {
                ClassIdentifier<T>::getIdentifier()->getObjects()->registerIterator(this);
            }

            /**
                @brief Constructor: Sets the element, whereon the iterator points, to a given element.
                @param element The element to start with
            */
            inline Iterator(ObjectListBaseElement* element) : IteratorBase((ObjectListBaseElement*)element)
            {
                ClassIdentifier<T>::getIdentifier()->getObjects()->registerIterator(this);
            }

            /**
                @brief Unregisters the Iterator from the ObjectList.
            */
            inline ~Iterator()
            {
                ClassIdentifier<T>::getIdentifier()->getObjects()->unregisterIterator(this);
            }

            /**
                @brief Overloading of the ++it operator: Iterator points to the next object in the list.
                @return The Iterator itself
            */
            inline Iterator<T> operator++()
            {
                IteratorBase::operator++();
                return *this;
            }

            /**
                @brief Overloading of the it++ operator: Iterator points to the next object in the list.
                @return The Iterator itself
            */
            inline Iterator<T> operator++(int i)
            {
                Iterator<T> copy = *this;
                IteratorBase::operator++();
                return copy;
            }

            /**
                @brief Overloading of the --it operator: Iterator points to the previous object in the list.
                @return The Iterator itself
            */
            inline Iterator<T> operator--()
            {
                IteratorBase::operator--();
                return *this;
            }

            /**
                @brief Overloading of the it-- operator: Iterator points to the previous object in the list.
                @return The Iterator itself
            */
            inline Iterator<T> operator--(int i)
            {
                Iterator<T> copy = *this;
                IteratorBase::operator--();
                return copy;
            }

            /**
                @brief Overloading of the *it operator: returns the pointer to the object.
                @return The object the Iterator points at
            */
            inline T* operator*()
            {
                return dynamic_cast<T*>(IteratorBase::operator*());
            }

            /**
                @brief Overloading of the it-> operator: returns the pointer to the object.
                @return The object the Iterator points at
            */
            inline T* operator->() const
            {
                return dynamic_cast<T*>(IteratorBase::operator->());
            }

            /**
                @brief Overloading of the == operator to compare with another Iterator.
                @param compare The other Iterator
                @return True if the iterators point to the same element
            */
            inline bool operator==(const Iterator<T>& compare)
            {
                return (this->element_ == compare.element_);
            }

            /**
                @brief Overloading of the != operator to compare with another Iterator.
                @param compare The other Iterator
                @return True if the iterators point to different elements
            */
            inline bool operator!=(const Iterator<T>& compare)
            {
                return (this->element_ != compare.element_);
            }
    };
}

// Include ObjectList.h so the user only has to include one file: Iterator.h
#include "ObjectList.h"

#endif /* _Iterator_H__ */
