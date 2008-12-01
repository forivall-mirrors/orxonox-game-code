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
#include "CompoundCollisionShape.h"

#include "BulletCollision/CollisionShapes/btCompoundShape.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"

namespace orxonox
{
    CreateFactory(CompoundCollisionShape);

    CompoundCollisionShape::CompoundCollisionShape(BaseObject* creator) : CollisionShape(creator)
    {
        RegisterObject(CompoundCollisionShape);

        this->bIsCompound_    = true;
        this->compoundShape_  = new btCompoundShape();
        this->collisionShape_ = this->compoundShape_;
    }

    CompoundCollisionShape::~CompoundCollisionShape()
    {
        if (this->isInitialized())
            delete this->compoundShape_;
    }

    void CompoundCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CompoundCollisionShape, XMLPort, xmlelement, mode);

        XMLPortObject(CompoundCollisionShape, CollisionShape, "", addChildShape, getChildShape, xmlelement, mode);
    }

    void CompoundCollisionShape::addChildShape(CollisionShape* shape)
    {
        this->childShapes_.push_back(shape);
        btTransform transf(omni_cast<btQuaternion>(shape->getOrientation()), omni_cast<btVector3>(shape->getPosition()));
        shape->getCollisionShape()->setLocalScaling(shape->getTotalScaling());
        this->compoundShape_->addChildShape(transf, shape->getCollisionShape());
    }

    CollisionShape* CompoundCollisionShape::getChildShape(unsigned int index) const
    {
        if (index < this->childShapes_.size())
            return this->childShapes_[index];
        else
            return 0;
    }
}
