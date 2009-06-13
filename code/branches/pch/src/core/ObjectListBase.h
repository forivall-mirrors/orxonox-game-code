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
    @brief Definition of the ObjectListBase class.

    The ObjectListBase is a double-linked list, used by Identifiers to store all objects of a given class.
    Newly created objects are added through the RegisterObject-macro in its constructor.
*/

#ifndef _ObjectListBase_H__
#define _ObjectListBase_H__

#include "CorePrereqs.h"

#include <vector>
#include "OrxonoxClass.h"

namespace orxonox
{
    // ###############################
    // ###  ObjectListBaseElement  ###
    // ###############################
    //! The list-element of the ObjectListBase
    class _CoreExport ObjectListBaseElement
    {
        public:
            /**
                @brief Constructor: Creates the list-element with an object.
                @param object The object to store
            */
            ObjectListBaseElement(OrxonoxClass* objectBase) : next_(0), prev_(0), objectBase_(objectBase) {}

            ObjectListBaseElement* next_;       //!< The next element in the list
            ObjectListBaseElement* prev_;       //!< The previous element in the list
            OrxonoxClass* objectBase_;
    };


    // ###############################
    // ###    ObjectListElement    ###
    // ###############################
    //! The list-element that actually contains the object
    template <class T>
    class ObjectListElement : public ObjectListBaseElement
    {
        public:
            ObjectListElement(T* object) : ObjectListBaseElement(static_cast<OrxonoxClass*>(object)), object_(object) {}
            T* object_;              //!< The object
    };


    // ###############################
    // ###     ObjectListBase      ###
    // ###############################
    //! The ObjectListBase contains all objects of a given class.
    /**
        The ObjectListBase is used by Identifiers to store all objects of their given class.
        Use ObjectList<T> to get the list of all T's and Iterator<T> to iterate through them.
    */
    class _CoreExport ObjectListBase
    {
        friend class MetaObjectListElement;

        public:
            ObjectListBase(Identifier* identifier);
            ~ObjectListBase();

            ObjectListBaseElement* add(ObjectListBaseElement* element);

            struct Export
            {
                Export(ObjectListBase* list, ObjectListBaseElement* element) : list_(list), element_(element) {}
                ObjectListBase* list_;
                ObjectListBaseElement* element_;
            };

            /** @brief Returns a pointer to the first element in the list. @return The element */
            inline Export begin() { return ObjectListBase::Export(this, this->first_); }
            /** @brief Returns a pointer to the element after the last element in the list. @return The element */
            inline Export end() { return ObjectListBase::Export(this, 0); }
            /** @brief Returns a pointer to the last element in the list. @return The element */
            inline Export rbegin() { return ObjectListBase::Export(this, this->last_); }
            /** @brief Returns a pointer to the element in front of the first element in the list. @return The element */
            inline Export rend() { return ObjectListBase::Export(this, 0); }

            inline void registerIterator(void* iterator) { this->iterators_.push_back(iterator); }
            inline void unregisterIterator(void* iterator)
            {
                for (unsigned int i = 0; i < this->iterators_.size(); ++i)
                {
                    if (iterators_[i] == iterator)
                    {
                        iterators_.erase(iterators_.begin() + i);
                        break;
                    }
                }
            }
            inline void registerObjectListIterator(void* iterator) { this->objectListIterators_.push_back(iterator); }
            inline void unregisterObjectListIterator(void* iterator)
            {
                for (unsigned int i = 0; i < this->objectListIterators_.size(); ++i)
                {
                    if (objectListIterators_[i] == iterator)
                    {
                        objectListIterators_.erase(objectListIterators_.begin() + i);
                        break;
                    }
                }
            }
            void notifyIterators(OrxonoxClass* object) const;

            inline Identifier* getIdentifier() const { return this->identifier_; }

        private:
            Identifier* identifier_;                 //!< The Iterator owning this list
            ObjectListBaseElement* first_;           //!< The first element in the list
            ObjectListBaseElement* last_;            //!< The last element in the list
            std::vector<void*> iterators_;           //!< A list of Iterators pointing on an element in this list
            std::vector<void*> objectListIterators_; //!< A list of ObjectListIterators pointing on an element in this list
    };
}

#endif /* _ObjectListBase_H__ */
