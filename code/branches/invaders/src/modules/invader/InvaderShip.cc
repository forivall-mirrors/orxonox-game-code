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
        Vector3 pos = getPosition();

        //Movement calculation
        lastTimeFront += dt * damping;
        lastTimeLeft += dt * damping;
        lastTime += dt;

        velocity.x = interpolate(clamp(lastTimeLeft, 0.0f, 1.0f), desiredVelocity.x, 0.0f);
        velocity.y = interpolate(clamp(lastTimeFront, 0.0f, 1.0f), desiredVelocity.y, 0.0f);

        //Execute movement
        if (this->hasLocalController())
        {
            float dist_y = velocity.y * dt;
            float dist_x = velocity.x * dt;
            if(dist_y + posforeward > -42*3 && dist_y + posforeward < 42*6)
                posforeward += dist_y;
            else
                velocity.y = 0;
            if (pos.z + dist_x > 42*2.5 || pos.z + dist_x < -42*3)
                velocity.x = 0;
            // this->setVelocity(Vector3(1000 + velocity.y, 0, velocity.x));
            pos += Vector3(1000 + velocity.y, 0, velocity.x) * dt;
        }

        if (isFireing)
            ControllableEntity::fire(0);

        // Camera
        Camera* camera = this->getCamera();
        if (camera != NULL)
        {
            camera->setPosition(Vector3(-pos.z, -posforeward, 0));
            camera->setOrientation(Vector3::UNIT_Z, Degree(90));
            // orxout() << "asbhajskjasjahg" << pos << endl;
        }



        // bring back on track!
        // if (pos.z > 42*2.5)
        //     pos.z = 42*2.5;
        // else if (pos.z < -42*3)
        //     pos.z = -42*3;
        if(pos.y != 0)
            pos.y = 0;
            

        setPosition(pos);
        setOrientation(Vector3::UNIT_Y, Degree(270));

        // Level up!
        if (pos.x > 42000)
        {
            updateLevel();
            setPosition(Vector3(0, 0, pos.z)); // pos - Vector3(30000, 0, 0)
        }

        SUPER(InvaderShip, tick, dt);
    }

    void InvaderShip::updateLevel()
    {
        lastTime = 0;
        //level++
    }

    void InvaderShip::moveFrontBack(const Vector2& value)
    {
        lastTimeLeft = 0;
        desiredVelocity.x = -value.x * speed;
    }

    void InvaderShip::moveRightLeft(const Vector2& value)
    {
        lastTimeFront = 0;
        desiredVelocity.y = value.y * speed * 42;
    }
    void InvaderShip::boost(bool bBoost)
    {
        isFireing = bBoost;
    }
    inline bool InvaderShip::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        // orxout() << "touch!!! " << endl; //<< otherObject << " at " << contactPoint;

        WeakPtr<InvaderEnemy> enemy = orxonox_cast<InvaderEnemy*>(otherObject);
        if (enemy != NULL && lastEnemy != enemy)
        {
            lastEnemy = enemy;
            orxout() << "Enemy!!!! " << endl;
            removeHealth(20);
            if (getHealth() <= 0)
            {
                orxout() << "DIED!!!! " << endl;
            }
            return false;
        }
        return false;
        // SUPER(InvaderShip, collidesAgainst, otherObject, contactPoint);
    }
}
