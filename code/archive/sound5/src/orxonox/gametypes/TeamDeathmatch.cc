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

#include "TeamDeathmatch.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "interfaces/TeamColourable.h"
#include "worldentities/TeamSpawnPoint.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateUnloadableFactory(TeamDeathmatch);

    TeamDeathmatch::TeamDeathmatch(BaseObject* creator) : Deathmatch(creator)
    {
        RegisterObject(TeamDeathmatch);

        this->teams_ = 2;

        this->setConfigValues();
    }

    void TeamDeathmatch::setConfigValues()
    {
        SetConfigValue(teams_, 2);

        static ColourValue colours[] =
        {
            ColourValue(1.0f, 0.3f, 0.3f),
            ColourValue(0.3f, 0.3f, 1.0f),
            ColourValue(0.3f, 1.0f, 0.3f),
            ColourValue(1.0f, 1.0f, 0.0f)
        };
        static std::vector<ColourValue> defaultcolours(colours, colours + sizeof(colours) / sizeof(ColourValue));

        SetConfigValue(teamcolours_, defaultcolours);
    }

    void TeamDeathmatch::playerEntered(PlayerInfo* player)
    {
        Deathmatch::playerEntered(player);

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

    bool TeamDeathmatch::playerLeft(PlayerInfo* player)
    {
        bool valid_player = Deathmatch::playerLeft(player);

        if (valid_player)
            this->teamnumbers_.erase(player);

        return valid_player;
    }

    bool TeamDeathmatch::allowPawnHit(Pawn* victim, Pawn* originator)
    {
        return (!this->pawnsAreInTheSameTeam(victim, originator) || !originator);
    }

    bool TeamDeathmatch::allowPawnDamage(Pawn* victim, Pawn* originator)
    {
        return (!this->pawnsAreInTheSameTeam(victim, originator) || !originator);
    }

    bool TeamDeathmatch::allowPawnDeath(Pawn* victim, Pawn* originator)
    {
        return (!this->pawnsAreInTheSameTeam(victim, originator) || !originator);
    }

    SpawnPoint* TeamDeathmatch::getBestSpawnPoint(PlayerInfo* player) const
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

        if (teamSpawnPoints.size() > 0)
        {
            unsigned int randomspawn = static_cast<unsigned int>(rnd(static_cast<float>(teamSpawnPoints.size())));
            unsigned int index = 0;
            for (std::set<SpawnPoint*>::const_iterator it = teamSpawnPoints.begin(); it != teamSpawnPoints.end(); ++it)
            {
                if (index == randomspawn)
                    return (*it);

                ++index;
            }
        }

        return 0;
    }

    void TeamDeathmatch::playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn)
    {
        if (!player)
            return;

        // Set the team colour
        std::map<PlayerInfo*, int>::const_iterator it_player = this->teamnumbers_.find(player);
        if (it_player != this->teamnumbers_.end() && it_player->second >= 0 && it_player->second < static_cast<int>(this->teamcolours_.size()))
        {
            if (pawn)
            {
                pawn->setRadarObjectColour(this->teamcolours_[it_player->second]);

                std::set<WorldEntity*> pawnAttachments = pawn->getAttachedObjects();
                for (std::set<WorldEntity*>::iterator it = pawnAttachments.begin(); it != pawnAttachments.end(); ++it)
                {
                    if ((*it)->isA(Class(TeamColourable)))
                    {
                        TeamColourable* tc = orxonox_cast<TeamColourable*>(*it);
                        tc->setTeamColour(this->teamcolours_[it_player->second]);
                    }
                }
            }
        }
    }

    bool TeamDeathmatch::pawnsAreInTheSameTeam(Pawn* pawn1, Pawn* pawn2)
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

    int TeamDeathmatch::getTeam(PlayerInfo* player)
    {
        std::map<PlayerInfo*, int>::const_iterator it_player = this->teamnumbers_.find(player);
        if (it_player != this->teamnumbers_.end())
            return it_player->second;
        else
            return -1;
    }
}
