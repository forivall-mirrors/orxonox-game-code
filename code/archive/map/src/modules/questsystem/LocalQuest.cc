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
    @brief Implementation of the LocalQuest class.
*/

#include "LocalQuest.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "QuestEffect.h"

namespace orxonox
{
    CreateFactory(LocalQuest);

    /**
    @brief
        Constructor. Registers and initializes the object.
    */
    LocalQuest::LocalQuest(BaseObject* creator) : Quest(creator)
    {
        RegisterObject(LocalQuest);
    }

    /**
    @brief
        Destructor.
    */
    LocalQuest::~LocalQuest()
    {

    }

    /**
    @brief
        Method for creating a LocalQuest object through XML.
    */
    void LocalQuest::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(LocalQuest, XMLPort, xmlelement, mode);

        COUT(3) << "New LocalQuest {" << this->getId() << "} created." << std::endl;
    }

    /**
    @brief
        Fails the Quest for a given player.
        Invokes all the failEffects on the player.
    @param player
        The player.
    @return
        Returns true if the Quest could be failed, false if not.
    */
    bool LocalQuest::fail(PlayerInfo* player)
    {
        if(!this->isFailable(player)) //!< Checks whether the quest can be failed.
        {
            COUT(4) << "A non-failable quest was trying to be failed." << std::endl;
            return false;
        }

        Quest::fail(player);

        QuestEffect::invokeEffects(player, this->getFailEffectList()); //!< Invoke the failEffects.
        return true;
    }

    /**
    @brief
        Completes the Quest for a given player.
    Invokes all the complete QuestEffects on the player.
    @param player
        The player.
    @return
        Returns true if the Quest could be completed, false if not.
    */
    bool LocalQuest::complete(PlayerInfo* player)
    {
        if(!this->isCompletable(player)) //!< Checks whether the Quest can be completed.
        {
            COUT(4) << "A non-completable quest was trying to be completed." << std::endl;
            return false;
        }

        Quest::complete(player);

        QuestEffect::invokeEffects(player, this->getCompleteEffectList()); //!< Invoke the complete QuestEffects.
        return true;
    }

    /**
    @brief
        Checks whether the Quest can be started.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the Quest can be started, false if not.
    @throws
        Throws an exception if isInactive(PlayerInfo*) throws one.
    */
    bool LocalQuest::isStartable(const PlayerInfo* player) const
    {
        if(!(this->getParentQuest() == NULL || this->getParentQuest()->isActive(player)))
        {
            return false;
        }
        return this->isInactive(player);
    }

    /**
    @brief
        Checks whether the Quest can be failed.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the Quest can be failed, false if not.
    @throws
        Throws an exception if isActive(PlayerInfo*) throws one.
    */
    bool LocalQuest::isFailable(const PlayerInfo* player) const
    {
        return this->isActive(player);
    }

    /**
    @brief
        Checks whether the Quest can be completed.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the Quest can be completed, false if not.
    @throws
        Throws an exception if isInactive(PlayerInfo*) throws one.
    */
    bool LocalQuest::isCompletable(const PlayerInfo* player) const
    {
        return this->isActive(player);
    }

    /**
    @brief
        Returns the status of the Quest for a specific player.
    @param player
        The player.
    @return
        Returns the status of the Quest for the input player.
    @throws
        Throws an Exception if player is NULL.
    */
    QuestStatus::Value LocalQuest::getStatus(const PlayerInfo* player) const
    {
        if(player == NULL) //!< No player has no defined status.
        {
            ThrowException(Argument, "The input PlayerInfo* is NULL.");
        }

        std::map<const PlayerInfo*, QuestStatus::Value>::const_iterator it = this->playerStatus_.find(player);
        if (it != this->playerStatus_.end()) //!< If there is a player in the map.
        {
            return it->second;
        }

        return QuestStatus::Inactive; //!< If the player is not yet in the map, that means the status of the quest form him is 'inactive'.
    }

    /**
    @brief
        Sets the status for a specific player.
        But be careful wit this one, the status will just be set without checking for its validity. You have to know what you're doing. Really!
    @param player
        The player the status should be set for.
    @param status
        The status to be set.
    @return
        Returns false if player is NULL.
    */
    bool LocalQuest::setStatus(PlayerInfo* player, const QuestStatus::Value & status)
    {
        if(player == NULL) //!< We can't set a status for no player.
        {
            return false;
        }

        this->playerStatus_[player] = status;
        return true;
    }

}
