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
    @file
    @brief Definition of the Quest class.
        The Quest is the parent class of LocalQuest and GlobalQuest.
*/

#ifndef _Quest_H__
#define _Quest_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <list>
#include "QuestItem.h"

namespace orxonox
{
    namespace QuestStatus
    {
        //!Different states of a Quest.
        enum Value
        {
            Inactive,
            Active,
            Failed,
            Completed
        };
    }

    /**
    @brief
        Represents a Quest in the game.
        A Quest has a list of subquests and a parentquest (if it is not a rootquest).
        Each Quest exists only once but it has a different status (inactive, active, failed or completed) for each player.
        A Quest has several hints (QuestHint) that can be unlocked through QuestEffects and then display aid in solving the Quest.
        A Quest has a list of QuestEffects that are invoked when the quest is failed and also a list of QuestEffects that are invoked, when the Quest is completed.

        Quest itself should not be instantiated, if you want to create a quest either go for LocalQuest or GlobalQuest, whichever suits you needs better.
    @author
        Damian 'Mozork' Frick
    */
    class _QuestsystemExport Quest : public QuestItem
    {
        public:
            Quest(BaseObject* creator);
            virtual ~Quest();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a Quest object through XML.

            /**
            @brief Returns the parentquest of the Quest.
            @return Returns a pointer to the parentquest of the Quest.
            */
            inline Quest* getParentQuest(void) const
                { return this->parentQuest_; }

            /**
            @brief Returns the list of subquests.
            @return Returns a reference to the list of subquests of the quest.
            */
            inline const std::list<Quest*> & getSubQuestList(void) const
                { return this->subQuests_; }

            /**
            @brief Returns the list of all QuestHints of this Quest.
            @return Returns a reference to the list of QuestHints of the Quest.
            */
            inline const std::list<QuestHint*> & getHintsList(void) const
                { return this->hints_; }

            bool isInactive(const PlayerInfo* player) const; //!< Returns true if the quest status for the specific player is 'inactive'.
            bool isActive(const PlayerInfo* player) const; //!< Returns true if the quest status for the specific player is 'active'.
            bool isFailed(const PlayerInfo* player) const; //!< Returns true if the quest status for the specific player is 'failed'.
            bool isCompleted(const PlayerInfo* player) const; //!< Returns true if the quest status for the specific player is 'completed'.

            bool start(PlayerInfo* player); //!< Sets a Quest to active.
            virtual bool fail(PlayerInfo* player); //!< Fails the Quest.
            virtual bool complete(PlayerInfo* player); //!< Completes the Quest.

            bool addListener(QuestListener* listener); //!< Adds a QuestListener to the list of QuestListeners listening to this Quest.

        protected:
            virtual bool isStartable(const PlayerInfo* player) const = 0; //!< Checks whether the Quest can be started.
            virtual bool isFailable(const PlayerInfo* player) const = 0; //!< Checks whether the Quest can be failed.
            virtual bool isCompletable(const PlayerInfo* player) const = 0; //!< Checks whether the Quest can be completed.

            const Quest* getSubQuest(unsigned int index) const; //!<Returns the subquest at the given index.
            const QuestHint* getHint(unsigned int index) const; //!< Returns the QuestHint at the given index.
            const QuestEffect* getFailEffect(unsigned int index) const; //!< Returns the fail QuestEffect at the given index.
            const QuestEffect* getCompleteEffect(unsigned int index) const; //!< Returns the complete QuestEffect at the given index.

            /**
            @brief Returns the list of fail QuestEffects.
            @return Returns a reference to the list of fail QuestEffects.
            */
            inline std::list<QuestEffect*> & getFailEffectList(void)
                { return this->failEffects_; }

            /**
            @brief Returns the list of complete QuestEffects.
            @return Returns a reference to the list of complete QuestEffects.
            */
            inline std::list<QuestEffect*> & getCompleteEffectList(void)
                { return this->completeEffects_; }

            virtual QuestStatus::Value getStatus(const PlayerInfo* player) const = 0; //!< Returns the status of the Quest for a specific player.
            virtual bool setStatus(PlayerInfo* player, const QuestStatus::Value & status) = 0; //!< Changes the status for a specific player.

        private:
            Quest* parentQuest_; //!< Pointer to the parentquest.
            std::list<Quest*> subQuests_; //!< List of all the subquests.

            std::list<QuestHint*> hints_; //!< A list of all the QuestHints tied to this Quest.

            std::list<QuestEffect*> failEffects_; //!< A list of all QuestEffects to be invoked, when the Quest has been failed.
            std::list<QuestEffect*> completeEffects_; //!< A list of QuestEffects to be invoked, when the Quest has been completed.

            std::list<QuestListener*> listeners_; //!< A list of QuestListeners, that listen to what exactly happens with this Quest.

            bool setParentQuest(Quest* quest); //!< Sets the parentquest of the Quest.
            bool addSubQuest(Quest* quest); //!< Adds a subquest to the Quest.
            bool addHint(QuestHint* hint); //!< Add a QuestHint to the list of QuestHints.
            bool addFailEffect(QuestEffect* effect); //!< Adds an QuestEffect to the list of fail QuestEffects.
            bool addCompleteEffect(QuestEffect* effect); //!< Adds an QuestEffect to the list of complete QuestEffects.

    };

}

#endif /* _Quest_H__ */
