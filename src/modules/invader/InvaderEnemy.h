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
    @file InvaderEnemy.h
    @brief Declaration of the InvaderEnemy class.
*/

#ifndef _InvaderEnemy_H__
#define _InvaderEnemy_H__

#include "invader/InvaderPrereqs.h"

#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    class _InvaderExport InvaderEnemy : public Pawn
    {
        public:
            InvaderEnemy(Context* context);

            virtual void tick(float dt);
            virtual bool collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint);
            virtual void damage(float damage, float healthdamage, float shielddamage, Pawn* originator);
            virtual void setPlayer(WeakPtr<InvaderShip> player){this->player = player;}

            int level;
        protected:
            WeakPtr<Invader> getGame();
            WeakPtr<Invader> game;
            WeakPtr<InvaderShip> player;
            Camera* camera;
            bool isFireing;
            float speed, damping;
            float lastTimeFront, lastTimeLeft;
            float lifetime;
            struct Velocity
            {
                float x;
                float y;
            } velocity, desiredVelocity;

    };
}

#endif /* _InvaderEnemy_H__ */
