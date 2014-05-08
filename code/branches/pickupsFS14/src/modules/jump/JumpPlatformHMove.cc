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
    @file JumpPlatformHMove.cc
    @brief Implementation of the JumpPlatform class.
*/

#include "JumpPlatformHMove.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"

#include "gametypes/Gametype.h"

#include "JumpFigure.h"

#include "sound/WorldSound.h"
#include "core/XMLPort.h"

#include "graphics/Backlight.h"

namespace orxonox
{
    RegisterClass(JumpPlatformHMove);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    JumpPlatformHMove::JumpPlatformHMove(Context* context) : JumpPlatform(context)
    {
        RegisterObject(JumpPlatformHMove);

        setProperties(-100,100,5);
    }

    /**
    @brief
        Destructor.
    */
    JumpPlatformHMove::~JumpPlatformHMove()
    {

    }

    //xml port for loading sounds
    void JumpPlatformHMove::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpPlatformHMove, XMLPort, xmlelement, mode);
    }

    /**
    @brief
        Is called every tick.
        Handles the movement of the ball and its interaction with the boundaries and bats.
    @param dt
        The time since the last tick.
    */
    void JumpPlatformHMove::tick(float dt)
    {
        SUPER(JumpPlatformHMove, tick, dt);

        // Get the current position, velocity and acceleration of the ball.
        Vector3 position = this->getPosition();
        Vector3 velocity = this->getVelocity();

        if ((position.x < leftBoundary_ && velocity.x < 0) || (position.x > rightBoundary_ && velocity.x > 0))

        {
        	//orxout() << "refelected platformHMove at " << position.x << endl;
        	velocity.x = -velocity.x;
        }

        // Set the position, velocity and acceleration of the ball, if they have changed.
        if (velocity != this->getVelocity())
            this->setVelocity(velocity);
        if (position != this->getPosition())
            this->setPosition(position);
    }

    void JumpPlatformHMove::setProperties(float leftBoundary, float rightBoundary, float speed)
    {
    	leftBoundary_ = leftBoundary;
    	rightBoundary_ = rightBoundary;

        this->setVelocity(Vector3(speed,0,0));

    }

    void JumpPlatformHMove::touchFigure()
    {
    	accelerateFigure();
    }
}
