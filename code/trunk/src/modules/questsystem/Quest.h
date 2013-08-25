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
    @brief Definition of the Quest class.
        The Quest class is the parent class of @ref orxonox::LocalQuest "LocalQuest" and @ref orxonox::GlobalQuest "GlobalQuest".
    @ingroup Questsystem
*/

#ifndef _Quest_H__
#define _Quest_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <list>
#include "QuestItem.h"

namespace orxonox // tolua_export
{ // tolua_export

    /**
    @brief
        Different states of a @ref orxonox::Quest "Quest".

    @ingroup Questsystem
    */
    namespace QuestStatus
    {
        enum Value
        {
            Inactive, //!< The @ref orxonox::Quest "Quest" is inactive.
            Active, //!< The @ref orxonox::Quest "Quest" is active.
            Failed, //!< The @ref orxonox::Quest "Quest" has been failed.
            Completed //!< The @ref orxonox::Quest "Quest" has been completed.
        };
    }

    /**
    @brief
        Represents a Quest in the game. A Quest is a task that the player can (or has to) fulfill upon which he will (possibly) receive some kind of reward.

        A Quest can have a list of sub-quests and has a parent-quest (if it is not a root-quest).
        Each Quest exists only once but it has a different status (inactive, active, failed or completed) for each player.
        A Quest can have several hints (QuestHint) that can be unlocked through @ref orxonox::QuestEffect "QuestEffects" and then display aid in solving the Quest.
        A Quest can have a list of @ref orxonox::QuestEffect "QuestEffects" that are invoked when the quest is failed and also a list of @ref orxonox::QuestEffect "QuestEffects" that are invoked, when the Quest is completed.

        Quest itself should not be instantiated, if you want to create a quest either use LocalQuest or GlobalQuest, whichever suits you needs better.

    @author
        Damian 'Mozork' Frick

    @ingroup Questsystem
    */
    class _QuestsystemExport Quest // tolua_export
        : public QuestItem
    { // tolua_export
        public:
            Quest(Context* context);
            virtual ~Quest();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a Quest object through XML.

            /**
            @brief Returns the parent-quest of the Quest.
            @return Returns a pointer to the parent-quest of the Quest.
            */
            inline Quest* getParentQuest(void) const
                { return this->parentQuest_; }

            /**
            @brief Returns the list of sub-quests.
            @return Returns a reference to the list of sub-quests of the quest.
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
            bool isActive(const orxonox::PlayerInfo* player) const; // tolua_export //!< Returns true if the quest status for the specific player is 'active'.
            bool isFailed(const orxonox::PlayerInfo* player) const; // tolua_export //!< Returns true if the quest status for the specific player is 'failed'.
            bool isCompleted(const orxonox::PlayerInfo* player) const; // tolua_export //!< Returns true if the quest status for the specific player is 'completed'.

            bool start(PlayerInfo* player); //!< Sets a Quest to active.
            virtual bool fail(PlayerInfo* player); //!< Fails the Quest.
            virtual bool complete(PlayerInfo* player); //!< Completes the Quest.

            bool addListener(QuestListener* listener); //!< Adds a QuestListener to the list of QuestListeners listening to this Quest.

        protected:
            virtual bool isStartable(const PlayerInfo* player) const = 0; //!< Checks whether the Quest can be started.
            virtual bool isFailable(const PlayerInfo* player) const = 0; //!< Checks whether the Quest can be failed.
            virtual bool isCompletable(const PlayerInfo* player) const = 0; //!< Checks whether the Quest can be completed.

            const Quest* getSubQuest(unsigned int index) const; //!<Returns the sub-quest at the given index.
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
            Quest* parentQuest_; //!< Pointer to the parent-quest.
            std::list<Quest*> subQuests_; //!< List of all the sub-quests.

            std::list<QuestHint*> hints_; //!< A list of all the QuestHints tied to this Quest.

            std::list<QuestEffect*> failEffects_; //!< A list of all QuestEffects to be invoked, when the Quest has been failed.
            std::list<QuestEffect*> completeEffects_; //!< A list of QuestEffects to be invoked, when the Quest has been completed.

            std::list<QuestListener*> listeners_; //!< A list of QuestListeners, that listen to what exactly happens with this Quest.

            bool setParentQuest(Quest* quest); //!< Sets the parent-quest of the Quest.
            bool addSubQuest(Quest* quest); //!< Adds a sub-quest to the Quest.
            bool addHint(QuestHint* hint); //!< Add a QuestHint to the list of QuestHints.
            bool addFailEffect(QuestEffect* effect); //!< Adds an QuestEffect to the list of fail QuestEffects.
            bool addCompleteEffect(QuestEffect* effect); //!< Adds an QuestEffect to the list of complete QuestEffects.

    }; // tolua_export

} // tolua_export

#endif /* _Quest_H__ */
