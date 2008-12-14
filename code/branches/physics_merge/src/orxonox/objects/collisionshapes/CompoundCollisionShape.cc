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

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "tools/BulletConversions.h"
#include "objects/worldentities/WorldEntity.h"

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
        {
            // Detatch all children first
            this->removeAllChildShapes();
            delete this->compoundShape_;
        }
    }

    void CompoundCollisionShape::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CompoundCollisionShape, XMLPort, xmlelement, mode);
        // Attached collision shapes
        XMLPortObject(CompoundCollisionShape, CollisionShape, "", addChildShape, getChildShape, xmlelement, mode);
    }

    void CompoundCollisionShape::addChildShape(CollisionShape* shape)
    {
        if (!shape || static_cast<CollisionShape*>(this) == shape)
            return;
        if (this->childShapes_.find(shape) != this->childShapes_.end())
        {
            CCOUT(2) << "Warning: Attaching a CollisionShape twice is not yet supported." << std::endl;
            return;
        }
        this->childShapes_[shape] = shape->getCollisionShape();

        if (shape->getCollisionShape())
        {
            // Only actually attach if we didn't pick a CompoundCollisionShape with no content
            btTransform transf(omni_cast<btQuaternion>(shape->getOrientation()), omni_cast<btVector3>(shape->getPosition()));
            this->compoundShape_->addChildShape(transf, shape->getCollisionShape());

            this->updatePublicShape();
        }

        // network synchro
        shape->setParent(this, this->getObjectID());
    }

    void CompoundCollisionShape::removeChildShape(CollisionShape* shape)
    {
        if (this->childShapes_.find(shape) != this->childShapes_.end())
        {
            shape->setParent(0, OBJECTID_UNKNOWN);
            this->childShapes_.erase(shape);
            if (shape->getCollisionShape())
                this->compoundShape_->removeChildShape(shape->getCollisionShape());

            this->updatePublicShape();
        }
    }

    void CompoundCollisionShape::removeAllChildShapes()
    {
        while (this->childShapes_.size() > 0)
            this->removeChildShape(this->childShapes_.begin()->first);
    }

    void CompoundCollisionShape::updateChildShape(CollisionShape* shape)
    {
        if (!shape)
            return;
        std::map<CollisionShape*, btCollisionShape*>::iterator it = this->childShapes_.find(shape);
        if (it == this->childShapes_.end())
        {
            CCOUT(2) << "Warning: Cannot update child shape: Instance not a child." << std::endl;
            return;
        }

        // Remove old btCollisionShape, stored in the children map
        if (it->second)
            this->compoundShape_->removeChildShape(it->second);
        if (shape->getCollisionShape())
        {
            // Only actually attach if we didn't pick a CompoundCollisionShape with no content
            btTransform transf(omni_cast<btQuaternion>(shape->getOrientation()), omni_cast<btVector3>(shape->getPosition()));
            this->compoundShape_->addChildShape(transf, shape->getCollisionShape());
            it->second = shape->getCollisionShape();
        }

        this->updatePublicShape();
    }

    void CompoundCollisionShape::updatePublicShape()
    {
        btCollisionShape* primitive = 0;
        bool bPrimitive = true;
        bool bEmpty = true;
        for (std::map<CollisionShape*, btCollisionShape*>::const_iterator it = this->childShapes_.begin(); it != this->childShapes_.end(); ++it)
        {
            if (it->second)
            {
                bEmpty = false;
                if (!it->first->hasTransform())
                    primitive = it->second;
                else
                    bPrimitive = false;
            }
        }
        if (bEmpty)
            this->collisionShape_ = 0;
        else if (bPrimitive)
        {
            // --> Only one shape to be added, no transform; return it directly
            this->collisionShape_ = primitive;
        }
        else
        {
            // Make sure we use the compound shape when returning a btCollisionShape
            this->collisionShape_ = this->compoundShape_;
        }
        this->updateParent();
    }

    void CompoundCollisionShape::updateParent()
    {
        if (this->parent_)
            this->parent_->updateChildShape(this);
        else
        {
            // We can do this, because the CompoundCollisionShape of a WorldEntity always belongs to it,
            // as long as its lifetime.
            WorldEntity* parent = dynamic_cast<WorldEntity*>(this->getCreator());
            if (parent)
                parent->notifyCollisionShapeChanged();
        }
    }

    CollisionShape* CompoundCollisionShape::getChildShape(unsigned int index) const
    {
        unsigned int i = 0;
        for (std::map<CollisionShape*, btCollisionShape*>::const_iterator it = this->childShapes_.begin(); it != this->childShapes_.end(); ++it)
        {
            if (i == index)
                return it->first;
            ++i;
        }
        return 0;
    }
}
