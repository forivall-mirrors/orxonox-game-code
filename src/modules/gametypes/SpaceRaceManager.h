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

#ifndef _SpaceRaceManager_H__
#define _SpaceRaceManager_H__

#include "gametypes/GametypesPrereqs.h"

#include <set>
#include <string>
#include <vector>

#include <util/Clock.h>

#include "gametypes/Gametype.h"
#include "tools/interfaces/Tickable.h"
#include "RaceCheckPoint.h"



namespace orxonox
{
    /**
    @brief
        The SpaceRaceManager class controls a space race level, where the player has to reach check points in a given order.
    */
    class _GametypesExport SpaceRaceManager : public BaseObject, public Tickable
    {
        friend class RaceCheckPoint;

        public:
            SpaceRaceManager(Context* context);
            virtual ~SpaceRaceManager() ;

            void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            void addCheckpoint(RaceCheckPoint* checkpoint);
            RaceCheckPoint* getCheckpoint(unsigned int index) const;

            RaceCheckPoint* findCheckpoint(int index) const;

            void checkpointReached(RaceCheckPoint* newCheckpoint, PlayerInfo* player);

            std::vector<RaceCheckPoint*> getAllCheckpoints();

            void tick(float dt);

        protected:
            bool reachedValidCheckpoint(RaceCheckPoint* oldCheckpoint, RaceCheckPoint* newCheckpoint, PlayerInfo* player) const;
            void updateRadarVisibility(RaceCheckPoint* oldCheckpoint, RaceCheckPoint* newCheckpoint) const;

        private:
            std::vector<RaceCheckPoint*> checkpoints_;
            bool firstcheckpointvisible_; ///< true if the first check point is visible.
            SpaceRace* race_; // needed to get the players
            //int amountOfPlayers;
            std::map<PlayerInfo*, Player> players_;
    };
}

#endif /* _SpaceRaceManager_H__ */
