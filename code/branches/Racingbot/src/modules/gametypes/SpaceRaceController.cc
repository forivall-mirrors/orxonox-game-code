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

    /*
     * Idea: Find static Point (checkpoints the spaceship has to reach)
     */
SpaceRaceController::SpaceRaceController(BaseObject* creator): ArtificialController(creator)
{

    std::vector<RaceCheckPoint*> checkpoints;
    // TODO Auto-generated constructor stub
    for (ObjectList<SpaceRaceManager >::iterator it = ObjectList<SpaceRaceManager>::begin(); it != ObjectList<SpaceRaceManager>::end(); ++it)
        checkpoints=it->getAllCheckpoints();
    //OrxAssert(checkpoints, "No Checkpoints in Level");


}
/*
RaceCheckPoint* SpaceRaceController::nextPoint(){
    return NULL;
}*/

std::vector<RaceCheckPoint*> SpaceRaceController::findStaticCheckpoints(std::vector<RaceCheckPoint*> allCheckpoints){
    std::map< RaceCheckPoint*, int> * zaehler= new std::map< RaceCheckPoint*, int>(); // counts how many times the checkpoit was reached for simulation
    for (unsigned int i=0; i<allCheckpoints.size() ; i++){
        zaehler->insert ( std::pair<RaceCheckPoint*,int>(allCheckpoints[i], 0));
    }
    rekSimulationCheckpointsReached(zaehler->begin()->first, & allCheckpoints, zaehler);
    //Werte auslesen und statische Checkpoints bestimmen
    delete zaehler;

}

void SpaceRaceController::rekSimulationCheckpointsReached(RaceCheckPoint* currentCheckpoint, std::vector<RaceCheckPoint*>* checkpoints, std::map< RaceCheckPoint*, int>* zaehler){
    zaehler->at(currentCheckpoint)++;
    if(!currentCheckpoint->isLast()){
        for( std::set<int>::iterator it=currentCheckpoint->getNextCheckpoints().begin(); it != currentCheckpoint->getNextCheckpoints().end(); ++it){
            rekSimulationCheckpointsReached( (*checkpoints)[(*it)], checkpoints, zaehler);
        }
    }
}


SpaceRaceController::~SpaceRaceController()
{
    // TODO Auto-generated destructor stub
}

void SpaceRaceController::XMLPort(Element& xmlelement, XMLPort::Mode mode){
            SUPER(SpaceRaceController, XMLPort, xmlelement, mode);

            XMLPortParam(ArtificialController, "accuracy", setAccuracy, getAccuracy, xmlelement, mode).defaultValues(100.0f);
            XMLPortObject(ArtificialController, WorldEntity, "waypoints", addWaypoint, getWaypoint,  xmlelement, mode);

}
void SpaceRaceController::tick(float dt){

}



}
