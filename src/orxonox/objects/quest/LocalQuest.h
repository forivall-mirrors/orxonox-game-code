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

#ifndef _LocalQuest_H__
#define _LocalQuest_H__

#include <map>
#include <string>

#include "core/XMLPort.h"
#include "Quest.h"

namespace orxonox {

    class Player; //Forward declaration, remove when fully integrated into the objecthirarchy.

    /**
    @brief
        Handles quests which have different states for different players.
    @author
        Damian 'Mozork' Frick
    */
    class LocalQuest : public Quest
    {
        public:
            LocalQuest(BaseObject* creator);
            virtual ~LocalQuest();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

        protected:
            virtual bool isStartable(const Player* player) const; //!< Checks whether the quest can be started.
            virtual bool isFailable(const Player* player) const; //!< Checks whether the quest can be failed.
            virtual bool isCompletable(const Player* player) const; //!< Checks whether the quest can be completed.

            virtual questStatus::Enum getStatus(const Player* player) const; //!< Returns the status of the quest for a specific player.
            virtual bool setStatus(Player* player, const questStatus::Enum & status); //!< Sets the status for a specific player.

        private:
            std::map<Player*, questStatus::Enum> playerStatus_; //!< List of the status for each player, with the Player-pointer as key.

            void initialize(void);

    };


}

#endif /* _LocalQuest_H__ */
