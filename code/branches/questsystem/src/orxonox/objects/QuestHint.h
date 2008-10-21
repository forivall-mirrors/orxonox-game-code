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
 
#ifndef _QuestHint_H__
#define _QuestHint_H__

#include <string>

#include "orxonox/quests/QuestDescription.h"
#include "Quest.h"
#include "QuestItem.h"

namespace orxonox
{

    /**
    @brief
        Represents a hint in the game towards completing a Quest.
        Consists of title and description in textual form.
    @author
        Damian 'Mozork' Frick
    */
    class QuestHint : public QuestItem
    {
    
	public:
	    QuestHint(std::string id, std::string title = "", std::string description = "");
	    ~QuestHint();
	    
	    bool isActive(const Player & player) const; //!< Returns true if the hint is active for the input player.
	    
	    void activate(const Player & player); //!< Activates the hint for the input player.
	    
	    void setQuest(Quest* quest); //!< Sets the quest the hint belongs to.
	    
        private: 
            
            Quest* quest_;
            //TDO: questHint statuslist.
    
    };

}

namespace questHintStatus
{

    enum Enum
    {
        inactive,
        active
    };

}

#endif /* _QuestHint_H__ */
