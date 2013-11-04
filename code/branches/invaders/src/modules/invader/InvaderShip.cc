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
 *      Florian Zinggeler
 *   Co-authors:
 *      ...
 *
 */

/**
    @file InvaderShip.cc
    @brief Implementation of the InvaderShip class.
*/

#include "InvaderShip.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
    RegisterClass(InvaderShip);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    InvaderShip::InvaderShip(Context* context) : SpaceShip(context)
    {
        RegisterObject(InvaderShip);

        speed = 500;
        isFireing = false;
        damping = 10;

    }

    void InvaderShip::tick(float dt)
    {
        // if (camera == NULL)
        //     camera = this->getCamera();
        // if (camera != NULL)
        //     camera->setPosition(Vector3(0, 0, 0) + this->getWorldPosition());

        if (this->hasLocalController())
        {
            this->setVelocity(Vector3(1000 + velocity.y, 0, velocity.x)); //
            //this->setVelocity(this->getOrientation() * Vector3(1, 0, 0));
        }
        lastTimeFront += dt * damping;
        lastTimeLeft += dt * damping;
        velocity.x = interpolate(clamp(lastTimeLeft, 0.0f, 1.0f), desiredVelocity.x, 0.0f);
        velocity.y = interpolate(clamp(lastTimeFront, 0.0f, 1.0f), desiredVelocity.y, 0.0f);

        if (isFireing)
            ControllableEntity::fire(0);


        // camera->setOrientation(Vector3::UNIT_X, Degree(0));


        SUPER(InvaderShip, tick, dt);
    }

    void InvaderShip::moveFrontBack(const Vector2& value)
    {
        // orxout(internal_error) << "move backfront" << value.x << value.y << endl;
        //velocity.y = value.y * speed * 10;
        lastTimeFront = 0;
        desiredVelocity.y = value.y * speed * 10;
    }

    void InvaderShip::moveRightLeft(const Vector2& value)
    {
        // orxout(internal_error) << "right left front" << value.x << value.y << endl;
        lastTimeLeft = 0;
        desiredVelocity.x = value.x * speed;
        // velocity.x = value.x * speed;
    }
    void InvaderShip::boost(bool bBoost)
    {
        isFireing = bBoost;
    }
}
