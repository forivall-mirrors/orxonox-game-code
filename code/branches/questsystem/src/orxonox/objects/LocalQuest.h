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

#ifndef _LocalQuest_H__
#define _LocalQuest_H__

#include <string>

#include "Quest.h"

namespace orxonox {

    /**
    @brief
        Handles quests which have different states for different players.
    @author
        Damian 'Mozork' Frick
    */
    class LocalQuest : public Quest
    {
	public:
	    LocalQuest(std::string id, std::string title = "", std::string description = "");
	    ~LocalQuest();
	    
	protected:
            virtual questStatus::Enum getStatus(const Player & player) const; //!< Returns the status of the quest for a specific player.
            virtual void setStatus(const Player & player, const questStatus::Enum & status); //!< Sets the status for a specific player.
                
	private:
            //TDO: List of Players and the status of the quest fo them.
            questStatus::Enum status_; //!< The status of the quest.
    
    };

}

#endif /* _LocalQuest_H__ */
