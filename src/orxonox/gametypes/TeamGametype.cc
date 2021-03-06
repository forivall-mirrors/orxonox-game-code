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

#include "TeamGametype.h"

#include "core/CoreIncludes.h"
#include "core/config/ConfigValueIncludes.h"
#include "infos/PlayerInfo.h"
#include "interfaces/TeamColourable.h"
#include "worldentities/TeamSpawnPoint.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/ControllableEntity.h"
#include "controllers/ArtificialController.h"

namespace orxonox
{
    RegisterUnloadableClass(TeamGametype);

    TeamGametype::TeamGametype(Context* context) : Gametype(context)
    {
        RegisterObject(TeamGametype);

        this->teams_ = 2; //most team gametypes use two different teams
        this->allowFriendlyFire_ = false;
        //this->playersPerTeam_ = 0;
        this->maxPlayers_ = 0; //Value "0": no limit is set.
        this->setConfigValues();
    }

    void TeamGametype::setConfigValues()
    {
        SetConfigValue(teams_, 2);

        static ColourValue colours[] =
        {
            ColourValue(0.2f, 0.2f, 1.0f),
            ColourValue(1.0f, 0.1f, 0.1f),
            ColourValue(0.3f, 1.0f, 0.3f),
            ColourValue(1.0f, 1.0f, 0.0f),
            ColourValue(0.0f, 1.0f, 1.0f),
            ColourValue(1.0f, 0.0f, 1.0f),
            ColourValue(7.0f, 7.0f, 7.0f),
            ColourValue(2.0f, 2.0f, 2.0f)
        };
        static std::vector<ColourValue> defaultcolours(colours, colours + sizeof(colours) / sizeof(ColourValue));

        SetConfigValue(teamcolours_, defaultcolours);
    }

    void TeamGametype::playerEntered(PlayerInfo* player)
    {
        if(player == NULL) return; // catch null pointers
        Gametype::playerEntered(player);
        this->findAndSetTeam(player);
        if( this->players_.size() <= maxPlayers_ || maxPlayers_  ==  0)
        {
            this->allowedInGame_[player]= true;
        }
        else
        {
            this->allowedInGame_[player]= false;
            orxout() << "not allowed in game: players = " << this->players_.size() << " > maximum: " << maxPlayers_ << endl;
        }
    }

    /**
    @brief
        Function that determines the player's team number when entering the game for the first time.
        Override this function for other team structures.
    */
    void TeamGametype::findAndSetTeam(PlayerInfo* player)
    {
        if(player == NULL) return; // catch null pointers
        std::vector<unsigned int> playersperteam(this->teams_, 0);

        for (std::map<PlayerInfo*, int>::iterator it = this->teamnumbers_.begin(); it != this->teamnumbers_.end(); ++it)
            if (it->second < static_cast<int>(this->teams_) && it->second >= 0)
                playersperteam[it->second]++;

        unsigned int minplayers = static_cast<unsigned int>(-1);
        size_t minplayersteam = 0;
        for (size_t i = 0; i < this->teams_; ++i)
        {
            if (playersperteam[i] < minplayers)
            {
                minplayers = playersperteam[i];
                minplayersteam = i;
            }
        }

        this->teamnumbers_[player] = minplayersteam;

    }

    bool TeamGametype::playerLeft(PlayerInfo* player)
    {
        bool valid_player = Gametype::playerLeft(player);
        if( (this->players_.size() >= maxPlayers_) && (allowedInGame_[player] == true) ) // if there's a "waiting list"
        {
            for (std::map<PlayerInfo*, bool>::iterator it = this->allowedInGame_.begin(); it != this->allowedInGame_.end(); ++it)
            {
                 if(it->second == false) // waiting player found
                 {it->second = true; break;} // allow player to enter
            }
        }

        if (valid_player)
        {   // clean up the maps
            this->teamnumbers_.erase(player);
            this->allowedInGame_.erase(player);
        }

        return valid_player;
    }

