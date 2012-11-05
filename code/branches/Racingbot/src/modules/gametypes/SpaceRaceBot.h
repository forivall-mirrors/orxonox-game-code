/*
 * SpaceRaceBot.h
 *
 *  Created on: 05.11.2012
 *      Author: purgham
 */

#ifndef SPACERACEBOT_H_
#define SPACERACEBOT_H_

#include "infos/Bot.h"
#include "gametypes/GametypesPrereqs.h"
#include "gametypes/SpaceRaceController.h"


namespace orxonox
{

    /**

    */
    class _GametypesExport SpaceRaceBot: public Bot
    {
        public:
        SpaceRaceBot(BaseObject* creator);
            virtual ~SpaceRaceBot() {}
    };
}


#endif /* SPACERACEBOT_H_ */
