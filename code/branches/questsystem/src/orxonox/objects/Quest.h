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

#ifndef _Quest_H__
#define _Quest_H__

#include <list>
#include <string>

#include "QuestDescription.h"
#include "QuestItem.h"
#include "QuestHint.h"
#include "QuestEffect.h"

namespace orxonox {

    /**
    @brief
        Represents a quest in the game.
        A quest has a sub
        
    @author
        Damian 'Mozork' Frick
    */
    class Quest : public QuestItem
    {
	public:
            Quest(std::string id, std::string title = "", std::string description = "");
	    virtual ~Quest();
    
            inline Quest* getParentQuest(void) const //!< Returns the parent quest of the quest.
                { return this->parentQuest_; }
	    inline const std::list<Quest> & getSubQuest(void) const //!< Returns the list of sub quests.
                { return this->subQuests_; }
	    
	    //TDO: Necessary? Shouldn't this be decided whilest creating the object?
	    bool setParentQuest(Quest* quest); //!< Sets the parent quest of the quest.
	    bool addSubQuest(Quest & quest); //!< Adds a sub quest to the quest.
	    
	    inline bool isInactive(const Player & player) const //!< Returns true if the quest status for the specific player is 'inactive'.
	       { return this->getStatus(player) == questStatus::inactive; }
	    inline bool isActive(const Player & player) const //!< Returns true if the quest status for the specific player is 'active'.
	       { return this->getStatus(player) == questStatus::active; }
	    inline bool isFailed(const Player & player) const //!< Returns true if the quest status for the specific player is 'failed'.
	       { return this->getStatus(player) == questStatus::failed; }
	    inline bool isCompleted(const Player & player) const //!< Returns true if the quest status for the specific player is 'completed'.
	       { return this->getStatus(player) == questStatus::completed; }
                
	    bool start(const Player & player); //!< Sets a quest to active.
	    bool fail(Player & player); //!< Fails the quest.
	    bool complete(Player & player); //!< Completes the quest.
                
            void addHint(QuestHint & hint); //!< Add a hint to the list of hints.
	    
        protected:
            void initialize(void); //!< Initialized the object.
            
            virtual bool isStartable(const Player & player) const = 0; //!< Checks whether the quest can be started.
            virtual bool isFailable(const Player & player) const = 0; //!< Checks whether the quest can be failed.
            virtual bool isCompletable(const Player & player) const = 0; //!< Checks whether the quest can be completed.
            
            virtual questStatus::Enum getStatus(const Player & player) const = 0; //!< Returns the status of the quest for a specific player.
            virtual void setStatus(const Player & player, const questStatus::Enum & status) = 0; //!< Changes the status for a specific player.
            
            Quest* parentQuest_; //!< Pointer to the parent quest.
            std::list<Quest> subQuests_; //!< List of all the sub quests.
            
            std::list<QuestHint> hints_; //!< A list of all the hints tied to this quest.
            
            std::list<QuestEffect> failEffects_; //!< A list of all effects to be invoked, when the quest has been failed.
            std::list<QuestEffect> completeEffects_; //!< A list of effects to be invoked, when the quest has been completed.

    };

}

namespace questStatus
{

    enum Enum
    {
        inactive,
        active,
        failed,
        completed
    };

}

#endif /* _Quest_H__ */
