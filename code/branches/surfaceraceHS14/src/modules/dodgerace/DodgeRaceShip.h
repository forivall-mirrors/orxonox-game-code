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
    @file DodgeRaceShip.h
    @brief Declaration of the DodgeRaceShip class.
*/

#ifndef _DodgeRaceShip_H__
#define _DodgeRaceShip_H__


#include "dodgerace/DodgeRacePrereqs.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "worldentities/pawns/SpaceShip.h"
#include "graphics/Camera.h"
#include "weapons/projectiles/Projectile.h"

#include "DodgeRace.h" // Is necessary for getGame function
//#include "DodgeRaceCenterPoint.h"

namespace orxonox
{
    class _DodgeRaceExport DodgeRaceShip : public SpaceShip
    {
        public:
            DodgeRaceShip(Context* context);

            virtual void tick(float dt);

            // overwrite for 2d movement
            virtual void moveFrontBack(const Vector2& value);
            virtual void moveRightLeft(const Vector2& value);

            // Starts or stops fireing
            virtual void boost(bool bBoost);

            //no rotation!
            virtual void rotateYaw(const Vector2& value){};
            virtual void rotatePitch(const Vector2& value){};

            //return to main menu if game has ended.
            virtual void rotateRoll(const Vector2& value){if (getGame()) if (getGame()->bEndGame) getGame()->end();};

            virtual void updateLevel();

            float speed, damping, posforeward;
            bool isFireing;

        protected:
            virtual void death();

        private:
            virtual inline bool collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint);
            WeakPtr<DodgeRace> getGame();
            WeakPtr<DodgeRace> game;
            WeakPtr<Projectile> lastShot;
            WeakPtr<WorldEntity> lastEntity;
            Camera* camera;
            float lastTimeFront, lastTimeLeft, lastTime;
            struct Velocity
            {
                float x;
                float y;
            } velocity, desiredVelocity;

    };
}

#endif /* _DodgeRaceShip_H__ */
