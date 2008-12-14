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

        this->normal_ = Vector3(0, 1, 0);
        this->offset_ = 0.0f;
        updatePlane();

        this->registerVariables();
    }

    PlaneCollisionShape::~PlaneCollisionShape()
    {
        if (this->isInitialized())
            delete this->collisionShape_;
    }

    void PlaneCollisionShape::registerVariables()
    {
        registerVariable(this->normal_, variableDirection::toclient, new NetworkCallback<PlaneCollisionShape>(this, &PlaneCollisionShape::updatePlane));
        registerVariable(this->offset_, variableDirection::toclient, new NetworkCallback<PlaneCollisionShape>(this, &PlaneCollisionShape::updatePlane));
    }

    void PlaneCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(PlaneCollisionShape, XMLPort, xmlelement, mode);

        XMLPortParam(PlaneCollisionShape, "normal", setNormal, getNormal, xmlelement, mode);
        XMLPortParam(PlaneCollisionShape, "offset", setOffset, getOffset, xmlelement, mode);    
    }

    void PlaneCollisionShape::updatePlane()
    {
        if (this->collisionShape_)
            delete this->collisionShape_;
        this->collisionShape_ = new btStaticPlaneShape(omni_cast<btVector3>(this->normal_), this->offset_);
        this->updateParent();
    }
}
