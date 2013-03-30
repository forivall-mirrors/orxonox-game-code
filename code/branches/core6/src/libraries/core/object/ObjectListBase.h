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
    @brief Declaration of the ObjectListBase class which stores all objects of each class.

    orxonox::ObjectListBase is a double-linked list, used by @ref orxonox::Identifier "Identifiers"
    to store all objects of a given class. Newly created objects are added to the list through the
    @c RegisterObject() macro in the constructor.
*/

#ifndef _ObjectListBase_H__
#define _ObjectListBase_H__

#include "core/CorePrereqs.h"
#include <vector>
#include "Context.h"

namespace orxonox
{
    // ###############################
    // ###  ObjectListBaseElement  ###
    // ###############################
    /// The list-element of the ObjectListBase
    class _CoreExport ObjectListBaseElement
    {
        public:
            /**
                @brief Constructor: Creates the list-element with an object.
                @param objectBase The object to store
            */
            ObjectListBaseElement(Listable* object) : next_(0), prev_(0), objectBase_(object), list_(0) {}
            virtual ~ObjectListBaseElement() { this->removeFromList(); }

            virtual void changeContext(Context* context) = 0;

            ObjectListBaseElement* next_;       //!< The next element in the list
            ObjectListBaseElement* prev_;       //!< The previous element in the list
            Listable* objectBase_;              //!< The object
            ObjectListBase* list_;              //!< The list

        protected:
            void removeFromList();
    };


    // ###############################
    // ###    ObjectListElement    ###
    // ###############################
    /// The list-element that actually contains the object
    template <class T>
    class ObjectListElement : public ObjectListBaseElement
    {
        public:
            ObjectListElement(T* object) : ObjectListBaseElement(static_cast<Listable*>(object)), object_(object) {}

            virtual void changeContext(Context* context)
            {
                this->removeFromList();
                context->getObjectList<T>()->addElement(this);
            }

            T* object_;              //!< The object
    };


    // ########################################
    // ### ObjectListElementRemovalListener ###
    // ########################################
    /// Gets called by the object list if an element is removed
    class _CoreExport ObjectListElementRemovalListener
    {
        public:
            virtual ~ObjectListElementRemovalListener() {}
            virtual void removedElement(ObjectListBaseElement* element) = 0;
    };

    // ###############################
    // ###     ObjectListBase      ###
    // ###############################
    /**
        @brief The ObjectListBase contains all objects of a given class.

        The ObjectListBase is used by Identifiers to store all objects of their class.
        You can use Identifier::getObjects() to get the object-list from an Identifier.
        Use @ref Iterator "Iterator<T>" to iterate through them.

        Alternatively you can also use the static helper class @ref orxonox::ObjectList "ObjectList<T>"
        to get the list of all objects of type @a T. Use @ref ObjectListIterator "ObjectListIterator<T>"
        or @ref Iterator "Iterator<T>" to iterate through them.
    */
    class _CoreExport ObjectListBase
    {
        public:
            ObjectListBase();
            ~ObjectListBase();

            template <class T>
            inline ObjectListBaseElement* add(T* object)
            {
                ObjectListBaseElement* element = new ObjectListElement<T>(object);
                this->addElement(element);
                return element;
            }

            void addElement(ObjectListBaseElement* element);
            void removeElement(ObjectListBaseElement* element);

            size_t size() const { return this->size_; }

            /// Returns a pointer to the first element in the list. Works only with Iterator.
            inline ObjectListBaseElement* begin() const { return this->first_; }
            /// Returns a pointer to the element after the last element in the list. Works only with Iterator.
            inline ObjectListBaseElement* end() const { return 0; }
            /// Returns a pointer to the last element in the list. Works only with Iterator.
            inline ObjectListBaseElement* rbegin() const { return this->last_; }
            /// Returns a pointer to the element in front of the first element in the list. Works only with Iterator.
            inline ObjectListBaseElement* rend() const { return 0; }

            inline void registerRemovalListener(ObjectListElementRemovalListener* listener) { this->listeners_.push_back(listener); }
            inline void unregisterRemovalListener(ObjectListElementRemovalListener* listener)
            {
                for (unsigned int i = 0; i < this->listeners_.size(); ++i)
                {
                    if (listeners_[i] == listener)
                    {
                        listeners_.erase(listeners_.begin() + i);
                        break;
                    }
                }
            }

        private:
            void notifyRemovalListeners(ObjectListBaseElement* element) const;

            ObjectListBaseElement* first_;                              //!< The first element in the list
            ObjectListBaseElement* last_;                               //!< The last element in the list
            size_t size_;                                               //!< The number of elements in the list
            std::vector<ObjectListElementRemovalListener*> listeners_;  //!< A list of Iterators pointing on an element in this list
    };
}

#endif /* _ObjectListBase_H__ */
