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

#ifndef _OldRaceCheckPoint_H__
#define _OldRaceCheckPoint_H__

#include "gametypes/GametypesPrereqs.h"

#include "objects/triggers/DistanceTrigger.h"
#include "interfaces/RadarViewable.h"

namespace orxonox
{
    /**
    @brief
        The OldRaceCheckPoint class enables the creation of a check point to use in a OldSpaceRace level.
        !!! Don't forget to control the indexes of your check points and to set one last check point!!!
    */
    class _GametypesExport OldRaceCheckPoint : public DistanceTrigger, public RadarViewable
    {
        public:
            OldRaceCheckPoint(Context* context);
            virtual ~OldRaceCheckPoint();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
            virtual void tick(float dt);

            protected:
            virtual void triggered(bool bIsTriggered);
            inline void setLast(bool isLast)
                { this->bIsLast_ = isLast; }
            inline bool getLast()
                { return this->bIsLast_; }
            inline void setCheckpointIndex(int checkpointIndex)
                { this->bCheckpointIndex_ = checkpointIndex; }
            inline int getCheckpointIndex()
                { return this->bCheckpointIndex_; }
            virtual void setTimelimit(float timeLimit);
            inline float getTimeLimit()
                { return this->bTimeLimit_;}
            inline const WorldEntity* getWorldEntity() const
                { return this; }

        private:
            int bCheckpointIndex_; //The index of this check point. This value will be compared with the number of check points reached in the level. The check points must be indexed in ascending order beginning from zero and without any jumps between the indexes.
            bool bIsLast_; //True if this check point is the last of the level. There can be only one last check point for each level and there must be a last check point in the level.
            float bTimeLimit_; //The time limit (from the start of the level) to reach this check point. If the check point is reached after this time, the game ends and the player looses.
      
    };
}

#endif /* _OldRaceCheckPoint_H__ */
