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
    const int MINDISTANCE = 5;
    /*
     * Idea: Find static Point (checkpoints the spaceship has to reach)
     */
    SpaceRaceController::SpaceRaceController(BaseObject* creator) :
        ArtificialController(creator)
    {
        RegisterObject(SpaceRaceController)
;        std::vector<RaceCheckPoint*> checkpoints;

        virtualCheckPointIndex = -2;
        for (ObjectList<SpaceRaceManager>::iterator it = ObjectList<SpaceRaceManager>::begin(); it != ObjectList<SpaceRaceManager>::end(); ++it)
        {
            checkpoints = it->getAllCheckpoints();
            nextRaceCheckpoint_ = it->findCheckpoint(0);
        }

        OrxAssert(!checkpoints.empty(), "No Checkpoints in Level");
        checkpoints_ = checkpoints;
        /*orxout()<<"es gibt: "<<checkpoints_.size()<<"checkpoints"<<endl;
        for(std::vector<RaceCheckPoint*>::iterator it=checkpoints_.begin(); it!=checkpoints_.end(); it++)
        {
            orxout()<<"Checkpoint "<<(*it)->getCheckpointIndex()<<"; NExtReal: ";
            std::set<int> temp =(*it)->getNextCheckpoints();
            for (std::set<int>::iterator ii =temp.begin(); ii!=temp.end(); ii++)
            {
                orxout()<<(*ii)<<", ";
            }

            orxout()<<" NextVirtual: ";
            temp=(*it)->getVirtualNextCheckpoints();
            for (std::set<int>::iterator ii =temp.begin(); ii!=temp.end(); ii++)
            {
                orxout()<<(*ii)<<", ";
            }
            orxout()<<endl<<endl;

        }//ausgabe*/

        for (std::vector<RaceCheckPoint*>::iterator it = checkpoints.begin(); it != checkpoints.end(); ++it)
        {
            std::set<int> nextCheckPoints = ((*it)->getNextCheckpoints());
            if(!nextCheckPoints.empty())
            {
                for (std::set<int>::iterator numb = nextCheckPoints.begin(); numb!=nextCheckPoints.end(); numb++)
                {
                    RaceCheckPoint* point2 = findCheckpoint((*numb));

                    if(point2 != NULL)
                    placeVirtualCheckpoints((*it), point2);
                }
            }
        }/*
        for(std::vector<RaceCheckPoint*>::iterator it=checkpoints_.begin(); it!=checkpoints_.end(); it++)
        {
            orxout()<<"Checkpoint "<<(*it)->getCheckpointIndex()<<"; NExtReal: ";
            std::set<int> temp =(*it)->getNextCheckpoints();
            for (std::set<int>::iterator ii =temp.begin(); ii!=temp.end(); ii++)
            {
                orxout()<<(*ii)<<", ";
            }

            orxout()<<" NextVirtual: ";
            temp=(*it)->getVirtualNextCheckpoints();
            for (std::set<int>::iterator ii =temp.begin(); ii!=temp.end(); ii++)
            {
                orxout()<<(*ii)<<", ";
            }
            orxout()<<endl;

        }//ausgabe
        orxout()<<"es gibt: "<<checkpoints_.size()<<"checkpoints"<<endl;*/
        staticRacePoints_ = findStaticCheckpoints(checkpoints);
        // initialisation of currentRaceCheckpoint_
        currentRaceCheckpoint_ = NULL;

        int i;
        for (i = -2; findCheckpoint(i) != NULL; i--)
        {
            continue;
        }
        //orxout()<<"Die ANzahl der virtuellen CP betraegt: "<< (-i)-2<<endl;

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
        std::map<RaceCheckPoint*, int> * zaehler = new std::map<RaceCheckPoint*, int>(); // counts how many times the checkpoint was reached (for simulation)
        for (unsigned int i = 0; i < allCheckpoints.size(); i++)
        {
            zaehler->insert(std::pair<RaceCheckPoint*, int>(allCheckpoints[i],0));
        }
        int maxWays = rekSimulationCheckpointsReached(zaehler->begin()->first, zaehler);

        std::vector<RaceCheckPoint*> returnVec;
        returnVec.clear();
        for (std::map<RaceCheckPoint*, int>::iterator iter = zaehler->begin(); iter != zaehler->end(); iter++)
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
            for (std::set<int>::iterator it = currentCheckpoint->getVirtualNextCheckpoints().begin(); it!= currentCheckpoint->getVirtualNextCheckpoints().end(); ++it)
            {
                if(currentCheckpoint == findCheckpoint(*it))
                {
                    //orxout() << currentCheckpoint->getCheckpointIndex()<<endl;
                    continue;
                }
                if(findCheckpoint(*it) == NULL)
                    {orxout()<<"Problematic Point: "<<(*it)<<endl;}
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
        int distances[] = {-1, -1, -1};
        int temp_i = 0;
        for (std::set<int>::iterator it =raceCheckpoint->getVirtualNextCheckpoints().begin(); it!= raceCheckpoint->getVirtualNextCheckpoints().end(); ++it)
        {
            distances[temp_i] = recCalculateDistance(findCheckpoint(*it), this->getControllableEntity()->getPosition());
            temp_i++;
        }
        if (distances[0] > distances[1] && distances[1] != -1)
        {
            if (distances[2] < distances[1] && distances[2] != -1)
            {
                return findCheckpoint(*raceCheckpoint->getVirtualNextCheckpoints().end()); // return checkpoint with ID of raceCheckpoint->getNextCheckpoints() [2]
            }
            else
            {
                std::set<int>::iterator temp = raceCheckpoint->getVirtualNextCheckpoints().begin();
                return findCheckpoint(*(++temp)); // return [1]
            }
        }
        else
        {
            if (distances[2] < distances[0] && distances[2] != -1)
            {
                return findCheckpoint(*raceCheckpoint->getVirtualNextCheckpoints().end()); // return [2]
            }
            else
            {
                return findCheckpoint(*raceCheckpoint->getVirtualNextCheckpoints().begin()); // return [0]
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
            for (std::set<int>::iterator it = currentCheckPoint->getVirtualNextCheckpoints().begin(); it != currentCheckPoint->getVirtualNextCheckpoints().end(); ++it)
            {
                int dist_currentCheckPoint_currentPosition = static_cast<int> ((currentPosition- currentCheckPoint->getPosition()).length());

                minimum = std::min(minimum, dist_currentCheckPoint_currentPosition + recCalculateDistance(findCheckpoint(*it), currentCheckPoint->getPosition()));
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
        if ((currentRaceCheckpoint_->getVirtualNextCheckpoints()).size() == 1) // no Adjust possible

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

    RaceCheckPoint* SpaceRaceController::addVirtualCheckPoint( RaceCheckPoint* previousCheckpoint, int indexFollowingCheckPoint , Vector3 virtualCheckPointPosition )
    {
        orxout()<<"add VCP at"<<virtualCheckPointPosition.x<<", "<<virtualCheckPointPosition.y<<", "<<virtualCheckPointPosition.z<<endl;
        RaceCheckPoint* newTempRaceCheckPoint;
        for (ObjectList<SpaceRaceManager>::iterator it = ObjectList<SpaceRaceManager>::begin(); it!= ObjectList<SpaceRaceManager>::end(); ++it)
        {
            newTempRaceCheckPoint = new RaceCheckPoint((*it));
        }
        newTempRaceCheckPoint->setVisible(false);
        newTempRaceCheckPoint->setPosition(virtualCheckPointPosition);
        newTempRaceCheckPoint->setCheckpointIndex(virtualCheckPointIndex);
        newTempRaceCheckPoint->setLast(false);
        newTempRaceCheckPoint->setNextVirtualCheckpointsAsVector3(Vector3(indexFollowingCheckPoint,-1,-1));

        Vector3 temp = previousCheckpoint->getVirtualNextCheckpointsAsVector3();
        //orxout()<<"temp bei 0: ="<< temp.x<< temp.y<< temp.z<<endl;
        checkpoints_.insert(checkpoints_.end(), newTempRaceCheckPoint);
        int positionInNextCheckPoint;
        for (int i = 0; i <3; i++)
        {
            if(previousCheckpoint->getVirtualNextCheckpointsAsVector3()[i] == indexFollowingCheckPoint)
            positionInNextCheckPoint=i;
        }
        switch(positionInNextCheckPoint)
        {
            case 0: temp.x=virtualCheckPointIndex; break;
            case 1: temp.y=virtualCheckPointIndex; break;
            case 2: temp.z=virtualCheckPointIndex; break;
        }
        previousCheckpoint->setNextVirtualCheckpointsAsVector3(temp); //Existiert internes Problem bei negativen index fueer next Checkpoint
        virtualCheckPointIndex--;
        //orxout()<<"temp bei 1: ="<< temp.x<< temp.y<< temp.z<<endl;
        //orxout()<<"temp nach ausgabe: "<<previousCheckpoint->getVirtualNextCheckpointsAsVector3().x<<previousCheckpoint->getVirtualNextCheckpointsAsVector3().y<<previousCheckpoint->getVirtualNextCheckpointsAsVector3().z<<endl;
        //OrxAssert(virtualCheckPointIndex < -1, "TO much virtual cp");
        /*orxout()<<"id: "<< previousCheckpoint->getCheckpointIndex() <<", following:"<<indexFollowingCheckPoint<<" :       "<<temp.x<<", "<<temp.y<<", "<<temp.z<<";       ";
         temp=previousCheckpoint->getNextCheckpointsAsVector3();
         orxout()<<"id: "<< previousCheckpoint->getCheckpointIndex() <<":       "<<temp.x<<", "<<temp.y<<", "<<temp.z<<";       ";
         orxout()<<endl;*/
        return newTempRaceCheckPoint;
    }

    SpaceRaceController::~SpaceRaceController()
    {
        for (int i =-1; i>virtualCheckPointIndex; i--)
        {
            delete findCheckpoint(i);
        }
    }

    void SpaceRaceController::tick(float dt)
    {
        if (this->getControllableEntity() == NULL || this->getControllableEntity()->getPlayer() == NULL )
        {   orxout()<<this->getControllableEntity()<< " in tick"<<endl; return;}
        //FOR virtual Checkpoints
        if(nextRaceCheckpoint_->getCheckpointIndex() < 0)
        {
            if( distanceSpaceshipToCheckPoint(nextRaceCheckpoint_) < 200)
            {
                currentRaceCheckpoint_=nextRaceCheckpoint_;
                nextRaceCheckpoint_ = nextPointFind(nextRaceCheckpoint_);
                lastPositionSpaceship=this->getControllableEntity()->getPosition();
                //orxout()<< "CP "<< currentRaceCheckpoint_->getCheckpointIndex()<<" chanched to: "<< nextRaceCheckpoint_->getCheckpointIndex()<<endl;
            }
        }

        if (nextRaceCheckpoint_->playerWasHere(this->getControllableEntity()->getPlayer()))
        {//Checkpoint erreicht

            currentRaceCheckpoint_ = nextRaceCheckpoint_;
            OrxAssert(nextRaceCheckpoint_, "next race checkpoint undefined");
            nextRaceCheckpoint_ = nextPointFind(nextRaceCheckpoint_);
            lastPositionSpaceship = this->getControllableEntity()->getPosition();
            //orxout()<< "CP "<< currentRaceCheckpoint_->getCheckpointIndex()<<" chanched to: "<< nextRaceCheckpoint_->getCheckpointIndex()<<endl;
        }
        else if ((lastPositionSpaceship-this->getControllableEntity()->getPosition()).length()/dt > ADJUSTDISTANCE)
        {
            nextRaceCheckpoint_ = adjustNextPoint();
            lastPositionSpaceship = this->getControllableEntity()->getPosition();
        }

        // Abmessung fuer MINDISTANCE gut;

        else if((lastPositionSpaceship - this->getControllableEntity()->getPosition()).length()/dt < MINDISTANCE )
        {
            this->moveToPosition(Vector3(rnd()*100, rnd()*100, rnd()*100));
            this->spin();
            //orxout(user_status) << "Mindistance reached" << std::endl;
            return;
        }
        //orxout(user_status) << "dt= " << dt << ";  distance= " << (lastPositionSpaceship-this->getControllableEntity()->getPosition()).length() <<std::endl;
        lastPositionSpaceship = this->getControllableEntity()->getPosition();
        this->moveToPosition(nextRaceCheckpoint_->getPosition());
    }

    // True if a coordinate of 'pointToPoint' is smaller then the corresponding coordinate of 'groesse'
    bool SpaceRaceController::vergleicheQuader(Vector3 pointToPoint, Vector3 groesse)
    {
        if(abs(pointToPoint.x) < groesse.x)
            return true;
        if(abs(pointToPoint.y) < groesse.y)
            return true;
        if(abs(pointToPoint.z) < groesse.z)
            return true;

    }

    bool SpaceRaceController::directLinePossible(RaceCheckPoint* racepoint1, RaceCheckPoint* racepoint2,std::vector<StaticEntity*> allObjects)
    {

        Vector3 cP1ToCP2 = (racepoint2->getPosition() - racepoint1->getPosition()) / (racepoint2->getPosition() - racepoint1->getPosition()).length(); //unit Vector
        Vector3 centerCP1 = racepoint1->getPosition();
        btVector3 positionObject;
        btScalar radiusObject;

        for (std::vector<StaticEntity*>::iterator it = allObjects.begin(); it != allObjects.end(); ++it)
        {
            for (int everyShape=0; (*it)->getAttachedCollisionShape(everyShape) != 0; everyShape++)
            {
                btCollisionShape* currentShape = (*it)->getAttachedCollisionShape(everyShape)->getCollisionShape();
                if(currentShape == NULL)
                continue;

                currentShape->getBoundingSphere(positionObject,radiusObject);
                Vector3 positionObjectNonBT(positionObject.x(), positionObject.y(), positionObject.z());
                if((powf((cP1ToCP2.dotProduct(centerCP1-positionObjectNonBT)),2)-(centerCP1-positionObjectNonBT).dotProduct(centerCP1-positionObjectNonBT)+powf(radiusObject, 2))>0)
                {
                    return false;
                }

            }
        }
        return true;

    }

    void SpaceRaceController::computeVirtualCheckpoint(RaceCheckPoint* racepoint1, RaceCheckPoint* racepoint2, std::vector<StaticEntity*> allObjects)
    {
        Vector3 cP1ToCP2=(racepoint2->getPosition()-racepoint1->getPosition()) / (racepoint2->getPosition()-racepoint1->getPosition()).length(); //unit Vector
        Vector3 centerCP1=racepoint1->getPosition();
        btVector3 positionObject;
        btScalar radiusObject;

        for (std::vector<StaticEntity*>::iterator it = allObjects.begin(); it != allObjects.end(); ++it)
        {
            for (int everyShape=0; (*it)->getAttachedCollisionShape(everyShape) != 0; everyShape++)
            {
                btCollisionShape* currentShape = (*it)->getAttachedCollisionShape(everyShape)->getCollisionShape();
                if(currentShape == NULL)
                continue;

                currentShape->getBoundingSphere(positionObject,radiusObject);
                Vector3 positionObjectNonBT(positionObject.x(), positionObject.y(), positionObject.z());
				Vector3 norm_r_CP = cP1ToCP2.crossProduct(centerCP1-positionObjectNonBT);

				if(norm_r_CP.length == 0){
					Vector3 zufall;
                    do{
                        zufall=Vector3(rnd(),rnd(),rnd());//random
                    }while((zufall.crossProduct(cP1ToCP2)).length() == 0);
					norm_r_CP=zufall.crossProduct(cP1ToCP2);
				}
				Vector3 VecToVCP = norm_r_CP.crossProduct(cP1ToCP2);
				float distanzToCP1 = sqrt(powf(radiusObject,4)/(powf((centerCP1-positionObjectNonBT).length(), 2)-powf(radiusObject,2))+powf(radiusObject,2));
				float distanzToCP2 = sqrt(powf(radiusObject,4)/(powf((racepoint2->getPosition()-positionObjectNonBT).length(), 2)-powf(radiusObject,2))+powf(radiusObject,2));
				//TODO float distanz=max(distanzToCP1,distanzToCP2);
				float distanz = 0.0f; //TEMPORARY
				Vector3 newCheckpointPositionPos = positionObjectNonBT+(distanz*VecToVCP)/VecToVCP.length();
				Vector3 newCheckpointPositionNeg = positionObjectNonBT-(distanz*VecToVCP)/VecToVCP.length();
				if((newCheckpointPositionPos-centerCP1).length+(newCheckpointPositionPos-(centerCP1+cP1ToCP2)).length < (newCheckpointPositionNeg-centerCP1).length+(newCheckpointPositionNeg-(centerCP1+cP1ToCP2)).length){
					RaceCheckPoint* newVirtualCheckpoint = addVirtualCheckPoint(racepoint1,racepoint2->getCheckpointIndex(), newCheckpointPositionPos);
				}else{
					RaceCheckPoint* newVirtualCheckpoint = addVirtualCheckPoint(racepoint1,racepoint2->getCheckpointIndex(), newCheckpointPositionNeg);
				}
				return;
            }
        }

    }

    void SpaceRaceController::placeVirtualCheckpoints(RaceCheckPoint* racepoint1, RaceCheckPoint* racepoint2)
    {
        Vector3 point1 = racepoint1->getPosition();
        Vector3 point2 = racepoint2->getPosition();
        std::vector<StaticEntity*> problematicObjects;

        for (ObjectList<StaticEntity>::iterator it = ObjectList<StaticEntity>::begin(); it!= ObjectList<StaticEntity>::end(); ++it)
        {

            if (dynamic_cast<RaceCheckPoint*>(*it) != NULL)
            {   continue;} // does not work jet

            problematicObjects.insert(problematicObjects.end(), *it);
            //it->getScale3D();// vector fuer halbe wuerfellaenge
        }

        if(!directLinePossible(racepoint1, racepoint2, problematicObjects))
        {
            //orxout()<<"From "<<racepoint1->getCheckpointIndex()<<" to "<<racepoint2->getCheckpointIndex()<<"produces: "<< virtualCheckPointIndex<<endl;
            computeVirtualCheckpoint(racepoint1, racepoint2, problematicObjects);
        }

        //
        //        do{
        //            zufall=Vector3(rnd(),rnd(),rnd());//random
        //        }while((zufall.crossProduct(objectmiddle-racepoint1->getPosition())).length()==0);
        //
        //        Vector3 normalvec=zufall.crossProduct(objectmiddle-racepoint1->getPosition());
        //        // a'/b'=a/b => a' =b'*a/b
        //        float laengeNormalvec=(objectmiddle-racepoint1->getPosition()).length()/sqrt((objectmiddle-racepoint1->getPosition()).squaredLength()-radius*radius)*radius;
        //        addVirtualCheckPoint(racepoint1,racepoint2->getCheckpointIndex(), objectmiddle+normalvec/normalvec.length()*laengeNormalvec);

        //        Vector3 richtungen [6];
        //        richtungen[0]= Vector3(1,0,0);
        //        richtungen[1]= Vector3(-1,0,0);
        //        richtungen[2]= Vector3(0,1,0);
        //        richtungen[3]= Vector3(0,-1,0);
        //        richtungen[4]= Vector3(0,0,1);
        //        richtungen[5]= Vector3(0,0,-1);
        //
        //        for (int i = 0; i< 6; i++)
        //        {
        //            const int STEPS=100;
        //            const float PHI=1.1;
        //            bool collision=false;
        //
        //            for (int j =0; j<STEPS; j++)
        //            {
        //                Vector3 tempPosition=(point1 - (point2-point1+richtungen[i]*PHI)*(float)j/STEPS);
        //                for (std::vector<StaticEntity*>::iterator it = problematicObjects.begin(); it!=problematicObjects.end(); ++it)
        //                {
        //                    btVector3 positionObject;
        //                    btScalar radiusObject;
        //                    if((*it)==NULL)
        //                    {   orxout()<<"Problempoint 1.1"<<endl; continue;}
        //                    //TODO: Probably it points on a wrong object
        //                    for (int everyShape=0; (*it)->getAttachedCollisionShape(everyShape)!=0; everyShape++)
        //                    {
        //                        if((*it)->getAttachedCollisionShape(everyShape)->getCollisionShape()==NULL)
        //                        {    continue;}
        //
        //                        orxout()<<"Problempoint 2.1"<<endl;
        //                        (*it)->getAttachedCollisionShape(everyShape)->getCollisionShape()->getBoundingSphere(positionObject,radiusObject);
        //                        Vector3 positionObjectNonBT(positionObject.x(), positionObject.y(), positionObject.z());
        //                        if (((tempPosition - positionObjectNonBT).length()<radiusObject) && (vergleicheQuader((tempPosition-positionObjectNonBT),(*it)->getScale3D())))
        //                        {
        //                            collision=true; break;
        //                        }
        //                    }
        //                    if(collision) break;
        //                }
        //                if(collision)break;
        //            }
        //            if(collision) continue;
        //            // no collision => possible Way
        //            for (float j =0; j<STEPS; j++)
        //            {
        //                Vector3 possiblePosition=(point1 - (point2-point1+richtungen[i]*PHI)*j/STEPS);
        //                collision=false;
        //                for(int ij=0; ij<STEPS; j++)
        //                {
        //                    Vector3 tempPosition=(possiblePosition - (point2-possiblePosition)*(float)ij/STEPS);
        //                    for (std::vector<StaticEntity*>::iterator it = problematicObjects.begin(); it!=problematicObjects.end(); ++it)
        //                    {
        //                        btVector3 positionObject;
        //                        btScalar radiusObject;
        //                        if((*it)==NULL)
        //                        {   orxout()<<"Problempoint 1"<<endl; continue;}
        //                        for (int everyShape=0; (*it)->getAttachedCollisionShape(everyShape)!=0; everyShape++)
        //                        {
        //                            if((*it)->getAttachedCollisionShape(everyShape)->getCollisionShape()==NULL)
        //                            {   orxout()<<"Problempoint 2.2"<<endl; continue;}
        //                            (*it)->getAttachedCollisionShape(everyShape)->getCollisionShape()->getBoundingSphere(positionObject,radiusObject);
        //                            Vector3 positionObjectNonBT(positionObject.x(), positionObject.y(), positionObject.z());
        //                            if (((tempPosition-positionObjectNonBT).length()<radiusObject) && (vergleicheQuader((tempPosition-positionObjectNonBT),(*it)->getScale3D())))
        //                            {
        //                                collision=true; break;
        //                            }
        //                        }
        //                        if(collision) break;
        //                    }
        //                    if(collision)break;
        //                    //addVirtualCheckPoint(racepoint1, racepoint2->getCheckpointIndex(), possiblePosition);
        //                    return;
        //                }
        //
        //            }
        //        }

    }
}
