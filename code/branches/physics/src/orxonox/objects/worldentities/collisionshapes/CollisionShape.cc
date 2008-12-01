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

#include "OrxonoxStableHeaders.h"
#include "CollisionShape.h"

#include "BulletCollision/CollisionShapes/btCollisionShape.h"

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"

namespace orxonox
{
    CreateFactory(CollisionShape);

    CollisionShape::CollisionShape(BaseObject* creator) : StaticEntity(creator)
    {
        RegisterObject(CollisionShape);

        this->bIsCompound_ = false;
    }

    CollisionShape::~CollisionShape()
    {
    }

    void CollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CollisionShape, XMLPort, xmlelement, mode);
    }

    bool CollisionShape::isCollisionTypeLegal(WorldEntity::CollisionType type) const
    {
        if (type != WorldEntity::None)
        {
            ThrowException(PhysicsViolation, "A CollisionShape can only have CollisionType 'None'.");
            return false;
        }
        else
            return true;
    }

    bool CollisionShape::hasNoTransform() const
    {
        return (this->getPosition().positionEquals(Vector3(0, 0, 0), 0.001) &&
                this->getOrientation().equals(Quaternion(1,0,0,0), Degree(0.1)));
    }

    btVector3 CollisionShape::getTotalScaling()
    {
        return omni_cast<btVector3>(this->node_->getScale());
    }
}
