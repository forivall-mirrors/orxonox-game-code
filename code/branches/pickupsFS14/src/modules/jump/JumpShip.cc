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
    @file JumpShip.cc
    @brief Implementation of the JumpShip class.
*/

#include "JumpShip.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "Jump.h"

namespace orxonox
{
    RegisterClass(JumpShip);

    JumpShip::JumpShip(Context* context) : SpaceShip(context)
    {
        RegisterObject(JumpShip);

        //speed = 500;
        //isFireing = false;
        //damping = 10;
        left = false;
        right = false;

    }

    void JumpShip::tick(float dt)
    {

        Vector3 pos = getPosition();

        /*
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
            {
                velocity.y = 0;
                // restart if game ended
                if (getGame())
                    if (getGame()->bEndGame)
                    {
                        getGame()->start();
                        return;
                    }
            }
            if (pos.z + dist_x > 42*2.5 || pos.z + dist_x < -42*3)
                velocity.x = 0;
            pos += Vector3(1000 + velocity.y, 0, velocity.x) * dt;

        }

        // shoot!
        if (isFireing)
            ControllableEntity::fire(0);

        // Camera
        WeakPtr<Camera> camera = this->getCamera();
        if (camera != NULL)
        {
            camera->setPosition(Vector3(-pos.z, -posforeward, 0));
            camera->setOrientation(Vector3::UNIT_Z, Degree(90));
        }



        // bring back on track!
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
        */

    	if (left == true)
    	{
    		pos += Vector3(100 + pos.y, 0, 0) * dt;
    		left = false;
    	}
    	else if (right == true)
    	{
    		right = false;
    	}

    	setPosition(pos);


        SUPER(JumpShip, tick, dt);
    }
/*
    void JumpShip::updateLevel()
    {
        lastTime = 0;
        if (getGame())
            getGame()->levelUp();
    }*/

    void JumpShip::moveFrontBack(const Vector2& value)
    {
        //lastTimeLeft = 0;
        //desiredVelocity.x = -value.x * speed;
    }

    void JumpShip::moveRightLeft(const Vector2& value)
    {
    	if (value.y < 0)
    	{
    		left = true;
    	}
    	else if (value.y > 0)
    	{
    		right = true;
    	}
        //lastTimeFront = 0;
        //desiredVelocity.y = value.y * speed * 42;
    }

    /*
    void JumpShip::boost(bool bBoost)
    {
        isFireing = bBoost;
    }
    inline bool JumpShip::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        // orxout() << "touch!!! " << endl; //<< otherObject << " at " << contactPoint;
        WeakPtr<JumpEnemy> enemy = orxonox_cast<JumpEnemy*>(otherObject);
        WeakPtr<Projectile> shot = orxonox_cast<Projectile*>(otherObject);
        // ensure that this gets only called once per enemy.
        if (enemy != NULL && lastEnemy != enemy)
        {
            lastEnemy = enemy;

            removeHealth(20);
            if (getGame())
            {
                getGame()->multiplier = 1;                   
            }
        }
        // was shot, decrease multiplier
        else if (shot != NULL  && lastShot != shot)
        {
            if (getGame() && orxonox_cast<JumpEnemy*>(shot->getShooter()) != NULL)
            {
                if (getGame()->multiplier > 1)
                {
                    lastShot = shot;
                    getGame()->multiplier -= 1;     
                }
            }
        }
        return false;
        // SUPER(JumpShip, collidesAgainst, otherObject, contactPoint);
    }

    void JumpShip::death()
    {
        getGame()->costLife();
        SpaceShip::death();
    }
*/
    WeakPtr<Jump> JumpShip::getGame()
    {
        if (game == NULL)
        {
            for (ObjectList<Jump>::iterator it = ObjectList<Jump>::begin(); it != ObjectList<Jump>::end(); ++it)
            {
                game = *it;
            }
        }
        return game;
    }

}
