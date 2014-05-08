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
    @file JumpEnemy.cc
    @brief Implementation of the JumpEnemy class.
*/

#include "JumpEnemy.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "graphics/Model.h"
#include "gametypes/Gametype.h"

#include "JumpFigure.h"

#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpEnemy);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    JumpEnemy::JumpEnemy(Context* context) : MovableEntity(context)
    {
        RegisterObject(JumpEnemy);

        this->figure_ = 0;
        setProperties(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        this->setPosition(Vector3(0,0,0));
        this->setVelocity(Vector3(0,0,0));
        this->setAcceleration(Vector3(0,0,0));
    }

    /**
    @brief
        Destructor.
    */
    JumpEnemy::~JumpEnemy()
    {
        /*if (this->isInitialized())
        {
            if (this->bDeleteBats_)
                delete this->figure_;

            delete[] this->batID_;
        }*/
    }

    //xml port for loading sounds
    void JumpEnemy::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpEnemy, XMLPort, xmlelement, mode);
    }

    /**
    @brief
        Is called every tick.
        Handles the movement of the ball and its interaction with the boundaries and bats.
    @param dt
        The time since the last tick.
    */
    void JumpEnemy::tick(float dt)
    {
        SUPER(JumpEnemy, tick, dt);

        // Get the current position, velocity and acceleration of the enemy.
        Vector3 position = getPosition();
        Vector3 velocity = getVelocity();

        if ((position.x < leftBoundary_ && velocity.x < 0) || (position.x > rightBoundary_ && velocity.x > 0))
        {
        	velocity.x = -velocity.x;
        }

        if ((position.z < lowerBoundary_ && velocity.z < 0) || (position.z > upperBoundary_ && velocity.z > 0))
        {
        	velocity.z = -velocity.z;
        }

        // Set the position, velocity and acceleration of the enemy, if they have changed.
        if (velocity != getVelocity())
            setVelocity(velocity);
        if (position != getPosition())
            setPosition(position);


    }

    void JumpEnemy::setProperties(float newLeftBoundary, float newRightBoundary, float newLowerBoundary, float newUpperBoundary, float newHSpeed, float newVSpeed)
    {
        leftBoundary_ = newLeftBoundary;
        rightBoundary_ = newRightBoundary;
        lowerBoundary_ = newLowerBoundary;
        upperBoundary_ = newUpperBoundary;

        this->setVelocity(Vector3(newHSpeed,0,newVSpeed));
    }

    /**
    @brief
        Set the bats for the ball.
    @param bats
        An array (of size 2) of weak pointers, to be set as the new bats.
    */
    void JumpEnemy::setFigure(WeakPtr<JumpFigure> newFigure)
    {
        figure_ = newFigure;
    }

    void JumpEnemy::touchFigure()
    {

    }
}
