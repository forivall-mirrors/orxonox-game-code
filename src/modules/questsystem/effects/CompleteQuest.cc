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
    @file CompleteQuest.cc
    @brief Implementation of the CompleteQuest class.
*/

#include "CompleteQuest.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "questsystem/Quest.h"
#include "questsystem/QuestManager.h"

namespace orxonox
{
    RegisterClass(CompleteQuest);

    /**
    @brief
        Constructor. Registers the object.
    */
    CompleteQuest::CompleteQuest(Context* context) : ChangeQuestStatus(context)
    {
        RegisterObject(CompleteQuest);
    }

    /**
    @brief
        Destructor.
    */
    CompleteQuest::~CompleteQuest()
    {

    }

    /**
    @brief
        Method for creating a CompleteQuest object through XML.
    */
    void CompleteQuest::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CompleteQuest, XMLPort, xmlelement, mode);

        orxout(verbose, context::quests) << "New CompleteQuest, with target Quest {" << this->getQuestId() << "}, created." << endl;
    }

    /**
    @brief
        Invokes the QuestEffect.
    @param player
        The player the QuestEffect is invoked on.
    @return
        Returns true if the QuestEffect was invoked successfully.
    */
    bool CompleteQuest::invoke(PlayerInfo* player)
    {
        assert(player);

        orxout(verbose_more, context::quests) << "CompleteQuest on player: " << player << " ." << endl;

        Quest* quest;

        try
        {
            quest = QuestManager::getInstance().findQuest(this->getQuestId());
            if(quest == NULL || !quest->complete(player))
               return false;
        }
        catch(const Exception& e)
        {
            orxout(internal_warning, context::quests) << e.getFullDescription() << endl;
            return false;
        }

        orxout(verbose, context::quests) << "Quest {" << quest->getId() << "} successfully completed by player: " << player << " ." << endl;
        return true;
    }


}
