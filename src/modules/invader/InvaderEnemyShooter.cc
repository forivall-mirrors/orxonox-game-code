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
    @file InvaderEnemyShooter.h
    @brief Declaration of the InvaderEnemyShooter class.
*/

#include "invader/InvaderPrereqs.h"
#include "InvaderEnemyShooter.h"
// #include "worldentities/pawns/SpaceShip.h"

namespace orxonox
{
    RegisterClass(InvaderEnemyShooter);

    InvaderEnemyShooter::InvaderEnemyShooter(Context* context) : InvaderEnemy(context)
    {
        RegisterObject(InvaderEnemyShooter);
        enableCollisionCallback();
        lifetime = 0;
        // shoot every second
        shootTimer.setTimer(1.0f, true, createExecutor(createFunctor(&InvaderEnemyShooter::shoot, this)));
    }

    void InvaderEnemyShooter::tick(float dt)
    {
        lifetime += dt;
        // die after 20 seconds.
        if (lifetime > 20000)
            removeHealth(2000);

        if (player != NULL)
        {
            float distPlayer = player->getPosition().z - getPosition().z;
            // orxout() << "i'm different!" << endl;
            float newZ = 2/(pow(abs(getPosition().x - player->getPosition().x) * 0.01, 2) + 1) * distPlayer;
            setVelocity(Vector3(950 - abs(distPlayer), 0, newZ));
        }
        Pawn::tick(dt);
    }

    void InvaderEnemyShooter::shoot()
    {
        ControllableEntity::fire(0);
    }

    void InvaderEnemyShooter::damage(float damage, float healthdamage, float shielddamage, Pawn* originator)
    {
        Pawn::damage(damage, healthdamage, shielddamage, originator);
        if (getGame() && orxonox_cast<InvaderShip*>(originator) != NULL && getHealth() <= 0)
            getGame()->addPoints(3*42);
    }
}