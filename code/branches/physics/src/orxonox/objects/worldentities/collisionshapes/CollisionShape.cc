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

#include "CompoundCollisionShape.h"

namespace orxonox
{
    CreateFactory(CollisionShape);

    CollisionShape::CollisionShape(BaseObject* creator)
        : BaseObject(creator)
        , network::Synchronisable(creator)
    {
        RegisterObject(CollisionShape);

        this->parent_ = 0;
        this->parentID_ = (unsigned int)-1;
        this->collisionShape_ = 0;
        this->position_ = Vector3::ZERO;
        this->orientation_ = Quaternion::IDENTITY;
        this->scale_ = Vector3::UNIT_SCALE;
    }

    CollisionShape::~CollisionShape()
    {
    }

    void CollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CollisionShape, XMLPort, xmlelement, mode);

        XMLPortParamTemplate(CollisionShape, "position", setPosition, getPosition, xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(CollisionShape, "orientation", setOrientation, getOrientation, xmlelement, mode, const Quaternion&);
        XMLPortParamLoadOnly(CollisionShape, "yaw",   yaw,   xmlelement, mode);
        XMLPortParamLoadOnly(CollisionShape, "pitch", pitch, xmlelement, mode);
        XMLPortParamLoadOnly(CollisionShape, "roll",  roll,  xmlelement, mode);
        XMLPortParamTemplate(CollisionShape, "scale3D", setScale3D, getScale3D, xmlelement, mode, const Vector3&);
        XMLPortParamLoadOnly(CollisionShape, "scale", setScale, xmlelement, mode);
    }

    void CollisionShape::registerVariables()
    {
        REGISTERDATA(this->parentID_, network::direction::toclient, new network::NetworkCallback<CollisionShape>(this, &CollisionShape::updateParent));
    }

    void CollisionShape::updateParent()
    {
        CompoundCollisionShape* parent = dynamic_cast<CompoundCollisionShape*>(Synchronisable::getSynchronisable(this->parentID_));
        if (parent)
            parent->addChildShape(this);
    }

    bool CollisionShape::hasTransform() const
    {
        return (!this->position_.positionEquals(Vector3(0, 0, 0), 0.001) ||
                !this->orientation_.equals(Quaternion(1,0,0,0), Degree(0.1)));
    }

    void CollisionShape::setScale3D(const Vector3& scale)
    {
        ThrowException(NotImplemented, "Cannot set the scale of a collision shape: Not yet implemented.");
    }

    void CollisionShape::setScale(float scale)
    {
        ThrowException(NotImplemented, "Cannot set the scale of a collision shape: Not yet implemented.");
    }
}
