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
 *      Damian 'Mozork' Frick
 *   Co-authors:
 *      ...
 *
 */

#include "core/CoreIncludes.h"

#include "LocalQuest.h"

namespace orxonox {

    CreateFactory(LocalQuest);

    /**
    @brief
        Constructor.
    @param id
        The unique identifier.
    @param title
        The title of the quest.
    @param description
        The description of the quest.
    */
    LocalQuest::LocalQuest(std::string id, std::string title = "", std::string description = "") : Quest(id, title, description)
    {
        RegisterObject(LocalQuest);
    }
    
    /**
    @brief
        Destructor.
    */
    LocalQuest::~LocalQuest()
    {
        
    }
    
    /**
    @brief
        Checks whether the quest can be started.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the quest can be started, false if not.
    */
    bool LocalQuest::isStartable(const Player & player) const
    {
        return this->isInactive(player);
    }
    
    /**
    @brief
        Checks whether the quest can be failed.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the quest can be failed, false if not.
    */
    bool LocalQuest::isFailable(const Player & player) const
    {
        return this->isActive(player);
    }
    
    /**
    @brief
        Checks whether the quest can be completed.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the quest can be completed, false if not.
    */
    bool LocalQuest::isCompletable(const Player & player) const
    {
        return this->isActive(player);
    }
    
    /**
    @brief
        Returns the status of the quest for a specific player.
    @param player
        The player.
    @return
        Returns the status of the quest for the input player.
    */
    virtual questStatus::Enum LocalQuest::getStatus(const Player & player) const
    {
        std::map<Player*, questStatus::Enum>::iterator it = this->playerStatus_.find(&player);
	if (it != this->playerStatus_.end())
	{
	    return it->second;
	}
	return questStatus::inactive;
    }
    
    /**
    @brief
        Sets the status for a specific player.
    @param player
        The player.
    @param status
        The status.
    */
    virtual void LocalQuest::setStatus(const Player & player, const questStatus::Enum & status)
    {
        this->playerStatus[&player] = status;
    }

}
