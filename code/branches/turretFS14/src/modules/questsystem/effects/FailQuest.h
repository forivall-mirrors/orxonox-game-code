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
    @file FailQuest.h
    @brief Definition of the FailQuest class.
    @ingroup QuestEffects
*/

#ifndef _FailQuest_H__
#define _FailQuest_H__

#include "questsystem/QuestsystemPrereqs.h"
#include "ChangeQuestStatus.h"

namespace orxonox
{

    /**
    @brief
        Fails a @ref orxonox::Quest "Quest" (with a specified id) for the player invoking the QuestEffect.

        Creating a FailQuest through XML goes as follows:
        @code
        <FailQuest questId="id" />  //Where id identifies the Quest that should be failed.
        @endcode

    @author
        Damian 'Mozork' Frick

    @ingroup QuestEffects
    */
    class _QuestsystemExport FailQuest : public ChangeQuestStatus
    {
        public:
            FailQuest(Context* context);
            virtual ~FailQuest();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a FailQuest object through XML.

            virtual bool invoke(PlayerInfo* player); //!< Invokes the QuestEffect.

    };

}

#endif /* _FailQuest_H__ */
