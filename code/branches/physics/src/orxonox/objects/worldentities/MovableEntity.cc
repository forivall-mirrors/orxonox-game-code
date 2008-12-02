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
 *      Martin Stypinski
 *
 */

#include "OrxonoxStableHeaders.h"
#include "MovableEntity.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "util/Debug.h"
#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "objects/Scene.h"

namespace orxonox
{
    MovableEntity::MovableEntity(BaseObject* creator) : WorldEntity(creator)
    {
        RegisterObject(MovableEntity);

        this->velocity_ = Vector3::ZERO;

        this->registerVariables();
    }

    MovableEntity::~MovableEntity()
    {
    }

    void MovableEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(MovableEntity, XMLPort, xmlelement, mode);

        XMLPortParamTemplate(MovableEntity, "velocity", setVelocity, getVelocity, xmlelement, mode, const Vector3&);
    }

    void MovableEntity::registerVariables()
    {
    }

    void MovableEntity::setPosition(const Vector3& position)
    {
        if (this->isDynamic())
        {
            btTransform transf = this->physicalBody_->getWorldTransform();
            transf.setOrigin(btVector3(position.x, position.y, position.z));
            this->physicalBody_->setWorldTransform(transf);
        }

        this->node_->setPosition(position);
        positionChanged();
    }

    void MovableEntity::translate(const Vector3& distance, Ogre::Node::TransformSpace relativeTo)
    {
        if (this->isDynamic())
        {
            OrxAssert(relativeTo == Ogre::Node::TS_LOCAL, "Cannot translate physical object relative \
                                                          to any other space than TS_LOCAL.");
            this->physicalBody_->translate(btVector3(distance.x, distance.y, distance.z));
        }

        this->node_->translate(distance, relativeTo);
        positionChanged();
    }

    void MovableEntity::setOrientation(const Quaternion& orientation)
    {
        if (this->isDynamic())
        {
            btTransform transf = this->physicalBody_->getWorldTransform();
            transf.setRotation(btQuaternion(orientation.w, orientation.x, orientation.y, orientation.z));
            this->physicalBody_->setWorldTransform(transf);
        }

        this->node_->setOrientation(orientation);
        orientationChanged();
    }

    void MovableEntity::rotate(const Quaternion& rotation, Ogre::Node::TransformSpace relativeTo)
    {
        if (this->isDynamic())
        {
            OrxAssert(relativeTo == Ogre::Node::TS_LOCAL, "Cannot rotate physical object relative \
                                                          to any other space than TS_LOCAL.");
            btTransform transf = this->physicalBody_->getWorldTransform();
            this->physicalBody_->setWorldTransform(transf * btTransform(btQuaternion(rotation.w, rotation.x, rotation.y, rotation.z)));
        }

        this->node_->rotate(rotation, relativeTo);
        orientationChanged();
    }

    void MovableEntity::yaw(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        if (this->isDynamic())
        {
            OrxAssert(relativeTo == Ogre::Node::TS_LOCAL, "Cannot yaw physical object relative \
                                                          to any other space than TS_LOCAL.");
            btTransform transf = this->physicalBody_->getWorldTransform();
            btTransform rotation(btQuaternion(angle.valueRadians(), 0.0f, 0.0f));
            this->physicalBody_->setWorldTransform(transf * rotation);
        }

        this->node_->yaw(angle, relativeTo);
        orientationChanged();
    }

    void MovableEntity::pitch(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        if (this->isDynamic())
        {
            OrxAssert(relativeTo == Ogre::Node::TS_LOCAL, "Cannot pitch physical object relative \
                                                          to any other space than TS_LOCAL.");
            btTransform transf = this->physicalBody_->getWorldTransform();
            btTransform rotation(btQuaternion(0.0f, angle.valueRadians(), 0.0f));
            this->physicalBody_->setWorldTransform(transf * rotation);
        }

        this->node_->pitch(angle, relativeTo);
        orientationChanged();
    }

    void MovableEntity::roll(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
    {
        if (this->isDynamic())
        {
            OrxAssert(relativeTo == Ogre::Node::TS_LOCAL, "Cannot roll physical object relative \
                                                          to any other space than TS_LOCAL.");
            btTransform transf = this->physicalBody_->getWorldTransform();
            btTransform rotation(btQuaternion(angle.valueRadians(), 0.0f, 0.0f));
            this->physicalBody_->setWorldTransform(transf * rotation);
        }

        this->node_->roll(angle, relativeTo);
        orientationChanged();
    }

    void MovableEntity::lookAt(const Vector3& target, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        if (this->isDynamic())
        {
            ThrowException(NotImplemented, "ControllableEntity::lookAt() is not yet supported for physical objects.");
            OrxAssert(relativeTo == Ogre::Node::TS_LOCAL, "Cannot align physical object relative \
                                                          to any other space than TS_LOCAL.");
            //btTransform transf = this->physicalBody_->getWorldTransform();
            //this->physicalBody_->setWorldTransform(transf);
        }

        this->node_->lookAt(target, relativeTo, localDirectionVector);
        orientationChanged();
    }

    void MovableEntity::setDirection(const Vector3& direction, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        if (this->isDynamic())
        {
            ThrowException(NotImplemented, "ControllableEntity::setDirection() is not yet supported for physical objects.");
            OrxAssert(relativeTo == Ogre::Node::TS_LOCAL, "Cannot align physical object relative \
                                                          to any other space than TS_LOCAL.");
            //btTransform transf = this->physicalBody_->getWorldTransform();
            //this->physicalBody_->setWorldTransform(transf);
        }

        this->node_->setDirection(direction, relativeTo, localDirectionVector);
        orientationChanged();
    }

    void MovableEntity::setVelocity(const Vector3& velocity)
    {
        if (this->isDynamic())
        {
            this->physicalBody_->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
        }

        this->velocity_ = velocity;
        velocityChanged();
    }

    bool MovableEntity::isCollisionTypeLegal(WorldEntity::CollisionType type) const
    {
        if (type == WorldEntity::Static)
        {
            ThrowException(PhysicsViolation, "Cannot tell a MovableEntity to have static collision type");
            return false;
        }
        else
            return true;
    }

    void MovableEntity::setWorldTransform(const btTransform& worldTrans)
    {
        // We use a dynamic body. So we translate our node accordingly.
        this->node_->setPosition(Vector3(worldTrans.getOrigin().x(), worldTrans.getOrigin().y(), worldTrans.getOrigin().z()));
        this->node_->setOrientation(Quaternion(worldTrans.getRotation().w(), worldTrans.getRotation().x(), worldTrans.getRotation().y(), worldTrans.getRotation().z()));
        const btVector3& velocity = this->physicalBody_->getLinearVelocity();
        this->velocity_.x = velocity.x();
        this->velocity_.y = velocity.y();
        this->velocity_.z = velocity.z();
        velocityChanged();
        positionChanged();
        orientationChanged();
    }

    void MovableEntity::getWorldTransform(btTransform& worldTrans) const
    {
        // We use a kinematic body 
        worldTrans.setOrigin(btVector3(node_->getPosition().x, node_->getPosition().y, node_->getPosition().z));
        worldTrans.setRotation(btQuaternion(node_->getOrientation().w, node_->getOrientation().x, node_->getOrientation().y, node_->getOrientation().z));
        if (this->isDynamic())
        {
            // This function gets called only once for dynamic objects to set the initial conditions
            // We have to set the velocity too.
            this->physicalBody_->setLinearVelocity(btVector3(velocity_.x, velocity_.y, velocity_.z));
        }
    }
}
