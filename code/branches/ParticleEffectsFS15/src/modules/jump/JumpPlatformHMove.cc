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
 *      Fabien Vultier
 *   Co-authors:
 *      ...
 *
 */

/**
    @file JumpPlatformHMove.cc
    @brief This platform can move horizontally.
*/

#include "JumpPlatformHMove.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "gametypes/Gametype.h"
#include "JumpFigure.h"
#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpPlatformHMove);

    JumpPlatformHMove::JumpPlatformHMove(Context* context) : JumpPlatform(context)
    {
        RegisterObject(JumpPlatformHMove);

        setProperties(-100,100,5);
    }

    JumpPlatformHMove::~JumpPlatformHMove()
    {

    }

    void JumpPlatformHMove::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpPlatformHMove, XMLPort, xmlelement, mode);
    }

    void JumpPlatformHMove::tick(float dt)
    {
        SUPER(JumpPlatformHMove, tick, dt);

        // Get the current position, velocity and acceleration of the ball.
        Vector3 position = getPosition();
        Vector3 velocity = getVelocity();

        if ((position.x < leftBoundary_ && velocity.x < 0) || (position.x > rightBoundary_ && velocity.x > 0))

        {
            velocity.x = -velocity.x;
        }

        // Set the position, velocity and acceleration of the ball, if they have changed.
        if (velocity != getVelocity())
        {
            setVelocity(velocity);
        }
        if (position != this->getPosition())
        {
            setPosition(position);
        }
    }

    void JumpPlatformHMove::setProperties(float leftBoundary, float rightBoundary, float speed)
    {
        leftBoundary_ = leftBoundary;
        rightBoundary_ = rightBoundary;
        setVelocity(Vector3(speed,0,0));
    }

    void JumpPlatformHMove::touchFigure()
    {
        figure_->JumpFromPlatform(this);
    }
}
