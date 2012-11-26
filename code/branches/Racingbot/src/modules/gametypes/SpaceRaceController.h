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
 *
 *  Created on: Oct 8, 2012
 *      Author: purgham
 */

#ifndef SPACERACECONTROLLER_H_
#define SPACERACECONTROLLER_H_

#include "gametypes/GametypesPrereqs.h"
#include "controllers/ArtificialController.h"
#include "gametypes/Gametype.h"
#include "gametypes/RaceCheckPoint.h"


namespace orxonox
{
    class _GametypesExport SpaceRaceController : public ArtificialController, public Tickable
    {
        private:
            std::vector<RaceCheckPoint*> staticRacePoints_;
            RaceCheckPoint* nextRaceCheckpoint_;    // checkpoint that should be reached
            RaceCheckPoint* currentRaceCheckpoint_; // last checkPoint (already reached)
            std::vector<RaceCheckPoint*> checkpoints_;
            Vector3 lastPositionSpaceship;
            int virtualCheckPointIndex;

            int recCalculateDistance(RaceCheckPoint*, Vector3 currentPosition);
            int distanceSpaceshipToCheckPoint(RaceCheckPoint*);
            RaceCheckPoint* nextPointFind(RaceCheckPoint*);
            RaceCheckPoint* adjustNextPoint();
            std::vector<RaceCheckPoint*> findStaticCheckpoints(std::vector<RaceCheckPoint*>);
            std::vector<RaceCheckPoint*> staticCheckpoints();
            int rekSimulationCheckpointsReached(RaceCheckPoint* , std::map< RaceCheckPoint*, int>*);
            // same as SpaceRaceManager, but needed to add virtuell Checkpoints ( Checkpoints which don't exist but needed to avoid collisions with big Objects)
            RaceCheckPoint* findCheckpoint(int index) const;
            bool addVirtualCheckPoint(int , RaceCheckPoint* , int , Vector3 );
            int pointToPointDistance(Vector3, Vector3);

        public:
          SpaceRaceController(BaseObject* creator);
          virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
          virtual ~SpaceRaceController();
          virtual void tick(float dt);
    };

}


#endif /* SPACERACECONTROLLER_H_ */
