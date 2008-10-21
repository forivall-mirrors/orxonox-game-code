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
#include "AddQuestHint.h"

namespace orxonox {

    CreateFactory(AddQuestHint);

    /**
    @brief
        Constructor.
    @param hintId
        The id of the hint to be set to active.
    */
    AddQuestHint::AddQuestHint(std::string hintId) : QuestEffect()
    {
        RegisterObject(AddQuestHint);
        this->hintId_ = hintId;
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
        Invokes the effect.
    @param player
        The player.
    */
    virtual void invoke(Player & player)
    {
        QuestHint hint = QuestManager::findHint(this->hintId_);
        hint.activate(player);
    }
}
