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
 *  Created on: Oct 8, 2012
 *      Author: purgham
 */

#include <gametypes/SpaceRaceController.h>
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "gametypes/SpaceRaceManager.h"

// Von SpaceRaceManager points einlesen
// Berechnungsklasse nextPoint zur verfuegung stellen
// ^- aufrufen ueber tick (if ob noetig)
namespace orxonox
{
    CreateFactory(SpaceRaceController);

    /*
     * Idea: Find static Point (checkpoints the spaceship has to reach)
     */
    SpaceRaceController::SpaceRaceController(BaseObject* creator) :
        ArtificialController(creator)
    {
        RegisterObject(SpaceRaceController);

        std::vector<RaceCheckPoint*> checkpoints;
        for (ObjectList<SpaceRaceManager>::iterator it = ObjectList<SpaceRaceManager>::begin(); it!= ObjectList<SpaceRaceManager>::end(); ++it)
        {
            checkpoints = it->getAllCheckpoints();
            nextRaceCheckpoint_=it->findCheckpoint(1);
        }

        OrxAssert(!checkpoints.empty(), "No Checkpoints in Level");
        checkpoints_=checkpoints;
        staticRacePoints_ = findStaticCheckpoints(checkpoints);

    }

    int SpaceRaceController::distanceSpaceshipToCheckPoint(RaceCheckPoint* CheckPoint)
    {
        if (this->getControllableEntity() != NULL)
        {
            return (CheckPoint->getPosition()- this->getControllableEntity()->getPosition()).length();
        }
        return -1;
    }

    RaceCheckPoint* SpaceRaceController::nextPointFind(RaceCheckPoint* raceCheckpoint)
    {
        int distances[] = { -1, -1, -1 };
        int temp_i = 0;
        for (std::set<int>::iterator it =raceCheckpoint->getNextCheckpoints().begin(); it!= raceCheckpoint->getNextCheckpoints().end(); ++it)
        {
            distances[temp_i] = recCalculateDistance(raceCheckpoint,this->getControllableEntity()->getPosition());
            temp_i++;
        }
        if (distances[0] > distances[1] && distances[1] != -1)
        {
            if (distances[2] < distances[1] && distances[2] != -1)
            {
                return checkpoints_[*raceCheckpoint->getNextCheckpoints().end()]; // return [2]
            }
            else
            {
                std::set<int>::iterator temp = raceCheckpoint->getNextCheckpoints().begin();
                return checkpoints_[*(++temp)];
            }
        }
        else
        {
            if (distances[2] < distances[0] && distances[2] != -1)
            {
                return checkpoints_[*raceCheckpoint->getNextCheckpoints().end()]; // return [2]
            }
            else
            {
                return checkpoints_[*raceCheckpoint->getNextCheckpoints().begin()]; // return [2]
            }
        }
    }

    RaceCheckPoint* SpaceRaceController::adjustNextPoint()
    {
        if (currentRaceCheckpoint_ == NULL) // no Adjust possible
        {
            return nextRaceCheckpoint_;
        }
        if ((currentRaceCheckpoint_->getNextCheckpoints()).size() == 1) // no Adjust possible
        {
            return nextRaceCheckpoint_;
        }

        //Adjust possible

        return nextPointFind(currentRaceCheckpoint_);
    }

    int SpaceRaceController::recCalculateDistance(RaceCheckPoint* currentCheckPoint, Vector3 currentPosition)
    {
        // if ( staticCheckPoint was reached)
        if (std::find(staticRacePoints_.begin(), staticRacePoints_.end(),currentCheckPoint) != staticRacePoints_.end())
        {
            return (currentCheckPoint->getPosition() - currentPosition).length();
        }
        else
        {
            int minimum = std::numeric_limits<int>::max();
            for (std::set<int>::iterator it = currentCheckPoint->getNextCheckpoints().begin(); it!= currentCheckPoint->getNextCheckpoints().end(); ++it)
            {
                minimum= std::min(minimum,(int) (currentPosition- currentCheckPoint->getPosition()).length() + recCalculateDistance(checkpoints_[(*it)], currentCheckPoint->getPosition()));
            }//TODO: fix cast
            return minimum;
        }
    }

    /*
     * returns a vector of static Point (checkpoints the spaceship has to reach)
     */
    std::vector<RaceCheckPoint*> SpaceRaceController::findStaticCheckpoints(
            std::vector<RaceCheckPoint*> allCheckpoints)
    {
        std::map<RaceCheckPoint*, int> * zaehler = new std::map<
                RaceCheckPoint*, int>(); // counts how many times the checkpoit was reached (for simulation)
        for (unsigned int i = 0; i < allCheckpoints.size(); i++)
        {
            zaehler->insert(std::pair<RaceCheckPoint*, int>(allCheckpoints[i],0));
        }
        int maxWays = rekSimulationCheckpointsReached(zaehler->begin()->first,&allCheckpoints, zaehler);

        std::vector<RaceCheckPoint*> returnVec;
        returnVec.clear();
        for (std::map<RaceCheckPoint*, int>::iterator iter = zaehler->begin(); iter!= zaehler->end(); iter++)
        {
            if (iter->second == maxWays)
            {
                //returnVec.insert(allCheckpoints[1]);
                returnVec.insert(returnVec.end(), iter->first);
            }
        }
        delete zaehler;
        return returnVec;
    }

    /*
     *
     * return how many ways go from the given checkpoint to the last one
     */
    int SpaceRaceController::rekSimulationCheckpointsReached(RaceCheckPoint* currentCheckpoint, std::vector<RaceCheckPoint*>* checkpoints, std::map<RaceCheckPoint*, int>* zaehler)
    {
        if (currentCheckpoint->isLast())
        {// last point reached
            (*zaehler)[currentCheckpoint] += 1;
            return 1; // 1 Way form the last point to this one
        }
        else
        {
            int numberOfWays = 0; // counts number of ways from this Point to the last point
            for (std::set<int>::iterator it =
                    currentCheckpoint->getNextCheckpoints().begin(); it
                    != currentCheckpoint->getNextCheckpoints().end(); ++it)
            {
                numberOfWays += rekSimulationCheckpointsReached((*checkpoints)[(*it)], checkpoints, zaehler);
            }
            (*zaehler)[currentCheckpoint] += numberOfWays;
            return numberOfWays; // returns the number of ways from this point to the last one
        }
    }

    SpaceRaceController::~SpaceRaceController()
    {
        // TODO Auto-generated destructor stub
    }

    void SpaceRaceController::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceRaceController, XMLPort, xmlelement, mode);
        XMLPortParam(ArtificialController, "accuracy", setAccuracy, getAccuracy, xmlelement, mode).defaultValues(
                100.0f);
        XMLPortObject(ArtificialController, WorldEntity, "waypoints", addWaypoint, getWaypoint, xmlelement, mode)
        ;

    }
    void SpaceRaceController::tick(float dt)
    {
        if (nextRaceCheckpoint_->playerWasHere(this->getControllableEntity()->getPlayer()))
        {//Checkpoint erreicht
            currentRaceCheckpoint_=nextRaceCheckpoint_;
            OrxAssert(nextRaceCheckpoint_, "next race checkpoint undefined");
            nextRaceCheckpoint_ = nextPointFind(nextRaceCheckpoint_);
        }
        else if (std::abs(lastDistance - distanceSpaceshipToCheckPoint(nextRaceCheckpoint_)) < 500)
        {
            nextRaceCheckpoint_ = adjustNextPoint();
        }
        this->moveToPosition(nextRaceCheckpoint_->getPosition());
    }

}
