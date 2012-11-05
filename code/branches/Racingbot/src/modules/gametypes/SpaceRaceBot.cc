/*
 * SpaceRAceBot.cc
 *
 *  Created on: 05.11.2012
 *      Author: purgham
 */

#include "SpaceRaceBot.h"
#include "core/CoreIncludes.h"

namespace orxonox
{
    CreateFactory(SpaceRaceBot);

    SpaceRaceBot::SpaceRaceBot(BaseObject* creator) : Bot(creator){
        RegisterObject(SpaceRaceBot);
        this->defaultController_ = Class(SpaceRaceController);// ClassByString("")
        this->createController();
    }
}
