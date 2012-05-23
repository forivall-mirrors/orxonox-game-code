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
 *      Nino Weingart
 *   Co-authors:
 *      ...
 *
 */
#include "CaptureTheFlag.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "interfaces/TeamColourable.h"
#include "worldentities/TeamSpawnPoint.h"
#include "worldentities/pawns/Pawn.h"



namespace orxonox
{
    CreateUnloadableFactory(CaptureTheFlag);

    CaptureTheFlag::CaptureTheFlag(BaseObject* creator) : TeamBaseMatch(creator)
    {
        RegisterObject(CaptureTheFlag);
        
        this->teams_ = 2;
        this->pointsTeam1_ = 0;
        this->pointsTeam2_ = 0;
        
        //this->flagTeamBlue = new FlagPickup(this);
    	//this->flagTeamRed = new FlagPickup(this);

        this->setConfigValues();
    }

    void CaptureTheFlag::tick(float dt){
    	if(this->flagTeamBlue->isPickedUp()){
    		if(getTeam(this->flagTeamBlue->pickedUpBy()->getPlayer()) == 1){
    		    this->flagTeamBlue->ignorePickedUp();
    		    
    			if(this->flagTeamBlue->pickedUpBy()->hasFlag()){
    				pointsTeam1_ += 1000;
    				this->flagTeamBlue->pickedUpBy()->setHasFlag(false);
    			}
    			this->flagTeamBlue->setPickedUp(false);
    		}
    		else{
    			this->flagTeamBlue->pickedUpBy()->setHasFlag(true);
    		}
    	}
    	if(this->flagTeamRed->isPickedUp()){
    		if(getTeam(this->flagTeamRed->pickedUpBy()->getPlayer()) == 0){
    		    this->flagTeamRed->ignorePickedUp();
    		    
    			if(this->flagTeamRed->pickedUpBy()->hasFlag()){
    				pointsTeam1_ += 1000;
    				this->flagTeamRed->pickedUpBy()->setHasFlag(false);
    			}
    			this->flagTeamRed->setPickedUp(false);
    		}
    		else{
    			this->flagTeamRed->pickedUpBy()->setHasFlag(true);
    		}
    	}
    }
    void CaptureTheFlag::setConfigValues()
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

    void CaptureTheFlag::playerEntered(PlayerInfo* player)
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

    bool CaptureTheFlag::playerLeft(PlayerInfo* player)
    {
        bool valid_player = Deathmatch::playerLeft(player);

        if (valid_player)
            this->teamnumbers_.erase(player);

        return valid_player;
    }

    bool CaptureTheFlag::allowPawnHit(Pawn* victim, Pawn* originator)
    {
        return (!this->pawnsAreInTheSameTeam(victim, originator) || !originator);
    }

    bool CaptureTheFlag::allowPawnDamage(Pawn* victim, Pawn* originator)
    {
        return (!this->pawnsAreInTheSameTeam(victim, originator) || !originator);
    }

    bool CaptureTheFlag::allowPawnDeath(Pawn* victim, Pawn* originator)
    {
        return (!this->pawnsAreInTheSameTeam(victim, originator) || !originator);
    }

    // collect Points for killing oppenents
        void CaptureTheFlag::playerScored(PlayerInfo* player)
        {
            int teamnr = this->getTeam(player);
            this->addTeamPoints(teamnr, 5);
        }
        
        
        // show points or each interval of time
        void CaptureTheFlag::showPoints()
        {
            if (!this->hasStarted() || this->hasEnded())
                return;

            orxout(message) << "Points standing:" << '\n' << "Team Blue: "<< pointsTeam1_ << '\n' << "Team Red: " << pointsTeam2_ << endl;
            if(pointsTeam1_ >=1700 && pointsTeam1_ < 2000) orxout(message) << "Team Blue is near victory!" << endl;
            if(pointsTeam2_ >=1700 && pointsTeam2_ < 2000) orxout(message) << "Team Red is near victory!" << endl;
        }
        
        // end game if one team reaches 2000 points
        void CaptureTheFlag::endGame()
        {
            if (this->pointsTeam1_ >= 2000 || this->pointsTeam2_ >= 2000)
            {
                int winningteam = -1;

                if (this->pointsTeam1_ > this->pointsTeam2_)
                {
                    orxout(message) << "Team Blue has won the match" << endl;
                    winningteam = 0;
                }
                else
                {
                    orxout(message) << "Team Red has won the match" << endl;
                    winningteam = 1;
                }

                for (std::map<PlayerInfo*, int>::iterator it = this->teamnumbers_.begin(); it != this->teamnumbers_.end(); ++it)
                {
                    if (it->first->getClientID() == NETWORK_PEER_ID_UNKNOWN)
                        continue;

                    if (it->second == winningteam)
                        this->gtinfo_->sendAnnounceMessage("You have won the match!", it->first->getClientID());
                    else
                        this->gtinfo_->sendAnnounceMessage("You have lost the match!", it->first->getClientID());
                }

                this->end();
                this->scoreTimer_.stopTimer();
                this->outputTimer_.stopTimer();
            }
        }
        
        // this function is called by the function winPoints() which adds points to the teams for every base and killed openents at a certain time
        void CaptureTheFlag::addTeamPoints(int team, int points)
        {
            if(team == 0)
            {
                this->pointsTeam1_ += points;
            }
            if(team == 1)
            {
                this->pointsTeam2_ += points;
            }

            this->endGame();
        }

        int TeamBaseMatch::getTeamPoints(int team)
        {
            if(team == 0)
            {
                return this->pointsTeam1_;
            }
            if(team == 1)
            {
                return this->pointsTeam2_;
            }

            return 0;
        }
        
    SpawnPoint* CaptureTheFlag::getBestSpawnPoint(PlayerInfo* player) const
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

    void CaptureTheFlag::playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn)
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

    bool CaptureTheFlag::pawnsAreInTheSameTeam(Pawn* pawn1, Pawn* pawn2)
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

    int CaptureTheFlag::getTeam(PlayerInfo* player)
    {
        std::map<PlayerInfo*, int>::const_iterator it_player = this->teamnumbers_.find(player);
        if (it_player != this->teamnumbers_.end())
            return it_player->second;
        else
            return -1;
    }
}
