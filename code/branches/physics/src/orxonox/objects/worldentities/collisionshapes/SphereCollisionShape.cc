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

#include "OrxonoxStableHeaders.h"
#include "SphereCollisionShape.h"

#include "BulletCollision/CollisionShapes/btSphereShape.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"

namespace orxonox
{
    CreateFactory(SphereCollisionShape);

    SphereCollisionShape::SphereCollisionShape(BaseObject* creator) : CollisionShape(creator)
    {
        RegisterObject(SphereCollisionShape);

        this->sphereShape_ = new btSphereShape(1.0f);
        this->collisionShape_ = this->sphereShape_;
    }

    SphereCollisionShape::~SphereCollisionShape()
    {
        if (this->isInitialized())
            delete this->collisionShape_;
    }

    void SphereCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SphereCollisionShape, XMLPort, xmlelement, mode);

        XMLPortParam(SphereCollisionShape, "radius", setRadius, getRadius, xmlelement, mode);
    }

    btVector3 SphereCollisionShape::getTotalScaling()
    {
        return omni_cast<btVector3>(this->node_->getScale()) * this->radius_;
    }
}
