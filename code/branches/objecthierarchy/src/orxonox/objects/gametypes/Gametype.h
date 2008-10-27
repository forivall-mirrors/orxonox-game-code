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

namespace orxonox
{
    class _OrxonoxExport Gametype : public BaseObject, public Tickable
    {
        friend class PlayerInfo;

        public:
            Gametype(BaseObject* creator);
            virtual ~Gametype() {}

            virtual void tick(float dt);

            virtual void start();
            virtual void end();
            virtual void playerEntered(PlayerInfo* player);
            virtual void playerLeft(PlayerInfo* player);
            virtual void playerSwitched(PlayerInfo* player, Gametype* newgametype);
            virtual void playerSwitchedBack(PlayerInfo* player, Gametype* oldgametype);
            virtual void playerChangedName(PlayerInfo* player);
            virtual void playerSpawned(PlayerInfo* player);
            virtual void playerDied(PlayerInfo* player);
            virtual void playerScored(PlayerInfo* player);

            inline const std::set<PlayerInfo*>& getPlayers() const
                { return this->players_; }

            inline void registerSpawnPoint(SpawnPoint* spawnpoint)
                { this->spawnpoints_.insert(spawnpoint); }

        private:
            virtual SpawnPoint* getBestSpawnPoint(PlayerInfo* player) const;

            void addPlayer(PlayerInfo* player);
            void removePlayer(PlayerInfo* player);

            void assignDefaultPawnsIfNeeded() const;
            void checkStart();
            void spawnPlayer(PlayerInfo* player);

            bool bStarted_;
            bool bEnded_;
            bool bAutoStart_;
            std::set<PlayerInfo*> players_;
            std::set<SpawnPoint*> spawnpoints_;
            SubclassIdentifier<ControllableEntity> defaultPawn_;
    };
}

#endif /* _Gametype_H__ */
