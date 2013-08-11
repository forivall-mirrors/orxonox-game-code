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
    @file AddQuestHint.cc
    @brief Implementation of the AddQuestHint class.
*/

#include "AddQuestHint.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "util/Exception.h"

#include "questsystem/QuestHint.h"
#include "questsystem/QuestManager.h"
#include "questsystem/QuestItem.h"

namespace orxonox
{
    RegisterClass(AddQuestHint);

    /**
    @brief
        Constructor. Registers the object.
    */
    AddQuestHint::AddQuestHint(Context* context) : QuestEffect(context)
    {
        RegisterObject(AddQuestHint);
    }

    /**
    @brief
        Destructor.
    */
    AddQuestHint::~AddQuestHint()
    {
    }

    /**
    @brief
        Method for creating a AddQuestHint object through XML.
    */
    void AddQuestHint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AddQuestHint, XMLPort, xmlelement, mode);

        XMLPortParam(AddQuestHint, "hintId", setHintId, getHintId, xmlelement, mode);

        orxout(verbose, context::quests) << "New AddQuestHint, with target QuestHint {" << this->getHintId() << "}, created." << endl;
    }

    /**
    @brief
        Sets the id of the QuestHint to be added to the player the QuestEffect is invoked on.
    @param id
        The QuestHint id.
    @return
        Returns true if successful.
    */
    bool AddQuestHint::setHintId(const std::string & id)
    {
        if(id == "")
        {
            orxout(internal_warning, context::quests) << "Invalid id. QuestItem id {" << id << "} could not be set." << endl;
            return false;
        }

        this->hintId_ = id;
        return true;
    }

    /**
    @brief
        Invokes the QuestEffect.
    @param player
        The player.
    @return
        Returns true if the QuestEffect was successfully invoked.
    */
    bool AddQuestHint::invoke(PlayerInfo* player)
    {
        assert(player);

        orxout(verbose_more, context::quests) << "AddQuestHint on player: " << player << " ." << endl;

        try
        {
            QuestHint* hint = QuestManager::getInstance().findHint(this->hintId_);
            if(hint == NULL || !hint->setActive(player))
                return false;
        }
        catch(const Exception& e)
        {
           orxout(internal_warning, context::quests) << e.getFullDescription() << endl;
           return false;
        }

        orxout(verbose, context::quests) << "QuestHint {" << this->getHintId() << "} successfully added to player: " << player << " ." << endl;
        return true;

    }
}
