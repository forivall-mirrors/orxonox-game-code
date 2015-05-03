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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
    @file CylinderCollisionShape.cc
    @brief Implementation of the CylinderCollisionShape class.
*/

#include "CylinderCollisionShape.h"

#include <BulletCollision/CollisionShapes/btCylinderShape.h>

#include "core/CoreIncludes.h"
#include "tools/BulletConversions.h"

namespace orxonox
{
    RegisterClass(CylinderCollisionShape);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    CylinderCollisionShape::CylinderCollisionShape(Context* context) : AbstractRadiusHeightCollisionShape(context)
    {
        RegisterObject(CylinderCollisionShape);

        updateShape();
    }

    /**
    @brief
        Creates a new internal collision shape for the CylinderCollisionShape.
    @return
        Returns a pointer to the newly created btCylinderShape.
    */
    btCollisionShape* CylinderCollisionShape::createNewShape() const
    {
        // divide height by 2 because bullet expects the half extents
        return new btCylinderShape(btVector3(this->getRadius(), this->getHeight() / 2, 0));
    }
}
