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

#include <map>
#include <string>

#include "Quest.h"

namespace orxonox {

    class Player; //Forward declaration, remove when fully integrated into the objecthirarchy.

    /**
    @brief
        Handles quests which have different states for different players.
    @author
        Damian 'Mozork' Frick
    */
    class LocalQuest : public Quest
    {
	public:
            LocalQuest();
	    LocalQuest(std::string id, std::string title = "", std::string description = "");
	    ~LocalQuest();
	    
	protected:
            virtual bool isStartable(Player* player); //!< Checks whether the quest can be started.
            virtual bool isFailable(Player* player); //!< Checks whether the quest can be failed.
            virtual bool isCompletable(Player* player); //!< Checks whether the quest can be completed.
	
            virtual questStatus::Enum getStatus(const Player* player); //!< Returns the status of the quest for a specific player.
            virtual bool setStatus(Player* player, const questStatus::Enum & status); //!< Sets the status for a specific player.
                
	private:
            std::map<Player*, questStatus::Enum> playerStatus_;
    
    };
    

}

#endif /* _LocalQuest_H__ */
