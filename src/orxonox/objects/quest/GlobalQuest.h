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

#ifndef _GlobalQuest_H__
#define _GlobalQuest_H__

#include <set>

#include "core/XMLPort.h"
#include "Quest.h"

namespace orxonox {

    class Player; //Forward declaration, remove when fully integrated into the objecthirarchy.

    /**
    @brief
        Global quests are quests, that have the same status for all players.
        This means, that when a player successfully completes this quest, it is completed for all players that have it.
    @author
        Damian 'Mozork' Frick
    */
    class GlobalQuest : public Quest
    {
	public:
            GlobalQuest();
	    ~GlobalQuest();
	    
	    virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);
	    
	protected:
            virtual bool isStartable(const Player* player) const; //!< Checks whether the quest can be started.
            virtual bool isFailable(const Player* player) const; //!< Checks whether the quest can be failed.
            virtual bool isCompletable(const Player* player) const; //!< Checks whether the quest can be completed.
	
	    virtual questStatus::Enum getStatus(const Player* player) const; //!< Returns the status of the quest for a specific player.
	    virtual bool setStatus(Player* player, const questStatus::Enum & status); //!< Sets the status for a specific player.
	    
	private:
            std::set<Player*> players_; //!< The set of players which possess this quest.
            questStatus::Enum status_; //!< The status of this quest.
            
            void initialize(void);
    
    };

}

#endif /* _GlobalQuest_H__ */
