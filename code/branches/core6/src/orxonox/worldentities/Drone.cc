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
 */


#include "Drone.h"

#include "core/XMLPort.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace orxonox
{
    RegisterClass(Drone);
    /**
    @brief
        Constructor. Registers the object and initializes some default values.
    */
    Drone::Drone(Context* context) : Pawn(context)
    {
        RegisterObject(Drone);

        this->myController_ = 0;

        this->localLinearAcceleration_.setValue(0, 0, 0);
        this->localAngularAcceleration_.setValue(0, 0, 0);
        this->setRadarVisibility(false);
        this->setCollisionType(WorldEntity::Dynamic);

        myController_ = new DroneController(this->getContext()); //!< Creates a new controller and passes our this pointer to it as creator.
        myController_->setDrone(this);

        this->setController(myController_);
    }

    /**
    @brief
        Destructor. Destroys controller, if present.
    */
    Drone::~Drone()
    {
        if( this->isInitialized() && this->myController_ )
            this->myController_->destroy();
    }

    /**
    @brief
        Method for creating a Drone through XML.
    */
    void Drone::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Drone, XMLPort, xmlelement, mode);

        XMLPortParam(Drone, "primaryThrust_", setPrimaryThrust, getPrimaryThrust, xmlelement, mode);
        XMLPortParam(Drone, "auxilaryThrust_", setAuxilaryThrust, getAuxilaryThrust, xmlelement, mode);
        XMLPortParam(Drone, "rotationThrust_", setRotationThrust, getRotationThrust, xmlelement, mode);
        XMLPortParam(Drone, "maxDistanceToOwner_", setMaxDistanceToOwner, getMaxDistanceToOwner, xmlelement, mode);
        XMLPortParam(Drone, "minDistanceToOwner_", setMinDistanceToOwner, getMinDistanceToOwner, xmlelement, mode);
        XMLPortParam(Drone, "maxShootingRange_", setMaxShootingRange, getMaxShootingRange, xmlelement, mode);
    }


    /**
    @brief
        Defines which actions the Drone has to take in each tick.
    @param dt
        The length of the tick.
    */
    void Drone::tick(float dt)
    {
        SUPER(Drone, tick, dt);

        //if (this->hasLocalController())
        //{
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
        //}
    }

    /**
    @brief
        Moves the Drone in the negative z-direction (Front/Back) by an amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the movement.
    */
    void Drone::moveFrontBack(const Vector2& value)
    {
        this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() - value.x);
    }

    /**
    @brief
        Moves the Drone in the x-direction (Right/Left) by an amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the movement.
    */
    void Drone::moveRightLeft(const Vector2& value)
    {
        this->localLinearAcceleration_.setX(this->localLinearAcceleration_.x() + value.x);
    }

    /**
    @brief
        Moves the Drone in the y-direction (Up/Down) by an amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the movement.
    */
    void Drone::moveUpDown(const Vector2& value)
    {
        this->localLinearAcceleration_.setY(this->localLinearAcceleration_.y() + value.x);
    }

    /**
    @brief
        Rotates the Drone around the y-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void Drone::rotateYaw(const Vector2& value)
    {
        this->localAngularAcceleration_.setY(this->localAngularAcceleration_.y() + value.x);
    }

    /**
    @brief
        Rotates the Drone around the x-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void Drone::rotatePitch(const Vector2& value)
    {
        this->localAngularAcceleration_.setX(this->localAngularAcceleration_.x() + value.x);
    }

    /**
    @brief
        Rotates the Drone around the z-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void Drone::rotateRoll(const Vector2& value)
    {
        this->localAngularAcceleration_.setZ(this->localAngularAcceleration_.z() + value.x);
    }

}
