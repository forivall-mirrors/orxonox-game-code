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
#include "util/Exception.h"

#include "QuestManager.h"
#include "QuestItem.h"
#include "AddQuestHint.h"

namespace orxonox {

    CreateFactory(AddQuestHint);

    AddQuestHint::AddQuestHint(BaseObject* creator) : QuestEffect(creator)
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

    void AddQuestHint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AddQuestHint, XMLPort, xmlelement, mode);

        XMLPortParam(AddQuestHint, "hintId", setHintId, getHintId, xmlelement, mode);

    }

    inline void AddQuestHint::setHintId(const std::string & id)
    {
        if(!QuestItem::isId(id))
        {
            COUT(2) << "Invalid id. QuestItem id {" << id << "} could not be set." << std::endl;
            return;
        }
        this->hintId_ = id;
    }

    /**
    @brief
        Invokes the effect.
    @param player
        The player.
    @return
        Returns true if the effect was successfully invoked.
    */
    bool AddQuestHint::invoke(Player* player)
    {
        if(player == NULL)
        {
            COUT(2) << "The input player is NULL." << std::endl;
            return false;
        }

        try
        {
            QuestHint* hint = QuestManager::findHint(this->hintId_);
            if(!hint->activate(player))
            {
                return false;
            }
        }
        catch(const Exception& e)
        {
           COUT(2) << e.getFullDescription() << std::endl;
           return false;
        }

        return true;

    }
}
