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
#include "util/Exception.h"

#include "LocalQuest.h"

namespace orxonox {

    CreateFactory(LocalQuest);

    LocalQuest::LocalQuest() : Quest()
    {
        this->initialize();
    }

    /**
    @brief
        Constructor.
    @param id
        The unique identifier.
    */
    LocalQuest::LocalQuest(std::string id) : Quest(id)
    {
        this->initialize();
    }
    
    /**
    @brief
        Destructor.
    */
    LocalQuest::~LocalQuest()
    {
        
    }
    
    void LocalQuest::initialize(void)
    {
        RegisterObject(LocalQuest);
    }
    
    /**
    @brief
        Checks whether the quest can be started.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the quest can be started, false if not.
    @throws
        Throws an exception if isInactive(Player*) throws one.
    */
    bool LocalQuest::isStartable(const Player* player) const
    {
        return this->isInactive(player);
    }
    
    /**
    @brief
        Checks whether the quest can be failed.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the quest can be failed, false if not.
    @throws
        Throws an exception if isActive(Player*) throws one.
    */
    bool LocalQuest::isFailable(const Player* player) const
    {
        return this->isActive(player);
    }
    
    /**
    @brief
        Checks whether the quest can be completed.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the quest can be completed, false if not.
    @throws
        Throws an exception if isInactive(Player*) throws one.
    */
    bool LocalQuest::isCompletable(const Player* player) const
    {
        return this->isActive(player);
    }
    
    /**
    @brief
        Returns the status of the quest for a specific player.
    @param player
        The player.
    @return
        Returns the status of the quest for the input player.
    @throws
        Throws an Exception if player is NULL.
    */
    questStatus::Enum LocalQuest::getStatus(const Player* player) const
    {
        if(player == NULL)
        {
            ThrowException(Argument, "The input Player* is NULL.");
        }
        
        std::map<Player*, questStatus::Enum>::const_iterator it = this->playerStatus_.find((Player*)(void*)player); //Thx. to x3n for the (Player*)(void*) 'hack'.
	if (it != this->playerStatus_.end())
	{
	    return it->second;
	}
	return questStatus::inactive;
    }
    
    /**
    @brief
        Sets the status for a specific player.
        But be careful wit this one, the status will just be set without checking for its validity. You have to know what you're doing.
    @param player
        The player.
    @param status
        The status.
    @return
        Returns false if player is NULL.
    */
    bool LocalQuest::setStatus(Player* player, const questStatus::Enum & status)
    {
        if(player == NULL)
        {
            return false;
	}
        this->playerStatus_[player] = status;
        return true;
    }

}
