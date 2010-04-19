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
    @brief Definition of the AddQuest class.
*/

#ifndef _AddQuest_H__
#define _AddQuest_H__

#include "questsystem/QuestsystemPrereqs.h"
#include "ChangeQuestStatus.h"

namespace orxonox
{
    /**
    @brief
        Adds a Quest, resp. changes the quests status to active for the player invoking the Quest.

        Creating a AddQuest through XML goes as follows:

        <AddQuest questId="id" />  //Where id is a GUID, see http://en.wikipedia.org/wiki/Globally_Unique_Identifier#Basic_structure for more information, and identifies the Quest that should be added.
    @author
        Damian 'Mozork' Frick
    */
    class _QuestsystemExport AddQuest : public ChangeQuestStatus
    {
        public:
        AddQuest(BaseObject* creator);
        virtual ~AddQuest();

        virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a AddQuest object through XML.

        virtual bool invoke(PlayerInfo* player); //!< Invokes the QuestEffect.

    };

}

#endif /* _AddQuest_H__ */
