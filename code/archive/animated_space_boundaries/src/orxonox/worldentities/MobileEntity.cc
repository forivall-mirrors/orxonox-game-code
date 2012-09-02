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

#include "MobileEntity.h"

#include <OgreSceneNode.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "Scene.h"

namespace orxonox
{
    MobileEntity::MobileEntity(BaseObject* creator) : WorldEntity(creator)
    {
        RegisterObject(MobileEntity);

        this->linearAcceleration_  = Vector3::ZERO;
        this->linearVelocity_      = Vector3::ZERO;
        this->angularAcceleration_ = Vector3::ZERO;
        this->angularVelocity_     = Vector3::ZERO;
    }

    MobileEntity::~MobileEntity()
    {
    }

    void MobileEntity::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(MobileEntity, XMLPort, xmlelement, mode);

        XMLPortParamTemplate(MobileEntity, "velocity",     setVelocity,     getVelocity,     xmlelement, mode, const Vector3&);

        Vector3 rotationAxis(this->getRotationAxis());
        Degree rotationRate = this->getRotationRate();
        XMLPortParamVariable(MobileEntity, "rotationaxis", rotationAxis, xmlelement, mode);
        XMLPortParamVariable(MobileEntity, "rotationrate", rotationRate, xmlelement, mode);
        if (mode == XMLPort::LoadObject)
        {
            if (rotationAxis == Vector3::ZERO)
                this->setAngularVelocity(Vector3::ZERO);
            else
                this->setAngularVelocity(rotationAxis.normalisedCopy() * rotationRate.valueRadians());
        }
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
                this->node_->translate(this->linearVelocity_ * dt);

    

                // Angular part
                // Note: angularVelocity_ is a Quaternion with w = 0 while angularAcceleration_ is a Vector3
                this->angularVelocity_.x += angularAcceleration_.x * dt;
                this->angularVelocity_.y += angularAcceleration_.y * dt;
                this->angularVelocity_.z += angularAcceleration_.z * dt;
                // Calculate new orientation with quaternion derivative. This is about 30% faster than with angle/axis method.
                float mult = dt * 0.5f;
                // TODO: this could be optimized by writing it out. The calls currently create 4 new Quaternions!
                Quaternion newOrientation(0.0f, this->angularVelocity_.x * mult, this->angularVelocity_.y * mult, this->angularVelocity_.z * mult);
                newOrientation = this->node_->getOrientation() + newOrientation * this->node_->getOrientation();
                newOrientation.normalise();
                this->node_->setOrientation(newOrientation);
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
    }

    void MobileEntity::setVelocity(const Vector3& velocity)
    {
        if (this->isDynamic())
            this->physicalBody_->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));

        this->linearVelocity_ = velocity;
    }

    void MobileEntity::setAngularVelocity(const Vector3& velocity)
    {
        if (this->isDynamic())
            this->physicalBody_->setAngularVelocity(btVector3(velocity.x, velocity.y, velocity.z));

        this->angularVelocity_ = velocity;
    }

    void MobileEntity::setAcceleration(const Vector3& acceleration)
    {
        if (this->isDynamic())
        {
            this->physicalBody_->applyCentralForce(btVector3(acceleration.x * this->getMass(), acceleration.y * this->getMass(), acceleration.z * this->getMass()));
        }

        // If not bullet-managed (deprecated? SpaceShip doesn't use this anymore for movement)
        this->linearAcceleration_ = acceleration;
    }

    void MobileEntity::addAcceleration(const Vector3 &acceleration, const Vector3 &relativePosition)
    {
        if(this->isDynamic())
        {
            this->physicalBody_->applyForce(this->getMass() * btVector3(acceleration.x, acceleration.y, acceleration.z), btVector3(relativePosition.x, relativePosition.y, relativePosition.z));
        }
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

    void MobileEntity::applyCentralForce(const Vector3& force)
    {
        if (this->isDynamic())
            this->physicalBody_->applyCentralForce(btVector3(force.x * this->getMass(), force.y * this->getMass(), force.z * this->getMass()));
    }

    bool MobileEntity::isCollisionTypeLegal(WorldEntity::CollisionType type) const
    {
        if (type == WorldEntity::Static)
        {
            CCOUT(1) << "Error: Cannot tell a MobileEntity to have static collision type! Ignoring." << std::endl;
            assert(false); // Only in debug mode
            return false;
        }
        else
            return true;
    }

    void MobileEntity::setWorldTransform(const btTransform& worldTrans)
    {
        // We use a dynamic body. So we translate our node accordingly.
        this->node_->setPosition(Vector3(worldTrans.getOrigin().x(), worldTrans.getOrigin().y(), worldTrans.getOrigin().z()));
        btQuaternion temp(worldTrans.getRotation());
        this->node_->setOrientation(Quaternion(temp.w(), temp.x(), temp.y(), temp.z()));
        this->linearVelocity_.x = this->physicalBody_->getLinearVelocity().x();
        this->linearVelocity_.y = this->physicalBody_->getLinearVelocity().y();
        this->linearVelocity_.z = this->physicalBody_->getLinearVelocity().z();
        this->angularVelocity_.x = this->physicalBody_->getAngularVelocity().x();
        this->angularVelocity_.y = this->physicalBody_->getAngularVelocity().y();
        this->angularVelocity_.z = this->physicalBody_->getAngularVelocity().z();
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
