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

#include "Rocket.h"

#include "core/XMLPort.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace orxonox
{
	CreateFactory(Rocket);    // put your code in here:
    // create the factory for the Rocket

    /**
    @brief
        Constructor. Registers the object and initializes some default values.
    */
    Rocket::Rocket(BaseObject* creator) : ControllableEntity(creator)
    {
        this->myController_ = 0;
        // put your code in here:
        RegisterObject(Rocket);// - register the Rocket class to the core
        
        this->localLinearAcceleration_.setValue(0, 0, 0);
        this->localAngularAcceleration_.setValue(0, 0, 0);
        this->primaryThrust_  = 100;
        this->auxilaryThrust_ = 100;
        this->rotationThrust_ = 10;
        
        this->setCollisionType(WorldEntity::Kinematic);
        
        this->myController_ = new RocketController(static_cast<BaseObject*>(this)); //!< Creates a new controller and passes our this pointer to it as creator.
    }

    /**
    @brief
        Destructor. Destroys controller, if present.
    */
    Rocket::~Rocket()
    {
        if( this->myController_ != NULL )
            this->myController_->destroy();
    }

    /**
    @brief
        Method for creating a Rocket through XML.
    */
    void Rocket::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        // this calls the XMLPort function of the parent class
        SUPER(Rocket, XMLPort, xmlelement, mode);

        // put your code in here:
		XMLPortParam(Rocket, "primaryThrust", setPrimaryThrust, getPrimaryThrust, xmlelement, mode);
		//XMLPortParam(Rocket, "auxilaryThrust", setAuxilaryThrust, getAuxilaryThrust, xmlelement, mode);
		//XMLPortParam(Rocket, "rotationThrust", setRotationThrust, getRotationThrust, xmlelement, mode);
        // make sure you add the variables primaryThrust_, auxilaryThrust_ and rotationThrust_ to xmlport 
        // make sure that the set- and get-functions exist.
        // variables can be added by the following command 
        // XMLPortParam(Classname, "xml-attribute-name (i.e. variablename)", setFunction, getFunction, xmlelement, mode)
 
    }

    /**
    @brief
        Defines which actions the Rocket has to take in each tick.
    @param dt
        The length of the tick.
    */
    void Rocket::tick(float dt)
    {
        SUPER(Rocket, tick, dt);
        
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
        Moves the Rocket in the negative z-direction (Front/Back) by an amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the movement.
    */
    void Rocket::moveFrontBack(const Vector2& value)
    {
        this->localLinearAcceleration_.setZ(this->localLinearAcceleration_.z() - value.x);
    }

    /**
    @brief
        Moves the Rocket in the x-direction (Right/Left) by an amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the movement.
    */
    void Rocket::moveRightLeft(const Vector2& value)
    {
        this->localLinearAcceleration_.setX(this->localLinearAcceleration_.x() + value.x);
    }

    /**
    @brief
        Moves the Rocket in the y-direction (Up/Down) by an amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the movement.
    */
    void Rocket::moveUpDown(const Vector2& value)
    {
        this->localLinearAcceleration_.setY(this->localLinearAcceleration_.y() + value.x);
    }

    /**
    @brief
        Rotates the Rocket around the y-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void Rocket::rotateYaw(const Vector2& value)
    {
        this->localAngularAcceleration_.setY(this->localAngularAcceleration_.y() - value.x);
    }

    /**
    @brief
        Rotates the Rocket around the x-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void Rocket::rotatePitch(const Vector2& value)
    {
        this->localAngularAcceleration_.setX(this->localAngularAcceleration_.x() + value.x);
    }

    /**
    @brief
        Rotates the Rocket around the z-axis by the amount specified by the first component of the input 2-dim vector.
    @param value
        The vector determining the amount of the angular movement.
    */
    void Rocket::rotateRoll(const Vector2& value)
    {
        this->localAngularAcceleration_.setZ(this->localAngularAcceleration_.z() + value.x);
    }
    
}
