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
 *      Celine Eggenberger
 *   Co-authors:
 *      ...
 *
 */

#include "SpaceRaceManager.h"
#include "SpaceRace.h"
#include "infos/PlayerInfo.h"

#include "core/XMLPort.h"

#include "core/CoreIncludes.h"

#include "util/Convert.h"
#include "util/Math.h"

namespace orxonox
{
    CreateFactory(SpaceRaceManager);

    SpaceRaceManager::SpaceRaceManager(BaseObject* creator) :
        BaseObject(creator)
    {
        RegisterObject(SpaceRaceManager);
        this->race_ = orxonox_cast<SpaceRace*>(this->getGametype().get());
        assert(race_);
        //amountOfPlayers=(race_->getPlayers()).size();
        this->firstcheckpointvisible_ = false;
    }

    SpaceRaceManager::~SpaceRaceManager()
    {
        for (size_t i = 0; i < this->checkpoints_.size(); ++i)
        this->checkpoints_[i]->destroy();
    }

    void SpaceRaceManager::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(SpaceRaceManager, XMLPort, xmlelement, mode);

        XMLPortObject(SpaceRaceManager, RaceCheckPoint, "checkpoints", addCheckpoint, getCheckpoint, xmlelement, mode);
    }

    void SpaceRaceManager::tick(float dt)
    {
        SUPER(SpaceRaceManager,tick,dt);

        this->players_ = this->race_->getPlayers();

        if (this->checkpoints_[0] != NULL && !this->firstcheckpointvisible_)
        {
            this->checkpoints_[0]->setRadarVisibility(true);
            this->firstcheckpointvisible_ = true;
        }

        for ( std::map< PlayerInfo*, Player>::iterator it = players_.begin(); it != players_.end(); ++it)
        {

            for (size_t i = 0; i < this->checkpoints_.size(); ++i)
            {
                if (this->checkpoints_[i]->playerWasHere(it->first)){
                this->checkpointReached(this->checkpoints_[i], it->first /*this->checkpoints_[i]->getPlayer()*/);
                }
            }
        }

    }

    void SpaceRaceManager::addCheckpoint(RaceCheckPoint* checkpoint)
    {
        this->checkpoints_.push_back(checkpoint);
    }

    RaceCheckPoint* SpaceRaceManager::getCheckpoint(unsigned int index) const
    {
        if (index < this->checkpoints_.size())
        return this->checkpoints_[index];
        else
        return 0;
    }

    std::vector<RaceCheckPoint*> SpaceRaceManager::getAllCheckpoints()
    {
        return checkpoints_;
    }

    /**
     @brief Returns the checkpoint with the given checkpoint-index (@see RaceCheckPoint::getCheckpointIndex).
     */
    RaceCheckPoint* SpaceRaceManager::findCheckpoint(int index) const
    {
        for (size_t i = 0; i < this->checkpoints_.size(); ++i)
        if (this->checkpoints_[i]->getCheckpointIndex() == index)
        return this->checkpoints_[i];
        return 0;
    }

    bool SpaceRaceManager::reachedValidCheckpoint(RaceCheckPoint* oldCheckpoint, RaceCheckPoint* newCheckpoint, PlayerInfo* player) const
    {
        if (oldCheckpoint != NULL)
        {
            // the player already visited an old checkpoint; see which checkpoints are possible now
            const std::set<int>& possibleCheckpoints = oldCheckpoint->getNextCheckpoints();
            for (std::set<int>::const_iterator it = possibleCheckpoints.begin(); it != possibleCheckpoints.end(); ++it)
            if (this->findCheckpoint(*it) == newCheckpoint)
            return true;
            return false;
        }
        else
        {
            // the player hasn't visited a checkpoint yet, so he must reach the checkpoint with index 0 (hack?)
            return (newCheckpoint->getCheckpointIndex() == 0);
        }
    }

    void SpaceRaceManager::checkpointReached(RaceCheckPoint* newCheckpoint, PlayerInfo* player)
    {
        SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
        assert(gametype);
        if (!gametype)
        return;

        RaceCheckPoint* oldCheckpoint = gametype->getCheckpointReached(player); // returns the last from player reached checkpoint

        if (this->reachedValidCheckpoint(oldCheckpoint, newCheckpoint, player))
        {
            // the player reached a valid checkpoint
            gametype->getClock().capture();
            float time = gametype->getClock().getSecondsPrecise();
            if (newCheckpoint->getTimeLimit() != 0 && time > newCheckpoint->getTimeLimit())
            {
                // time's up - the player has lost the game
                gametype->setTimeIsUp();
                gametype->end();
            }
            else if (newCheckpoint->isLast())
            {
                // the last checkpoint was reached - the player has won the game
                gametype->end();
            }
            else
            {
                // adjust the radarvisibility
                gametype->newCheckpointReached(newCheckpoint, player);
                this->updateRadarVisibility(oldCheckpoint, newCheckpoint);
            }
        }

        // newCheckpoint->resetPlayer(); loescht playerpointer TODO: check if problems occur
    }

    void SpaceRaceManager::updateRadarVisibility(RaceCheckPoint* oldCheckpoint, RaceCheckPoint* newCheckpoint) const
    {
        if (oldCheckpoint)
        {
            const std::set<int>& oldVisible = oldCheckpoint->getNextCheckpoints();
            for (std::set<int>::const_iterator it = oldVisible.begin(); it != oldVisible.end(); ++it)
            this->findCheckpoint(*it)->setRadarVisibility(false);
        }

        if (newCheckpoint)
        {
            newCheckpoint->setRadarVisibility(false);

            const std::set<int>& newVisible = newCheckpoint->getNextCheckpoints();
            for (std::set<int>::const_iterator it = newVisible.begin(); it != newVisible.end(); ++it)
            this->findCheckpoint(*it)->setRadarVisibility(true);
        }
    }
}
