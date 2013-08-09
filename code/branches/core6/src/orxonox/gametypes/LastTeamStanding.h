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
/**
    @file LastTeamStanding.h
    @brief Declaration of the Gametype "Last Team Standing".
*/

#ifndef _LastTeamStanding_H__
#define _LastTeamStanding_H__

#include "OrxonoxPrereqs.h"
#include "TeamDeathmatch.h"
#include <map>
#include <vector>

//TODO: Hud doesn load; problem with destructor; teams are not assigned properly; 

namespace orxonox
{
    class _OrxonoxExport LastTeamStanding : public TeamDeathmatch
    {
    /**
    @brief
        Last Team Standing is a gametype where each team fights against each other, until one team remains. 
    @author
        Johannes Ritz
    */
        protected:
            int lives; //!< Standard amount of lives. Each player starts a game with so many lives.
            std::map< PlayerInfo*, int > playerLives_; //!< Each player's lives are stored here.
            std::vector<int> eachTeamsPlayers; //!<Number of players in each team.
            int teamsAlive; //!< Counter counting teams with more than one player remaining.
//Data for CamperPunishment
            float timeRemaining; //!< Each player has a certain time where he or she has to hit an opponent or will be punished.
            std::map<PlayerInfo*, float> timeToAct_; //!< Each player's time till she/he will be punished is stored here.
            bool bNoPunishment; //!< Config value to switch off Punishment function if it is set to true.
            bool bHardPunishment; //!< Switches between damage and death as punishment.
            float punishDamageRate; //!< Makes Damage adjustable.
//Data for RespawnDelay
            float respawnDelay; //!<Time in seconds when a player will respawn after death.
            std::map<PlayerInfo*, float> playerDelayTime_; //!< Stores each Player's delay time.
            std::map<PlayerInfo*, bool> inGame_; //!< Indicates each Player's state.

            virtual void spawnDeadPlayersIfRequested(); //!< Prevents dead players to respawn.
            virtual int getMinLives(); //!< Returns minimum of each player's lives; players with 0 lives are skipped;

        public:
            LastTeamStanding(Context* context); //!< Default Constructor.
            virtual ~LastTeamStanding(); //!< Default Destructor.

            virtual void playerEntered(PlayerInfo* player); //!< Initializes values.
            virtual bool playerLeft(PlayerInfo* player); //!< Manages all local variables.

            virtual bool allowPawnDeath(Pawn* victim, Pawn* originator = 0); //!< Manages each player's lost lives.
            virtual bool allowPawnDamage(Pawn* victim, Pawn* originator = 0); //!< If a player shoot's an opponent, his punishment countdown will be resetted.
            virtual void playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn); //!< Resets punishment time and respawn delay.
            void tick (float dt); //!< used to end the game
            virtual void end(); //!< Sends an end message.
            void punishPlayer(PlayerInfo* player); //!< Function in order to kill a player. Punishment for hiding longer than "timeRemaining".
            int playerGetLives(PlayerInfo* player); //!< getFunction for the map "playerLives_".
            inline int getNumTeamsAlive() const//!< Returns the number of players that are still alive.
            {return this->teamsAlive;}
            void setConfigValues(); //!< Makes values configurable.

    };
}

#endif /* _LastTeamStanding_H__ */
