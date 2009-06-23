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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "LevelManager.h"

#include <map>
#include "PlayerManager.h"
#include "objects/Level.h"
#include "objects/infos/HumanPlayer.h"

namespace orxonox
{
    LevelManager* LevelManager::singletonRef_s = 0;

    LevelManager::LevelManager()
    {
        assert(singletonRef_s == 0);
        singletonRef_s = this;
    }

    LevelManager::~LevelManager()
    {
        assert(singletonRef_s != 0);
        singletonRef_s = 0;
    }

    void LevelManager::requestActivity(Level* level)
    {
        this->levels_s.push_back(level);
        if (this->levels_s.size() == 1)
            this->activateNextLevel();
    }

    void LevelManager::releaseActivity(Level* level)
    {
        if (this->levels_s.size() > 0)
        {
            if (this->levels_s.front() == level)
            {
                level->setActive(false);
                this->levels_s.pop_front();
                this->activateNextLevel();
            }
            else
            {
                for (std::list<Level*>::iterator it = this->levels_s.begin(); it != this->levels_s.end(); ++it)
                    if ((*it) == level)
                        this->levels_s.erase(it);
            }
        }
    }

    Level* LevelManager::getActiveLevel()
    {
        if (this->levels_s.size() > 0)
            return this->levels_s.front();
        else
            return 0;
    }

    void LevelManager::activateNextLevel()
    {
        if (this->levels_s.size() > 0)
        {
            this->levels_s.front()->setActive(true);
            for (std::map<unsigned int, PlayerInfo*>::const_iterator it = PlayerManager::getInstance().getClients().begin(); it != PlayerManager::getInstance().getClients().end(); ++it)
                this->levels_s.front()->playerEntered(it->second);
        }
    }
}
