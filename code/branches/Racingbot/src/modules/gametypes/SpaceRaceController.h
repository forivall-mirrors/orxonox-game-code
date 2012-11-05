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
            RaceCheckPoint* nextRaceCheckpoint_;
            RaceCheckPoint* currentRaceCheckpoint_;
            std::vector<RaceCheckPoint*> checkpoints_;
            int lastDistance;

            int recCalculateDistance(RaceCheckPoint*, Vector3 currentPosition);
            int distanceSpaceshipToCheckPoint(RaceCheckPoint*);
            RaceCheckPoint* nextPointFind(RaceCheckPoint*);
            RaceCheckPoint* adjustNextPoint();
            std::vector<RaceCheckPoint*> findStaticCheckpoints(std::vector<RaceCheckPoint*>);
            std::vector<RaceCheckPoint*> staticCheckpoints();
            int rekSimulationCheckpointsReached(RaceCheckPoint* , std::vector<RaceCheckPoint*>* checkpoints, std::map< RaceCheckPoint*, int>*);

        public:
          SpaceRaceController(BaseObject* creator);
          virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
          virtual ~SpaceRaceController();
          virtual void tick(float dt);
    };

}


#endif /* SPACERACECONTROLLER_H_ */
