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
    @file GlobalQuest.h
    @brief
	Definition of the GlobalQuest class.
*/

#ifndef _GlobalQuest_H__
#define _GlobalQuest_H__

#include "OrxonoxPrereqs.h"

#include <set>
#include <list>

#include "core/XMLPort.h"
#include "Quest.h"

namespace orxonox {

    /**
    @brief
        Global quests are quests, that have the same status for all players.
        This means, that when a player successfully completes a GlobalQuest, it is completed for all players that have it.
        
        Creating a GlobalQuest through XML goes as follows:
        
        <GlobalQuest id="questId"> //Where questId is a GUID, see http://en.wikipedia.org/wiki/Globally_Unique_Identifier#Basic_structure for more information
            <QuestDescription title="Title" description="Description." /> //The description of the quest.
            <subquests>
		<Quest id ="questId1" /> //A list of n subquest, be aware, each of the <Quest /> tags must have a description and so on and so forth as well.
		...
		<Quest id="questIdn" />
	    </subquests>
	    <hints>
		<QuestHint id="hintId1" /> //A list of n QuestHints, see QuestHint for the full XML representation of those.
		...
		<QuestHint id="hintIdn" />
	    </hints>
            <fail-effects>
                <QuestEffect /> //A list of QuestEffects, invoked on all players possessing this quest, when the Quest is failed, see QuestEffect for the full XML representation.
                ...
                <QuestEffect />
            </fail-effects>
            <complete-effects>
                <QuestEffect /> //A list of QuestEffects, invoked on all players possessing this quest, when the Quest is completed, see QuestEffect for the full XML representation.
                ...
                <QuestEffect />
            </complete-effects>
            <reward-effects>
                <QuestEffect /> //A list of QuestEffects, invoked on the player completing this quest. See QuestEffect for the full XML representation.
                ...
                <QuestEffect />
            </reward-effects>
        </GlobalQuest>
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport GlobalQuest : public Quest
    {
        public:
            GlobalQuest(BaseObject* creator);
            virtual ~GlobalQuest();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a GlobalQuest object through XML.
            
            virtual bool fail(ControllableEntity* player); //!< Fails the quest.
            virtual bool complete(ControllableEntity* player); //!< Completes the quest.

        protected:
            virtual bool isStartable(const ControllableEntity* player) const; //!< Checks whether the quest can be started.
            virtual bool isFailable(const ControllableEntity* player) const; //!< Checks whether the quest can be failed.
            virtual bool isCompletable(const ControllableEntity* player) const; //!< Checks whether the quest can be completed.

            virtual questStatus::Enum getStatus(const ControllableEntity* player) const; //!< Returns the status of the quest for a specific player.
            virtual bool setStatus(ControllableEntity* player, const questStatus::Enum & status); //!< Sets the status for a specific player.

        private:
            std::set<ControllableEntity*> players_; //!< The set of players which possess this quest.
            questStatus::Enum status_; //!< The status of this quest.
            std::list<QuestEffect*> rewards_; //!< Reward effects only invoked on the player completing the quest.
            
            bool addRewardEffect(QuestEffect* effect); //!< Adds a reward effect to the list of reward effects.
            const QuestEffect* getRewardEffects(unsigned int index) const; //!< Returns the reward effect at the given index.

            void initialize(void); //!< Initializes the object.

    };

}

#endif /* _GlobalQuest_H__ */
