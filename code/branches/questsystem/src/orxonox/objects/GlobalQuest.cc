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

#include "GlobalQuest.h"

namespace orxonox {

    CreateFactory(GlobalQuest);

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
    GlobalQuest::GlobalQuest(std::string id, std::string title = "", std::string description = "") : Quest(id, title, description)
    {
        RegisterObject(GlobalQuest);
    }
    
    /**
    @brief
        Destructor.
    */
    GlobalQuest::~GlobalQuest()
    {
        
    }

    /**
    @brief
        Returns the status of the quest for a specific player.
    @param player
        The player.
    */
    virtual questStatus::Enum getStatus(const Player & player) const
    {
        //TDO: Does this really work???
        if (this->players_.find(&player) != this->players_.end())
	{
	    return this->status_;
	}
	else
	{
	   return questStatus::inactive;
	}

    }
    
    /**
    @brief
        Sets the status for a specific player.
    @param player
        The player.
    @param status
        The status to be set.
    */
    virtual void setStatus(const Player & player, const questStatus::Enum & status)
    {
        //TDO: Implement.
    }

}
