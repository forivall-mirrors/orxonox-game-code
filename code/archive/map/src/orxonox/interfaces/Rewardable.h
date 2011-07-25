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
    @brief Definition of the Rewardable class.
*/

#ifndef _Rewardable_H__
#define _Rewardable_H__

#include "OrxonoxPrereqs.h"
#include "core/OrxonoxClass.h"

namespace orxonox
{
    /**
    @brief
        Rewardable is an Interface, that can be implemented by any object to enable it to be given as reward to a player through QuestEffects. (With the AddReward effect.)

        It just needs to inherit form Rewardable, and implement the reward() method.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport Rewardable : public OrxonoxClass
    {
        public:
            Rewardable();
            virtual ~Rewardable() {}

            /**
            @brief
                Method to transcribe a rewardable object to the player.
                Must be implemented by every class inheriting from Rewardable.
            @param player
                A pointer to the ControllableEntity, do whatever you want with it.
            @return
                Return true if successful.
            */
            virtual bool reward(PlayerInfo* player) = 0;

    };
}

#endif /* _Rewardable_H__ */
