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
 *      Aurelian Jaggi
 *   Co-authors:
 *      ...
 *
 */

#include "Asteroids.h"

#include "core/CoreIncludes.h"
#include "chat/ChatManager.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateUnloadableFactory(Asteroids);

    Asteroids::Asteroids(BaseObject* creator) : Gametype(creator)
    {
        RegisterObject(Asteroids);
        this->firstCheckpointReached_ = false;
        this->numberOfBots_=0; //sets number of default bots temporarly to 0
    }

    void Asteroids::tick(float dt)
    {
        SUPER(Asteroids, tick, dt);

        if (firstCheckpointReached_ && !this->timerIsActive_)
        {
            this->startTimer();
        }

        if (this->time_ < 0 && !this->hasEnded() && this->timerIsActive_)
        {
            this->gtinfo_->sendAnnounceMessage("Time's up - you have lost the match!");
            this->end();
        }
    }

    void Asteroids::pawnKilled(Pawn* victim, Pawn* killer)
    {
        if (victim && victim->getPlayer())
        {
            this->gtinfo_->sendAnnounceMessage("You're dead - you have lost the match!");
            this->end();
        }
    }

    void Asteroids::start()
    {
        Gametype::start();

        std::string message("The match has started! Reach the first chekpoint within 15 seconds! But be aware, there may be pirates around...");
        ChatManager::message(message);

    }

    void Asteroids::end()
    {
        Gametype::end();

        std::string message("The match has ended.");
        ChatManager::message(message);
    }
}
