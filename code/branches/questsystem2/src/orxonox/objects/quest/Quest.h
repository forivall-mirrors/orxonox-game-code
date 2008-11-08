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
 
/**
    @file Quest.h
    @brief
	Definition of the Quest class.
	
	The Quest is the parent class of LocalQuest and GlobalQuest.
*/
 
#ifndef _Quest_H__
#define _Quest_H__

#include "OrxonoxPrereqs.h"

#include <list>
#include <string>

#include "core/XMLPort.h"
#include "QuestItem.h"


namespace questStatus
{

    //!Different states of a quest.
    enum Enum
    {
        inactive,
        active,
        failed,
        completed
    };

}

namespace orxonox {

    /**
    @brief
        Represents a quest in the game.
        A quest has a list of subquests and a parentquest (if it is not a rootquest).
        Each quest exists only once but it has a different status (inactive, active, failed or completed) for each player.
        A quest has several hints (QuestHint) that can be unlocked through QuestEffects and then display aid in solving the quest.
        A quest has a list of QuestEffects that are invoked when the quest is failed and also a list of effects that are invoked, when the quest is completed.
        
        Quest itself should not be instantiated, if you want to create a quest either go for LocalQuest or GlobalQuest, whichever suits you needs better.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport Quest : public QuestItem
    {
        public:
            Quest(BaseObject* creator);
            virtual ~Quest();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a Quest object through XML.

            inline Quest* getParentQuest(void) const //!< Returns the parent quest of the quest.
                { return this->parentQuest_; }
            inline const std::list<Quest*> & getSubQuestList(void) const //!< Returns the list of sub quests.
                { return this->subQuests_; }
	    inline const std::list<QuestHint*> & getHintsList(void) const //!< Returns the list of all hints of this quest.
                { return this->hints_; }

            bool isInactive(const ControllableEntity* player) const; //!< Returns true if the quest status for the specific player is 'inactive'.
            bool isActive(const ControllableEntity* player) const; //!< Returns true if the quest status for the specific player is 'active'.
            bool isFailed(const ControllableEntity* player) const; //!< Returns true if the quest status for the specific player is 'failed'.
            bool isCompleted(const ControllableEntity* player) const; //!< Returns true if the quest status for the specific player is 'completed'.

            bool start(ControllableEntity* player); //!< Sets a quest to active.
	    virtual bool fail(ControllableEntity* player) = 0; //!< Fails the quest.
            virtual bool complete(ControllableEntity* player) = 0; //!< Completes the quest.

        protected:
            virtual bool isStartable(const ControllableEntity* player) const = 0; //!< Checks whether the quest can be started.
            virtual bool isFailable(const ControllableEntity* player) const = 0; //!< Checks whether the quest can be failed.
            virtual bool isCompletable(const ControllableEntity* player) const = 0; //!< Checks whether the quest can be completed.

            const Quest* getParentQuest(void); //!< Returns the parent quest of the quest.
            const Quest* getSubQuest(unsigned int index) const; //!<Returns the sub quest of the given index.
            const QuestHint* getHint(unsigned int index) const; //!< Returns the hint of the given index.
            const QuestEffect* getFailEffect(unsigned int index) const; //!< Returns the failEffect of the given index.
            const QuestEffect* getCompleteEffect(unsigned int index) const; //!< Returns the completeEffect of the given index.
            inline std::list<QuestEffect*> & getFailEffectList(void) //!< Returns the list of failEffects.
                { return this->failEffects_; }
	    inline std::list<QuestEffect*> & getCompleteEffectList(void) //!< Returns the list of completeEffects.
                { return this->completeEffects_; }

            virtual questStatus::Enum getStatus(const ControllableEntity* player) const = 0; //!< Returns the status of the quest for a specific player.
            virtual bool setStatus(ControllableEntity* player, const questStatus::Enum & status) = 0; //!< Changes the status for a specific player.
            
	private:
            Quest* parentQuest_; //!< Pointer to the parent quest.
            std::list<Quest*> subQuests_; //!< List of all the sub quests.

            std::list<QuestHint*> hints_; //!< A list of all the hints tied to this quest.

            std::list<QuestEffect*> failEffects_; //!< A list of all effects to be invoked, when the quest has been failed.
            std::list<QuestEffect*> completeEffects_; //!< A list of effects to be invoked, when the quest has been completed.
            
            bool setParentQuest(Quest* quest); //!< Sets the parent quest of the quest.
            bool addSubQuest(Quest* quest); //!< Adds a sub quest to the quest.
            bool addHint(QuestHint* hint); //!< Add a hint to the list of hints.
            bool addFailEffect(QuestEffect* effect); //!< Adds an effect to the list of failEffects.
            bool addCompleteEffect(QuestEffect* effect); //!< Adds an effect to the list of completeEffects.

    };

}

#endif /* _Quest_H__ */
