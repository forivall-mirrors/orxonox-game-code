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

#include <boost/filesystem.hpp>

#include "items/Engine.h"
#include "controllers/ArtificialController.h"

#include "core/CoreIncludes.h"
#include "core/command/ConsoleCommand.h"
#include "core/config/ConfigValueIncludes.h"
#include "infos/PlayerInfo.h"
#include "network/Host.h"
#include "worldentities/pawns/Pawn.h"
#include "LevelManager.h"

namespace orxonox
{
    SetConsoleCommand("Mission", "endMission", &Mission::endMission);
    SetConsoleCommand("Mission", "setLives", &Mission::setLivesWrapper);
    RegisterUnloadableClass(Mission);

    Mission::Mission(Context* context) : TeamGametype(context)
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
        else if (this->lives_ == 0)
        {
            this->missionAccomplished_ = false;
            this->end();
        }
    }

    void Mission::pawnKilled(Pawn* victim, Pawn* killer)
    {
        if (victim && victim->getPlayer() && victim->getPlayer()->isHumanPlayer())
        {
            this->lives_--;
        }
    }

    void Mission::start()
    {
        std::fstream myfile;

        Gametype::start();
        this->setTeams();
        //just for testing
        //this->missionAccomplished_=true;
        this->gtinfo_->sendAnnounceMessage("Your mission has started!");
    }

    void Mission::end()
    {

        if (this->missionAccomplished_ && !this->gtinfo_->hasEnded())
        {
            this->gtinfo_->sendAnnounceMessage("Mission accomplished!");

            LevelManager::getInstance().setLastFinishedCampaignMission(this->getFilename());
        }

        else if (!this->gtinfo_->hasEnded())
            this->gtinfo_->sendAnnounceMessage("Mission failed!");
        Gametype::end();
    }

    void Mission::setTeams()
    { //Set pawn-colours
        for (ObjectList<Pawn>::iterator it = ObjectList<Pawn>::begin(); it != ObjectList<Pawn>::end(); ++it)
        {
            Pawn* pawn = static_cast<Pawn*>(*it);
            if (!pawn)
                continue;
            this->setDefaultObjectColour(pawn);
        }
    }
    void Mission::endMission(bool accomplished)
    {
        for (ObjectList<Mission>::iterator it = ObjectList<Mission>::begin(); it != ObjectList<Mission>::end(); ++it)
        { //TODO: make sure that only the desired mission is ended !! This is a dirty HACK, that would end ALL missions!
            it->setMissionAccomplished(accomplished);
            it->end();

        }
    }

    void Mission::setLivesWrapper(unsigned int amount)
    {
        for (ObjectList<Mission>::iterator it = ObjectList<Mission>::begin(); it != ObjectList<Mission>::end(); ++it)
        { //TODO: make sure that only the desired mission is ended !! This is a dirty HACK, that would affect ALL missions!
            it->setLives(amount);
        }
    }

}
