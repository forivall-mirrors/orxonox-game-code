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
    @file QuestHint.cc
    @brief Implementation of the QuestHint class.
*/

#include "QuestHint.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Quest.h"
#include "QuestDescription.h"
#include "QuestManager.h"

namespace orxonox
{
    CreateFactory(QuestHint);

    /**
    @brief
        Constructor. Registers the object.
    */
    QuestHint::QuestHint(Context* context) : QuestItem(context)
    {
        RegisterObject(QuestHint);
    }

    /**
    @brief
        Destructor.
    */
    QuestHint::~QuestHint()
    {
        if(this->isRegistered())
            QuestManager::getInstance().unregisterHint(this);
    }

    /**
    @brief
        Method for creating a QuestHint object through XML.
    */
    void QuestHint::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(QuestHint, XMLPort, xmlelement, mode);

        QuestManager::getInstance().registerHint(this); // Registers the QuestHint with the QuestManager.

        orxout(verbose, context::quests) << "New QuestHint {" << this->getId() << "} created." << endl;
    }


    /**
    @brief
        Checks whether the QuestHint is active for a specific player.
    @param player
        The player.
    @return
        Returns true if the QuestHint is active for the specified player.
    */
    bool QuestHint::isActive(const PlayerInfo* player) const
    {
        if(player == NULL) // If the player is NULL, the Quest obviously can't be active.
            return false;

        // Find the player.
        std::map<const PlayerInfo*, QuestHintStatus::Value>::const_iterator it = this->playerStatus_.find(player);
        if (it != this->playerStatus_.end()) // If the player is in the map.
            return it->second;

        return QuestStatus::Inactive;
    }

    /**
    @brief
        Activates a QuestHint for a given player.
    @param player
        The player.
    @return
        Returns true if the activation was successful, false if there were problems.
    */
    bool QuestHint::setActive(PlayerInfo* player)
    {
        if(this->quest_->isActive(player)) // For a hint to get activated the quest must be active.
        {
            if(!(this->isActive(player)))  // If the hint is already active, activation is pointless.
            {
                this->playerStatus_[player] = QuestHintStatus::Active;

                this->getDescription()->sendAddHintNotification(player);
                return true;
            }
            else
            {
                orxout(verbose, context::quests) << "An already active questHint was trying to get activated." << endl;
                return false;
            }
        }

        orxout(verbose, context::quests) << "A hint of a non-active quest was trying to get activated." << endl;
        return false;
    }

    /**
    @brief
        Sets the Quest the QuestHint belongs to.
    @param quest
        The Quest to be set as Quest the QuestHint is attached to.
    @return
        Returns true if successful.
    */
    bool QuestHint::setQuest(Quest* quest)
    {
        assert(quest);

        this->quest_ = quest;
        return true;
    }

}
