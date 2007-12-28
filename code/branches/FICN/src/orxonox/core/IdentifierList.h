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
    @file IdentifierList.h
    @brief Definition of the IdentifierList class

    The IdentifierList is a single-linked list, containing Identifiers.
    The IdentifierList is used to store parents and childs of each Identifier.
*/

#ifndef _IdentifierList_H__
#define _IdentifierList_H__

#include <string>

#include "CorePrereqs.h"

namespace orxonox
{
    //! The list-element of the IdentifierList
    class IdentifierListElement
    {
        public:
            IdentifierListElement(const Identifier* identifier);

            const Identifier* identifier_;      //!< The identifier
            IdentifierListElement* next_;       //!< The next element in the list
    };

    //! The IdentifierList contains Identifiers
    class _CoreExport IdentifierList
    {
        public:
            IdentifierList();
            ~IdentifierList();
            void add(const Identifier* identifier);
            void remove(const Identifier* identifier);
            bool isInList(const Identifier* identifier) const;
            std::string toString() const;

            IdentifierListElement* first_;      //!< The first element in the list
    };
}

#endif /* _IdentifierList_H__ */
