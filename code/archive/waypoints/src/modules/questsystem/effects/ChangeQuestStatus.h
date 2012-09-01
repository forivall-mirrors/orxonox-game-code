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
    @file ChangeQuestStatus.h
    @brief Definition of the ChangeQuestStatus class.
    @ingroup QuestEffects
*/

#ifndef _ChangeQuestStatus_H__
#define _ChangeQuestStatus_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <string>
#include "questsystem/QuestEffect.h"

namespace orxonox
{

    /**
    @brief
        A QuestEffect which changes the status of a specified @ref orxonox::Quest "Quest" for the player invoking the QuestEffect.

    @author
        Damian 'Mozork' Frick

    @ingroup QuestEffects
    */
    class _QuestsystemExport ChangeQuestStatus : public QuestEffect
    {
        public:
            ChangeQuestStatus(BaseObject* creator);
            virtual ~ChangeQuestStatus();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a ChangeQuestStatus object through XML.

            virtual bool invoke(PlayerInfo* player) = 0; //!< Invokes the QuestEffect.

        protected:
            /**
            @brief Returns the id of the Quest.
            @return Returns the id of the Quest.
            */
            inline const std::string & getQuestId(void) const
                { return this->questId_; }

        private:
            std::string questId_; //!< The id of the Quest the status should be changed of.

            bool setQuestId(const std::string & id); //!< Sets the id of the Quest.

    };

}

#endif /* _ChangeQuestStatus_H__ */
