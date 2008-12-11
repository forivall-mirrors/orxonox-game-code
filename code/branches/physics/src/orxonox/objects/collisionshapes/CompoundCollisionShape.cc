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

        this->compoundShape_  = new btCompoundShape();
    }

    CompoundCollisionShape::~CompoundCollisionShape()
    {
        if (this->isInitialized())
            delete this->compoundShape_;
    }

    void CompoundCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CompoundCollisionShape, XMLPort, xmlelement, mode);
        // Attached collision shapes
        XMLPortObject(CompoundCollisionShape, CollisionShape, "", addChildShape, getChildShape, xmlelement, mode);
    }

    btCollisionShape* CompoundCollisionShape::getCollisionShape() const
    {
        // Note: Returning collisionShape_ means that it's the only one and has no transform.
        //       So we can get rid of the additional overhead with the compound shape.
        if (this->collisionShape_)
            return this->collisionShape_;
        else if (!this->empty())
            return this->compoundShape_;
        else
            return 0;
    }

    void CompoundCollisionShape::addChildShape(CollisionShape* shape, bool bWorldEntityRoot)
    {
        if (!shape)
            return;
        this->childShapes_.push_back(shape);

        if (shape->getCollisionShape())
        {
            // Only actually attach if we didn't pick a CompoundCollisionShape with no content
            btTransform transf(omni_cast<btQuaternion>(shape->getOrientation()), omni_cast<btVector3>(shape->getPosition()));
            this->compoundShape_->addChildShape(transf, shape->getCollisionShape());

            if (this->childShapes_.size() == 1 && !this->childShapes_[0]->hasTransform())
            {
                // --> Only shape to be added, no transform; add it directly
                this->collisionShape_ = shape->getCollisionShape();
            }
            else
            {
                // Make sure we use the compound shape when returning the btCollisionShape
                this->collisionShape_ = 0;
            }
        }

        // network synchro
        if (!bWorldEntityRoot)
            shape->setParent(this, this->getObjectID());
    }

    CollisionShape* CompoundCollisionShape::getChildShape(unsigned int index) const
    {
        if (index < this->childShapes_.size())
            return this->childShapes_[index];
        else
            return 0;
    }
}
