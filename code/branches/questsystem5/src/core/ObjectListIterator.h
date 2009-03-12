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

    The ObjectListIterator of a given class allows to iterate through the
    ObjectList of this class, containing all objects of that type.
    This is the only way to access the objects stored in an ObjectList.

    Usage:
    for (ObjectListIterator<myClass> it = ObjectList<myClass>::begin(); it != ObjectList<myClass>::end(); ++it)
    {
        it->someFunction(...);
        myClass* myObject = *it;
    }
*/

#ifndef _ObjectListIterator_H__
#define _ObjectListIterator_H__

#include "CorePrereqs.h"
#include "ObjectListBase.h"

namespace orxonox
{
    //! The Iterator allows to iterate through the ObjectList of a given class.
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
                this->iterator_ = ClassIdentifier<T>::getIdentifier()->getObjects()->registerObjectListIterator(this);
            }

            /**
                @brief Constructor: Sets this element to a given element.
                @param element The element to start with
            */
            inline ObjectListIterator(ObjectListElement<T>* element)
            {
                this->element_ = element;
                this->iterator_ = ClassIdentifier<T>::getIdentifier()->getObjects()->registerObjectListIterator(this);
            }

            /**
                @brief Constructor: Sets this element to the element of another ObjectListIterator.
                @param other The other ObjectListIterator
            */
            inline ObjectListIterator(const ObjectListIterator<T>& other)
            {
                this->element_ = other.element_;
                this->iterator_ = ClassIdentifier<T>::getIdentifier()->getObjects()->registerObjectListIterator(this);
            }

            /**
                @brief Unregisters the ObjectListIterator from the ObjectList.
            */
            inline ~ObjectListIterator()
            {
                ClassIdentifier<T>::getIdentifier()->getObjects()->unregisterObjectListIterator(this->iterator_);
            }

            /**
                @brief Assigns an ObjectListElement.
                @param element The ObjectListElement
            */
            inline const ObjectListIterator<T>& operator=(ObjectListElement<T>* element)
            {
                this->element_ = element;
                return (*this);
            }

            /**
                @brief Assigns the element of another ObjectListIterator.
                @param element The other ObjectListIterator
            */
            inline const ObjectListIterator<T>& operator=(const ObjectListIterator<T>& other)
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
                if (this->element_)
                    this->element_ = static_cast<ObjectListElement<T>*>(this->element_->next_);
                return *this;
            }

            /**
                @brief Overloading of the it++ operator: ObjectListIterator points to the next object in the list.
                @return The ObjectListIterator itself
            */
            inline ObjectListIterator<T> operator++(int i)
            {
                ObjectListIterator<T> copy = *this;
                if (this->element_)
                    this->element_ = static_cast<ObjectListElement<T>*>(this->element_->next_);
                return copy;
            }

            /**
                @brief Overloading of the --it operator: ObjectListIterator points to the previous object in the list.
                @return The ObjectListIterator itself
            */
            inline const ObjectListIterator<T>& operator--()
            {
                if (this->element_)
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
                if (this->element_)
                    this->element_ = static_cast<ObjectListElement<T>*>(this->element_->prev_);
                return copy;
            }

            /**
                @brief Overloading of the *it operator: returns the pointer to the object.
                @return The object the ObjectListIterator points at
            */
            inline T* operator*() const
            {
                if (this->element_)
                    return this->element_->object_;
                else
                    return 0;
            }

            /**
                @brief Overloading of the it-> operator: returns the pointer to the object.
                @return The object the ObjectListIterator points at
            */
            inline T* operator->() const
            {
                if (this->element_)
                    return this->element_->object_;
                else
                    return 0;
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
            ObjectListElement<T>* element_;        //!< The element the Iterator points at
            std::list<void*>::iterator iterator_;  //!< The iterator in the notifying list of the ObjectList
    };
}

// Include ObjectList.h so the user only has to include one file: Iterator.h
#include "ObjectList.h"

#endif /* _ObjectListIterator_H__ */
