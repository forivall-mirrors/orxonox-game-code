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

    SpaceRaceManager::SpaceRaceManager(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(SpaceRaceManager);

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

        XMLPortObject(SpaceRaceManager, RaceCheckPoint, "checkpoints", addCheckpoint, getCheckpoint,  xmlelement, mode);
    }

    void SpaceRaceManager::tick(float dt)
    {
        SUPER(SpaceRaceManager,tick,dt);

        if (this->checkpoints_[0] != NULL && !this->firstcheckpointvisible_)
        {
            this->checkpoints_[0]->setRadarVisibility(true);
            this->firstcheckpointvisible_ = true;
        }

        for (size_t i = 0; i < this->checkpoints_.size(); ++i)
        {
            if (this->checkpoints_[i]->getPlayer() != NULL)
                this->checkpointReached(this->checkpoints_[i], this->checkpoints_[i]->getPlayer());
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

    int SpaceRaceManager::getIndex(RaceCheckPoint* checkpoint)
    {
        for (size_t i = 0; i < this->checkpoints_.size(); ++i)
            if (this->checkpoints_[i] == checkpoint)
                return i;

        return -1;
    }

    void SpaceRaceManager::checkpointReached(RaceCheckPoint* check, PlayerInfo* player)
    {
        SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
        assert(gametype);

        bool reachedValidCheckpoint = false;

        int index = gametype->getCheckpointReached(player);
        if (index > -1)
        {
            Vector3 v = this->getCheckpoint(index)->getNextcheckpoint();

            if (this->getCheckpoint(v.x) == check)
            {
                reachedValidCheckpoint = true;
            }
            if (this->getCheckpoint(v.y) == check)
            {
                reachedValidCheckpoint = true;
            }
            if (this->getCheckpoint(v.z) == check)
            {
                reachedValidCheckpoint = true;
            }
        }
        else
        {
            reachedValidCheckpoint = (this->getIndex(check) == 0);
        }

        if (gametype && reachedValidCheckpoint)
        {
            gametype->getClock().capture();
            float time = gametype->getClock().getSecondsPrecise();
            if (check->getTimeLimit() != 0 && time > check->getTimeLimit())
            {
                gametype->setTimeIsUp();
                gametype->end();
            }
            else if (check->isLast())
                gametype->end();
            else
			{
                if (index > -1)
                    this->setRadVis(player, false);
                else
                    this->getCheckpoint(0)->setRadarVisibility(false);

                gametype->newCheckpointReached(check, player);
                this->setRadVis(player, true);
            }
        }

        check->resetPlayer();
    }

    void SpaceRaceManager::setRadVis(PlayerInfo* player, bool bVisible)
    {
        SpaceRace* gametype = orxonox_cast<SpaceRace*>(this->getGametype().get());
        assert(gametype);
        int index = gametype->getCheckpointReached(player);
        Vector3 v = this->getCheckpoint(index)->getNextcheckpoint();

        if (v.x > -1)
        {
            this->getCheckpoint(v.x)->setRadarVisibility(bVisible);
            this->getCheckpoint(v.x)->settingsChanged();
        }
        if (v.y > -1)
        {
            this->getCheckpoint(v.y)->setRadarVisibility(bVisible);
            this->getCheckpoint(v.y)->settingsChanged();
        }
        if (v.z > -1)
        {
            this->getCheckpoint(v.z)->setRadarVisibility(bVisible);
            this->getCheckpoint(v.z)->settingsChanged();
        }
    }
}
