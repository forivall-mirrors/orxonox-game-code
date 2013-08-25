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
    @file ChangeQuestStatus.cc
    @brief Implementation of the ChangeQuestStatus class.
    @ingroup QuestEffects
*/

#include "ChangeQuestStatus.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "questsystem/QuestItem.h"

namespace orxonox
{
    RegisterAbstractClass(ChangeQuestStatus).inheritsFrom(Class(QuestEffect));

    /**
    @brief
        Constructor. Registers the object.
    */
    ChangeQuestStatus::ChangeQuestStatus(Context* context) : QuestEffect(context)
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

    /**
    @brief
        Method for creating a ChangeQuestStatus object through XML.
    */
    void ChangeQuestStatus::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(ChangeQuestStatus, XMLPort, xmlelement, mode);

        XMLPortParam(ChangeQuestStatus, "questId", setQuestId, getQuestId, xmlelement, mode);
    }

    /**
    @brief
        Sets the id of the Quest the Questffect changes the status of.
    @param id
        The id of the Quest.
    @return
        Returns true if successful.
    */
    bool ChangeQuestStatus::setQuestId(const std::string & id)
    {
        if(id == "")
        {
            orxout(internal_warning, context::quests) << "Invalid id. QuestItem id {" << id << "} could not be set." << endl;
            return false;
        }

        this->questId_ = id;
        return true;
    }

}
