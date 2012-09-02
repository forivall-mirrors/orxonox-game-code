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
    @brief Definition of the GlobalQuest class.
    @ingroup Questsystem
*/

#ifndef _GlobalQuest_H__
#define _GlobalQuest_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <list>
#include <set>
#include "Quest.h"

namespace orxonox
{

    /**
    @brief
        GlobalQuests are @ref orxonox::Quest "Quests", that have the same status for all players.
        This means, that when a player successfully completes a GlobalQuest, it is completed for all players that have it.

        Creating a GlobalQuest through XML goes as follows:
        @code
        <GlobalQuest id="questId">
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
        @endcode

    @author
        Damian 'Mozork' Frick

    @ingroup Questsystem
    */
    class _QuestsystemExport GlobalQuest : public Quest
    {
        public:
            GlobalQuest(BaseObject* creator);
            virtual ~GlobalQuest();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a GlobalQuest object through XML.

            virtual bool fail(PlayerInfo* player); //!< Fails the Quest.
            virtual bool complete(PlayerInfo* player); //!< Completes the Quest.

        protected:
            virtual bool isStartable(const PlayerInfo* player) const; //!< Checks whether the Quest can be started.
            virtual bool isFailable(const PlayerInfo* player) const; //!< Checks whether the Quest can be failed.
            virtual bool isCompletable(const PlayerInfo* player) const; //!< Checks whether the Quest can be completed.

            virtual QuestStatus::Value getStatus(const PlayerInfo* player) const; //!< Returns the status of the Quest for a specific player.

            virtual bool setStatus(PlayerInfo* player, const QuestStatus::Value & status); //!< Sets the status for a specific player.

        private:
            std::set<PlayerInfo*> players_; //!< The set of players which possess this Quest.
            QuestStatus::Value status_; //!< The status of this Quest.
            std::list<QuestEffect*> rewards_; //!< Reward QuestEffects only invoked on the player completing the Quest.

            bool addRewardEffect(QuestEffect* effect); //!< Adds a reward QuestEffect to the list of reward QuestEffects.
            const QuestEffect* getRewardEffects(unsigned int index) const; //!< Returns the reward QuestEffect at the given index.

    };

}

#endif /* _GlobalQuest_H__ */
