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
#include "TeamGametype.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "objects/Teamcolourable.h"
#include "objects/worldentities/TeamSpawnPoint.h"

namespace orxonox
{
    CreateUnloadableFactory(TeamGametype);

    TeamGametype::TeamGametype(BaseObject* creator) : Gametype(creator)
    {
        RegisterObject(TeamGametype);

        this->teams_ = 2;

        this->setConfigValues();
    }

    void TeamGametype::setConfigValues()
    {
        SetConfigValue(teams_, 2);

        static ColourValue colours[] =
        {
            ColourValue(1.0, 0.3, 0.3),
            ColourValue(0.3, 0.3, 1.0),
            ColourValue(0.3, 1.0, 0.3),
            ColourValue(1.0, 1.0, 0.0)
        };
        static std::vector<ColourValue> defaultcolours(colours, colours + sizeof(colours) / sizeof(ColourValue));

        SetConfigValueVector(teamcolours_, defaultcolours);
    }

    void TeamGametype::playerEntered(PlayerInfo* player)
    {
        Gametype::playerEntered(player);

        std::vector<unsigned int> playersperteam(this->teams_, 0);

        for (std::map<PlayerInfo*, int>::iterator it = this->teamnumbers_.begin(); it != this->teamnumbers_.end(); ++it)
            if (it->second < this->teams_ && it->second >= 0)
                playersperteam[it->second]++;

        unsigned int minplayers = (unsigned int)-1;
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

    void TeamGametype::playerLeft(PlayerInfo* player)
    {
        Gametype::playerLeft(player);

        this->players_.erase(player);
    }

    bool TeamGametype::allowPawnHit(Pawn* victim, Pawn* originator)
    {
        return (!this->pawnsAreInTheSameTeam(victim, originator));
    }

    bool TeamGametype::allowPawnDamage(Pawn* victim, Pawn* originator)
    {
        return (!this->pawnsAreInTheSameTeam(victim, originator));
    }

    bool TeamGametype::allowPawnDeath(Pawn* victim, Pawn* originator)
    {
        return (!this->pawnsAreInTheSameTeam(victim, originator));
    }

    SpawnPoint* TeamGametype::getBestSpawnPoint(PlayerInfo* player) const
    {
        int desiredTeamNr = -1;
        std::map<PlayerInfo*, int>::const_iterator it_player = this->teamnumbers_.find(player);
        if (it_player != this->teamnumbers_.end())
            desiredTeamNr = it_player->second;

        // Only use spawnpoints of the own team (or non-team-spawnpoints)
        std::set<SpawnPoint*> teamSpawnPoints = this->spawnpoints_;
        for (std::set<SpawnPoint*>::const_iterator it = teamSpawnPoints.begin(); it != teamSpawnPoints.end(); )
        {
            if ((*it)->isA(Class(TeamSpawnPoint)))
            {
                TeamSpawnPoint* tsp = dynamic_cast<TeamSpawnPoint*>(*it);
                if (tsp && tsp->getTeamNumber() != desiredTeamNr)
                {
                    teamSpawnPoints.erase(it++);
                    continue;
                }
            }

            ++it;
        }

        if (teamSpawnPoints.size() > 0)
        {
            unsigned int randomspawn = (unsigned int)rnd(teamSpawnPoints.size());
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

    void TeamGametype::playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn)
    {
        if (!player)
            return;

        // Set the team colour
        std::map<PlayerInfo*, int>::const_iterator it_player = this->teamnumbers_.find(player);
        if (it_player != this->teamnumbers_.end() && it_player->second >= 0 && it_player->second < this->teamcolours_.size())
        {
            if (pawn)
            {
                std::set<WorldEntity*> pawnAttachments = pawn->getAttachedObjects();
                for (std::set<WorldEntity*>::iterator it = pawnAttachments.begin(); it != pawnAttachments.end(); ++it)
                {
                    if ((*it)->isA(Class(Teamcolourable)))
                    {
                        Teamcolourable* tc = dynamic_cast<Teamcolourable*>(*it);
                        tc->setTeamColour(this->teamcolours_[it_player->second]);
                    }
                }
            }
        }
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
}
