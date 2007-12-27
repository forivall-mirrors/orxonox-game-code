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

/*!
    @file ObjectList.h
    @brief Definition and implementation of the ObjectList class.

    The ObjectList is a double-linked list, used by Identifiers to store all objects of a given class.
    Newly created objects are added through the RegisterObject-macro in its constructor.
    Use Iterator<class> to iterate through all objects of the class.
*/

#ifndef _ObjectList_H__
#define _ObjectList_H__

#include "CorePrereqs.h"

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
            static ObjectList<T>* getList();

            ObjectListElement<T>* add(T* object);
//            void remove(OrxonoxClass* object, bool bIterateForwards = true);

            /** @returns the first element in the list */
            inline static Iterator<T> start()
                { return Iterator<T>(getList()->first_); }

            /** @returns the last element in the list */
            inline static Iterator<T> end()
                { return Iterator<T>(getList()->last_); }

            ObjectListElement<T>* first_;       //!< The first element in the list
            ObjectListElement<T>* last_;        //!< The last element in the list

        private:
            ObjectList();
            ~ObjectList();
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
        @returns a pointer to the only existing instance for the given class T.
    */
    template <class T>
    ObjectList<T>* ObjectList<T>::getList()
    {
        static ObjectList<T> theOnlyObjectListObjectForClassT = ObjectList<T>();
        return &theOnlyObjectListObjectForClassT;
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


//    /**
//        @brief Removes an object from the list.
//        @param object The object to remove
//        @param bIterateForwards If true: Start searching the object at the beginning of the list
//    */
    /*
    template <class T>
    void ObjectList<T>::remove(OrxonoxClass* object, bool bIterateForwards)
    {
        if (!object || !this->first_ || !this->last_)
            return;

        // If there's only one object in the list, we have to set first_ and last_ to zero
        if (this->first_ == this->last_)
        {
            if (this->first_->object_ == object)
            {
                delete this->first_;
                this->first_ = 0;
                this->last_ = 0;
            }

            return;
        }

        // Now we are sure we have more than one element in the list
        if (bIterateForwards)
        {
            // Start at the beginning of the list

            // Check if it's the first object
            if (this->first_->object_ == object)
            {
                ObjectListElement<T>* temp = this->first_->next_;
                delete this->first_;
                this->first_ = temp;
                this->first_->prev_ = 0;

                return;
            }

            // Iterate through the whole list
            ObjectListElement<T>* temp = this->first_;
            while (temp->next_)
            {
                if (temp->next_->object_ == object)
                {
                    ObjectListElement<T>* temp2 = temp->next_->next_;
                    delete temp->next_;
                    temp->next_ = temp2;
                    if (temp2)
                        temp2->prev_ = temp;
                    else
                        this->last_ = temp; // If there is no next_, we deleted the last element and have to update the last_ pointer.

                    return;
                }

                temp = temp->next_;
            }
        }
        else
        {
            // Start at the end of the list

            // Check if it's the last object
            if (this->last_->object_ == object)
            {
                ObjectListElement<T>* temp = this->last_->prev_;
                delete this->last_;
                this->last_ = temp;
                this->last_->next_ = 0;

                return;
            }

            // Iterate through the whole list
            ObjectListElement<T>* temp = this->last_;
            while (temp->prev_)
            {
                if (temp->prev_->object_ == object)
                {
                    ObjectListElement<T>* temp2 = temp->prev_->prev_;
                    delete temp->prev_;
                    temp->prev_ = temp2;
                    if (temp2)
                        temp2->next_ = temp;
                    else
                        this->first_ = temp; // If there is no prev_, we deleted the first element and have to update the first_ pointer.

                    return;
                }

                temp = temp->prev_;
            }
        }
    }
    */
}

#endif /* _ObjectList_H__ */
