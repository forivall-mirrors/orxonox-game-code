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
    @file JumpPlatformVMove.cc
    @brief This platform can move vertically.
*/

#include "JumpPlatformVMove.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "gametypes/Gametype.h"
#include "JumpFigure.h"
#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpPlatformVMove);

    JumpPlatformVMove::JumpPlatformVMove(Context* context) : JumpPlatform(context)
    {
        RegisterObject(JumpPlatformVMove);

        setProperties(0,80,10);
    }

    JumpPlatformVMove::~JumpPlatformVMove()
    {

    }

    void JumpPlatformVMove::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpPlatformVMove, XMLPort, xmlelement, mode);
    }

    void JumpPlatformVMove::tick(float dt)
    {
        SUPER(JumpPlatformVMove, tick, dt);

        // Get the current position, velocity and acceleration of the ball.
        Vector3 position = getPosition();
        Vector3 velocity = getVelocity();

        if ((position.z < lowerBoundary_ && velocity.z < 0) || (position.z > upperBoundary_ && velocity.z > 0))
        {
        	velocity.z = -velocity.z;
        }

        // Set the position, velocity and acceleration of the ball, if they have changed.
        if (velocity != getVelocity())
        {
            setVelocity(velocity);
        }
        if (position != getPosition())
        {
            setPosition(position);
        }
    }

    void JumpPlatformVMove::setProperties(float lowerBoundary, float upperBoundary, float speed)
    {
    	lowerBoundary_ = lowerBoundary;
    	upperBoundary_ = upperBoundary;

        setVelocity(Vector3(0,0,speed));

    }

    void JumpPlatformVMove::touchFigure()
    {
    	figure_->JumpFromPlatform(this);
    }
}
