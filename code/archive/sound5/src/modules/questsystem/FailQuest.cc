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
    @brief Implementation of the FailQuest class.
*/

#include "FailQuest.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "QuestManager.h"
#include "Quest.h"

namespace orxonox
{
    CreateFactory(FailQuest);

    /**
    @brief
        Constructor. Registers the object.
    */
    FailQuest::FailQuest(BaseObject* creator) : ChangeQuestStatus(creator)
    {
        RegisterObject(FailQuest);
    }

    /**
    @brief
        Destructor.
    */
    FailQuest::~FailQuest()
    {
    }

    /**
    @brief
        Method for creating a FailQuest object through XML.
    */
    void FailQuest::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(FailQuest, XMLPort, xmlelement, mode);

        COUT(3) << "New FailQUest, with target Quest {" << this->getQuestId() << "}, created." << std::endl;
    }

    /**
    @brief
        Invokes the QuestEffect.
    @param player
        The player the QuestEffect is invoked on.
    @return
        Returns true if the QuestEffect was invoked successfully.
    */
    bool FailQuest::invoke(PlayerInfo* player)
    {
        if(player == NULL) //!< We don't know what to do with no player.
        {
            COUT(2) << "Input player is NULL." << std::endl;
            return false;
        }

        COUT(3) << "FailQuest on player: " << player << " ." << std::endl;

        Quest* quest;
        try
        {
            quest = QuestManager::getInstance().findQuest(this->getQuestId());
            if(quest == NULL || !quest->fail(player))
            {
               return false;
            }
        }
        catch(const Exception& e)
        {
            COUT(2) << e.getFullDescription() << std::endl;
            return false;
        }

        COUT(3) << "Quest {" << quest->getId() << "} failed by player: " << player << " ." << std::endl;
        return true;
    }


}
