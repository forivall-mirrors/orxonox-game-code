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
 *      Johannes Ritz
 *   Co-authors:
 *      ...
 *
 */

#include "Dynamicmatch.h"

#include "core/CoreIncludes.h"
#include "network/Host.h"
#include "infos/PlayerInfo.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateUnloadableFactory(Dynamicmatch);

    Dynamicmatch::Dynamicmatch(BaseObject* creator) : Gametype(creator)
    {
        RegisterObject(Dynamicmatch);
	   this->scoreTimer_.setTimer(10, true, createExecutor(createFunctor(&Dynamicmatch::winPoints, this)));//bad
           this->outputTimer_.setTimer(10, true, createExecutor(createFunctor(&Dynamicmatch::showPoints, this)));//bad
	   for (int player=0; player<50; player++) { this->pointsScored[player]=0;}//
        
    }

bool Gametype::allowPawnDamage(Pawn* victim, Pawn* originator)//
    {
        return false;
    }

bool Gametype::allowPawnDeath(Pawn* victim, Pawn* originator)//
    {
        return false;
    }

void Dynamicmatch::winPoints() // Points scored for each player
    {
	/* Points: for each hit of the victim - is it possible to destinguish different weapontypes (1pt per hit)
		the victim scores points during the duration of the victim state (1p per second)
	*/



        /*int amountControlled = 0;
        int amountControlled2 = 0;

        for (std::set<TeamBaseMatchBase*>::const_iterator it = this->bases_.begin(); it != this->bases_.end(); ++it)
        {
            if((*it)->getState() == BaseState::ControlTeam1)
            {
                amountControlled++;
            }
            if((*it)->getState() == BaseState::ControlTeam2)
            {
                amountControlled2++;
            }
        }

        this->addTeamPoints(0, (amountControlled * 30));
        this->addTeamPoints(1, (amountControlled2 * 30));
	*/
    }



    void Dynamicmatch::start()
    {
        Gametype::start();

        std::string message("The match has started!");
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
    }

    void Dynamicmatch::end()
    {
        Gametype::end();

        std::string message("The match has ended.");
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
    }

    void Dynamicmatch::playerEntered(PlayerInfo* player)
    {
        Gametype::playerEntered(player);

        const std::string& message = player->getName() + " entered the game";
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
    }

    bool Dynamicmatch::playerLeft(PlayerInfo* player)
    {
        bool valid_player = Gametype::playerLeft(player);

        if (valid_player)
        {
            const std::string& message = player->getName() + " left the game";
            COUT(0) << message << std::endl;
            Host::Broadcast(message);
        }

        return valid_player;
    }

    bool Dynamicmatch::playerChangedName(PlayerInfo* player)
    {
        bool valid_player = Gametype::playerChangedName(player);

        if (valid_player)
        {
            const std::string& message = player->getOldName() + " changed name to " + player->getName();
            COUT(0) << message << std::endl;
            Host::Broadcast(message);
        }

        return valid_player;
    }

    void Dynamicmatch::pawnKilled(Pawn* victim, Pawn* killer)
    {
        if (victim && victim->getPlayer())
        {
            std::string message;
            if (killer)
            {
                if (killer->getPlayer())
                    message = victim->getPlayer()->getName() + " was killed by " + killer->getPlayer()->getName();
                else
                    message = victim->getPlayer()->getName() + " was killed";
            }
            else
                message = victim->getPlayer()->getName() + " died";

            COUT(0) << message << std::endl;
            Host::Broadcast(message);
        }

        Gametype::pawnKilled(victim, killer);
    }

    void Dynamicmatch::playerScored(PlayerInfo* player)
    {
        Gametype::playerScored(player);

        if (player)
        {
            const std::string& message = player->getName() + " scores!";
            COUT(0) << message << std::endl;
            Host::Broadcast(message);
        }
    }
}