    void TeamGametype::spawnDeadPlayersIfRequested()
    {
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)\
        {
            if(allowedInGame_[it->first] == false)//check if dead player is allowed to enter
            {
                continue;
            }
            if (it->second.state_ == PlayerState::Dead)
            {
                if ((it->first->isReadyToSpawn() || this->bForceSpawn_))
                {
                   this->spawnPlayer(it->first);
                }
            }
        }
    }


    bool TeamGametype::allowPawnHit(Pawn* victim, Pawn* originator)
    {// hit allowed: if victim & originator are foes or if originator doesnot exist or if friendlyfire is allowed
        return (!this->pawnsAreInTheSameTeam(victim, originator) || !originator || this->allowFriendlyFire_);
    }

    bool TeamGametype::allowPawnDamage(Pawn* victim, Pawn* originator)
    {
        return (!this->pawnsAreInTheSameTeam(victim, originator) || !originator || this->allowFriendlyFire_);
    }

    bool TeamGametype::allowPawnDeath(Pawn* victim, Pawn* originator)
    {
        return (!this->pawnsAreInTheSameTeam(victim, originator) || !originator || this->allowFriendlyFire_);
    }

    int TeamGametype::getTeamScore(PlayerInfo* player)
    {
        int teamscore = 0;
        if(!player || this->getTeam(player) == -1)
            return 0;
        for (std::map<PlayerInfo*, Player>::iterator it = this->players_.begin(); it != this->players_.end(); ++it)
        {
            if ( this->getTeam(it->first) ==  this->getTeam(player) )
            {
                teamscore += it->second.frags_;
            }
        }
        return teamscore;
    }

    int TeamGametype::getTeamSize(int team)
    {
        int teamSize = 0;
        for (std::map<PlayerInfo*, int>::iterator it = this->teamnumbers_.begin(); it != this->teamnumbers_.end(); ++it)
        {
            if (it->second == team)
                teamSize++;
        }
        return teamSize;
    }

    int TeamGametype::getHumansInTeam(int team)
    {
        int teamSize = 0;
        for (std::map<PlayerInfo*, int>::iterator it = this->teamnumbers_.begin(); it != this->teamnumbers_.end(); ++it)
        {
            if (it->second == team  && it->first->isHumanPlayer())
                teamSize++;
        }
        return teamSize;
    }


    SpawnPoint* TeamGametype::getBestSpawnPoint(PlayerInfo* player) const
    {
        int desiredTeamNr = -1;
        std::map<PlayerInfo*, int>::const_iterator it_player = this->teamnumbers_.find(player);
        if (it_player != this->teamnumbers_.end())
            desiredTeamNr = it_player->second;

        // Only use spawnpoints of the own team (or non-team-spawnpoints)
        std::set<SpawnPoint*> teamSpawnPoints = this->spawnpoints_;
        for (std::set<SpawnPoint*>::iterator it = teamSpawnPoints.begin(); it != teamSpawnPoints.end(); )
        {
            if ((*it)->isA(Class(TeamSpawnPoint)))
            {
                TeamSpawnPoint* tsp = orxonox_cast<TeamSpawnPoint*>(*it);
                if (tsp && static_cast<int>(tsp->getTeamNumber()) != desiredTeamNr)
                {
                    teamSpawnPoints.erase(it++);
                    continue;
                }
            }

            ++it;
        }

        SpawnPoint* fallbackSpawnPoint = NULL;
        if (teamSpawnPoints.size() > 0)
        {
            unsigned int randomspawn = static_cast<unsigned int>(rnd(static_cast<float>(teamSpawnPoints.size())));
            unsigned int index = 0;
            // Get random fallback spawnpoint in case there is no active SpawnPoint.
            for (std::set<SpawnPoint*>::const_iterator it = teamSpawnPoints.begin(); it != teamSpawnPoints.end(); ++it)
            {
                if (index == randomspawn)
                {
                    fallbackSpawnPoint = (*it);
                    break;
                }

                ++index;
            }

            // Remove all inactive SpawnPoints from the list.
            for (std::set<SpawnPoint*>::iterator it = teamSpawnPoints.begin(); it != teamSpawnPoints.end(); )
            {
                if(!(*it)->isActive())
                {
                    teamSpawnPoints.erase(it++);
                    continue;
                }

                ++it;
            }

            randomspawn = static_cast<unsigned int>(rnd(static_cast<float>(teamSpawnPoints.size())));
            index = 0;
            for (std::set<SpawnPoint*>::const_iterator it = teamSpawnPoints.begin(); it != teamSpawnPoints.end(); ++it)
            {
                if (index == randomspawn)
                    return (*it);

                ++index;
            }

            return fallbackSpawnPoint;
        }

        return 0;
    }

    void TeamGametype::playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn)
    {
        if (!player)
            return;

        this->setTeamColour(player,pawn);
    }

    bool TeamGametype::pawnsAreInTheSameTeam(Pawn* pawn1, Pawn* pawn2)
    {
        if (pawn1 && pawn2)
        {
            std::map<PlayerInfo*, int>::const_iterator it1 = this->teamnumbers_.find(pawn1->getPlayer());
            std::map<PlayerInfo*, int>::const_iterator it2 = this->teamnumbers_.find(pawn2->getPlayer());

            if (it1 != this->teamnumbers_.end() && it2 != this->teamnumbers_.end())
                return (it1->second == it2->second);
        }
        return false;
    }

    int TeamGametype::getTeam(PlayerInfo* player)
    {
        std::map<PlayerInfo*, int>::const_iterator it_player = this->teamnumbers_.find(player);
        if (it_player != this->teamnumbers_.end())
            return it_player->second;
        else
            return -1;
    }

    void TeamGametype::setTeamColour(PlayerInfo* player, Pawn* pawn)
    {
        // Set the team colour
        std::map<PlayerInfo*, int>::const_iterator it_player = this->teamnumbers_.find(player);
        if (it_player != this->teamnumbers_.end() && it_player->second >= 0 && it_player->second < static_cast<int>(this->teamcolours_.size()))
        {
            this->colourPawn(pawn, it_player->second);
        }
    }

    /**
    @brief
        Colours a pawn depending on the team values set in XML.
        A pawn is coloured depending on it's team set via XML.
        If there is a controller the pawn is coloured depending on its team which also can be set via XML.
    */
    void TeamGametype::setDefaultObjectColour(Pawn* pawn)
    {
        if(pawn == NULL)
            return;

        int teamnumber = pawn->getTeam();

        if(teamnumber >= 0)
        {
            this->colourPawn(pawn, teamnumber); return;
        }
        //get Pawn's controller
        ControllableEntity* entity = orxonox_cast<ControllableEntity*>(pawn);

        Controller* controller = 0;
        if (entity->getController())
            controller = entity->getController();
        else if (entity->getXMLController())
            controller = entity->getXMLController();
        else
            return;

        ArtificialController* artificial =  orxonox_cast<ArtificialController*>(controller);
        //get Teamnumber - get the data
        if(artificial == NULL)
            return;
        teamnumber= artificial->getTeam();

        //set ObjectColour
        this->colourPawn(pawn, teamnumber);
    }

    void TeamGametype::colourPawn(Pawn* pawn, int teamNr)
    {// catch: no-colouring-case and wrong input
        if(teamNr < 0 || teamNr+1 > int(this->teamcolours_.size()) ||pawn == NULL) return;
        pawn->setRadarObjectColour(this->teamcolours_[teamNr]);

        std::set<WorldEntity*> pawnAttachments = pawn->getAttachedObjects();
        for (std::set<WorldEntity*>::iterator it = pawnAttachments.begin(); it != pawnAttachments.end(); ++it)
        {
            if ((*it)->isA(Class(TeamColourable)))
            {
                TeamColourable* tc = orxonox_cast<TeamColourable*>(*it);
                tc->setTeamColour(this->teamcolours_[teamNr]);
            }
         }
    }

    void TeamGametype::announceTeamWin(int winnerTeam)
    {
        for (std::map<PlayerInfo*, int>::iterator it3 = this->teamnumbers_.begin(); it3 != this->teamnumbers_.end(); ++it3)
        {
            if (it3->first->getClientID() == NETWORK_PEER_ID_UNKNOWN)
                continue;
            if (it3->second == winnerTeam)
            {
                this->gtinfo_->sendAnnounceMessage("Your team has won the match!", it3->first->getClientID());
            }
            else
            {
                this->gtinfo_->sendAnnounceMessage("Your team has lost the match!", it3->first->getClientID());
            }
        }   
    }

}
