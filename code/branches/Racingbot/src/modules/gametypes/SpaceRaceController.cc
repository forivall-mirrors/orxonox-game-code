/*
 * SpaceRaceController.cc
 *
 *  Created on: Oct 8, 2012
 *      Author: purgham
 */

#include <gametypes/SpaceRaceController.h>
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"


// Von SpaceRaceManager points einlesen
// Berechnungsklasse nextPoint zur verfuegung stellen
// ^- aufrufen ueber tick (if ob noetig)
namespace orxonox
{

SpaceRaceController::SpaceRaceController(BaseObject* creator): ArtificialController(creator)
{

    std::vector<RaceCheckPoint*> wayOptions = NULL;
    std::vector<RaceCheckPoint*> wayChosen = new std::vector<RaceCheckPoint*>();
    // TODO Auto-generated constructor stub
    for (ObjectList<SpaceRaceManager >::iterator it = ObjectList<SpaceRaceManager>::begin(); it != ObjectList<SpaceRaceManager>::end(); ++it)
        wayOptions=it->getAllCheckpoints();


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
