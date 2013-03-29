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
    @brief Definition of the IteratorBase class, used to iterate through object-lists.
*/

#ifndef _IteratorBase_H__
#define _IteratorBase_H__

#include "core/CorePrereqs.h"

#include "ObjectListBase.h"

namespace orxonox
{
    /**
        @brief The Iterator allows to iterate through object lists.
        It serves as base class for @ref ObjectListIterator and @ref Iterator
    */
    template <class T, class I>
    class IteratorBase
    {
        public:
            /**
                @brief Constructor: Sets the element, whereon the iterator points, to zero.
            */
            inline IteratorBase(ObjectListBaseElement* element)
            {
                this->element_ = element;
                this->registerIterator();
            }

            /**
                @brief Constructor: Sets this element to the element of another Iterator.
                @param other The other Iterator
            */
            inline IteratorBase(const IteratorBase& other)
            {
                this->element_ = other.element_;
                this->registerIterator();
            }

            /**
                @brief Unregisters the Iterator from the ObjectList.
            */
            inline ~IteratorBase()
            {
                this->unregisterIterator();
            }

            /**
                @brief Assigns a given element.
                @param element The element
            */
            inline IteratorBase<T, I>& operator=(ObjectListElement<T>* element)
            {
                this->setElement(element);
                return (*this);
            }

            /**
                @brief Assigns the element of another Iterator.
                @param other The other Iterator
            */
            inline IteratorBase<T, I>& operator=(const IteratorBase<T, I>& other)
            {
                this->setElement(other.element_);
                return (*this);
            }

            /**
                @brief Overloading of the ++it operator: Iterator points to the next object in the list.
                @return The Iterator itself
            */
            inline const IteratorBase<T, I>& operator++()
            {
                this->element_ = this->element_->next_;
                return *this;
            }

            /**
                @brief Overloading of the it++ operator: Iterator points to the next object in the list.
                @return The Iterator itself
            */
            inline I operator++(int)
            {
                I copy = *this;
                this->element_ = this->element_->next_;
                return copy;
            }

            /**
                @brief Overloading of the --it operator: Iterator points to the previous object in the list.
                @return The Iterator itself
            */
            inline const IteratorBase<T, I>& operator--()
            {
                this->element_ = this->element_->prev_;
                return *this;
            }

            /**
                @brief Overloading of the it-- operator: Iterator points to the previous object in the list.
                @return The Iterator itself
            */
            inline I operator--(int i)
            {
                I copy = *this;
                this->element_ = this->element_->prev_;
                return copy;
            }

            /**
                @brief Overloading of the typecast-operator to bool: returns true if the iterator points to an existing object.
                @return True if the Iterator points to an existing object.
            */
            inline operator bool() const
            {
                return (this->element_ != NULL);
            }

            /**
                @brief Overloading of the == operator to compare with another Iterator.
                @param compare The other Iterator
                @return True if the iterators point to the same element
            */
            inline bool operator==(const IteratorBase<T, I>& compare) const
            {
                return (this->element_ == compare.element_);
            }

            /**
                @brief Overloading of the != operator to compare with another Iterator.
                @param compare The other Iterator
                @return True if the iterators point to different elements
            */
            inline bool operator!=(const IteratorBase<T, I>& compare) const
            {
                return (this->element_ != compare.element_);
            }

            /**
                @brief Increments the Iterator if it points at the given object.
                @param object The object to compare with
            */
            inline void incrementIfEqual(Listable* object)
            {
                if (this->element_ && this->element_->objectBase_ == object)
                    this->operator++();
            }

        protected:
            inline void setElement(ObjectListBaseElement* element)
            {
                this->unregisterIterator();
                this->element_ = element;
                this->registerIterator();
            }

            /**
             * @brief Registers the Iterator at the list to which it belongs
             */
            inline void registerIterator()
            {
                if (this->element_)
                {
                    this->list_ = this->element_->list_;
                    this->list_->registerIterator(this);
                }
                else
                    this->list_ = NULL;
            }

            /**
             * @brief Unregisters the Iterator from the list (if any)
             */
            inline void unregisterIterator()
            {
                if (this->list_)
                    this->list_->unregisterIterator(this);
            }

            ObjectListBaseElement* element_;    //!< The element the Iterator points at
            ObjectListBase* list_;              //!< The list in which the Iterator registered itself
    };
}

#endif /* _IteratorBase_H__ */
