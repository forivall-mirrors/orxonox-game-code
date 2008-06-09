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
    @file IteratorBase.h
    @brief Definition and implementation of the Iterator class.

    The Iterator of a given class allows to iterate through an ObjectListBase, containing all objects of that type.
    This is the only way to access the objects stored in an ObjectListBase.
*/

#ifndef _IteratorBase_H__
#define _IteratorBase_H__

#include "CorePrereqs.h"

#include "ObjectListBase.h"

namespace orxonox
{
    //! The IteratorBase allows to iterate through an ObjectListBase
    class _CoreExport IteratorBase
    {
        public:
            /**
                @brief Default Constructor: Sets the element to zero.
            */
            inline IteratorBase()
            {
                this->element_ = 0;
            }

            /**
                @brief Constructor: Sets the element, whereon the iterator points, to a given element.
                @param element The element to start with
            */
            inline IteratorBase(ObjectListBaseElement* element)
            {
                this->element_ = element;
            }

            /**
                @brief Constructor: Sets this element to the element of the other IteratorBase.
                @param other The other IteratorBase
            */
            inline IteratorBase(const IteratorBase& other)
            {
                this->element_ = other.element_;
            }

            /**
                @brief Assigns the element of another IteratorBase.
                @param element The other IteratorBase
            */
            inline IteratorBase operator=(const IteratorBase& other)
            {
                this->element_ = other.element_;
                return (*this);
            }

            /**
                @brief Overloading of the ++it operator: IteratorBase points to the next object in the list.
                @return The IteratorBase itself
            */
            inline IteratorBase operator++()
            {
                if (this->element_)
                    this->element_ = this->element_->next_;
                return (*this);
            }

            /**
                @brief Overloading of the it++ operator: IteratorBase points to the next object in the list.
                @return The IteratorBase itself
            */
            inline IteratorBase operator++(int i)
            {
                IteratorBase copy = *this;
                if (this->element_)
                    this->element_ = this->element_->next_;
                return copy;
            }

            /**
                @brief Overloading of the --it operator: IteratorBase points to the previous object in the list.
                @return The IteratorBase itself
            */
            inline IteratorBase operator--()
            {
                if (this->element_)
                    this->element_ = this->element_->prev_;
                return (*this);
            }

            /**
                @brief Overloading of the it-- operator: IteratorBase points to the previous object in the list.
                @return The IteratorBase itself
            */
            inline IteratorBase operator--(int i)
            {
                IteratorBase copy = *this;
                if (this->element_)
                    this->element_ = this->element_->prev_;
                return copy;
            }

            /**
                @brief Overloading of the *it operator: returns the pointer to the object.
                @return The object the IteratorBase points at
            */
            inline OrxonoxClass* operator*()
            {
                if (this->element_)
                    return this->element_->object_;
                else
                    return 0;
            }

            /**
                @brief Overloading of the it-> operator: returns the pointer to the object.
                @return The object the IteratorBase points at
            */
            inline OrxonoxClass* operator->() const
            {
                if (this->element_)
                    return this->element_->object_;
                else
                    return 0;
            }

            /**
                @brief Overloading of the typecast-operator to bool: returns true if the iterator points to an existing object.
                @return True if the IteratorBase points to an existing object.
            */
            inline operator bool()
            {
                return (this->element_ != 0);
            }

            /**
                @brief Overloading of the == operator to compare with another IteratorBase.
                @param compare The other IteratorBase
                @return True if the iterators point to the same element
            */
            inline bool operator==(const IteratorBase& compare)
            {
                return (this->element_ == compare.element_);
            }

            /**
                @brief Overloading of the != operator to compare with another IteratorBase.
                @param compare The other IteratorBase
                @return True if the iterators point to different elements
            */
            inline bool operator!=(const IteratorBase& compare)
            {
                return (this->element_ != compare.element_);
            }

        protected:
            ObjectListBaseElement* element_;     //!< The element the Iterator points at
    };
}

#endif /* _IteratorBase_H__ */
