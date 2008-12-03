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
#include "PlaneCollisionShape.h"

#include "BulletCollision/CollisionShapes/btStaticPlaneShape.h"

#include "tools/BulletConversions.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"

namespace orxonox
{
    CreateFactory(PlaneCollisionShape);

    PlaneCollisionShape::PlaneCollisionShape(BaseObject* creator) : CollisionShape(creator)
    {
        RegisterObject(PlaneCollisionShape);

        this->planeShape_ = new btStaticPlaneShape(btVector3(1,1,1), 0);
        this->collisionShape_ = this->planeShape_;
    }

    PlaneCollisionShape::~PlaneCollisionShape()
    {
        if (this->isInitialized())
            delete this->collisionShape_;
    }

    void PlaneCollisionShape::setNormal(const Vector3& normal)
    {
        btScalar offset = this->planeShape_->getPlaneConstant();
        delete this->collisionShape_;
        this->planeShape_ = new btStaticPlaneShape(omni_cast<btVector3>(normal), offset);
        this->collisionShape_ = this->planeShape_;
    }

    void PlaneCollisionShape::setOffset(float offset)
    {
        btVector3 normal = this->planeShape_->getPlaneNormal();
        delete this->collisionShape_;
        this->planeShape_ = new btStaticPlaneShape(normal, offset);
        this->collisionShape_ = this ->planeShape_;
    }

    void PlaneCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PlaneCollisionShape, XMLPort, xmlelement, mode);

        XMLPortParam(PlaneCollisionShape, "normal", setNormal, getNormal, xmlelement, mode);
        XMLPortParam(PlaneCollisionShape, "offset", setOffset, getOffset, xmlelement, mode);    
    }

    btVector3 PlaneCollisionShape::getTotalScaling()
    {
        return omni_cast<btVector3>(this->node_->getScale()); //* this->radius_;
    }
}
