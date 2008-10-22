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
#include "AddReward.h"

namespace orxonox {
    
    CreateFactory(AddReward);
    
    /**
    @brief
        Constructor. Creates a new AddReward effect with an input reward.
    @param reward
        A reward.
    */
    AddReward::AddReward(Rewardable & reward)
    {
        this->initialize();
        this->addRewardable(reward);
    }
    
    /**
    @brief
        Constructor. Creates a new AddReward effect with an input list of rewards.
    @param rewards
        A list of rewards.
    */
    AddReward::AddReward(std::list<Rewardable> & rewards)
    {
        this->initialize();
        this->rewards_ = rewards;
    }
    
    /**
    @brief
        Destructor.
    */
    AddReward::~AddReward()
    {
    }

    /**
    @brief
        Initializes the object. Needs to be called first by every constructor of this class.
    */
    void AddReward::initialize(void)
    {
        RegisterObject(AddReward);
    }

    /**
    @brief
        Invokes the effect.
    @param player
        The player.
    */
    void AddReward::invoke(Player & player)
    {
        if ( this->rewards_ == NULL )
        {
            COUT(2) << "NULL-Rewards list encountered." << std::endl;
            return;
	}
        for ( std::list<Rewardable>::iterator = this->rewards_.begin(); reward != this->rewards_.end(); ++reward )
	{
	    reward.reward(player);
	}
    }

}
