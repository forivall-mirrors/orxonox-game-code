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
 *      Damian 'Mozork' Frick
 *
 */

#include "AutonomousDrone.h"

#include "core/CoreIncludes.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace orxonox
{
    //TODO: Put your code in here:
    // Create the factory for the drone.

    /**
    @brief
        Constructor. Registers the object and initializes some default values.
    @param creator
        The creator of this object.
    */

	RegisterClass(AutonomousDrone);
    AutonomousDrone::AutonomousDrone(Context* context) : ControllableEntity(context)
    {
        //TODO: Put your code in here:
        // Register the drone class to the core.

    	RegisterObject(AutonomousDrone);
        this->myController_ = NULL;

        this->localLinearAcceleration_.setValue(0, 0, 0);
        this->localAngularAcceleration_.setValue(0, 0, 0);
        this->primaryThrust_  = 100;
        this->auxiliaryThrust_ = 100;
        this->rotationThrust_ = 10;

        this->setCollisionType(WorldEntity::Dynamic);

        //this->myController_ = new AutonomousDroneController(this); // Creates a new controller and passes our this pointer to it as creator.
    }

    /**
    @brief
        Destructor. Destroys controller, if present.
    */
    AutonomousDrone::~AutonomousDrone()
    {
        // Deletes the controller if the object was initialized and the pointer to the controller is not NULL.
        if( this->isInitialized() && this->myController_ != NULL )
            delete this->myController_;
    }

    /**
    @brief
        Method for creating a AutonomousDrone through XML.
    */
    void AutonomousDrone::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        // This calls the XMLPort function of the parent class
        SUPER(AutonomousDrone, XMLPort, xmlelement, mode);

        XMLPortParam(AutonomousDrone, "primaryThrust", setPrimaryThrust, getPrimaryThrust, xmlelement, mode);
        XMLPortParam(AutonomousDrone, "auxiliaryThrust", setAuxiliaryThrust, getAuxiliaryThrust, xmlelement, mode);
        XMLPortParam(AutonomousDrone, "rotationThrust", setRotationThrust, getRotationThrust, xmlelement, mode);
        //TODO: Put your code in here:
        // Make sure you add the variables auxiliaryThrust_ and rotationThrust_ to XMLPort.
        // Variables can be added by the following command
        // XMLPortParam(Classname, "xml-attribute-name (i.e. variablename)", setFunctionName, getFunctionName, xmlelement, mode);
        // Also make sure that you also create the get- and set-functions in AutonomousDrone.h. As you can see, the get- and set-functions for the variable primaryThrust_ has already been specified there, so you can get your inspiration from there.
        

    }

    /**
    @brief
        Defines which actions the AutonomousDrone has to take in each tick.
    @param dt
        The length of the tick.
    */
    void AutonomousDrone::tick(float dt)
    {
        SUPER(AutonomousDrone, tick, dt);

        this->localLinearAcceleration_.setX(this->localLinearAcceleration_.x() * getMass() * this->auxiliaryThrust_);
        this->localLinearAcceleration_.setY(this->localLinearAcceleration_.y() * getMass() * this->auxiliaryThrust_);
        if (this->localLinearAcceleration_.z() > 0)
            this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() * getMass() * this->auxiliaryThrust_);
        else
            this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() * getMass() * this->primaryThrust_);
        this->physicalBody_->applyCentralForce(physicalBody_->getWorldTransform().getBasis() * this->localLinearAcceleration_);
        this->localLinearAcceleration_.setValue(0, 0, 0);

        this->localAngularAcceleration_ *= this->getLocalInertia() * this->rotationThrust_;
        this->physicalBody_->applyTorque(physicalBody_->getWorldTransform().getBasis() * this->localAngularAcceleration_);
        this->localAngularAcceleration_.setValue(0, 0, 0);
    }

    /**
    @brief
        Moves the AutonomousDrone in the negative z-direction (Front/Back) by an amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the movement.
    */
    void AutonomousDrone::moveFrontBack(const Vector2& value)
    {
        this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() - value.x);
    }

    /**
    @brief
        Moves the AutonomousDrone in the x-direction (Right/Left) by an amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the movement.
    */
    void AutonomousDrone::moveRightLeft(const Vector2& value)
    {
        this->localLinearAcceleration_.setX(this->localLinearAcceleration_.x() + value.x);
    }

    /**
    @brief
        Moves the AutonomousDrone in the y-direction (Up/Down) by an amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the movement.
    */
    void AutonomousDrone::moveUpDown(const Vector2& value)
    {
        this->localLinearAcceleration_.setY(this->localLinearAcceleration_.y() + value.x);
    }

    /**
    @brief
        Rotates the AutonomousDrone around the y-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void AutonomousDrone::rotateYaw(const Vector2& value)
    {
        this->localAngularAcceleration_.setY(this->localAngularAcceleration_.y() - value.x);
    }

    /**
    @brief
        Rotates the AutonomousDrone around the x-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void AutonomousDrone::rotatePitch(const Vector2& value)
    {
        this->localAngularAcceleration_.setX(this->localAngularAcceleration_.x() + value.x);
    }

    /**
    @brief
        Rotates the AutonomousDrone around the z-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void AutonomousDrone::rotateRoll(const Vector2& value)
    {
        this->localAngularAcceleration_.setZ(this->localAngularAcceleration_.z() + value.x);
    }

}
