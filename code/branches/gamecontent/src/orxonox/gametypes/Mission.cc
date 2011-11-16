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
 *   Author:
 *      Johannes Ritz
 *   Co-authors:
 *      ...
 *
 */

#include "Mission.h"
//#include "TeamGametype.h"
#include "items/Engine.h"
#include "controllers/ArtificialController.h"

#include "core/CoreIncludes.h"
#include "network/Host.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{
    CreateUnloadableFactory(Mission);

    Mission::Mission(BaseObject* creator) : TeamGametype(creator)
    {
        RegisterObject(Mission);
        this->missionAccomplished_ = false;
        this->lives_ = 10; // should be 1 as default value
        this->numberOfBots_ = 0; //sets number of default bots temporarly to 0
    }

    void Mission::tick(float dt)
    {
        SUPER(Mission, tick, dt);

        if (missionAccomplished_)
        {
            this->gtinfo_->sendAnnounceMessage("Mission accomplished!");
            this->end();
        }
    }

    void Mission::pawnKilled(Pawn* victim, Pawn* killer)
    {
        if (victim && victim->getPlayer() && this->lives_ == 1)
        {
            this->missionAccomplished_ = false;
	    this->end();
        }
    }

    void Mission::start()
    {
        Gametype::start();
        this->setTeams();
        /*for (ObjectList<Engine>::iterator it = ObjectList<Engine>::begin(); it != ObjectList<Engine>::end(); ++it)
            it->setActive(false); // works -> @sr :*/
        this->gtinfo_->sendAnnounceMessage("Your mission has started!");
    }

    //void Mission::addBots(unsigned int amount)

    void Mission::end()
    {
        Gametype::end();
	/*if (this->missionAccomplished_) 
            this->gtinfo_->sendAnnounceMessage("Mission accomplished!");
	else
            this->gtinfo_->sendAnnounceMessage("Mission failed!");
	 * */
    }

    void Mission::setTeams()
    {
        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
            this->setObjectColour(*it);
    }



}
