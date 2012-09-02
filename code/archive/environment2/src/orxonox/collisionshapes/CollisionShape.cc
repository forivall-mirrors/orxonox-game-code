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

#include "CollisionShape.h"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/WorldEntity.h"
#include "CompoundCollisionShape.h"
#include "WorldEntityCollisionShape.h"

namespace orxonox
{
    CollisionShape::CollisionShape(BaseObject* creator)
        : BaseObject(creator)
        , Synchronisable(creator)
    {
        RegisterObject(CollisionShape);

        this->parent_ = 0;
        this->parentID_ = OBJECTID_UNKNOWN;
        this->collisionShape_ = 0;
        this->position_ = Vector3::ZERO;
        this->orientation_ = Quaternion::IDENTITY;
        this->scale_ = Vector3::UNIT_SCALE;

        this->registerVariables();
    }

    CollisionShape::~CollisionShape()
    {
        // Detach from parent
        if (this->isInitialized() && this->parent_)
            this->parent_->detach(this);
    }

    void CollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CollisionShape, XMLPort, xmlelement, mode);

        XMLPortParamTemplate(CollisionShape, "position", setPosition, getPosition, xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(CollisionShape, "orientation", setOrientation, getOrientation, xmlelement, mode, const Quaternion&);
        XMLPortParamTemplate(CollisionShape, "scale3D", setScale3D, getScale3D, xmlelement, mode, const Vector3&);
        XMLPortParamLoadOnly(CollisionShape, "scale", setScale, xmlelement, mode);
        XMLPortParamLoadOnly(CollisionShape, "yaw",   yaw,   xmlelement, mode);
        XMLPortParamLoadOnly(CollisionShape, "pitch", pitch, xmlelement, mode);
        XMLPortParamLoadOnly(CollisionShape, "roll",  roll,  xmlelement, mode);
    }

    void CollisionShape::registerVariables()
    {
        registerVariable(this->parentID_, VariableDirection::ToClient, new NetworkCallback<CollisionShape>(this, &CollisionShape::parentChanged));
    }

    void CollisionShape::parentChanged()
    {
        Synchronisable* parent = Synchronisable::getSynchronisable(this->parentID_);
        // Parent can either be a WorldEntity or a CompoundCollisionShape. The reason is that the
        // internal collision shape (which is compound) of a WE doesn't get synchronised.
        CompoundCollisionShape* parentCCS = orxonox_cast<CompoundCollisionShape*>(parent);
        if (parentCCS)
            parentCCS->attach(this);
        else
        {
            WorldEntity* parentWE = orxonox_cast<WorldEntity*>(parent);
            if (parentWE)
                parentWE->attachCollisionShape(this);
        }
    }

    bool CollisionShape::notifyBeingAttached(CompoundCollisionShape* newParent)
    {
        if (this->parent_)
            this->parent_->detach(this);

        this->parent_ = newParent;

        WorldEntityCollisionShape* parentWECCS = orxonox_cast<WorldEntityCollisionShape*>(newParent);
        if (parentWECCS)
            this->parentID_ = parentWECCS->getWorldEntityOwner()->getObjectID();
        else
            this->parentID_ = newParent->getObjectID();

        return true;
    }

    void CollisionShape::notifyDetached()
    {
        this->parent_ = 0;
        this->parentID_ = OBJECTID_UNKNOWN;
    }

    void CollisionShape::updateParent()
    {
        if (this->parent_)
            this->parent_->updateAttachedShape(this);
    }

    bool CollisionShape::hasTransform() const
    {
        return (!this->position_.positionEquals(Vector3(0, 0, 0), 0.001f) ||
                !this->orientation_.equals(Quaternion(1,0,0,0), Degree(0.1f)));
    }

    void CollisionShape::setScale3D(const Vector3& scale)
    {
        CCOUT(2) << "Warning: Cannot set the scale of a collision shape: Not yet implemented." << std::endl;
    }

    void CollisionShape::setScale(float scale)
    {
        CCOUT(2) << "Warning: Cannot set the scale of a collision shape: Not yet implemented." << std::endl;
    }

    void CollisionShape::updateShape()
    {
        btCollisionShape* oldShape = this->collisionShape_;
        this->collisionShape_ = this->createNewShape();
        // When we recreate the shape, we have to inform the parent about this to update the shape
        this->updateParent();
        if (oldShape)
            delete oldShape;
    }

    void CollisionShape::calculateLocalInertia(float mass, btVector3& inertia) const
    {
        if (this->collisionShape_)
            this->collisionShape_->calculateLocalInertia(mass, inertia);
        else
            inertia.setValue(0, 0, 0);
    }
}
