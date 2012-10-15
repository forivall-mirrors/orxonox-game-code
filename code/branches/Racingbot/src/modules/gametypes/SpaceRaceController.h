/*
 * SpaceRaceController.h
 *
 *  Created on: Oct 8, 2012
 *      Author: purgham
 */

#ifndef SPACERACECONTROLLER_H_
#define SPACERACECONTROLLER_H_

#include "gametypes/GametypesPrereqs.h"
#include "controllers/ArtificialController.h"
#include "gametypes/Gametype.h"



namespace orxonox
{
    class _GametypesExport SpaceRaceController : public ArtificialController, public Tickable
    {
        private:
            vector<RaceCheckPoint*> lastRaceCheckpoint;
            vector<RaceCheckPoint*> nextRaceCheckpoint;
        public:
          SpaceRaceController(BaseObject* creator);
          virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
          virtual ~SpaceRaceController();
          virtual void tick(float dt);
    };

}


#endif /* SPACERACECONTROLLER_H_ */
