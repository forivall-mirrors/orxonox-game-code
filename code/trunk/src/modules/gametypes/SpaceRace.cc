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
 *     Mauro Salomon
 *   Co-authors:
 *      Celine Egger
 *
 */

#include "SpaceRace.h"

#include "items/Engine.h"

#include "core/CoreIncludes.h"
#include "chat/ChatManager.h"
#include "util/Convert.h"
#include "util/Math.h"
#include "SpaceRaceBot.h"
#include "items/Engine.h"

namespace orxonox
{
    RegisterUnloadableClass(SpaceRace);

    SpaceRace::SpaceRace(Context* context) : Gametype(context)
    {
        RegisterObject(SpaceRace);

        this->botclass_ = Class(SpaceRaceBot);//ClassByString("")
        this->cantMove_ = false;
        this->bTimeIsUp_ = false;
        this->numberOfBots_ = 0; // quick fix: don't allow default-bots to enter the race
                                 // remove this line, if a raceBot has been created.
    }

    void SpaceRace::end()
    {
        this->clock_.capture();
        int s = this->clock_.getSeconds();
        int ms = static_cast<int>(this->clock_.getMilliseconds() - 1000*s);
        std::string message;

        if (this->bTimeIsUp_)
        {
            message = multi_cast<std::string>(s) + "." + multi_cast<std::string>(ms) + " seconds !!\n"
                        + "You didn't reach the check point  before the time limit. You lose!";
        }
        else
        {
            message = "You win!! You have reached the last check point after "+ multi_cast<std::string>(s)
                        + "." + multi_cast<std::string>(ms) + " seconds.";
        }
        if (!this->hasEnded())
        {
            this->getGametypeInfo()->sendAnnounceMessage(message);
            ChatManager::message(message);
        }
        this->Gametype::end();
    }

    void SpaceRace::tick(float dt)
    {
        SUPER(SpaceRace,tick,dt);

        // spawn the players already when the countdown starts, but deactivate their engines
        if (this->isStartCountdownRunning() && !this->cantMove_)
        {
            this->spawnPlayersIfRequested();
            this->cantMove_ = true;

            for (ObjectList<Engine>::iterator it = ObjectList<Engine>::begin(); it; ++it)
                it->setActive(false);
        }

        // activate the engines again if the countdown ends
        if (!this->isStartCountdownRunning() && this->cantMove_)
        {
            for (ObjectList<Engine>::iterator it = ObjectList<Engine>::begin(); it; ++it)
                it->setActive(true);

            this->cantMove_= false;

            std::string message = "The match has started! Reach the check points as quickly as possible!";
            this->getGametypeInfo()->sendAnnounceMessage(message);
            ChatManager::message(message);
        }
    }

    void SpaceRace::newCheckpointReached(RaceCheckPoint* checkpoint, PlayerInfo* player)
    {
        this->checkpointReached_[player] = checkpoint;

        this->clock_.capture();
        int s = this->clock_.getSeconds();
        int ms = this->clock_.getMilliseconds() % 1000;

        const std::string& message = "Checkpoint " + multi_cast<std::string>(checkpoint->getCheckpointIndex() + 1)
            + " reached after " + multi_cast<std::string>(s) + "." + multi_cast<std::string>(ms) + " seconds.";
        this->getGametypeInfo()->sendAnnounceMessage(message);
        ChatManager::message(message);
    }

    bool SpaceRace::allowPawnHit(Pawn* victim, Pawn* originator)
    {
        return false;
    }

    bool SpaceRace::allowPawnDamage(Pawn* victim, Pawn* originator)
    {
        return false;
    }

    bool SpaceRace::allowPawnDeath(Pawn* victim, Pawn* originator)
    {
        return false;
    }
}
