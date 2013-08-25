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
    @file CompleteQuest.h
    @brief Definition of the CompleteQuest class.
    @ingroup QuestEffects
*/

#ifndef _CompleteQuest_H__
#define _CompleteQuest_H__

#include "questsystem/QuestsystemPrereqs.h"
#include "ChangeQuestStatus.h"

namespace orxonox
{

    /**
    @brief
        Completes a @ref orxonox::Quest "Quest" (with a specified id) for the player invoking the QuestEffect.

        Creating a CompleteQuest through XML goes as follows:
        @code
        <CompleteQuest questId="id" />  //Where id identifies the Quest that should be completed.
        @endcode

    @author
        Damian 'Mozork' Frick

    @ingroup QuestEffects
    */
    class _QuestsystemExport CompleteQuest : public ChangeQuestStatus
    {
        public:
            CompleteQuest(Context* context);
            virtual ~CompleteQuest();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a CompleteQuest object through XML.

            virtual bool invoke(PlayerInfo* player); //!< Invokes the QuestEffect.

    };

}

#endif /* _CompleteQuest_H__ */
