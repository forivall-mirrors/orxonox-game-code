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
    @file GlobalQuest.cc
    @brief Implementation of the GlobalQuest class.
*/

#include "GlobalQuest.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "QuestEffect.h"

namespace orxonox
{
    CreateFactory(GlobalQuest);

    /**
    @brief
        Constructor. Registers the object.
    */
    GlobalQuest::GlobalQuest(BaseObject* creator) : Quest(creator)
    {
        RegisterObject(GlobalQuest);
    }

    /**
    @brief
        Destructor.
    */
    GlobalQuest::~GlobalQuest()
    {

    }

    /**
    @brief
        Method for creating a GlobalQuest object through XML.
    */
    void GlobalQuest::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(GlobalQuest, XMLPort, xmlelement, mode);

        XMLPortObject(GlobalQuest, QuestEffect, "reward-effects", addRewardEffect, getRewardEffects, xmlelement, mode);

        COUT(4) << "New GlobalQuest {" << this->getId() << "} created." << std::endl;
    }

    /**
    @brief
        Fails the Quest for all players.
        Invokes the fail QuestEffects on all the players possessing this Quest.
    @param player
        The player failing it.
    @return
        Returns true if the Quest could be failed, false if not.
    */
    bool GlobalQuest::fail(PlayerInfo* player)
    {
        if(!this->isFailable(player)) // Check whether the Quest can be failed.
        {
            COUT(4) << "A non-completable quest was trying to be failed." << std::endl;
            return false;
        }

        Quest::fail(player);

        // Iterate through all players possessing this Quest.
        for(std::set<PlayerInfo*>::const_iterator it = players_.begin(); it != players_.end(); it++)
            QuestEffect::invokeEffects(*it, this->getFailEffectList());

        return true;
    }

    /**
    @brief
        Completes the Quest for all players.
        Invokes the complete QuestEffects on all the players possessing this Quest.
        Invokes the reward QuestEffects on the player completing the Quest.
    @param player
        The player completing it.
    @return
        Returns true if the Quest could be completed, false if not.
    */
    bool GlobalQuest::complete(PlayerInfo* player)
    {
        if(!this->isCompletable(player)) // Check whether the Quest can be completed.
        {
            COUT(4) << "A non-completable quest was trying to be completed." << std::endl;
            return false;
        }

        // Iterate through all players possessing the Quest.
        for(std::set<PlayerInfo*>::const_iterator it = players_.begin(); it != players_.end(); it++)
            QuestEffect::invokeEffects(*it, this->getCompleteEffectList());

        Quest::complete(player);

        QuestEffect::invokeEffects(player, this->rewards_); // Invoke reward QuestEffects on the player completing the Quest.
        return true;
    }

    /**
    @brief
        Checks whether the Quest can be started.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the quest can be started, false if not.
    */
    bool GlobalQuest::isStartable(const PlayerInfo* player) const
    {
        if(!(this->getParentQuest() == NULL || this->getParentQuest()->isActive(player)))
            return false;

        return (this->isInactive(player) && !(this->status_ == QuestStatus::Completed || this->status_ == QuestStatus::Failed));
    }

    /**
    @brief
        Checks whether the Quest can be failed.
    @param player
        The player for whom is to be checked.
    @return
        Returns true if the Quest can be failed, false if not.
    */
    bool GlobalQuest::isFailable(const PlayerInfo* player) const
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
    */
    bool GlobalQuest::isCompletable(const PlayerInfo* player) const
    {
        return this->isActive(player);
    }

    /**
    @brief
        Returns the status of the Quest for a specific player.
    @param player
        The player.
    */
    QuestStatus::Value GlobalQuest::getStatus(const PlayerInfo* player) const
    {
        assert(player);

        // Find the player.
        std::set<PlayerInfo*>::const_iterator it = this->players_.find((PlayerInfo*)(void*)player);
        if (it != this->players_.end()) // If the player was found.
            return this->status_;

        return QuestStatus::Inactive;
    }

    /**
    @brief
        Sets the status for a specific player.
        But be careful wit this one, the status will just be set without checking for its validity. You have to know what you're doing.
    @param player
        The player.
    @param status
        The status to be set.
    @return
        Returns false if player is NULL.
    */
    bool GlobalQuest::setStatus(PlayerInfo* player, const QuestStatus::Value & status)
    {
        assert(player);

        // Find the player.
        std::set<PlayerInfo*>::const_iterator it = this->players_.find(player);
        if (it == this->players_.end()) // Player is not yet in the list.
            this->players_.insert(player); // Add the player to the set.

        this->status_ = status; // Set the status, which is global, remember...?
        return true;
    }

    /**
    @brief
        Adds a reward QuestEffect to the list of reward QuestEffects.
    @param effect
        The QuestEffect to be added.
    @return
        Returns true if successful.
    */
    bool GlobalQuest::addRewardEffect(QuestEffect* effect)
    {
        assert(effect);

        this->rewards_.push_back(effect); // Add the QuestEffect to the list.

        COUT(4) << "Reward effect was added to Quest {" << this->getId() << "}." << std::endl;
        return true;
    }

    /**
    @brief
        Returns the reward QuestEffect at the given index.
    @param index
        The index.
    @return
        Returns the QuestEffect at the given index.
    */
    const QuestEffect* GlobalQuest::getRewardEffects(unsigned int index) const
    {
        int i = index;
        for (std::list<QuestEffect*>::const_iterator effect = this->rewards_.begin(); effect != this->rewards_.end(); ++effect)
        {
            if(i == 0)
               return *effect;

            i--;
        }
        return NULL;
    }

}
