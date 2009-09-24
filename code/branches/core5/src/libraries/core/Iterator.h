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
    @brief Definition and implementation of the Iterator class.

    The Iterator of a given class allows to iterate through an ObjectList. Objects in
    this list are casted to the template argument of the Iterator.

    Usage:
    for (Iterator<myClass> it = anyidentifier->getObjects()->begin(); it != anyidentifier->getObjects()->end(); ++it)
    {
        it->someFunction(...);
        myClass* myObject = *it;
    }
*/

#ifndef _Iterator_H__
#define _Iterator_H__

#include "CorePrereqs.h"

#include "Identifier.h"
#include "ObjectListBase.h"

namespace orxonox
{
    //! The Iterator allows to iterate through a given ObjectList
    template <class T = OrxonoxClass>
    class Iterator
    {
        public:
            /**
                @brief Constructor: Sets the element, whereon the iterator points, to zero.
            */
            inline Iterator()
            {
                this->element_ = 0;
                this->list_ = 0;
            }

            /**
                @brief Constructor: Sets this element to the exported element.
                @param exp The exported element
            */
            inline Iterator(const ObjectListBase::Export& exp)
            {
                this->element_ = exp.element_;
                this->list_ = exp.list_;
                this->list_->registerIterator(this);
            }

            /**
                @brief Constructor: Sets this element to the element of another Iterator.
                @param other The other Iterator
            */
            inline Iterator(const Iterator<T>& other)
            {
                this->element_ = other.element_;
                this->list_ = other.list_;
                this->list_->registerIterator(this);
            }

            /**
                @brief Constructor: Sets this element to a given element
                @param element The element
            */
            template <class O>
            inline Iterator(ObjectListElement<O>* element)
            {
                this->element_ = (element) ? static_cast<ObjectListBaseElement*>(element) : 0;
                this->list_ = ClassIdentifier<O>::getIdentifier()->getObjects();
                this->list_->registerIterator(this);
            }

            /**
                @brief Constructor: Sets this element to the element an ObjectListIterator.
                @param other The ObjectListIterator
            */
            template <class O>
            inline Iterator(const ObjectListIterator<O>& other)
            {
                this->element_ = (other.element_) ? static_cast<ObjectListBaseElement*>(other.element_) : 0;
                this->list_ = ClassIdentifier<O>::getIdentifier()->getObjects();
                this->list_->registerIterator(this);
            }

            /**
                @brief Unregisters the Iterator from the ObjectList.
            */
            inline ~Iterator()
            {
                this->list_->unregisterIterator(this);
            }

            /**
                @brief Assigns an exported element.
                @param exp The exported element
            */
            inline const Iterator<T>& operator=(const ObjectListBase::Export& exp)
            {
                if (this->list_)
                    this->list_->unregisterIterator(this);

                this->element_ = exp.element_;
                this->list_ = exp.list_;
                this->list_->registerIterator(this);

                return (*this);
            }

            /**
                @brief Assigns the element of another Iterator.
                @param other The other Iterator
            */
            inline const Iterator<T>& operator=(const Iterator<T>& other)
            {
                if (this->list_)
                    this->list_->unregisterIterator(this);

                this->element_ = other.element_;
                this->list_ = other.list_;
                this->list_->registerIterator(this);

                return (*this);
            }

            /**
                @brief Assigns a given element.
                @param element The element
            */
            template <class O>
            inline const Iterator<T>& operator=(ObjectListElement<O>* element)
            {
                if (this->list_)
                    this->list_->unregisterIterator(this);

                this->element_ = (element) ? static_cast<ObjectListBaseElement*>(element) : 0;
                this->list_ = ClassIdentifier<O>::getIdentifier()->getObjects();
                this->list_->registerIterator(this);

                return (*this);
            }

            /**
                @brief Assigns the element of an ObjectListIterator.
                @param other The ObjectListIterator
            */
            template <class O>
            inline const Iterator<T>& operator=(const ObjectListIterator<O>& other)
            {
                if (this->list_)
                    this->list_->unregisterIterator(this);

                this->element_ = (other.element_) ? static_cast<ObjectListBaseElement*>(other.element_) : 0;
                this->list_ = ClassIdentifier<O>::getIdentifier()->getObjects();
                this->list_->registerIterator(this);

                return (*this);
            }

            /**
                @brief Overloading of the ++it operator: Iterator points to the next object in the list.
                @return The Iterator itself
            */
            inline const Iterator<T>& operator++()
            {
                this->element_ = this->element_->next_;
                return *this;
            }

            /**
                @brief Overloading of the it++ operator: Iterator points to the next object in the list.
                @return The Iterator itself
            */
            inline Iterator<T> operator++(int i)
            {
                Iterator<T> copy = *this;
                this->element_ = this->element_->next_;
                return copy;
            }

            /**
                @brief Overloading of the --it operator: Iterator points to the previous object in the list.
                @return The Iterator itself
            */
            inline const Iterator<T>& operator--()
            {
                this->element_ = this->element_->prev_;
                return *this;
            }

            /**
                @brief Overloading of the it-- operator: Iterator points to the previous object in the list.
                @return The Iterator itself
            */
            inline Iterator<T> operator--(int i)
            {
                Iterator<T> copy = *this;
                this->element_ = this->element_->prev_;
                return copy;
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

            /**
                @brief Overloading of the typecast-operator to bool: returns true if the iterator points to an existing object.
                @return True if the Iterator points to an existing object.
            */
            inline operator bool() const
            {
                return (this->element_ != 0);
            }

            /**
                @brief Overloading of the == operator to compare with another Iterator.
                @param compare The other Iterator
                @return True if the iterators point to the same element
            */
            inline bool operator==(const Iterator<T>& compare) const
            {
                return (this->element_ == compare.element_);
            }

            /**
                @brief Overloading of the != operator to compare with another Iterator.
                @param compare The other Iterator
                @return True if the iterators point to different elements
            */
            inline bool operator!=(const Iterator<T>& compare) const
            {
                return (this->element_ != compare.element_);
            }

            /**
                @brief Increments the Iterator if it points at the given object.
                @param object The object to compare with
            */
            inline void incrementIfEqual(OrxonoxClass* object)
            {
                if (this->element_ && this->element_->objectBase_ == object)
                    this->operator++();
            }

        protected:
            ObjectListBaseElement* element_;       //!< The element the Iterator points at
            ObjectListBase* list_;                 //!< The list wherein the element is
    };

    typedef Iterator<OrxonoxClass> BaseIterator;
}

#endif /* _Iterator_H__ */
