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
 *      Matthias Mock
 *   Co-authors:
 *      ...
 *
 */

#include "UnderAttack.h"

#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "core/config/ConfigValueIncludes.h"
#include "chat/ChatManager.h"
#include "worldentities/pawns/Destroyer.h"
#include "infos/PlayerInfo.h"

namespace orxonox
{
    const int attacker_ = 0; // teamnumber of the attacking team
    const int defender_ = 1; // defender's teamnumber

    RegisterUnloadableClass(UnderAttack);

    UnderAttack::UnderAttack(Context* context) : TeamDeathmatch(context)
    {
        RegisterObject(UnderAttack);
        this->gameTime_ = 180;
        this->teams_ = 2;
        this->destroyer_ = 0;
        this->destroyer_.setCallback(createFunctor(&UnderAttack::killedDestroyer, this));
        this->gameEnded_ = false;

        this->setConfigValues();
        this->timesequence_ = static_cast<int>(this->gameTime_);
    }

    void UnderAttack::setConfigValues()
    {
        SetConfigValue(gameTime_, 180);
    }

    void UnderAttack::addDestroyer(Destroyer* destroyer)
    {
        this->destroyer_ = destroyer;
    }


    void UnderAttack::killedDestroyer()
    {
        this->end(); //end gametype
        std::string message("Ship destroyed! Team 0 has won!");
        ChatManager::message(message);
        this->gameEnded_ = true;

        for (std::map<PlayerInfo*, int>::iterator it = this->teamnumbers_.begin(); it != this->teamnumbers_.end(); ++it)
        {
            if (it->first->getClientID() == NETWORK_PEER_ID_UNKNOWN)
                continue;

            if (it->second == attacker_)
                this->gtinfo_->sendAnnounceMessage("You have won the match!", it->first->getClientID());
            else
                this->gtinfo_->sendAnnounceMessage("You have lost the match!", it->first->getClientID());
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

        if (this->hasStarted() && !gameEnded_)
        {
            gameTime_ = gameTime_ - dt;
            if (gameTime_<= 0)
            {
                this->gameEnded_ = true;
                this->end();
                std::string message("Time is up! Team 1 has won!");
                ChatManager::message(message);

                for (std::map<PlayerInfo*, int>::iterator it = this->teamnumbers_.begin(); it != this->teamnumbers_.end(); ++it)
                {
                    if (it->first->getClientID() == NETWORK_PEER_ID_UNKNOWN)
                        continue;

                    if (it->second == 1)
                        this->gtinfo_->sendAnnounceMessage("You have won the match!", it->first->getClientID());
                    else
                        this->gtinfo_->sendAnnounceMessage("You have lost the match!", it->first->getClientID());
                }
            }

             //prints gametime
            if ( gameTime_ <= timesequence_ && gameTime_ > 0)
            {
                const std::string& message = multi_cast<std::string>(timesequence_) + " seconds left!";
/*
                ChatManager::message(message);
*/
                this->gtinfo_->sendAnnounceMessage(message);

                if (timesequence_ >= 30 && timesequence_ <= 60)
                {
                    timesequence_ = timesequence_ - 10;
                }
                else if (timesequence_ <= 30)
                {
                    timesequence_ = timesequence_ - 5;
                }
                else
                {
                    timesequence_ = timesequence_ - 30;
                }
            }
        }
    }

    void UnderAttack::playerEntered(PlayerInfo* player)
    {
        if (!player)
            return;
        TeamDeathmatch::playerEntered(player);
        this->setTransporterHealth();
    }

    void UnderAttack::setTransporterHealth()
    {
        if (this->destroyer_ != 0)
        {
            //Calculation: Each attacker deals about 3500 damage. A human attacker deals 1500 damage additionally.
            //Each defender prevents 500 damage. If human 2000 damage will be additionally be prevented.
            //TODO: use gametime and the damage dealt so far in order to calculate the transporter's life more precisely
            float health = this->getTeamSize(attacker_)*2000.0f + this->getHumansInTeam(attacker_)*3000.0f - this->getTeamSize(defender_)*500.0f - this->getHumansInTeam(defender_)*2000.0f ;
            this->destroyer_->setHealth(std::max(health, 5000.0f)); //the destoyer should have at least 5000.0f life. 
        }
    }


}
