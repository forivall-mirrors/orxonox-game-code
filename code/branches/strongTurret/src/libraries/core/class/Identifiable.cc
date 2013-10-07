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
    @brief Implementation of Identifiable.
*/

#include "Identifiable.h"

#include <cassert>
#include "core/CoreIncludes.h"
#include "core/object/Context.h"
#include "Identifier.h"

namespace orxonox
{
    RegisterClassNoArgs(Identifiable);

    /**
        @brief Constructor: Sets the default values.
    */
    Identifiable::Identifiable()
    {
        this->identifier_ = 0;
        this->objectPointers_.reserve(6); // Optimisation

        RegisterObject(Identifiable);
    }

    /// Returns true if the object's class is of the given type or a derivative.
    bool Identifiable::isA(const Identifier* identifier)
        { return this->getIdentifier()->isA(identifier); }
    /// Returns true if the object's class is exactly of the given type.
    bool Identifiable::isExactlyA(const Identifier* identifier)
        { return this->getIdentifier()->isExactlyA(identifier); }
    /// Returns true if the object's class is a child of the given type.
    bool Identifiable::isChildOf(const Identifier* identifier)
        { return this->getIdentifier()->isChildOf(identifier); }
    /// Returns true if the object's class is a direct child of the given type.
    bool Identifiable::isDirectChildOf(const Identifier* identifier)
        { return this->getIdentifier()->isDirectChildOf(identifier); }
    /// Returns true if the object's class is a parent of the given type.
    bool Identifiable::isParentOf(const Identifier* identifier)
        { return this->getIdentifier()->isParentOf(identifier); }
    /// Returns true if the object's class is a direct parent of the given type.
    bool Identifiable::isDirectParentOf(const Identifier* identifier)
        { return this->getIdentifier()->isDirectParentOf(identifier); }


    /// Returns true if the object's class is of the given type or a derivative.
    bool Identifiable::isA(const Identifiable* object)
        { return this->getIdentifier()->isA(object->getIdentifier()); }
    /// Returns true if the object's class is exactly of the given type.
    bool Identifiable::isExactlyA(const Identifiable* object)
        { return this->getIdentifier()->isExactlyA(object->getIdentifier()); }
    /// Returns true if the object's class is a child of the given type.
    bool Identifiable::isChildOf(const Identifiable* object)
        { return this->getIdentifier()->isChildOf(object->getIdentifier()); }
    /// Returns true if the object's class is a direct child of the given type.
    bool Identifiable::isDirectChildOf(const Identifiable* object)
        { return this->getIdentifier()->isDirectChildOf(object->getIdentifier()); }
    /// Returns true if the object's class is a parent of the given type.
    bool Identifiable::isParentOf(const Identifiable* object)
        { return this->getIdentifier()->isParentOf(object->getIdentifier()); }
    /// Returns true if the object's class is a direct child of the given type.
    bool Identifiable::isDirectParentOf(const Identifiable* object)
        { return this->getIdentifier()->isDirectParentOf(object->getIdentifier()); }
}
