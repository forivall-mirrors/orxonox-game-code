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
 *      Martin Stypinski
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "PlaneCollisionShape.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"

namespace orxonox
{
    CreateFactory(PlaneCollisionShape);

    PlaneCollisionShape::PlaneCollisionShape(BaseObject* creator) : CollisionShape(creator)
    {
        RegisterObject(PlaneCollisionShape);

        this->planeShape_ = new btStaticPlaneShape(btVector3(1, 1, 1), 0);
        this->collisionShape_ = this->planeShape_;
        this->planeNormal_ = Vector3(1, 1, 1);
        this->planeOffset_ = 0.0f;
    }

    PlaneCollisionShape::~PlaneCollisionShape()
    {
        if (this->isInitialized())
            delete this->planeShape_;
    }

    void PlaneCollisionShape::registerVariables()
    {
        REGISTERDATA(this->planeNormal_, network::direction::toclient, new network::NetworkCallback<PlaneCollisionShape>(this, &PlaneCollisionShape::planeNormalChanged));
        REGISTERDATA(this->planeOffset_, network::direction::toclient, new network::NetworkCallback<PlaneCollisionShape>(this, &PlaneCollisionShape::planeOffsetChanged));
    }

    void PlaneCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PlaneCollisionShape, XMLPort, xmlelement, mode);

        XMLPortParam(PlaneCollisionShape, "planeNormal", setNormal, getNormal, xmlelement, mode);
        XMLPortParam(PlaneCollisionShape, "planeOffset", setOffset, getOffset, xmlelement, mode);    
    }

    void PlaneCollisionShape::setNormal(const Vector3& normal)
    {
        delete this->planeShape_;
        this->planeNormal_ = normal;
        this->planeShape_ = new btStaticPlaneShape(omni_cast<btVector3>(normal), this->planeOffset_);
        this->collisionShape_ = this->planeShape_;
    }

    void PlaneCollisionShape::setOffset(float offset)
    {
        delete this->planeShape_;
        this->planeOffset_ = offset;
        this->planeShape_ = new btStaticPlaneShape(omni_cast<btVector3>(this->planeNormal_), offset);
        this->collisionShape_ = this->planeShape_;
    }
}
