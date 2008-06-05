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
    @file ObjectList.h
    @brief Definition and implementation of the ObjectList class.

    The ObjectList is a double-linked list, used by Identifiers to store all objects of a given class.
    Newly created objects are added through the RegisterObject-macro in its constructor.
    Use Iterator<class> to iterate through all objects of the class.
*/

#ifndef _ObjectList_H__
#define _ObjectList_H__

#include <set>

#include "CorePrereqs.h"

#include "Iterator.h"
#include "Identifier.h"

namespace orxonox
{
    // ###############################
    // ###    ObjectListElement    ###
    // ###############################
    //! The list-element of the ObjectList
    template <class T>
    class ObjectListElement
    {
        public:
            ObjectListElement(T* object);

            T* object_;                     //!< The object
            ObjectListElement* next_;       //!< The next element in the list
            ObjectListElement* prev_;       //!< The previous element in the list
    };

    /**
        @brief Constructor: Creates the list-element with an object.
        @param object The object to store
    */
    template <class T>
    ObjectListElement<T>::ObjectListElement(T* object)
    {
        this->object_ = object;
        this->next_ = 0;
        this->prev_ = 0;
    }


    // ###############################
    // ###       ObjectList        ###
    // ###############################
    //! The ObjectList contains all objects of a given class.
    /**
        The ObjectList is used by Identifiers to store all objects of a given class.
        Use Iterator<class> to iterate through all objects in the list.
    */
    template <class T>
    class ObjectList
    {
        public:
            ObjectList();
            ~ObjectList();

            ObjectListElement<T>* add(T* object);

            /** @brief Returns the first element in the list. @return The first element */
            inline static Iterator<T> start()
                { return Iterator<T>(ClassIdentifier<T>::getIdentifier()->getObjects()->first_); }

            /** @brief Returns the first element in the list. @return The first element */
            inline static Iterator<T> begin()
                { return Iterator<T>(ClassIdentifier<T>::getIdentifier()->getObjects()->first_); }

            /** @brief Returns the last element in the list. @return The last element */
            inline static Iterator<T> end()
                { return Iterator<T>(ClassIdentifier<T>::getIdentifier()->getObjects()->last_); }

            inline void registerIterator(Iterator<T>* iterator)
                { this->iterators_.insert(this->iterators_.end(), (void*)iterator); }
            inline void unregisterIterator(Iterator<T>* iterator)
                { this->iterators_.erase((void*)iterator); }
            void notifyIterators(ObjectListElement<T>* element);

            ObjectListElement<T>* first_;       //!< The first element in the list
            ObjectListElement<T>* last_;        //!< The last element in the list

        private:
            std::set<void*> iterators_;  //!< A list of iterators pointing on an element in this list
    };

    /**
        @brief Constructor: Sets default values.
    */
    template <class T>
    ObjectList<T>::ObjectList()
    {
        this->first_ = 0;
        this->last_ = 0;
    }

    /**
        @brief Destructor: Deletes all list-elements, but NOT THE OBJECTS.
    */
    template <class T>
    ObjectList<T>::~ObjectList()
    {
        ObjectListElement<T>* temp;
        while (this->first_)
        {
            temp = this->first_->next_;
            delete this->first_;
            this->first_ = temp;
        }
    }

    /**
        @brief Increases all Iterators that currently point on the given element (because it gets removed).
        @param element The element that gets removed
    */
    template <class T>
    void ObjectList<T>::notifyIterators(ObjectListElement<T>* element)
    {
        for (std::set<void*>::iterator it = this->iterators_.begin(); it != this->iterators_.end(); ++it)
            if ((*(*((Iterator<T>*)(*it)))) == element->object_)
                ++(*((Iterator<T>*)(*it)));
    }

    /**
        @brief Adds a new object to the end of the list.
        @param object The object to add
        @return The pointer to the new ObjectListElement, needed by the MetaObjectList of the added object
    */
    template <class T>
    ObjectListElement<T>* ObjectList<T>::add(T* object)
    {
        if (!this->last_)
        {
            // If the list is empty
            this->last_ = new ObjectListElement<T>(object);
            this->first_ = this->last_; // There's only one object in the list now
        }
        else
        {
            // If the list isn't empty
            ObjectListElement<T>* temp = this->last_;
            this->last_ = new ObjectListElement<T>(object);
            this->last_->prev_ = temp;
            temp->next_ = this->last_;
        }

        return this->last_;
    }
}

#endif /* _ObjectList_H__ */
