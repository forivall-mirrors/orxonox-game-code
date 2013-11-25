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
    }

    void InvaderEnemy::tick(float dt)
    {
        SUPER(InvaderEnemy, tick, dt);
    }

    inline bool InvaderEnemy::collidesAgainst(WorldEntity* otherObject, btManifoldPoint& contactPoint)
    {
        orxout() << "sadsadasfasf" << endl;
        removeHealth(2000);
        setVelocity(Vector3(0,0,1000));
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