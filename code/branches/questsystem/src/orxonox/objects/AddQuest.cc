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

#include <string>

#include "core/CoreIncludes.h"

#include "QuestManager.h"
#include "Quest.h"
#include "AddQuest.h"

namespace orxonox {

    CreateFactory(AddQuest);


    AddQuest::AddQuest() : ChangeQuestStatus()
    {
        
    }

    /**
    @brief
        Constructor.
    @param questId
        The id of the quest to be added.
    */
    AddQuest::AddQuest(std::string questId) : ChangeQuestStatus(questId)
    {
        RegisterObject(AddQuest);
    }
    
    /**
    @brief
        Destructor.
    */ 
    AddQuest::~AddQuest()
    {
    }
    
    /**
    @brief
        Invokes the effect.
    @param player
        The player the effect is invoked on.
    */
    void AddQuest::invoke(Player* player)
    {
        Quest* quest = QuestManager::findQuest(this->getQuestId());
        quest->start(player);
    }


}
