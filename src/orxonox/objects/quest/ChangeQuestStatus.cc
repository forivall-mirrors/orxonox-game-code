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

#include "QuestItem.h"
#include "ChangeQuestStatus.h"

namespace orxonox {

    ChangeQuestStatus::ChangeQuestStatus(BaseObject* creator) : QuestEffect(creator)
    {
        RegisterObject(ChangeQuestStatus);
    }

    /**
    @brief
        Destructor.
    */
    ChangeQuestStatus::~ChangeQuestStatus()
    {
    }

    void ChangeQuestStatus::setQuestId(const std::string & id)
    {
        if(!QuestItem::isId(id))
        {
            COUT(2) << "Invalid id. QuestItem id {" << id << "} could not be set." << std::endl;
            return;
        }
        this->questId_ = id;
    }

    void ChangeQuestStatus::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ChangeQuestStatus, XMLPort, xmlelement, mode);

        XMLPortParam(ChangeQuestStatus, "questId", setQuestId, getQuestId, xmlelement, mode);
    }

}
