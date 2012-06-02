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
 *      Mauro Salomon
 *   Co-authors:
 *      ...
 *
 */

#ifndef _SpaceRace_H__
#define _SpaceRace_H__

#include "gametypes/GametypesPrereqs.h"

#include <set>
#include <string>
# include <vector>

#include <util/Clock.h>

#include "gametypes/Gametype.h"

#include "SpaceRaceManager.h"

namespace orxonox
{
    /**
    @brief
        The SpaceRace class enables the creation of a space race level, where the player has to reach check points in a given order.
    */
    class _GametypesExport SpaceRace : public Gametype
    {
        friend class RaceCheckPoint;


        public:
            SpaceRace(BaseObject* creator);
            virtual ~SpaceRace() {}

            void tick(float dt);

            virtual void end();

            void newCheckpointReached(RaceCheckPoint* checkpoint, PlayerInfo* player);
            inline RaceCheckPoint* getCheckpointReached(PlayerInfo* player)
                { return this->checkpointReached_[player]; }

            inline void setTimeIsUp()
                { this->bTimeIsUp_ = true;}
            inline Clock& getClock()
                { return this->clock_; }

            bool allowPawnHit(Pawn* victim, Pawn* originator);
            bool allowPawnDamage(Pawn* victim, Pawn* originator);
            bool allowPawnDeath(Pawn* victim, Pawn* originator);

        private:
            bool cantMove_;                                            ///< Helper variable, used to stall the engines before the race starts.
            std::map<PlayerInfo*, RaceCheckPoint*> checkpointReached_; ///< The number of the last check point reached by each player.
            bool bTimeIsUp_;                                           ///< True if one of the check points is reached too late.

            Clock clock_; ///< The clock starts running at the beginning of the game. It is used to give the time at each check point, the give the time at the end of the game, and to stop the game if a check point is reached too late.
    };
}

#endif /* _SpaceRace_H__ */
