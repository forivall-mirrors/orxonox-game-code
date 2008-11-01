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

#include "core/CoreIncludes.h"

#include "Quest.h"
#include "QuestManager.h"

namespace orxonox {

    Quest::Quest() : QuestItem()
    {
        this->initialize();
    }
    
    /**
    @brief
        Destructor.
    */
    Quest::~Quest()
    {
        
    }
    
    void Quest::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Quest, XMLPort, xmlelement, mode);
        
        XMLPortObject(Quest, Quest, "", addSubQuest, getSubQuests, xmlelement, mode);
        XMLPortObject(Quest, QuestHint, "", addHint, getHints, xmlelement, mode);
        XMLPortObject(Quest, QuestEffect, "fail-effects", addFailEffect, getFailEffects, xmlelement, mode);
        XMLPortObject(Quest, QuestEffect, "complete-effects", addCompleteEffect, getCompleteEffects, xmlelement, mode);
        
        QuestManager::registerQuest(this); //Registers the quest with the QuestManager.
    }
    
    /**
    @brief
        Initializes the object. Needs to be called first in every constructor of this class.
    */
    void Quest::initialize(void)
    {
        RegisterObject(Quest);
        
        this->parentQuest_ = NULL;
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
        if(quest == NULL)
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
        if(quest == NULL)
        {
            COUT(2) << "The subquest to be added to quest {" << this->getId() << "} was NULL." << std::endl;
            return false;
        }
        
        quest->setParentQuest(this);
        this->subQuests_.push_back(quest);
        
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
        if(hint == NULL)
        {
            COUT(2) << "A NULL-QuestHint was trying to be added." << std::endl;
            return false;
        }
        
	this->hints_.push_back(hint);
	hint->setQuest(this);
	
	COUT(3) << "QuestHint {" << hint->getId() << "} was added to Quest {" << this->getId() << "}." << std::endl;
	return true;
    }
    
    /**
    @brief
        
    */
    bool Quest::addFailEffect(QuestEffect* effect)
    {
        if(effect == NULL)
        {
            COUT(2) << "A NULL-QuestEffect was trying to be added" << std::endl;
            return false;
        }
        
        this->failEffects_.push_back(effect);
        
        COUT(3) << "A FailEffect was added to Quest {" << this->getId() << "}." << std::endl;
        return true;
    }
    
    /**
    @brief
        
    */
    bool Quest::addCompleteEffect(QuestEffect* effect)
    {
        if(effect == NULL)
        {
            COUT(2) << "A NULL-QuestEffect was trying to be added" << std::endl;
            return false;
        }
        
        this->completeEffects_.push_back(effect);
        
        COUT(3) << "A CompleteEffect was added to Quest {" << this->getId() << "}." << std::endl;
        return true;
    }
    
    /**
    @brief
        
    */
    const Quest* Quest::getParentQuest(void)
    {
        return this->parentQuest_;
    }
    
    /**
    @brief
        
    */
    const Quest* Quest::getSubQuests(unsigned int index) const
    {
        int i = index;
        for (std::list<Quest*>::const_iterator subQuest = this->subQuests_.begin(); subQuest != this->subQuests_.end(); ++subQuest)
	{
	    if(i == 0)
	    {
	       return *subQuest;
	    }
	    i--;
	}
        return NULL;
    }
    
    /**
    @brief
        
    */
    const QuestHint* Quest::getHints(unsigned int index) const
    {
        int i = index;
        for (std::list<QuestHint*>::const_iterator hint = this->hints_.begin(); hint != this->hints_.end(); ++hint)
	{
	    if(i == 0)
	    {
	       return *hint;
	    }
	    i--;
	}
        return NULL;
    }
    
    /**
    @brief
        
    */
    const QuestEffect* Quest::getFailEffects(unsigned int index) const
    {
        int i = index;
        for (std::list<QuestEffect*>::const_iterator effect = this->failEffects_.begin(); effect != this->failEffects_.end(); ++effect)
	{
	    if(i == 0)
	    {
	       return *effect;
	    }
	    i--;
	}
        return NULL;
    }
    
    /**
    @brief
        
    */
    const QuestEffect* Quest::getCompleteEffects(unsigned int index) const
    {
        int i = index;
        for (std::list<QuestEffect*>::const_iterator effect = this->completeEffects_.begin(); effect != this->completeEffects_.end(); ++effect)
	{
	    if(i == 0)
	    {
	       return *effect;
	    }
	    i--;
	}
        return NULL;
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
    bool Quest::isInactive(const Player* player) const
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
    bool Quest::isActive(const Player* player) const
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
    bool Quest::isFailed(const Player* player) const
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
    bool Quest::isCompleted(const Player* player) const
    {
        return this->getStatus(player) == questStatus::completed;
    }
    
    /**
    @brief
        Starts the quest.
    @param player
        The player.
    @return
        Returns true if the quest could be started, false if not.
    */
    bool Quest::start(Player* player)
    {
        if(this->isStartable(player))
        {
            this->setStatus(player, questStatus::active);
            return true;
        }
        COUT(2) << "A non-startable quest was trying to be started." << std::endl;
        return false;
    }
    
    /**
    @brief
        Fails the quest.
    @param player
        The player.
    @return
        Returns true if the quest could be failed, false if not.
    */
    bool Quest::fail(Player* player)
    {
        if(this->isFailable(player))
        {
            this->setStatus(player, questStatus::failed);
            QuestEffect::invokeEffects(player, this->failEffects_);
            return true;
        }
        COUT(2) << "A non-failable quest was trying to be failed." << std::endl;
        return false;
    }
    
    /**
    @brief
        Completes the quest.
    @param player
        The player.
    @return
        Returns true if the quest could be completed, false if not.
    */
    bool Quest::complete(Player* player)
    {
        if(this->isCompletable(player))
        {
            this->setStatus(player, questStatus::completed);
            QuestEffect::invokeEffects(player, this->completeEffects_);
            return true;
        }
        COUT(2) << "A non-completable quest was trying to be completed." << std::endl;
        return false;
    }

}
