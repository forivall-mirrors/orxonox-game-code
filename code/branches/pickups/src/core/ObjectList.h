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
    @brief Definition and implementation of the ObjectList class.

    The ObjectList is a wrapper of an ObjectListBase of a given class.
    Use Iterator<class> to iterate through all objects of the class.
*/

#ifndef _ObjectList_H__
#define _ObjectList_H__

#include "CorePrereqs.h"

#include "Identifier.h"
#include "ObjectListIterator.h"

namespace orxonox
{
    // ###############################
    // ###       ObjectList        ###
    // ###############################
    //! The ObjectList contains all objects of the given class.
    /**
        Wraps the ObjectListBase of the corresponding Identifier.
        Use ObjectListIterator<class> to iterate through all objects in the list.
    */
    template <class T>
    class ObjectList
    {
        public:
            typedef ObjectListIterator<T> iterator;

            /** @brief Returns an Iterator to the first element in the list. @return The Iterator */
            inline static ObjectListElement<T>* begin()
                { return ((ObjectListElement<T>*)ClassIdentifier<T>::getIdentifier()->getObjects()->begin().element_); }

            /** @brief Returns an Iterator to the element after the last element in the list. @return The Iterator */
            inline static ObjectListElement<T>* end()
                { return ((ObjectListElement<T>*)ClassIdentifier<T>::getIdentifier()->getObjects()->end().element_); }

            /** @brief Returns an Iterator to the last element in the list. @return The Iterator */
            inline static ObjectListElement<T>* rbegin()
                { return ((ObjectListElement<T>*)ClassIdentifier<T>::getIdentifier()->getObjects()->rbegin().element_); }

            /** @brief Returns an Iterator to the element before the first element in the list. @return The Iterator */
            inline static ObjectListElement<T>* rend()
                { return ((ObjectListElement<T>*)ClassIdentifier<T>::getIdentifier()->getObjects()->rend().element_); }
    };
}

#endif /* _ObjectList_H__ */
