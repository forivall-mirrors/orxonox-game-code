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
    @brief Declaration of the MetaObjectList class.

    The MetaObjectList is a single-linked list, containing all list-elements and their
    lists wherein the object, owning the MetaObjectList, is registered.
    This allows much faster deletion of objects because no iteration is needed.
*/

#ifndef _MetaObjectList_H__
#define _MetaObjectList_H__

#include "CorePrereqs.h"

namespace orxonox
{
    // ###############################
    // ###  MetaObjectListElement  ###
    // ###############################
    /// The list-element of the MetaObjectList
    class _CoreExport MetaObjectListElement
    {
        public:
            /**
                @brief Constructor: Creates the list-element with given list and element.
            */
            MetaObjectListElement(ObjectListBase* list, ObjectListBaseElement* element) : next_(0), element_(element), list_(list) {}
            ~MetaObjectListElement();

            MetaObjectListElement* next_;       //!< The next Element in the list
            ObjectListBaseElement* element_;    //!< The list element, containing the object
            ObjectListBase* list_;              //!< The list, containing the element
    };


    // ###############################
    // ###     MetaObjectList      ###
    // ###############################
    /**
        @brief The MetaObjectList contains ObjectListBaseElements and their ObjectListBases.

        The MetaObjectList is a single-linked list, containing all list-elements and their
        lists wherein the object that owns the MetaObjectList is registered.
        This allows much faster deletion of objects because no iteration is needed.
    */
    class _CoreExport MetaObjectList
    {
        public:
            MetaObjectList();
            ~MetaObjectList();
            void add(ObjectListBase* list, ObjectListBaseElement* element);

            MetaObjectListElement* first_;      //!< The first element in the list
    };
}

#endif /* _MetaObjectList_H__ */
