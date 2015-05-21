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
    @file JumpItem.cc
    @brief All items in this minigame inherit from this class. Items can move around like platforms and enemies.
*/

#include "JumpItem.h"

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "graphics/Model.h"
#include "gametypes/Gametype.h"

#include "JumpFigure.h"

#include "sound/WorldSound.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(JumpItem);

    JumpItem::JumpItem(Context* context) : MovableEntity(context)
    {
        RegisterObject(JumpItem);

        attachedToFigure_ = false;

        figure_ = 0;
        height_ = 0.0;
        width_ = 0.0;
        setPosition(Vector3(0,0,0));
        setVelocity(Vector3(0,0,0));
        setAcceleration(Vector3(0,0,0));
    }

    JumpItem::~JumpItem()
    {

    }

    void JumpItem::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(JumpItem, XMLPort, xmlelement, mode);

        XMLPortParam(JumpItem, "height", setHeight, getHeight, xmlelement, mode);
        XMLPortParam(JumpItem, "width", setWidth, getWidth, xmlelement, mode);
    }

    void JumpItem::tick(float dt)
    {
        SUPER(JumpItem, tick, dt);
        // Get the current position, velocity and acceleration of the item.
        Vector3 position = getWorldPosition();
        Vector3 velocity = getVelocity();

        if ((position.x < leftBoundary_ && velocity.x < 0) || (position.x > rightBoundary_ && velocity.x > 0))
        {
            velocity.x = -velocity.x;
        }

        if ((position.z < lowerBoundary_ && velocity.z < 0) || (position.z > upperBoundary_ && velocity.z > 0))
        {
            velocity.z = -velocity.z;
        }

        // Set the position, velocity and acceleration of the item, if they have changed.
        if (velocity != getVelocity())
        {
            setVelocity(velocity);
        }
    }

    void JumpItem::setProperties(float newLeftBoundary, float newRightBoundary, float newLowerBoundary, float newUpperBoundary, float newHSpeed, float newVSpeed)
    {
        leftBoundary_ = newLeftBoundary;
        rightBoundary_ = newRightBoundary;
        lowerBoundary_ = newLowerBoundary;
        upperBoundary_ = newUpperBoundary;

        setVelocity(Vector3(newHSpeed,0,newVSpeed));
    }

    void JumpItem::setFigure(WeakPtr<JumpFigure> newFigure)
    {
        figure_ = newFigure;
    }

    void JumpItem::touchFigure()
    {

    }
}
