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
    @file AddReward.h
    @brief Definition of the AddReward class.
    @ingroup QuestEffects
*/

#ifndef _AddReward_H__
#define _AddReward_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <list>
#include "questsystem/QuestEffect.h"

namespace orxonox
{
    
    /**
    @brief
        Adds a list of @ref orxonox::Rewardable "Rewardables" to a player.

        Creating a AddReward through XML goes as follows:
        @code
        <AddReward>
            <Rewardable /> //A list of Rewardable objects to be rewarded the player, see the specific Rewardables for their respective XML representations.
            ...
            <Rewardable />
        </AddReward>
        @endcode

    @author
        Damian 'Mozork' Frick

    @ingroup QuestEffects
    */
    class _QuestsystemExport AddReward : public QuestEffect
    {
        public:
            AddReward(Context* context);
            virtual ~AddReward();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a AddReward object through XML.

            virtual bool invoke(PlayerInfo* player); //!< Invokes the QuestEffect.

        private:
            std::list<Rewardable*> rewards_; //!< A list of Rewardables to be added to the player invoking the QuestEffect.

            /**
            @brief Add a Rewardable object to the list of objects to be awarded to the player invoking the QuestEffect.
            @param reward Pointer to the Rewardable to be added.
            */
            inline void addRewardable(Rewardable* reward)
                { this->rewards_.push_back(reward); }

            const Rewardable* getRewardables(unsigned int index) const; //!< Returns the Rewardable object at the given index.

    };

}

#endif /* _AddReward_H__ */
