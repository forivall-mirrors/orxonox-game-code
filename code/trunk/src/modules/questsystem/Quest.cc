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
    @brief Implementation of the Quest class.
*/

#include "Quest.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "QuestManager.h"
#include "QuestDescription.h"
#include "QuestHint.h"
#include "QuestEffect.h"
#include "QuestListener.h"

namespace orxonox
{
    /**
    @brief
        Constructor. Registers and initializes object.
    */
    Quest::Quest(BaseObject* creator) : QuestItem(creator)
    {
        RegisterObject(Quest);

        this->parentQuest_ = NULL;
    }

    /**
    @brief
        Destructor.
    */
    Quest::~Quest()
    {

    }

    /**
    @brief
        Method for creating a Quest object through XML.
    */
    void Quest::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Quest, XMLPort, xmlelement, mode);

        XMLPortObject(Quest, Quest, "subquests", addSubQuest, getSubQuest, xmlelement, mode);
        XMLPortObject(Quest, QuestHint, "hints", addHint, getHint, xmlelement, mode);
        XMLPortObject(Quest, QuestEffect, "fail-effects", addFailEffect, getFailEffect, xmlelement, mode);
        XMLPortObject(Quest, QuestEffect, "complete-effects", addCompleteEffect, getCompleteEffect, xmlelement, mode);

        QuestManager::getInstance().registerQuest(this); //!<Registers the Quest with the QuestManager.
    }

    /**
    @brief
        Sets the parentquest of the Quest.
    @param quest
        A pointer to the Quest to be set as parentquest.
    @return
        Returns true if the parentquest could be set.
    */
    bool Quest::setParentQuest(Quest* quest)
    {
        if(quest == NULL) //!< We don't want to set NULL-Pointers.
        {
            COUT(2) << "The parentquest to be added to quest {" << this->getId() << "} was NULL." << std::endl;
            return false;
        }

        this->parentQuest_ = quest;

        COUT(3) << "Parent Quest {" << quest->getId() << "} was added to Quest {" << this->getId() << "}." << std::endl;
        return true;
    }

    /**
    @brief
        Adds a subquest to the Quest.
    @param quest
        A pointer to the Quest to be set as subquest.
    @return
        Returns true if the subquest could be set.
    */
    bool Quest::addSubQuest(Quest* quest)
    {
        if(quest == NULL) //!< We don't want to set NULL-Pointers.
        {
            COUT(2) << "The subquest to be added to quest {" << this->getId() << "} was NULL." << std::endl;
            return false;
        }

        quest->setParentQuest(this); //!< Sets the currentQuest (this) as parentquest for the added subquest.
        this->subQuests_.push_back(quest); //!< Adds the Quest to the end of the list of subquests.

        COUT(3) << "Sub Quest {" << quest->getId() << "} was added to Quest {" << this->getId() << "}." << std::endl;
        return true;
    }


    /**
    @brief
        Adds a QuestHint to the list of QuestHints
    @param hint
        The QuestHint that should be added to the list of QuestHints.
    @return
        Returns true if the hint was successfully added.
    */
    bool Quest::addHint(QuestHint* hint)
    {
        if(hint == NULL) //!< We don't want to set NULL-Pointers. Seriously!
        {
            COUT(2) << "A NULL-QuestHint was trying to be added." << std::endl;
            return false;
        }

        hint->setQuest(this); //!< Sets the current Quest (this) as Quest for the added QuestHint.
        this->hints_.push_back(hint); //!< Adds the QuestHint to the end of the list of QuestHints.

        COUT(3) << "QuestHint {" << hint->getId() << "} was added to Quest {" << this->getId() << "}." << std::endl;
        return true;
    }

    /**
    @brief
        Adds an QuestEffect to the list of fail QuestEffects.
    @param effect
        The QuestEffect to be added.
    @return
        Returns true if successful.
    */
    bool Quest::addFailEffect(QuestEffect* effect)
    {
        if(effect == NULL) //!< We don't want to set NULL-Pointers.
        {
            COUT(2) << "A NULL-QuestEffect was trying to be added" << std::endl;
            return false;
        }

        this->failEffects_.push_back(effect); //!< Adds the QuestEffect to the end of the list of fail QuestEffects.

        COUT(3) << "A FailEffect was added to Quest {" << this->getId() << "}." << std::endl;
        return true;
    }

    /**
    @brief
        Adds an QuestEffect to the list of complete QuestEffects.
    @param effect
        The QuestEffect to be added.
    @return
        Returns true if successful.
    */
    bool Quest::addCompleteEffect(QuestEffect* effect)
    {
        if(effect == NULL) //!< We don't want to set NULL-Pointers.
        {
            COUT(2) << "A NULL-QuestEffect was trying to be added" << std::endl;
            return false;
        }

        this->completeEffects_.push_back(effect); //!< Adds the QuestEffect to the end of the list of complete QuestEffects.

        COUT(3) << "A CompleteEffect was added to Quest {" << this->getId() << "}." << std::endl;
        return true;
    }

    /**
    @brief
        Returns the subquest at the given index.
    @param
        The index.
    @return
        Returns a pointer to the subquest at the given index. NULL if there is no element at the given index.
    */
    const Quest* Quest::getSubQuest(unsigned int index) const
    {
        int i = index;

        //! Iterate through all subquests.
        for (std::list<Quest*>::const_iterator subQuest = this->subQuests_.begin(); subQuest != this->subQuests_.end(); ++subQuest)
        {
            if(i == 0) //!< We're counting down...
            {
               return *subQuest;
            }
            i--;
        }

        return NULL; //!< If the index is greater than the number of elements in the list.
    }

    /**
    @brief
        Returns the QuestHint at the given index.
    @param
        The index.
    @return
        Returns a pointer to the QuestHint at the given index. NULL if there is no element at the given index.
    */
    const QuestHint* Quest::getHint(unsigned int index) const
    {
        int i = index;

        //! Iterate through all QuestHints.
        for (std::list<QuestHint*>::const_iterator hint = this->hints_.begin(); hint != this->hints_.end(); ++hint)
        {
            if(i == 0) //!< We're counting down...
            {
               return *hint;
            }
            i--;
        }
        return NULL; //!< If the index is greater than the number of elements in the list.
    }

    /**
    @brief
        Returns the fail QuestEffect at the given index.
    @param
        The index.
    @return
        Returns a pointer to the fail QuestEffect at the given index. NULL if there is no element at the given index.
    */
    const QuestEffect* Quest::getFailEffect(unsigned int index) const
    {
        int i = index;

        //! Iterate through all fail QuestEffects.
        for (std::list<QuestEffect*>::const_iterator effect = this->failEffects_.begin(); effect != this->failEffects_.end(); ++effect)
        {
            if(i == 0) //!< We're counting down...
            {
               return *effect;
            }
            i--;
        }
        return NULL; //!< If the index is greater than the number of elements in the list.
    }

    /**
    @brief
        Returns the complete QuestEffect at the given index.
    @param
        The index.
    @return
        Returns a pointer to the complete QuestEffect at the given index. NULL if there is no element at the given index.
    */
    const QuestEffect* Quest::getCompleteEffect(unsigned int index) const
    {
        int i = index;

        //! Iterate through all complete QuestEffects.
        for (std::list<QuestEffect*>::const_iterator effect = this->completeEffects_.begin(); effect != this->completeEffects_.end(); ++effect)
        {
            if(i == 0) //!< We're counting down...
            {
               return *effect;
            }
            i--;
        }
        return NULL; //!< If the index is greater than the number of elements in the list.
    }

    /**
    @brief
        Returns true if the quest status for the specific player is 'inactive'.
    @param player
        The player.
    @return
        Returns true if the quest status for the specific player is 'inactive'.
    @throws
        Throws an exception if getStatus throws one.
    */
    bool Quest::isInactive(const PlayerInfo* player) const
    {
        return this->getStatus(player) == QuestStatus::Inactive;
    }

    /**
    @brief
        Returns true if the quest status for the specific player is 'active'.
    @param player
        The player.
    @return
        Returns true if the quest status for the specific player is 'active'.
    @throws
        Throws an exception if getStatus throws one.
    */
    bool Quest::isActive(const PlayerInfo* player) const
    {

        return this->getStatus(player) == QuestStatus::Active;
    }

    /**
    @brief
        Returns true if the quest status for the specific player is 'failed'.
    @param player
        The player.
    @return
        Returns true if the quest status for the specific player is 'failed'.
    @throws
        Throws an exception if getStatus throws one.
    */
    bool Quest::isFailed(const PlayerInfo* player) const
    {
        return this->getStatus(player) == QuestStatus::Failed;
    }

    /**
    @brief
        Returns true if the quest status for the specific player is 'completed'.
    @param player
        The player.
    @return
        Returns true if the quest status for the specific player is 'completed'.
    @throws
        Throws an exception if getStatus throws one.
    */
    bool Quest::isCompleted(const PlayerInfo* player) const
    {
        return this->getStatus(player) == QuestStatus::Completed;
    }

    /**
    @brief
        Fails the Quest for an input player.
    @param player
        The player.
    @return
        Returns true if the Quest could be failed, false if not.
    */
    bool Quest::fail(PlayerInfo* player)
    {
        QuestListener::advertiseStatusChange(this->listeners_, "fail"); //!< Tells the QuestListeners, that the status has changed to failed.
        this->setStatus(player, QuestStatus::Failed);

        COUT(4) << "Quest {" << this->getId() << "} is failed for player: " << player << " ." <<std::endl;

        this->getDescription()->sendFailQuestNotification();
        return true;
    }

    /**
    @brief
        Completes the Quest for an input player.
    @param player
        The player.
    @return
        Returns true if the Quest could be completed, false if not.
    */
    bool Quest::complete(PlayerInfo* player)
    {
        QuestListener::advertiseStatusChange(this->listeners_, "complete"); //!< Tells the QuestListeners, that the status has changed to completed.
        this->setStatus(player, QuestStatus::Completed);

        COUT(4) << "Quest {" << this->getId() << "} is completed for player: " << player << " ." <<std::endl;

        this->getDescription()->sendCompleteQuestNotification();
        return true;
    }

    /**
    @brief
        Starts the Quest for an input player.
    @param player
        The player.
    @return
        Returns true if the Quest could be started, false if not.
    */
    bool Quest::start(PlayerInfo* player)
    {
        if(!this->isStartable(player)) //!< Checks whether the quest can be started.
        {
            COUT(4) << "A non-startable quest was trying to be started." << std::endl;
            return false;
        }

        COUT(4) << "Quest {" << this->getId() << "} is started for player: " << player << " ." <<std::endl;

        QuestListener::advertiseStatusChange(this->listeners_, "start"); //!< Tells the QuestListeners, that the status has changed to active.

        this->setStatus(player, QuestStatus::Active);

        this->getDescription()->sendAddQuestNotification();
        return true;
    }

    /**
    @brief
        Adds a QuestListener to the list of QuestListeners listening to this Quest.
    @param listener
        The QuestListener to be added.
    @return
        Returns true if successful, false if not.
    */
    bool Quest::addListener(QuestListener* listener)
    {
        if(listener == NULL)
        {
            COUT(2) << "A NULL-QuestListener was trying to be added to a Quests listeners." << std::endl;
            return false;
        }

        this->listeners_.push_back(listener);
        return true;
    }

}
