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
    @file JumpShip.h
    @brief Declaration of the JumpShip class.
*/

#ifndef _JumpShip_H__
#define _JumpShip_H__

#include "jump/JumpPrereqs.h"

#include "worldentities/pawns/SpaceShip.h"
#include "graphics/Camera.h"
#include "weapons/projectiles/Projectile.h"
#include "JumpPlatform.h"

namespace orxonox
{
    class _JumpExport JumpShip : public SpaceShip
    {
        public:
            JumpShip(Context* context);

            virtual void tick(float dt);

            // overwrite for 2d movement
            virtual void moveFrontBack(const Vector2& value);
            virtual void moveRightLeft(const Vector2& value);
/*
            // Starts or stops fireing
            virtual void boost(bool bBoost);
*/
            //no rotation!
            virtual void rotateYaw(const Vector2& value){}; 
            virtual void rotatePitch(const Vector2& value){};
            //return to main menu if game has ended.
            virtual void rotateRoll(const Vector2& value){};

            /*virtual void updateLevel();

            virtual inline bool collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint);

        protected:
            virtual void death();*/
        private:
            WeakPtr<Jump> getGame();
            WeakPtr<Jump> game;
            const float xVelocity = 150.0f;
            const float xBoundary = 200.0f;


            //bool isFireing;
            //float speed;
            //float damping
            //float posforeward;
            //float lastTimeFront;
            //float lastTimeLeft;
            //float lastTime;
            //WeakPtr<JumpEnemy> lastEnemy;
            //WeakPtr<Projectile> lastShot;

            /*
            struct Velocity
            {
                float x;
                float y;
            } velocity, desiredVelocity;*/

            bool leftPressed;
            bool rightPressed;
            bool upPressed;
            bool downPressed;

            float yVelocity;
            const float yAcceleration = 10.0f;
            const float ySpeedAfterJump = 300.0f;
    };
}

#endif /* _JumpShip_H__ */
