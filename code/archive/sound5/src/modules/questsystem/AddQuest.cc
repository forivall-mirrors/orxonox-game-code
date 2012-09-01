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
    @brief Implementation of the AddQuest class.
*/

#include "AddQuest.h"

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "QuestManager.h"
#include "Quest.h"

namespace orxonox
{
    CreateFactory(AddQuest);

    /**
    @brief
        Constructor. Registers the object.
    */
    AddQuest::AddQuest(BaseObject* creator) : ChangeQuestStatus(creator)
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
        Method for creating a AddQuest object through XML.
    */
    void AddQuest::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AddQuest, XMLPort, xmlelement, mode);

        COUT(3) << "New AddQuest, with target Quest {" << this->getQuestId() << "}, created." << std::endl;
    }

    /**
    @brief
        Invokes the QuestEffect.
    @param player
        The player the QuestEffect is invoked on.
    @return
        Returns true if the QuestEffect was successfully invoked.
    */
    bool AddQuest::invoke(PlayerInfo* player)
    {
        if(player == NULL) //!< Null-pointers are badass.
        {
            COUT(2) << "Input player is NULL." << std::endl;
            return false;
        }

        COUT(3) << "AddQuest on player: " << player << " ." << std::endl;

        try
        {
            Quest* quest = QuestManager::getInstance().findQuest(this->getQuestId());
            if(quest == NULL || !quest->start(player))
            {
               return false;
            }
        }
        catch(const orxonox::Exception& ex)
        {
            COUT(2) << ex.getFullDescription() << std::endl;
            return false;
        }

        COUT(3) << "Quest {" << this->getQuestId() << "} successfully added to player: " << player << " ." << std::endl;
        return true;
    }


}
