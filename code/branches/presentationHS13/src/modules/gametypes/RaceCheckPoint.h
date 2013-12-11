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

#ifndef _RaceCheckPoint_H__
#define _RaceCheckPoint_H__

#include "gametypes/GametypesPrereqs.h"
#include "objects/triggers/DistanceMultiTrigger.h"
#include "interfaces/RadarViewable.h"

namespace orxonox
{
    /**
     @brief
     The RaceCheckPoint class enables the creation of a check point to use in a SpaceRace level.
     Don't forget to control the indexes of your check points and to set one last check point
     */
    class _GametypesExport RaceCheckPoint: public DistanceMultiTrigger,
            public RadarViewable
    {
        public:
            RaceCheckPoint(Context* context);
            virtual ~RaceCheckPoint();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

            inline void setCheckpointIndex(int checkpointIndex)
            {
                this->checkpointIndex_ = checkpointIndex;
            }
            inline int getCheckpointIndex() const
            {
                return this->checkpointIndex_;
            }

            void setNextCheckpointsAsVector3(const Vector3& checkpoints);
            Vector3 getNextCheckpointsAsVector3();
            Vector3 getVirtualNextCheckpointsAsVector3() const;
            void setNextVirtualCheckpointsAsVector3(const Vector3& checkpoints);
            int changeVirtualToRealCheckPoint(int);

            const std::set<int>& getVirtualNextCheckpoints() const
            {
                return this->nextCheckpointsVirtual_;
            }

            std::set<int> getNextCheckpoints()
            {
                return nextCheckpoints_;
                std::set<int> temp;
                std::set<int> temp2=getVirtualNextCheckpoints();
                for (std::set<int>::iterator it = temp2.begin(); it!=temp2.end(); ++it){
                    temp.insert(changeVirtualToRealCheckPoint((*it)));
                }
                return temp;
            }
            inline void setLast(bool isLast)
            {
                this->bIsLast_ = isLast;
            }
            inline bool isLast() const
            {
                return this->bIsLast_;
            }

            virtual void setTimelimit(float timeLimit);
            inline float getTimeLimit() const
            {
                return this->timeLimit_;
            }

            PlayerInfo* getPlayer(unsigned int clientID) const;

            bool playerWasHere(PlayerInfo* ) const;

            inline void resetPlayer()
            {
                this->players_.clear();
            }

        protected:

            virtual void fire(bool bIsTriggered, BaseObject* originator);

            inline const WorldEntity* getWorldEntity() const
            {
                return this;
            }

        private:

            int checkpointIndex_; ///< The index of this check point. The race starts with the check point with the index 0
            std::set<int> nextCheckpoints_; ///< the indexes of the next check points
            bool bIsLast_; ///< True if this check point is the last of the level. There can be only one last check point for each level and there must be a last check point in the level.
            float timeLimit_; ///< The time limit (from the start of the level) to reach this check point. If the check point is reached after this time, the game ends and the player looses.
            std::vector<PlayerInfo*> players_; ///< The player that reached the checkpoint
            Vector3 myPosition_;
            std::set<int>  nextCheckpointsVirtual_;
            std::map<int,int> virtualToRealCheckPoints_; // if virtualChepoint was inserted the original can be reconstructed
    };
}

#endif /* _RaceCheckPoint_H__ */
