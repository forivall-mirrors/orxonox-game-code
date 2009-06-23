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
 *      Oli Scheuss
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "Drone.h"

#include "core/XMLPort.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace orxonox
{
    // PLACE YOUR CODE HERE
    // create the factory for the drone

    Drone::Drone(BaseObject* creator) : ControllableEntity(creator)
    {
        this->myController_ = 0;
        // PLACE YOUR CODE HERE
        // - register the drone class to the core
        // - create a new controller and pass our this pointer to it as creator

        this->localLinearAcceleration_.setValue(0, 0, 0);
        this->localAngularAcceleration_.setValue(0, 0, 0);
        this->primaryThrust_  = 100;
        this->auxilaryThrust_ = 100;
        this->rotationThrust_ = 10;
        this->steering_ = Vector3::ZERO;

        this->setCollisionType(WorldEntity::Dynamic);

        this->myController_ = new DroneController(this);
    }

    Drone::~Drone()
    {
        if (this->isInitialized() && this->myController_)
            delete this->myController_;
    }

    void Drone::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        // this calls the XMLPort function of the parent class
        SUPER(Drone, XMLPort, xmlelement, mode);

        // PLACE YOUR CODE HERE
        // make sure you add the variables primaryThrust_, auxilaryThrust_ and rotationThrust_ to xmlport
        // variables can be added by the following command
        // XMLPortParamVariable(Class, "xml-attribute-name",  variable_name,  xmlelement, mode);
    }

    void Drone::tick(float dt)
    {
        // PLACE YOUR CODE HERE
        // make sure the tick function of the base class gets called here

        this->localLinearAcceleration_.setX(this->localLinearAcceleration_.x() * getMass() * this->auxilaryThrust_);
        this->localLinearAcceleration_.setY(this->localLinearAcceleration_.y() * getMass() * this->auxilaryThrust_);
        if (this->localLinearAcceleration_.z() > 0)
            this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() * getMass() * this->auxilaryThrust_);
        else
            this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() * getMass() * this->primaryThrust_);
        this->physicalBody_->applyCentralForce(physicalBody_->getWorldTransform().getBasis() * this->localLinearAcceleration_);
        this->localLinearAcceleration_.setValue(0, 0, 0);

        this->localAngularAcceleration_ *= this->getLocalInertia() * this->rotationThrust_;
        this->physicalBody_->applyTorque(physicalBody_->getWorldTransform().getBasis() * this->localAngularAcceleration_);
        this->localAngularAcceleration_.setValue(0, 0, 0);
    }


    void Drone::moveFrontBack(const Vector2& value)
    {
        this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() - value.x);
        this->steering_.z = -value.x;
    }

    void Drone::moveRightLeft(const Vector2& value)
    {
        this->localLinearAcceleration_.setX(this->localLinearAcceleration_.x() + value.x);
        this->steering_.x = value.x;
    }

    void Drone::moveUpDown(const Vector2& value)
    {
        this->localLinearAcceleration_.setY(this->localLinearAcceleration_.y() + value.x);
        this->steering_.y = value.x;
    }

    void Drone::rotateYaw(const Vector2& value)
    {
        this->localAngularAcceleration_.setY(this->localAngularAcceleration_.y() - value.x);
    }

    void Drone::rotatePitch(const Vector2& value)
    {
        this->localAngularAcceleration_.setX(this->localAngularAcceleration_.x() + value.x);
    }

    void Drone::rotateRoll(const Vector2& value)
    {
        this->localAngularAcceleration_.setZ(this->localAngularAcceleration_.z() + value.x);
    }
}
