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
        Constructor. Creates a quest with a given id, title and description.
    @param id
        The unique identifier of the quest.
    @param title
        The title of the quest.
    @param description
        The description of the quest.
    */
    Quest::Quest(std::string id) : QuestItem(id)
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
    
    /**
    @brief
        Initializes the object. Needs to be called first in every constructor of this class.
    */
    void Quest::initialize(void)
    {
        RegisterObject(Quest);
        
        this->parentQuest_ = NULL;
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
        if(quest == NULL)
        {
            COUT(2) << "The parentquest to be added to quest {" << this->getId() << "} was NULL." << std::endl;
            return false;
        }
        
        this->parentQuest_ = quest;
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
        
        this->subQuests_.push_back(quest);
        return true;
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
	return true;
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
