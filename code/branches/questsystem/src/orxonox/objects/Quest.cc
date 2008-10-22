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

namespace orxonox {

    CreateFactory(Quest);

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
    Quest::Quest(std::string id, std::string title = "", std::string description = "") : QuestItem(id, title, description)
    {
        initialize();
    }
    
    /**
    @brief
        Destructor.
    */
    Quest::~Quest()
    {
        //TDO: Unload lists...
    }
    
    /**
    @brief
        Initializes the object. Needs to be called first in every constructor of this class.
    */
    void Quest::initialize(void)
    {
        RegisterObject(Quest);
        
        this->parentQuest_ = 0;
    }

    /**
    @brief
        Sets the parent quest of the quest.
    @param quest
        A pointer to the quest to be set as parent quest.
    */
    bool setParentQuest(Quest* quest)
    {
        this->parentQuest_ = quest;
        return true;
    }
    
    /**
    @brief
        Adds a sub quest to the quest.
    @param quest
        A pointer to the quest to be set as sub quest.
    */
    bool addSubQuest(Quest & quest)
    {
        this->subQuests_.push_back = quest;
        return true;
    }

    /**
    @brief
        Adds a Hint to the list of hints 
    @param hint
        The hint that should be added to the list of hints.
    */
    void Quest::addHint(QuestHint & hint)
    {
        if ( hint != NULL )
        {
            this->hints_.push_back(hint);
            hint.setQuest(this);
	}
        else
        {
            COUT(2) << "A NULL-QuestHint was trying to be added." << std::endl;
	}
    }
    
    /**
    @brief
        Starts the quest.
    @param player
        The player.
    @return
        Returns true if the quest could be started, false if not.
    */
    bool Quest::start(const Player & player)
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
    void Quest::fail(Player & player)
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
    void Quest::complete(Player & player)
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
