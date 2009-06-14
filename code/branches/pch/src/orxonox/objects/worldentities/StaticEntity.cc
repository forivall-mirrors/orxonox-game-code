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
 *      Fabian 'x3n' Landau
 *      Reto Grieder (physics)
 *   Co-authors:
 *      ...
 *
 */

#include "StaticEntity.h"

#include <OgreSceneNode.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "util/OrxAssert.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    CreateFactory(StaticEntity);

    StaticEntity::StaticEntity(BaseObject* creator) : WorldEntity(creator)
    {
        RegisterObject(StaticEntity);
        
        this->setPriority(priority::very_low);

        this->registerVariables();
    }

    StaticEntity::~StaticEntity()
    {
    }

    void StaticEntity::registerVariables()
    {
        registerVariable(this->getPosition(),    variableDirection::toclient, new NetworkCallback<StaticEntity>(this, &StaticEntity::positionChanged));
        registerVariable(this->getOrientation(), variableDirection::toclient, new NetworkCallback<StaticEntity>(this, &StaticEntity::orientationChanged));
    }


    void StaticEntity::setPosition(const Vector3& position)
    {
        if (this->addedToPhysicalWorld())
        {
            CCOUT(2) << "Warning: Attempting to change the position of a StaticEntity at physics run time. Ignoring change." << std::endl;
            return;
        }
        if (this->isStatic())
        {
            btTransform transf = this->physicalBody_->getWorldTransform();
            transf.setOrigin(btVector3(position.x, position.y, position.z));
            this->physicalBody_->setWorldTransform(transf);
        }

        this->node_->setPosition(position);
    }

    void StaticEntity::setOrientation(const Quaternion& orientation)
    {
        if (this->addedToPhysicalWorld())
        {
            CCOUT(2) << "Warning: Attempting to change the orientation of a StaticEntity at physics run time. Ignoring change." << std::endl;
            return;
        }
        if (this->isStatic())
        {
            btTransform transf = this->physicalBody_->getWorldTransform();
            transf.setRotation(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
            this->physicalBody_->setWorldTransform(transf);
        }

        this->node_->setOrientation(orientation);
    }

    bool StaticEntity::isCollisionTypeLegal(WorldEntity::CollisionType type) const
    {
        if (type == WorldEntity::Kinematic || type == WorldEntity::Dynamic)
        {
            CCOUT(1) << "Error: Cannot tell a StaticEntity to have kinematic or dynamic collision type! Ignoring." << std::endl;
            assert(false); // Only in debug mode
            return false;
        }
        else
            return true;
    }

    void StaticEntity::setWorldTransform(const btTransform& worldTrans)
    {
        OrxAssert(false, "Setting world transform of a StaticEntity, which is CF_STATIC!");
    }

    void StaticEntity::getWorldTransform(btTransform& worldTrans) const
    {
        worldTrans.setOrigin(btVector3(node_->getPosition().x, node_->getPosition().y, node_->getPosition().z));
        worldTrans.setRotation(btQuaternion(node_->getOrientation().x, node_->getOrientation().y, node_->getOrientation().z, node_->getOrientation().w));
    }
}
