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

        this->radius_ = 1.0f;
        updateSphere();

        this->registerVariables();
    }

    SphereCollisionShape::~SphereCollisionShape()
    {
        if (this->isInitialized())
            delete this->collisionShape_;
    }

    void SphereCollisionShape::registerVariables()
    {
        registerVariable(this->radius_, variableDirection::toclient, new NetworkCallback<SphereCollisionShape>(this, &SphereCollisionShape::updateSphere));
    }

    void SphereCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SphereCollisionShape, XMLPort, xmlelement, mode);

        XMLPortParam(SphereCollisionShape, "radius", setRadius, getRadius, xmlelement, mode);
    }

    void SphereCollisionShape::updateSphere()
    {
        if (this->collisionShape_)
            delete this->collisionShape_;
        // When we recreate the shape, we have to inform the parent about this to update the shape
        this->collisionShape_ = new btSphereShape(this->radius_);
        this->updateParent();
    }
}
