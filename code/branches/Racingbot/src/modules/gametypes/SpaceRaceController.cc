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

/**
 * Conventions:
 * -first Checkpoint has index 0
 * -staticCheckPoint= static Point (see def over = constructor)
 */


/*TODO:
 * tICK KORRIGIEREN
 *
 *
 */
#include <gametypes/SpaceRaceController.h>
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "gametypes/SpaceRaceManager.h"
#include "collisionshapes/CollisionShape.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"

namespace orxonox
{
    CreateFactory(SpaceRaceController);

    const int ADJUSTDISTANCE = 500;
    const int MINDISTANCE=5;
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
            nextRaceCheckpoint_=it->findCheckpoint(0);
        }

        OrxAssert(!checkpoints.empty(), "No Checkpoints in Level");
        checkpoints_=checkpoints;
        for( std::vector<RaceCheckPoint*>::iterator it = checkpoints.begin(); it!=checkpoints.end(); ++it){
            for (std::set<int>::iterator numb = ((*it)->getNextCheckpoints()).begin(); numb!=((*it)->getNextCheckpoints()).end(); ++numb)
                placeVirtualCheckpoints((*it), findCheckpoint((*numb)));
        }
        staticRacePoints_ = findStaticCheckpoints(checkpoints);
        // initialisation of currentRaceCheckpoint_
        currentRaceCheckpoint_ = NULL;
        /*
         // find first Checkpoint
         for (int i=0; true; i++){
         if(checkpoints_[i]->getCheckpointIndex()==0){
         nextRaceCheckpoint_=checkpoints_[i];
         break;
         }
         }*/

        virtualCheckPointIndex=-1;
    }


    //------------------------------
    // functions for initialisation

    void SpaceRaceController::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceRaceController, XMLPort, xmlelement, mode);
        XMLPortParam(ArtificialController, "accuracy", setAccuracy, getAccuracy, xmlelement, mode).defaultValues(100.0f);
        XMLPortObject(ArtificialController, WorldEntity, "waypoints", addWaypoint, getWaypoint, xmlelement, mode);

    }

    /*
     * called from constructor 'SpaceRaceController'
     * returns a vector of static Point (checkpoints the spaceship has to reach)
     */
    std::vector<RaceCheckPoint*> SpaceRaceController::findStaticCheckpoints(std::vector<RaceCheckPoint*> allCheckpoints)
    {
        std::map<RaceCheckPoint*, int> * zaehler = new std::map<
        RaceCheckPoint*, int>(); // counts how many times the checkpoit was reached (for simulation)
        for (unsigned int i = 0; i < allCheckpoints.size(); i++)
        {
            zaehler->insert(std::pair<RaceCheckPoint*, int>(allCheckpoints[i],0));
        }
        int maxWays = rekSimulationCheckpointsReached(zaehler->begin()->first, zaehler);

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
     * called from 'findStaticCheckpoints'
     * return how many ways go from the given Checkpoint to the last Checkpoint (of the Game)
     */
    int SpaceRaceController::rekSimulationCheckpointsReached(RaceCheckPoint* currentCheckpoint, std::map<RaceCheckPoint*, int>* zaehler)
    {
        if (currentCheckpoint->isLast())
        {// last point reached
            (*zaehler)[currentCheckpoint] += 1;
            return 1; // 1 Way form the last point to this one
        }
        else
        {
            int numberOfWays = 0; // counts number of ways from this Point to the last point
            for (std::set<int>::iterator it = currentCheckpoint->getNextCheckpoints().begin(); it!= currentCheckpoint->getNextCheckpoints().end(); ++it)
            {
                numberOfWays += rekSimulationCheckpointsReached(findCheckpoint(*it), zaehler);
            }
            (*zaehler)[currentCheckpoint] += numberOfWays;
            return numberOfWays; // returns the number of ways from this point to the last one
        }
    }



    //-------------------------------------
    // functions for dynamic Way-search

    int SpaceRaceController::distanceSpaceshipToCheckPoint(RaceCheckPoint* CheckPoint)
    {
        if (this->getControllableEntity() != NULL)
        {
            return (CheckPoint->getPosition()- this->getControllableEntity()->getPosition()).length();
        }
        return -1;
    }

    /*
     * called by: 'tick' or  'adjustNextPoint'
     * returns the next Checkpoint which the shortest way contains
     */
    RaceCheckPoint* SpaceRaceController::nextPointFind(RaceCheckPoint* raceCheckpoint)
    {
        int distances[] = {   -1, -1, -1};
        int temp_i = 0;
        for (std::set<int>::iterator it =raceCheckpoint->getNextCheckpoints().begin(); it!= raceCheckpoint->getNextCheckpoints().end(); ++it)
        {
            distances[temp_i] = recCalculateDistance(findCheckpoint(*it), this->getControllableEntity()->getPosition());
            temp_i++;
        }
        if (distances[0] > distances[1] && distances[1] != -1)
        {
            if (distances[2] < distances[1] && distances[2] != -1)
            {
                return findCheckpoint(*raceCheckpoint->getNextCheckpoints().end()); // return checkpoint with ID of raceCheckpoint->getNextCheckpoints() [2]
            }
            else
            {
                std::set<int>::iterator temp = raceCheckpoint->getNextCheckpoints().begin();
                return findCheckpoint(*(++temp)); // return [1]
            }
        }
        else
        {
            if (distances[2] < distances[0] && distances[2] != -1)
            {
                return findCheckpoint(*raceCheckpoint->getNextCheckpoints().end()); // return [2]
            }
            else
            {
                return findCheckpoint(*raceCheckpoint->getNextCheckpoints().begin()); // return [0]
            }
        }
    }

    /*
     * called from 'nextPointFind'
     * returns the distance between "currentPosition" and the next static checkpoint that can be reached from "currentCheckPoint"
     */
    int SpaceRaceController::recCalculateDistance(RaceCheckPoint* currentCheckPoint, Vector3 currentPosition)
    {
        // find: looks if the currentCheckPoint is a staticCheckPoint (staticCheckPoint is the same as: static Point)
        if (std::find(staticRacePoints_.begin(), staticRacePoints_.end(), currentCheckPoint) != staticRacePoints_.end())
        {
            return (currentCheckPoint->getPosition() - currentPosition).length();
        }
        else
        {
            int minimum = std::numeric_limits<int>::max();
            for (std::set<int>::iterator it = currentCheckPoint->getNextCheckpoints().begin(); it!= currentCheckPoint->getNextCheckpoints().end(); ++it)
            {
                int dist_currentCheckPoint_currentPosition = static_cast<int> ((currentPosition- currentCheckPoint->getPosition()).length());

                minimum= std::min(minimum, dist_currentCheckPoint_currentPosition + recCalculateDistance(findCheckpoint(*it), currentCheckPoint->getPosition()));
                // minimum of distanz from 'currentPosition' to the next static Checkpoint
            }
            return minimum;
        }
    }

    /*called by 'tick'
     *adjust chosen way of the Spaceship every "AdjustDistance" because spaceship could be displaced through an other one
     */
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

    RaceCheckPoint* SpaceRaceController::findCheckpoint(int index) const
       {
           for (size_t i = 0; i < this->checkpoints_.size(); ++i)
           if (this->checkpoints_[i]->getCheckpointIndex() == index)
           return this->checkpoints_[i];
           return NULL;
       }

    RaceCheckPoint* SpaceRaceController::addVirtualCheckPoint( RaceCheckPoint* previousCheckpoint, int indexFollowingCheckPoint , Vector3 virtualCheckPointPosition ){

        RaceCheckPoint* newTempRaceCheckPoint = new RaceCheckPoint(this);
        newTempRaceCheckPoint->setPosition(virtualCheckPointPosition);
        newTempRaceCheckPoint->setCheckpointIndex(virtualCheckPointIndex);
        newTempRaceCheckPoint->setLast(false);
        newTempRaceCheckPoint->setNextCheckpointsAsVector3(Vector3(indexFollowingCheckPoint,-1,-1));

        Vector3 temp = previousCheckpoint->getNextCheckpointsAsVector3();
        checkpoints_.insert(checkpoints_.end(), newTempRaceCheckPoint);
        int positionInNextCheckPoint;
        for (int i = 0; i <3 ; i++){
            if(previousCheckpoint->getNextCheckpointsAsVector3()[i]==indexFollowingCheckPoint)
                positionInNextCheckPoint=i;
        }
        switch(positionInNextCheckPoint){
            case 0: temp.x=virtualCheckPointIndex; break;
            case 1: temp.y=virtualCheckPointIndex; break;
            case 2: temp.z=virtualCheckPointIndex; break;
        }
        previousCheckpoint->setNextCheckpointsAsVector3(temp);
        virtualCheckPointIndex--;
        return newTempRaceCheckPoint;
    }



    SpaceRaceController::~SpaceRaceController()
    {
        for (int i =-1; i>virtualCheckPointIndex ; i--){
            delete findCheckpoint(i);
        }
    }

    void SpaceRaceController::tick(float dt)
    {
        if (this->getControllableEntity() == NULL || this->getControllableEntity()->getPlayer() == NULL )
        {   orxout()<<this->getControllableEntity()<< " in tick"<<endl; return;}
        //FOR virtual Checkpoints
        if(nextRaceCheckpoint_->getCheckpointIndex() < 0){
            if( distanceSpaceshipToCheckPoint(nextRaceCheckpoint_) < 30){
                currentRaceCheckpoint_=nextRaceCheckpoint_;
                nextRaceCheckpoint_ = nextPointFind(nextRaceCheckpoint_);
                lastPositionSpaceship=this->getControllableEntity()->getPosition();
            }
        }

        if (nextRaceCheckpoint_->playerWasHere(this->getControllableEntity()->getPlayer()))
        {//Checkpoint erreicht
            currentRaceCheckpoint_=nextRaceCheckpoint_;
            OrxAssert(nextRaceCheckpoint_, "next race checkpoint undefined");
            nextRaceCheckpoint_ = nextPointFind(nextRaceCheckpoint_);
            lastPositionSpaceship=this->getControllableEntity()->getPosition();
        }
        else if ((lastPositionSpaceship-this->getControllableEntity()->getPosition()).length()/dt > ADJUSTDISTANCE)
        {
            nextRaceCheckpoint_ = adjustNextPoint();
            lastPositionSpaceship=this->getControllableEntity()->getPosition();
        }
        //TODO: korrigieren!
        else if((lastPositionSpaceship-this->getControllableEntity()->getPosition()).length()/dt< MINDISTANCE  ){
            this->moveToPosition(Vector3(rnd()*100,rnd()*100,rnd()*100));
            this->spin();
            //orxout(user_status) << "Mindistance reached" << std::endl;
            return;
        }
        //orxout(user_status) << "dt= " << dt << ";  distance= " << (lastPositionSpaceship-this->getControllableEntity()->getPosition()).length() <<std::endl;
        lastPositionSpaceship=this->getControllableEntity()->getPosition();
        this->moveToPosition(nextRaceCheckpoint_->getPosition());
    }

    // True if a coordinate of 'pointToPoint' is smaller then the corresponding coordinate of 'groesse'
    bool SpaceRaceController::vergleicheQuader(Vector3 pointToPoint, Vector3 groesse){
        if(abs(pointToPoint.x)<groesse.x)
            return true;
        if(abs(pointToPoint.y)<groesse.y)
                    return true;
        if(abs(pointToPoint.z)<groesse.z)
                    return true;

    }

    void SpaceRaceController::placeVirtualCheckpoints(RaceCheckPoint* racepoint1, RaceCheckPoint* racepoint2){
        Vector3 point1 = racepoint1->getPosition();
        Vector3 point2 = racepoint2->getPosition();
        std::vector<StaticEntity*> problematicObjects;

        for (ObjectList<StaticEntity>::iterator it = ObjectList<StaticEntity>::begin(); it!= ObjectList<StaticEntity>::end(); ++it)
                        {

                            if (dynamic_cast<RaceCheckPoint*>(*it)!=NULL){continue;} // does not work jet

                            problematicObjects.insert(problematicObjects.end(), *it);
                            //it->getScale3D();// vector fuer halbe wuerfellaenge
                        }
        Vector3 richtungen [6];
        richtungen[0]= Vector3(1,0,0);
        richtungen[1]= Vector3(-1,0,0);
        richtungen[2]= Vector3(0,1,0);
        richtungen[3]= Vector3(0,-1,0);
        richtungen[4]= Vector3(0,0,1);
        richtungen[5]= Vector3(0,0,-1);

        for (int i = 0; i< 6; i++){
            const int STEPS=100;
            const float PHI=1.1;
            bool collision=false;

            for (int j =0; j<STEPS; j++){
                Vector3 tempPosition=(point1 - (point2-point1+richtungen[i]*PHI)*(float)j/STEPS);
                for (std::vector<StaticEntity*>::iterator it = problematicObjects.begin(); it!=problematicObjects.end(); ++it){
                    btVector3 positionObject;
                    btScalar radiusObject;
                    //TODO: Probably it points on a wrong object
                    for (int everyShape=0; (*it)->getAttachedCollisionShape(everyShape)!=0; everyShape++){
                        (*it)->getAttachedCollisionShape(everyShape)->getCollisionShape()->getBoundingSphere(positionObject,radiusObject);
                        Vector3 positionObjectNonBT(positionObject.x(), positionObject.y(), positionObject.z());
                        if (((tempPosition - positionObjectNonBT).length()<radiusObject) && (vergleicheQuader((tempPosition-positionObjectNonBT),(*it)->getScale3D()))){
                            collision=true; break;
                        }
                    }
                    if(collision) break;
                }
                if(collision)break;
            }
            if(collision) continue;
            // no collision => possible Way
            for (float j =0; j<STEPS; j++){
                Vector3 possiblePosition=(point1 - (point2-point1+richtungen[i]*PHI)*j/STEPS);
                collision=false;
                for(int ij=0; ij<STEPS; j++){
                    Vector3 tempPosition=(possiblePosition - (point2-possiblePosition)*(float)ij/STEPS);
                            for (std::vector<StaticEntity*>::iterator it = problematicObjects.begin(); it!=problematicObjects.end(); ++it){
                                btVector3 positionObject;
                                btScalar radiusObject;
                                for (int everyShape=0; (*it)->getAttachedCollisionShape(everyShape)!=0; everyShape++){
                                    (*it)->getAttachedCollisionShape(everyShape)->getCollisionShape()->getBoundingSphere(positionObject,radiusObject);
                                    Vector3 positionObjectNonBT(positionObject.x(), positionObject.y(), positionObject.z());
                                    if (((tempPosition-positionObjectNonBT).length()<radiusObject) && (vergleicheQuader((tempPosition-positionObjectNonBT),(*it)->getScale3D()))){
                                        collision=true; break;
                                    }
                                }
                            if(collision) break;
                            }
                    if(collision)break;
                    addVirtualCheckPoint(racepoint1, racepoint2->getCheckpointIndex(), possiblePosition);
                    return;
                }

            }
        }


    }
}
