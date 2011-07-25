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

#ifndef _Gametype_H__
#define _Gametype_H__

#include "OrxonoxPrereqs.h"

#include <map>

#include "core/BaseObject.h"
#include "core/Identifier.h"
#include "objects/worldentities/ControllableEntity.h"
#include "objects/Tickable.h"
#include "objects/infos/GametypeInfo.h"

namespace orxonox
{
    namespace PlayerState
    {
        enum Enum
        {
            Uninitialized,
            Joined,
            Alive,
            Dead
        };
    }

    struct Player
    {
        PlayerInfo* info_;
        PlayerState::Enum state_;
        int frags_;
        int killed_;
    };

    class _OrxonoxExport Gametype : public BaseObject, public Tickable
    {
        friend class PlayerInfo;

        public:
            Gametype(BaseObject* creator);
            virtual ~Gametype() {}

            void setConfigValues();

            virtual void tick(float dt);

            inline const GametypeInfo* getGametypeInfo() const
                { return &this->gtinfo_; }

            inline bool hasStarted() const
                { return this->gtinfo_.bStarted_; }
            inline bool hasEnded() const
                { return this->gtinfo_.bEnded_; }

            virtual void start();
            virtual void end();
            virtual void playerEntered(PlayerInfo* player);
            virtual bool playerLeft(PlayerInfo* player);
            virtual void playerSwitched(PlayerInfo* player, Gametype* newgametype);
            virtual void playerSwitchedBack(PlayerInfo* player, Gametype* oldgametype);
            virtual bool playerChangedName(PlayerInfo* player);

            virtual void playerScored(PlayerInfo* player);

            virtual bool allowPawnHit(Pawn* victim, Pawn* originator = 0);
            virtual bool allowPawnDamage(Pawn* victim, Pawn* originator = 0);
            virtual bool allowPawnDeath(Pawn* victim, Pawn* originator = 0);

            virtual void pawnKilled(Pawn* victim, Pawn* killer = 0);
            virtual void pawnPreSpawn(Pawn* pawn);
            virtual void pawnPostSpawn(Pawn* pawn);
            virtual void playerPreSpawn(PlayerInfo* player);
            virtual void playerPostSpawn(PlayerInfo* player);

            virtual void playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn);
            virtual void playerStopsControllingPawn(PlayerInfo* player, Pawn* pawn);

            inline const std::map<PlayerInfo*, Player>& getPlayers() const
                { return this->players_; }

            int getScore(PlayerInfo* player) const;

            inline void registerSpawnPoint(SpawnPoint* spawnpoint)
                { this->spawnpoints_.insert(spawnpoint); }

            inline bool isStartCountdownRunning() const
                { return this->gtinfo_.bStartCountdownRunning_; }
            inline float getStartCountdown() const
                { return this->gtinfo_.startCountdown_; }

            inline void setHUDTemplate(const std::string& name)
                { this->gtinfo_.hudtemplate_ = name; }
            inline const std::string& getHUDTemplate() const
                { return this->gtinfo_.hudtemplate_; }

            void addBots(unsigned int amount);
            void killBots(unsigned int amount = 0);

            inline unsigned int getNumberOfPlayers() const
                { return this->players_.size(); }

        protected:
            virtual SpawnPoint* getBestSpawnPoint(PlayerInfo* player) const;

            virtual void assignDefaultPawnsIfNeeded();
            virtual void checkStart();
            virtual void spawnPlayer(PlayerInfo* player);
            virtual void spawnPlayersIfRequested();
            virtual void spawnDeadPlayersIfRequested();

            GametypeInfo gtinfo_;

            bool bAutoStart_;
            bool bForceSpawn_;

            float initialStartCountdown_;
            unsigned int numberOfBots_;
            SubclassIdentifier<Bot> botclass_;

            std::map<PlayerInfo*, Player> players_;
            std::set<SpawnPoint*> spawnpoints_;
            SubclassIdentifier<ControllableEntity> defaultControllableEntity_;

            OverlayGroup* scoreboard_;

            // Config Values
            std::string scoreboardTemplate_;
    };
}

#endif /* _Gametype_H__ */
