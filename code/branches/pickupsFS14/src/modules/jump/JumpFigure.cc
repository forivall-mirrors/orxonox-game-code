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
 *   Co-authors:
 *      ...
 *
 */

/**
    @file JumpFigure.cc
    @brief Implementation of the JumpFigure class.
*/

#include "JumpFigure.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpFigure);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    JumpFigure::JumpFigure(Context* context) : ControllableEntity(context)
    {
        RegisterObject(JumpFigure);

        leftHand_ = NULL;
        rightHand_ = NULL;

        fieldHeight_ = 0;
        fieldWidth_ = 0;

        jumpSpeed_ = 0.0;
        handSpeed_ = 0.0;
        handMaxAngle_ = 0.0;
        handMinAngle_ = 0.0;
        rocketPos_ = 0.0;
        propellerPos_ = 0.0;
        bootsPos_ = 0.0;

        moveUpPressed = false;
        moveDownPressed = false;
        moveLeftPressed = false;
        moveDownPressed = false;
        firePressed = false;
        fireSignal = false;
        timeSinceLastFire = 0.0;

        gravityAcceleration = 8.0;
        mouseFactor_ = 75.0;
        maxFireRate = 0.3;

        handAngle_ = 0.0;
        animateHands_ = false;
        turnUp_ = false;

        rocketActive_ = false;
        propellerActive_ = false;
        bootsActive_ = false;
        shieldActive_ = false;
        rocketSpeed_ = 0.0;
        propellerSpeed_ = 0.0;

        dead_ = false;
    }

    /**
    @brief
        Method to create a JumpCenterpoint through XML.
    */
    void JumpFigure::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpFigure, XMLPort, xmlelement, mode);
        XMLPortParam(JumpFigure, "mouseFactor", setMouseFactor, getMouseFactor, xmlelement, mode);
        XMLPortParam(JumpFigure, "modelLefthand", setModelLeftHand, getModelLeftHand, xmlelement, mode);
        XMLPortParam(JumpFigure, "modelRighthand", setModelRightHand, getModelRightHand, xmlelement, mode);
        XMLPortParam(JumpFigure, "rocketPos", setRocketPos, getRocketPos, xmlelement, mode);
        XMLPortParam(JumpFigure, "propellerPos", setPropellerPos, getPropellerPos, xmlelement, mode);
        XMLPortParam(JumpFigure, "bootsPos", setBootsPos, getBootsPos, xmlelement, mode);
        XMLPortParam(JumpFigure, "jumpSpeed", setJumpSpeed, getJumpSpeed, xmlelement, mode);
        XMLPortParam(JumpFigure, "rocketSpeed", setRocketSpeed, getRocketSpeed, xmlelement, mode);
        XMLPortParam(JumpFigure, "propellerSpeed", setPropellerSpeed, getPropellerSpeed, xmlelement, mode);
        XMLPortParam(JumpFigure, "handMinAngle", setHandMinAngle, getHandMinAngle, xmlelement, mode);
        XMLPortParam(JumpFigure, "handMaxAngle", setHandMaxAngle, getHandMaxAngle, xmlelement, mode);
        XMLPortParam(JumpFigure, "handSpeed", setHandSpeed, getHandSpeed, xmlelement, mode);
    }

    /**
    @brief
        Is called each tick.
        Moves the bat.
    @param dt
        The time since last tick.
    */
    void JumpFigure::tick(float dt)
    {
    	SUPER(JumpFigure, tick, dt);

        // If the bat is controlled (but not over the network).
        if (hasLocalController())
        {
        	timeSinceLastFire += dt;

        	// Move up/down
        	Vector3 velocity = getVelocity();
        	if (rocketActive_ == true)
        	{
        		velocity.z = rocketSpeed_;
        	}
        	else if (propellerActive_ == true)
        	{
        		velocity.z = propellerSpeed_;
        	}
        	else
        	{
        		velocity.z -= gravityAcceleration;
        	}

        	// Animate Hands
        	if (animateHands_ == true)
        	{
        		if (turnUp_ == true)
        		{
        			handAngle_ += handSpeed_ * dt;
        		}
        		else
				{
					handAngle_ -= handSpeed_ * dt;
				}
            	if (handAngle_ > handMaxAngle_)
            	{
            		turnUp_ = false;
            	}
            	if (handAngle_ <= handMinAngle_)
            	{
            		animateHands_ = false;
            	}

				if (leftHand_ != NULL)
				{
					leftHand_->setOrientation(Vector3(0.0, 1.0, 0.0), Degree(-handAngle_));
				}
				if (rightHand_ != NULL)
				{
					rightHand_->setOrientation(Vector3(0.0, 1.0, 0.0), Degree(handAngle_));
				}
        	}

        	// Move left/right
        	if (dead_ == false)
        	{
        		velocity.x = -mouseFactor_*horizontalSpeed;
        	}
        	else
        	{
        		velocity.x = 0.0;
        	}

        	// Cheats
        	if (moveUpPressed == true)
        	{
        		velocity.z = 200.0f;
        		moveUpPressed = false;
        		dead_ = false;
        	}
        	if (moveDownPressed == true)
        	{
        		moveDownPressed = false;
        	}

        	setVelocity(velocity);


        	if (firePressed && timeSinceLastFire >= maxFireRate)
        	{
				firePressed = false;
				timeSinceLastFire = 0.0;
				fireSignal = true;
				//orxout() << "fired signal set" << endl;
        	}
        }

        // Move through the left and right screen boundaries
        Vector3 position = getPosition();
        if (position.x < -fieldWidth_*1.1)
        {
        	position.x = fieldWidth_*1.1;
        }
        else if (position.x > fieldWidth_*1.1)
        {
        	position.x = -fieldWidth_*1.1;
        }
        setPosition(position);

        // Reset key variables
        moveUpPressed = false;
        moveDownPressed = false;
        moveLeftPressed = false;
        moveDownPressed = false;
        firePressed = false;
    }

    void JumpFigure::JumpFromPlatform(JumpPlatform* platform)
    {
    	if (dead_ == false)
    	{
        	Vector3 velocity = getVelocity();
        	velocity.z = (bootsActive_ ? 1.2*jumpSpeed_ : jumpSpeed_);
        	setVelocity(velocity);

        	animateHands_ = true;
        	handAngle_ = 0.0;
        	turnUp_ = true;
    	}
    }

    void JumpFigure::JumpFromSpring(JumpSpring* spring)
    {
    	if (dead_ == false)
    	{
        	Vector3 velocity = getVelocity();
        	velocity.z = 1.2*jumpSpeed_;
        	setVelocity(velocity);
    	}
    }

    void JumpFigure::CollisionWithEnemy(JumpEnemy* enemy)
	{
    	if (rocketActive_ == false && propellerActive_ == false && shieldActive_ == false)
		{
			dead_ = true;
		}
	}

    bool JumpFigure::StartRocket(JumpRocket* rocket)
    {
    	if (rocketActive_ == false && propellerActive_ == false && bootsActive_ == false)
    	{
        	attach(rocket);
        	rocket->setPosition(0.0, rocketPos_, 0.0);
        	rocket->setVelocity(0.0, 0.0, 0.0);
        	rocketActive_ = true;

        	return true;
    	}

    	return false;
    }

    void JumpFigure::StopRocket(JumpRocket* rocket)
    {
		rocket->setPosition(0.0, 0.0, -1000.0);
    	rocket->setVelocity(0.0, 0.0, 0.0);
    	detach(rocket);
		rocket->destroy();
		rocketActive_ = false;
    }

    bool JumpFigure::StartPropeller(JumpPropeller* propeller)
    {
    	if (rocketActive_ == false && propellerActive_ == false && bootsActive_ == false)
    	{
        	attach(propeller);
        	propeller->setPosition(0.0, 0.0, propellerPos_);
        	propeller->setVelocity(0.0, 0.0, 0.0);
        	propellerActive_ = true;

        	return true;
    	}

    	return false;
    }

    void JumpFigure::StopPropeller(JumpPropeller* propeller)
    {
    	propeller->setPosition(0.0, 0.0, -1000.0);
    	propeller->setVelocity(0.0, 0.0, 0.0);
    	detach(propeller);
    	propeller->destroy();
    	propellerActive_ = false;
    }

    bool JumpFigure::StartBoots(JumpBoots* boots)
    {
    	if (rocketActive_ == false && propellerActive_ == false && bootsActive_ == false)
    	{
        	attach(boots);
        	boots->setPosition(0.0, 0.0, bootsPos_);
        	boots->setVelocity(0.0, 0.0, 0.0);
        	bootsActive_ = true;

        	return true;
    	}

    	return false;
    }

    void JumpFigure::StopBoots(JumpBoots* boots)
    {
    	boots->setPosition(0.0, 0.0, -1000.0);
    	boots->setVelocity(0.0, 0.0, 0.0);
    	detach(boots);
    	boots->destroy();
    	bootsActive_ = false;
    }

    bool JumpFigure::StartShield(JumpShield* shield)
    {
    	if (shieldActive_ == false)
    	{
        	attach(shield);
        	shield->setPosition(0.0, 0.0, propellerPos_);
        	shield->setVelocity(0.0, 0.0, 0.0);
        	shieldActive_ = true;

        	return true;
    	}

    	return false;
    }

    void JumpFigure::StopShield(JumpShield* shield)
    {
    	shield->setPosition(0.0, 0.0, -1000.0);
    	shield->setVelocity(0.0, 0.0, 0.0);
    	detach(shield);
    	shield->destroy();
    	shieldActive_ = false;
    }

    void JumpFigure::InitializeAnimation(Context* context)
    {
    	leftHand_ = new Model(context);
    	rightHand_ = new Model(context);

    	leftHand_->addTemplate(modelLeftHand_);
    	rightHand_->addTemplate(modelRightHand_);

		attach(leftHand_);
		attach(rightHand_);
    }

    /**
    @briefhandPosition_
        Overloaded the function to steer the bat up and down.
    @param value
        A vector whose first component is the inverse direction in which we want to steer the bat.
    */
    void JumpFigure::moveFrontBack(const Vector2& value)
    {
    	if (value.x > 0)
    	{
    		//orxout() << "up pressed" << endl;
    		moveUpPressed = true;
    		moveDownPressed = false;
    	}
    	else
    	{
    		//orxout() << "down pressed" << endl;
    		moveUpPressed = false;
    		moveDownPressed = true;
    	}
    }

    /**
    @brief
        Overloaded the function to steer the bat up and down.
    @param value
        A vector whose first component is the direction in which we wnat to steer the bat.
    */
    void JumpFigure::moveRightLeft(const Vector2& value)
    {
    	if (value.x > 0)
    	{
    		moveLeftPressed = false;
    		moveRightPressed = true;
    	}
    	else
    	{
    		moveLeftPressed = true;
    		moveRightPressed = false;
    	}
    }

    void JumpFigure::rotateYaw(const Vector2& value)
    {
    	horizontalSpeed = value.x;
    }

    void JumpFigure::rotatePitch(const Vector2& value)
    {


    }

    void JumpFigure::rotateRoll(const Vector2& value)
    {


    }

    void JumpFigure::fire(unsigned int firemode)
    {
    	//SUPER(JumpFigure, fire, firemode);
    }

    void JumpFigure::fired(unsigned int firemode)
    {
    	//SUPER(JumpFigure, fired, firemode);
    	firePressed = true;
    }
}
