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
    @defgroup ObjectList Object-lists and iterators
    @ingroup Object
*/

/**
    @file
    @ingroup Object ObjectList
    @brief Definition of the ObjectList class, a wrapper of ObjectListBase.

    @ref orxonox::ObjectList "ObjectList<T>" is a wrapper of an @ref orxonox::ObjectListBase
    "ObjectListBase" of class @a T. Use @ref orxonox::ObjectListIterator "ObjectListIterator<T>"
    to iterate through the list.
*/

#ifndef _ObjectList_H__
#define _ObjectList_H__

#include "core/CorePrereqs.h"

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include "ObjectListBase.h"
#include "ObjectListIterator.h"
#include "Context.h"

namespace orxonox
{
    // ###############################
    // ###       ObjectList        ###
    // ###############################
    /**
        @brief The ObjectList contains all objects of the given class.

        Wraps the ObjectListBase which contains all objects of type @a T. Use @ref ObjectListIterator
        "ObjectListIterator<T>" or its typedef ObjectList<T>::iterator to iterate through all objects
        in the list.
    */
    template <class T>
    class ObjectList
    {
        BOOST_STATIC_ASSERT((boost::is_base_of<Listable, T>::value));

        public:
            typedef ObjectListIterator<T> iterator;

            /// Returns the size of the list
            inline static size_t size()
            {
                return Context::getRootContext()->getObjectList<T>()->size();
            }

            /// Returns an Iterator to the first element in the list.
            inline static ObjectListElement<T>* begin()
            {
                ObjectListBase* list = Context::getRootContext()->getObjectList<T>();
                return static_cast<ObjectListElement<T>*>(list->begin());
            }

            /// Returns an Iterator to the element after the last element in the list.
            inline static ObjectListElement<T>* end()
            {
                ObjectListBase* list = Context::getRootContext()->getObjectList<T>();
                return static_cast<ObjectListElement<T>*>(list->end());
            }

            /// Returns an Iterator to the last element in the list.
            inline static ObjectListElement<T>* rbegin()
            {
                ObjectListBase* list = Context::getRootContext()->getObjectList<T>();
                return static_cast<ObjectListElement<T>*>(list->rbegin());
            }

            /// Returns an Iterator to the element before the first element in the list.
            inline static ObjectListElement<T>* rend()
            {
                ObjectListBase* list = Context::getRootContext()->getObjectList<T>();
                return static_cast<ObjectListElement<T>*>(list->rend());
            }
    };
}

#endif /* _ObjectList_H__ */
