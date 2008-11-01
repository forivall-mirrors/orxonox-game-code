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
#include "Rewardable.h"

namespace orxonox {

    CreateFactory(AddReward);

    AddReward::AddReward(BaseObject* creator) : QuestEffect(creator)
    {
        RegisterObject(AddReward);

        this->initialize();
    }

    /**
    @brief
        Destructor.
    */
    AddReward::~AddReward()
    {
    }

    void AddReward::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(AddReward, XMLPort, xmlelement, mode);

        XMLPortObject(AddReward, Rewardable, "", addRewardable, getRewardables, xmlelement, mode);

    }

    /**
    @brief
        Initializes the object. Needs to be called first by every constructor of this class.
    */
    void AddReward::initialize(void)
    {
        RegisterObject(AddReward);
    }

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
        Invokes the effect.
    @param player
        The player.
    @return
        Returns true if the effect was invoked successfully.
    */
    bool AddReward::invoke(Player* player)
    {
        bool check = true;
        for ( std::list<Rewardable*>::iterator reward = this->rewards_.begin(); reward != this->rewards_.end(); ++reward )
        {
            check = check && (*reward)->reward(player);
        }

        return check;
    }

}
