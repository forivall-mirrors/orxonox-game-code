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
    @file Quest.cc
    @brief
	Implementation of the Quest class.
*/

#include "OrxonoxStableHeaders.h"
#include "Quest.h"

#include "core/CoreIncludes.h"

#include "orxonox/objects/worldentities/ControllableEntity.h"
#include "QuestManager.h"
#include "QuestDescription.h"
#include "QuestHint.h"
#include "QuestEffect.h"

namespace orxonox {

    /**
    @brief
        Constructor. Initializes object.
    */
    Quest::Quest(BaseObject* creator) : QuestItem(creator)
    {
        this->initialize();
    }
    
    /**
    @brief
        Initializes the object. Needs to be called first in every constructor of this class.
        Sets defaults.
    */
    void Quest::initialize(void)
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

        XMLPortObject(Quest, Quest, "subquests", addSubQuest, getSubQuests, xmlelement, mode);
        XMLPortObject(Quest, QuestHint, "hints", addHint, getHints, xmlelement, mode);
        XMLPortObject(Quest, QuestEffect, "fail-effects", addFailEffect, getFailEffects, xmlelement, mode);
        XMLPortObject(Quest, QuestEffect, "complete-effects", addCompleteEffect, getCompleteEffects, xmlelement, mode);

        QuestManager::registerQuest(this); //Registers the quest with the QuestManager.
    }

    /**
    @brief
        Sets the parent quest of the quest.
    @param quest
        A pointer to the quest to be set as parent quest.
    @return
        Returns true if the parentQuest could be set.
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
        Adds a sub quest to the quest.
    @param quest
        A pointer to the quest to be set as sub quest.
    @return
        Returns true if the subQuest vould be set.
    */
    bool Quest::addSubQuest(Quest* quest)
    {
        if(quest == NULL) //!< We don't want to set NULL-Pointers.
        {
            COUT(2) << "The subquest to be added to quest {" << this->getId() << "} was NULL." << std::endl;
            return false;
        }

        quest->setParentQuest(this); //!< Sets the current quest (this) as parent quest for the added subquest.
        this->subQuests_.push_back(quest); //!< Adds the quest to the end of the list of subquests.

        COUT(3) << "Sub Quest {" << quest->getId() << "} was added to Quest {" << this->getId() << "}." << std::endl;
        return true;
    }


    /**
    @brief
        Adds a Hint to the list of hints
    @param hint
        The hint that should be added to the list of hints.
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

        hint->setQuest(this); //!< Sets the current quest (this) as quest for the added hint.
        this->hints_.push_back(hint); //!< Adds the hint to the end of the list of hints.

        COUT(3) << "QuestHint {" << hint->getId() << "} was added to Quest {" << this->getId() << "}." << std::endl;
        return true;
    }

    /**
    @brief
        Adds an effect to the list of failEffects.
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

        this->failEffects_.push_back(effect); //!< Adds the effect to the end of the list of failEffects.

        COUT(3) << "A FailEffect was added to Quest {" << this->getId() << "}." << std::endl;
        return true;
    }

    /**
    @brief
        Adds an effect to the list of completeEffects.
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

        this->completeEffects_.push_back(effect); //!< Adds the effect to the end of the list of completeEffects.

        COUT(3) << "A CompleteEffect was added to Quest {" << this->getId() << "}." << std::endl;
        return true;
    }

    /**
    @brief
        Returns the parent quest of the quest.
    @return
        Returns the parent quest of the quest.
    */
    const Quest* Quest::getParentQuest(void)
    {
        return this->parentQuest_;
    }

    /**
    @brief
        Returns the sub quest of the given index.
    @param
        The index.
    @return
        Returns the subquest of the given index. NULL if there is no element on the given index.
    */
    const Quest* Quest::getSubQuests(unsigned int index) const
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
        Returns the hint of the given index.
    @param
        The index.
    @return
        Returns the hint of the given index. NULL if there is no element on the given index.
    */
    const QuestHint* Quest::getHints(unsigned int index) const
    {
        int i = index;
        
        //! Iterate through all hints.
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
        Returns the failEffect of the given index.
    @param
        The index.
    @return
        Returns the failEffect of the given index. NULL if there is no element on the given index.
    */
    const QuestEffect* Quest::getFailEffects(unsigned int index) const
    {
        int i = index;
        
        //! Iterate through all failEffects.
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
        Returns the completeEffect of the given index.
    @param
        The index.
    @return
        Returns the completeEffect of the given index. NULL if there is no element on the given index.
    */
    const QuestEffect* Quest::getCompleteEffects(unsigned int index) const
    {
        int i = index;
        
        //! Iterate through all completeEffects.
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
    bool Quest::isInactive(const ControllableEntity* player) const
    {
        return this->getStatus(player) == questStatus::inactive;
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
    bool Quest::isActive(const ControllableEntity* player) const
    {

        return this->getStatus(player) == questStatus::active;
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
    bool Quest::isFailed(const ControllableEntity* player) const
    {
        return this->getStatus(player) == questStatus::failed;
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
    bool Quest::isCompleted(const ControllableEntity* player) const
    {
        return this->getStatus(player) == questStatus::completed;
    }

    /**
    @brief
        Starts the quest for an input player.
    @param player
        The player.
    @return
        Returns true if the quest could be started, false if not.
    */
    bool Quest::start(ControllableEntity* player)
    {
        if(this->isStartable(player)) //!< Checks whether the quest can be started.
        {
            this->setStatus(player, questStatus::active);
            return true;
        }
        
        COUT(2) << "A non-startable quest was trying to be started." << std::endl;
        return false;
    }

}
