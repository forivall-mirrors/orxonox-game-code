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

#ifndef _OldSpaceRace_H__
#define _OldSpaceRace_H__

#include "gametypes/GametypesPrereqs.h"

#include <set>
#include <string>

#include <util/Clock.h>

#include "gametypes/Gametype.h"

#include "OldRaceCheckPoint.h"

namespace orxonox
{
  /**
  @brief
    The OldSpaceRace class enables the creation of a space race level, where the player has to reach check points in a given order.
  */
    class _GametypesExport OldSpaceRace : public Gametype
    {
        friend class OldRaceCheckPoint;

        public:
            OldSpaceRace(BaseObject* creator);
            virtual ~OldSpaceRace() {}

            virtual void start();
            virtual void end();

            virtual void newCheckpointReached();
            virtual void addBots(unsigned int amount){} //<! overwrite function in order to bypass the addbots command.
                                                        //<! This is only a temporary solution. Better: create racingBots.

            inline void setCheckpointsReached(int n)
                { this->checkpointsReached_ = n;}
            inline int getCheckpointsReached()
                { return this->checkpointsReached_; }
            inline void timeIsUp()
                { this->bTimeIsUp_ = true;}

        protected:

        private:
            int checkpointsReached_; //The current number of check points reached by the player.
            std::set<float> scores_; //The times of the players are saved in a set.
            bool bTimeIsUp_; //True if one of the check points is reached too late.
            Clock clock_; //The clock starts running at the beginning of the game. It is used to give the time at each check point, the give the time at the end of the game, and to stop the game if a check point is reached too late.
    };
}

#endif /* _OldSpaceRace_H__ */
