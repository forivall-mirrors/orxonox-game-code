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
    @brief Definition of the QuestHint class.
*/

#ifndef _QuestHint_H__
#define _QuestHint_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <map>
#include "QuestItem.h"

namespace orxonox
{
    namespace QuestHintStatus
    {
        //! The state of the hint.
        enum Value
        {
            Inactive,
            Active
        };
    }

    /**
    @brief
        Represents a hint in the game towards completing a Quest.
        Consists of title and description (which is stored in a QuestDescription object) in textual form and must belong to a quest.
        A QuestHint has a defined status (inactive or active, where inactive is default) for each player, which means each a QuestHint exists only once for all players, it doesn't belong to a player, it just has different states for each of them.

        Creating a QuestHint through XML goes as follows:

        <QuestHint id="hintId">  //Where hintId is a GUID, see http://en.wikipedia.org/wiki/Globally_Unique_Identifier#Basic_structure for more information
            <QuestDesctription title="" description="" />
        </QuestHint>
    @author
        Damian 'Mozork' Frick
    */
    class _QuestsystemExport QuestHint : public QuestItem
    {

        public:
            QuestHint(BaseObject* creator);
            virtual ~QuestHint();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a QuestHint object through XML.

            bool isActive(const PlayerInfo* player) const; //!< Returns true if the QuestHint is active for the input player.

            bool setActive(PlayerInfo* player); //!< Activates the QuestHint for the input player.
            bool setQuest(Quest* quest); //!< Sets the Quest the QuestHint belongs to.

            /**
            @brief Returns the Quest the QuestHint is attached to.
            @return  Returns a pointer to the Quest the QuestHint is attached to.
            */
            inline Quest* getQuest(void)
               { return this->quest_; }

        private:
            Quest* quest_; //!< The Quest the QuestHint belongs to.
            std::map<const PlayerInfo*, QuestHintStatus::Value> playerStatus_; //!< List of the states for each player, with the Player-pointer as key.

    };

}

#endif /* _QuestHint_H__ */
