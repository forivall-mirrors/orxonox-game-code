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
#include "MobileEntity.h"

#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "util/Debug.h"
#include "util/MathConvert.h"
#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "objects/Scene.h"

namespace orxonox
{
    MobileEntity::MobileEntity(BaseObject* creator) : WorldEntity(creator)
    {
        RegisterObject(MobileEntity);

        this->linearAcceleration_  = Vector3::ZERO;
        this->linearVelocity_      = Vector3::ZERO;
        this->angularAcceleration_ = Vector3::ZERO;
        this->angularVelocity_     = Vector3::ZERO;

        this->registerVariables();
    }

    MobileEntity::~MobileEntity()
    {
    }

    void MobileEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(MobileEntity, XMLPort, xmlelement, mode);

        XMLPortParamTemplate(MobileEntity, "velocity",     setVelocity,     getVelocity,     xmlelement, mode, const Vector3&);
        XMLPortParamTemplate(MobileEntity, "rotationaxis", setRotationAxis, getRotationAxis, xmlelement, mode, const Vector3&);
        XMLPortParam(MobileEntity, "rotationrate", setRotationRate, getRotationRate, xmlelement, mode);
    }

    void MobileEntity::registerVariables()
    {
    }

    void MobileEntity::tick(float dt)
    {
        if (this->isActive())
        {
            // Check whether Bullet doesn't do the physics for us
            if (!this->isDynamic())
            {
                // Linear part
                this->linearVelocity_.x += this->linearAcceleration_.x * dt;
                this->linearVelocity_.y += this->linearAcceleration_.y * dt;
                this->linearVelocity_.z += this->linearAcceleration_.z * dt;
                linearVelocityChanged(true);
                this->node_->translate(this->linearVelocity_ * dt);
                positionChanged(true);

                // Angular part
                // Note: angularVelocity_ is a Quaternion with w = 0 while angularAcceleration_ is a Vector3
                this->angularVelocity_.x += angularAcceleration_.x * dt;
                this->angularVelocity_.y += angularAcceleration_.y * dt;
                this->angularVelocity_.z += angularAcceleration_.z * dt;
                angularVelocityChanged(true);
                // Calculate new orientation with quaternion derivative. This is about 30% faster than with angle/axis method.
                float mult = dt * 0.5;
                // TODO: this could be optimized by writing it out. The calls currently create 4 new Quaternions!
                Quaternion newOrientation(0.0f, this->angularVelocity_.x * mult, this->angularVelocity_.y * mult, this->angularVelocity_.z * mult);
                newOrientation = this->node_->getOrientation() + newOrientation * this->node_->getOrientation();
                newOrientation.normalise();
                this->node_->setOrientation(newOrientation);
                orientationChanged(true);
            }
        }
    }

    void MobileEntity::setPosition(const Vector3& position)
    {
        if (this->isDynamic())
        {
            btTransform transf = this->physicalBody_->getWorldTransform();
            transf.setOrigin(btVector3(position.x, position.y, position.z));
            this->physicalBody_->setWorldTransform(transf);
        }

        this->node_->setPosition(position);
        positionChanged(false);
    }

    void MobileEntity::translate(const Vector3& distance, Ogre::Node::TransformSpace relativeTo)
    {
        if (this->isDynamic())
        {
            OrxAssert(relativeTo == Ogre::Node::TS_LOCAL, "Cannot translate physical object relative \
                                                          to any other space than TS_LOCAL.");
            this->physicalBody_->translate(btVector3(distance.x, distance.y, distance.z));
        }

        this->node_->translate(distance, relativeTo);
        positionChanged(false);
    }

    void MobileEntity::setOrientation(const Quaternion& orientation)
    {
        if (this->isDynamic())
        {
            btTransform transf = this->physicalBody_->getWorldTransform();
            transf.setRotation(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w));
            this->physicalBody_->setWorldTransform(transf);
        }

        this->node_->setOrientation(orientation);
        orientationChanged(false);
    }

    void MobileEntity::rotate(const Quaternion& rotation, Ogre::Node::TransformSpace relativeTo)
    {
        if (this->isDynamic())
        {
            OrxAssert(relativeTo == Ogre::Node::TS_LOCAL, "Cannot rotate physical object relative \
                                                          to any other space than TS_LOCAL.");
            btTransform transf = this->physicalBody_->getWorldTransform();
            this->physicalBody_->setWorldTransform(transf * btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w)));
        }

        this->node_->rotate(rotation, relativeTo);
        orientationChanged(false);
    }

    void MobileEntity::yaw(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
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
        orientationChanged(false);
    }

    void MobileEntity::pitch(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
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
        orientationChanged(false);
    }

    void MobileEntity::roll(const Degree& angle, Ogre::Node::TransformSpace relativeTo)
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
        orientationChanged(false);
    }

    void MobileEntity::lookAt(const Vector3& target, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        if (this->isDynamic())
        {
            ThrowException(NotImplemented, "ControllableEntity::lookAt() is not yet supported for physical objects.");
            OrxAssert(relativeTo == Ogre::Node::TS_LOCAL, "Cannot align physical object relative \
                                                          to any other space than TS_LOCAL.");
        }

        this->node_->lookAt(target, relativeTo, localDirectionVector);
        orientationChanged(false);
    }

    void MobileEntity::setDirection(const Vector3& direction, Ogre::Node::TransformSpace relativeTo, const Vector3& localDirectionVector)
    {
        if (this->isDynamic())
        {
            ThrowException(NotImplemented, "ControllableEntity::setDirection() is not yet supported for physical objects.");
            OrxAssert(relativeTo == Ogre::Node::TS_LOCAL, "Cannot align physical object relative \
                                                          to any other space than TS_LOCAL.");
        }

        this->node_->setDirection(direction, relativeTo, localDirectionVector);
        orientationChanged(false);
    }

    void MobileEntity::setVelocity(const Vector3& velocity)
    {
        if (this->isDynamic())
            this->physicalBody_->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));

        this->linearVelocity_ = velocity;
        linearVelocityChanged(false);
    }

    void MobileEntity::setAngularVelocity(const Vector3& velocity)
    {
        if (this->isDynamic())
            this->physicalBody_->setAngularVelocity(btVector3(velocity.x, velocity.y, velocity.z));

        this->angularVelocity_ = velocity;
        angularVelocityChanged(false);
    }

    void MobileEntity::setAcceleration(const Vector3& acceleration)
    {
        if (this->isDynamic())
            this->physicalBody_->applyCentralForce(btVector3(acceleration.x * this->getMass(), acceleration.y * this->getMass(), acceleration.z * this->getMass()));

        this->linearAcceleration_ = acceleration;
    }

    void MobileEntity::setAngularAcceleration(const Vector3& acceleration)
    {
        if (this->isDynamic())
        {
            btVector3 inertia(btVector3(1, 1, 1) / this->physicalBody_->getInvInertiaDiagLocal());
            this->physicalBody_->applyTorque(btVector3(acceleration.x, acceleration.y, acceleration.z) * inertia);
        }

        this->angularAcceleration_ = acceleration;
    }

    bool MobileEntity::isCollisionTypeLegal(WorldEntity::CollisionType type) const
    {
        if (type == WorldEntity::Static)
        {
            ThrowException(PhysicsViolation, "Cannot tell a MobileEntity to have static collision type");
            return false;
        }
        else
            return true;
    }

    void MobileEntity::setWorldTransform(const btTransform& worldTrans)
    {
        // We use a dynamic body. So we translate our node accordingly.
        this->node_->setPosition(Vector3(worldTrans.getOrigin().x(), worldTrans.getOrigin().y(), worldTrans.getOrigin().z()));
        this->node_->setOrientation(Quaternion(worldTrans.getRotation().w(), worldTrans.getRotation().x(), worldTrans.getRotation().y(), worldTrans.getRotation().z()));
        this->linearVelocity_.x = this->physicalBody_->getLinearVelocity().x();
        this->linearVelocity_.y = this->physicalBody_->getLinearVelocity().y();
        this->linearVelocity_.z = this->physicalBody_->getLinearVelocity().z();
        this->angularVelocity_.x = this->physicalBody_->getAngularVelocity().x();
        this->angularVelocity_.y = this->physicalBody_->getAngularVelocity().y();
        this->angularVelocity_.z = this->physicalBody_->getAngularVelocity().z();
        linearVelocityChanged(true);
        angularVelocityChanged(true);
        positionChanged(true);
        orientationChanged(true);
    }

    void MobileEntity::getWorldTransform(btTransform& worldTrans) const
    {
        // We use a kinematic body
        worldTrans.setOrigin(btVector3(node_->getPosition().x, node_->getPosition().y, node_->getPosition().z));
        worldTrans.setRotation(btQuaternion(node_->getOrientation().x, node_->getOrientation().y, node_->getOrientation().z, node_->getOrientation().w));
        if (this->isDynamic())
        {
            // This function gets called only once for dynamic objects to set the initial conditions
            // We have to set the velocities too.
            this->physicalBody_->setLinearVelocity(btVector3(linearVelocity_.x, linearVelocity_.y, linearVelocity_.z));
            this->physicalBody_->setAngularVelocity(btVector3(angularVelocity_.x, angularVelocity_.y, angularVelocity_.z));
        }
    }
}
