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
    @file PongBat.cc
    @brief Implementation of the PongBat class.
*/

#include "PongBat.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(PongBat);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    PongBat::PongBat(Context* context) : ControllableEntity(context)
    {
        RegisterObject(PongBat);

        this->movement_ = 0;
        this->bMoveLocal_ = false;
        this->speed_ = 60;
        this->length_ = 0.25;
        this->fieldHeight_ = 100;
        this->bSteadiedPosition_ = false;

        this->registerVariables();
    }

    /**
    @brief
        Registers variables to be synchronized over the network.
    */
    void PongBat::registerVariables()
    {
        registerVariable(this->speed_);
        registerVariable(this->fieldHeight_);
        registerVariable(this->length_);
    }

    /**
    @brief
        Is called each tick.
        Moves the bat.
    @param dt
        The time since last tick.
    */
    void PongBat::tick(float dt)
    {
        // If the bat is controlled (but not over the network).
        if (this->hasLocalController())
        {
            if (this->movement_ != 0)
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
            }
        }

        SUPER(PongBat, tick, dt);

        // Restrict the position of the bats, for them to always be between the upper and lower delimiters. i.e. the bats stall if they reach the upper or lower boundary.
        Vector3 position = this->getPosition();
        if (position.z > this->fieldHeight_ / 2 - this->fieldHeight_ * this->length_ / 2)
            position.z = this->fieldHeight_ / 2 - this->fieldHeight_ * this->length_ / 2;
        if (position.z < -this->fieldHeight_ / 2 + this->fieldHeight_ * this->length_ / 2)
            position.z = -this->fieldHeight_ / 2 + this->fieldHeight_ * this->length_ / 2;
        if (position != this->getPosition())
        {
            this->setPosition(position);
            this->setVelocity( Vector3::ZERO );
        }
    }

    /**
    @brief
        Overloaded the function to steer the bat up and down.
    @param value
        A vector whose first component is the inverse direction in which we want to steer the bat.
    */
    void PongBat::moveFrontBack(const Vector2& value)
    {
        this->bMoveLocal_ = false;
        this->movement_ = -value.x;
    }

    /**
    @brief
        Overloaded the function to steer the bat up and down.
    @param value
        A vector whose first component is the direction in which we wnat to steer the bat.
    */
    void PongBat::moveRightLeft(const Vector2& value)
    {
        this->bMoveLocal_ = true;
        this->movement_ = value.x;
    }

    /**
    @brief
        Is called when the player changed.
    */
    void PongBat::changedPlayer()
    {
        this->setVelocity(0, 0, 0);
    }
}
