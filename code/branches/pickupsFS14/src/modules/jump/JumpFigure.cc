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

        this->movement_ = 0;
        this->bMoveLocal_ = false;
        this->length_ = 0.25;
        this->fieldWidth_ = 180;
        this->bSteadiedPosition_ = false;

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
    }

    /**
    @brief
        Method to create a JumpCenterpoint through XML.
    */
    void JumpFigure::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpFigure, XMLPort, xmlelement, mode);
        XMLPortParam(JumpFigure, "mouseFactor", setMouseFactor, getMouseFactor, xmlelement, mode);
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
        if (this->hasLocalController())
        {
        	timeSinceLastFire += dt;

            /*if (this->movement_ != 0)
            {
                // The absolute value of the movement is restricted to be lesser or equal than the speed of the bat.
                this->movement_ = clamp(this->movement_, -1.0f, 1.0f) * this->speed_;

                // If moveRightLeft() is used the movement is dependento on wehther it is the right or the left bat, so, it is i.e. dependent on the orientation of the bat.
                if (this->bMoveLocal_)
                    this->setVelocity(this->getOrientation() * Vector3(this->movement_, 0, 0));
                else
                    this->setVelocity(0, 0, this->movement_);

                this->movement_ = 0;
                this->bSteadiedPosition_ = false;
            }
            // If there is no movement but the position has not been steadied, the velocity is set to zero and the position is reaffirmed.
            else if (!this->bSteadiedPosition_)
            {
                // To ensure network synchronicity
                this->setVelocity(0, 0, 0);
                this->setPosition(this->getPosition());
                this->bSteadiedPosition_ = true;
            }*/


        	Vector3 velocity = getVelocity();

        	velocity.z -= gravityAcceleration;

        	/*if (moveLeftPressed == true)
        	{
        		velocity.x = -accelerationFactor;
        		moveLeftPressed = false;
        	}
        	if (moveRightPressed == true)
        	{
        		velocity.x = accelerationFactor;
        		moveRightPressed = false;
        	}*/

        	velocity.x = -mouseFactor_*horizontalSpeed;

        	if (moveUpPressed == true)
        	{
        		velocity.z = 200.0f;
        		moveUpPressed = false;
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

        Vector3 position = this->getPosition();

        if (position.x < -fieldWidth_*1.1)
        {
        	position.x = fieldWidth_*1.1;
        }
        else if (position.x > fieldWidth_*1.1)
        {
        	position.x = -fieldWidth_*1.1;
        }

        this->setPosition(position);

        moveUpPressed = false;
        moveDownPressed = false;
        moveLeftPressed = false;
        moveDownPressed = false;
        firePressed = false;
    }

    void JumpFigure::JumpFromPlatform(JumpPlatform* platform)
    {
    	Vector3 velocity = getVelocity();
    	velocity.z = 200.0f;
    	setVelocity(velocity);
    }


    /**
    @brief
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
    		//orxout() << "right pressed" << endl;
    		moveLeftPressed = false;
    		moveRightPressed = true;
    	}
    	else
    	{
    		//orxout() << "left pressed" << endl;
    		moveLeftPressed = true;
    		moveRightPressed = false;
    	}
        /*this->bMoveLocal_ = true;
        this->movement_ = value.x;*/
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

    void JumpFigure::fired(unsigned int firemode)
    {
    	orxout() << "fire pressed" << endl;
    	firePressed = true;
    }
}
