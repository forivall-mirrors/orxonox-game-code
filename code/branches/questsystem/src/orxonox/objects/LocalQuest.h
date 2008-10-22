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
	    virtual ~LocalQuest();
	    
	protected:
            virtual bool isStartable(const Player & player) const; //!< Checks whether the quest can be started.
            virtual bool isFailable(const Player & player) const; //!< Checks whether the quest can be failed.
            virtual bool isCompletable(const Player & player) const; //!< Checks whether the quest can be completed.
	
            virtual questStatus::Enum getStatus(const Player & player) const; //!< Returns the status of the quest for a specific player.
            virtual void setStatus(const Player & player, const questStatus::Enum & status); //!< Sets the status for a specific player.
                
	private:
            std::map<Player*, questStatus::Enum> playerStatus_;
    
    };
    

}

#endif /* _LocalQuest_H__ */
