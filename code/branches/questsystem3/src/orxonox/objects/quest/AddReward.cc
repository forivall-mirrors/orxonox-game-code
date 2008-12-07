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
    @file AddReward.cc
    @brief Implementation of the AddReward class.
*/

#include "OrxonoxStableHeaders.h"
#include "AddReward.h"

#include "core/CoreIncludes.h"

#include "orxonox/objects/infos/PlayerInfo.h"
#include "Rewardable.h"

namespace orxonox {

    CreateFactory(AddReward);

    /**
    @brief
        Constructor. Registers the object.
    */
    AddReward::AddReward(BaseObject* creator) : QuestEffect(creator)
    {
        RegisterObject(AddReward);
    }

    /**
    @brief
        Destructor.
    */
    AddReward::~AddReward()
    {
    }

    /**
        Method for creating a AddReward object through XML.
    */
    void AddReward::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AddReward, XMLPort, xmlelement, mode);

        XMLPortObject(AddReward, Rewardable, "", addRewardable, getRewardables, xmlelement, mode);
        
        COUT(3) << "New AddReward, with " << this->rewards_.size() << " Rewardables created." << std::endl;
    }

    /**
    @brief
        Returns the Rewardable object at the given index.
    @param index
        The index.
    @return
        Returns a pointer to the Rewardable object at the given index.
    */
    const Rewardable* AddReward::getRewardables(unsigned int index) const
    {
        int i = index;
        for (std::list<Rewardable*>::const_iterator reward = this->rewards_.begin(); reward != this->rewards_.end(); ++reward)
        {
            if(i == 0)
            {
               return *reward;
            }
            i--;
        }
        return NULL;
    }

    /**
    @brief
        Invokes the QuestEffect.
    @param player
        The player.
    @return
        Returns true if the QuestEffect was invoked successfully.
    */
    bool AddReward::invoke(PlayerInfo* player)
    {
        bool check = true;
        for ( std::list<Rewardable*>::iterator reward = this->rewards_.begin(); reward != this->rewards_.end(); ++reward )
        {
            check = check && (*reward)->reward(player);
        }

        return check;
    }

}
