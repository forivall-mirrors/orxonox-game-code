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
    @brief Implementation of Listable.
*/

#include "Listable.h"

#include "core/object/MetaObjectList.h"

namespace orxonox
{
    /**
        @brief Constructor: creates the meta-object-list.
    */
    Listable::Listable()
    {
        this->metaList_ = new MetaObjectList();
    }

    /**
        @brief Destructor: Removes the object from the object-lists
    */
    Listable::~Listable()
    {
        this->unregisterObject();
    }

    /**
        @brief Removes this object from the object-lists.
    */
    void Listable::unregisterObject()
    {
        if (this->metaList_)
            delete this->metaList_;
        this->metaList_ = 0;
    }
}
