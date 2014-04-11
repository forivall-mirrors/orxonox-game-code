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
        leftPressed = false;
        rightPressed = false;
        upPressed = false;
        downPressed = false;
        yVelocity = 0;
    }

    void JumpShip::tick(float dt)
    {


    	Vector3 movement(0,0,0);
        Vector3 shipPosition = getPosition();

    	// Berechne Bewegung anhand der Eingabe
    	if (leftPressed == true)
    	{
    		movement -= Vector3(xVelocity, 0, 0);
    		leftPressed = false;
    	}
    	else if (rightPressed == true)
    	{
    		movement += Vector3(xVelocity, 0, 0);
    		rightPressed = false;
    	}

    	if (upPressed == true)
    	{
    		//movement += Vector3(0, xVelocity, 0);
    		yVelocity = ySpeedAfterJump;
    		upPressed = false;
    	}
    	else if (downPressed == true)
    	{
    		movement -= Vector3(0, 0, 0);
    		downPressed = false;
    	}

    	movement += Vector3(0, yVelocity, 0);
    	yVelocity -= yAcceleration;

    	// Skalierung der Bewegung je nach vergangener Zeit
    	movement *= dt;

    	// Verschiebe das Schiff um den berechneten Vektor movement und verhindere Verlassen des Bildschrims
    	shipPosition.x = clamp(shipPosition.x + movement.x, -xBoundary, xBoundary);
		shipPosition.y += movement.y;

    	setPosition(shipPosition);

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
    	if (value.y < 0)
    	{
    		downPressed = true;
    	}
    	else if (value.y > 0)
    	{
    		upPressed = true;
    	}
        //lastTimeLeft = 0;
        //desiredVelocity.x = -value.x * speed;
    }

    void JumpShip::moveRightLeft(const Vector2& value)
    {
    	if (value.x < 0)
    	{
    		leftPressed = true;
    	}
    	else if (value.x > 0)
    	{
    		rightPressed = true;
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
