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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "UnderAttack.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "objects/Teamcolourable.h"
#include "objects/worldentities/TeamSpawnPoint.h"

namespace orxonox
{
    CreateUnloadableFactory(UnderAttack);

    UnderAttack::UnderAttack(BaseObject* creator) : TeamDeathmatch(creator)
    {
        RegisterObject(UnderAttack);
        this->gameTime_ = 30;
        this->teams_ = 2;
        this->destroyer_ = 0;
        this->gameEnded_ = false;

        this->setConfigValues();
    }

    void UnderAttack::addDestroyer(Destroyer* destroyer)
    {
        this->destroyer_ = destroyer;
    }


    void UnderAttack::destroyedPawn(Pawn* pawn)
    {
        if (pawn == this->destroyer_)
        {

            COUT(0) << "Ship destroyed! Team 0 has won!" << std::endl;
            // Todo: end game
        }
    }

    bool UnderAttack::allowPawnHit(Pawn* victim, Pawn* originator)
    {
        if (victim == this->destroyer_)
        {
            if (originator && originator->getPlayer() && !gameEnded_)
            {
                if (this->getTeam(originator->getPlayer()) == 0)
                    return true;
                else
                    return false;
            }

            return false;
        }

        return TeamDeathmatch::allowPawnHit(victim, originator);
    }

    bool UnderAttack::allowPawnDamage(Pawn* victim, Pawn* originator)
    {
        if (victim == this->destroyer_)
        {
            if (originator && originator->getPlayer() && !gameEnded_)
            {
                if (this->getTeam(originator->getPlayer()) == 0)
                    return true;
                else
                    return false;
            }

            return false;
        }

        return TeamDeathmatch::allowPawnDamage(victim, originator);
    }

    bool UnderAttack::allowPawnDeath(Pawn* victim, Pawn* originator)
    {
        if (victim == this->destroyer_)
        {
            if (originator && originator->getPlayer() && !gameEnded_)
            {
                if (this->getTeam(originator->getPlayer()) == 0)
                    return true;
                else
                    return false;
            }

            return false;
        }

        return TeamDeathmatch::allowPawnDeath(victim, originator);
    }
    void UnderAttack::tick(float dt)
    {
        SUPER(UnderAttack, tick, dt);

        if (this->hasStarted())
        {
            gameTime_ = gameTime_ - dt;
            if (gameTime_<= 0 && !gameEnded_)
            {
                gameEnded_ = true;
                COUT(0) << "Time is up! Team 1 has won!" << std::endl;
            }
        }
    }

}
