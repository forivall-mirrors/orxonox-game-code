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
#include "ConeCollisionShape.h"

#include "BulletCollision/CollisionShapes/btConeShape.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"

namespace orxonox
{
    CreateFactory(ConeCollisionShape);

    ConeCollisionShape::ConeCollisionShape(BaseObject* creator) : CollisionShape(creator)
    {
        RegisterObject(ConeCollisionShape);

        this->radius_ = 1.0f;
        this->height_ = 1.0f;
        updateCone();

        this->registerVariables();
    }

    ConeCollisionShape::~ConeCollisionShape()
    {
        if (this->isInitialized())
            delete this->collisionShape_;
    }

    void ConeCollisionShape::registerVariables()
    {
        registerVariable(this->radius_, variableDirection::toclient, new NetworkCallback<ConeCollisionShape>(this, &ConeCollisionShape::updateCone));
        registerVariable(this->height_, variableDirection::toclient, new NetworkCallback<ConeCollisionShape>(this, &ConeCollisionShape::updateCone));
    }

    void ConeCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ConeCollisionShape, XMLPort, xmlelement, mode);

        XMLPortParam(ConeCollisionShape, "radius", setRadius, getRadius, xmlelement, mode);
        XMLPortParam(ConeCollisionShape, "height", setHeight, getHeight, xmlelement, mode);    
    }

    void ConeCollisionShape::updateCone()
    {
        if (this->collisionShape_)
            delete this->collisionShape_;
        this->collisionShape_ = new btConeShape(this->radius_, this->height_);
        this->updateParent();
    }
}
