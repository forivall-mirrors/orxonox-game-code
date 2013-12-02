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

#include "invader/InvaderPrereqs.h"
#include "InvaderEnemy.h"
// #include "worldentities/pawns/SpaceShip.h"

namespace orxonox
{
    RegisterClass(InvaderEnemy);

    InvaderEnemy::InvaderEnemy(Context* context) : Pawn(context)
    {
        RegisterObject(InvaderEnemy);
        enableCollisionCallback();
        lifetime = 0;
    }

    void InvaderEnemy::tick(float dt)
    {
        lifetime += dt;
        if (lifetime > 5000)
        {
            removeHealth(2000);
        }
        if (player != NULL)
        {
            float newZ = 2/(pow(abs(getPosition().x - player->getPosition().x) * 0.01, 2) + 1) * (player->getPosition().z - getPosition().z);
            // if (newZ < 0)
            //     newZ = (-100 < newZ)?-100:newZ;
            // else
            //     newZ = (100 > newZ)?100:newZ;
            setVelocity(Vector3(1000 - level * 100 , 0, newZ));
        }
        SUPER(InvaderEnemy, tick, dt);
    }

    inline bool InvaderEnemy::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        // setVelocity(Vector3(1500,0,0));
        removeHealth(2000);
        return false;
    }

    WeakPtr<Invader> InvaderEnemy::getGame()
    {
        if (game == NULL)
        {
            for (ObjectList<Invader>::iterator it = ObjectList<Invader>::begin(); it != ObjectList<Invader>::end(); ++it)
                game = *it;
        }
        return game;
    }

    void InvaderEnemy::damage(float damage, float healthdamage, float shielddamage, Pawn* originator)
    {
        if (getGame())
            getGame()->addPoints(42);
        // SUPER(InvaderEnemy, damage, damage, healthdamage, shielddamage, originator);
    }
}