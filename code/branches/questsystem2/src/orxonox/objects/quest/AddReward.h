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
    @brief
	Definition of the AddReward class.
*/

#ifndef _AddReward_H__
#define _AddReward_H__

#include "OrxonoxPrereqs.h"

#include <list>

#include "core/XMLPort.h"
#include "QuestEffect.h"

namespace orxonox {

    /**
    @brief
        Adds a list of rewards to a player.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport AddReward : public QuestEffect
    {
        public:
            AddReward(BaseObject* creator);
            virtual ~AddReward();

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a AddReward object through XML.

            virtual bool invoke(ControllableEntity* player); //!< Invokes the effect.

        private:
            std::list<Rewardable*> rewards_; //!< A list of rewards to be added to the player invoking the effect.

            inline void addRewardable(Rewardable* reward) //!< Add a rewardable object to the list of objects to be awarded to the player invoking the effect.
                { this->rewards_.push_back(reward); }
            const Rewardable* getRewardables(unsigned int index) const; //!< Returns the rewardable object at the given index.

    };

}

#endif /* _AddReward_H__ */
