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
    @brief Implementation of the QuestManager class.
*/

#include "QuestManager.h"

#include <CEGUIWindow.h>

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/GUIManager.h"
#include "core/ConsoleCommand.h"
#include "core/LuaState.h"
#include "core/ScopedSingletonManager.h"
#include "infos/PlayerInfo.h"
#include "overlays/GUIOverlay.h"

#include "ToluaBindQuestsystem.h"
#include "Quest.h"
#include "QuestHint.h"
#include "QuestItem.h"

namespace orxonox
{
    // Register tolua_open function when loading the library
    DeclareToluaInterface(Questsystem);

    ManageScopedSingleton(QuestManager, ScopeID::Root, false);

    /**
    @brief
        Constructor. Registers the object.
    @todo
        Is inheriting from BaseObject proper?
    */
    QuestManager::QuestManager()
    {
        RegisterRootObject(QuestManager);
    }

    /**
    @brief
        Destructor.
    */
    QuestManager::~QuestManager()
    {

    }

    /**
    @brief
        Retreive all Quests.
    @return
        Returns a map with all Quests indexed by their id's.
    */
    std::map<std::string, Quest*> & QuestManager::getQuests(void)
    {
        return this->questMap_;
    }

    /**
    @brief
        Registers a Quest with the QuestManager to make it globally accessable.
        Uses it's id to make sure to be able to be identify and retrieve it later.
    @param quest
        The Quest that is to be registered.
    @return
        Returns true if successful, false if not.
    */
    bool QuestManager::registerQuest(Quest* quest)
    {
        if(quest == NULL) //!< Doh! Just as if there were actual quests behind NULL-pointers.
        {
            COUT(2) << "Registration of Quest in QuestManager failed, because inserted Quest-pointer was NULL." << std::endl;
            return false;
        }

        std::pair<std::map<std::string, Quest*>::iterator,bool> result;
        result = this->questMap_.insert( std::pair<std::string,Quest*>(quest->getId(),quest) ); //!< Inserting the Quest.

        if(result.second) //!< If inserting was a success.
        {
            quest->setRegistered();
            COUT(4) << "Quest with questId {" << quest->getId() << "} successfully inserted." << std::endl;
            return true;
        }
        else
        {
           COUT(2) << "Quest with the same id was already present." << std::endl;
           return false;
        }
    }

    /**
    @brief
        Unregisters a Quest in the QuestManager.
    */
    bool QuestManager::unregisterQuest(Quest* quest)
    {
        return this->questMap_.erase(quest->getId()) == 1;
    }

    /**
    @brief
        Registers a QuestHint with the QuestManager to make it globally accessable.
        Uses it's id to make sure to be able to be identify and retrieve it later.
    @param hint
        The QuestHint to be registered.
    @return
        Returns true if successful, false if not.
    */
    bool QuestManager::registerHint(QuestHint* hint)
    {
        if(hint == NULL) //!< Still not liking NULL-pointers.
        {
            COUT(2) << "Registration of QuestHint in QuestManager failed, because inserted QuestHint-pointer was NULL." << std::endl;
            return false;
        }

        std::pair<std::map<std::string, QuestHint*>::iterator,bool> result;
        result = this->hintMap_.insert ( std::pair<std::string,QuestHint*>(hint->getId(),hint) ); //!< Inserting the QuestHSint.

        if(result.second) //!< If inserting was a success.
        {
            hint->setRegistered();
            COUT(4) << "QuestHint with hintId {" << hint->getId() << "} successfully inserted." << std::endl;
            return true;
        }
        else
        {
           COUT(2) << "QuestHint with the same id was already present." << std::endl;
           return false;
        }
    }

    /**
    @brief
        Unregisters a QuestHint in the QuestManager.
    */
    bool QuestManager::unregisterHint(QuestHint* hint)
    {
        return this->hintMap_.erase(hint->getId()) == 1;
    }

    /**
    @brief
        Finds a Quest with the given id.
    @param questId
        The id of the Quest sought for.
    @return
        Returns a pointer to the Quest with the input id.
        Returns NULL if there is no Quest with the given questId.
    @throws
        Throws an exception if the given questId is invalid.
    */
    Quest* QuestManager::findQuest(const std::string & questId)
    {
        if(questId.compare(BLANKSTRING) == 1) //!< Check vor validity of the given id.
        {
            ThrowException(Argument, "Invalid questId.");
        }

        Quest* quest;
        std::map<std::string, Quest*>::iterator it = this->questMap_.find(questId);
        if (it != this->questMap_.end()) //!< If the Quest is registered.
        {
            quest = it->second;
        }
        else
        {
           quest = NULL;
           COUT(2) << "The quest with id {" << questId << "} is nowhere to be found." << std::endl;
        }

        return quest;

    }

    /**
    @brief
        Finds a QuestHint with the given id.
    @param hintId
        The id of the QuestHint sought for.
    @return
        Returns a pointer to the QuestHint with the input id.
        Returns NULL if there is no QuestHint with the given hintId.
    @throws
        Throws an exception if the given hintId is invalid.
    */
    QuestHint* QuestManager::findHint(const std::string & hintId)
    {
        if(hintId.compare(BLANKSTRING) == 1) //!< Check vor validity of the given id.
        {
            ThrowException(Argument, "Invalid hintId.");
        }

        QuestHint* hint;
        std::map<std::string, QuestHint*>::iterator it = this->hintMap_.find(hintId);
        if (it != this->hintMap_.end()) //!< If the QuestHint is registered.
        {
            hint = it->second;
        }
        else
        {
           hint = NULL;
           COUT(2) << "The hint with id {" << hintId << "} is nowhere to be found." << std::endl;
        }

        return hint;

    }

    int QuestManager::getNumParentQuests(PlayerInfo* player)
    {
        int numQuests = 0;
        for(std::map<std::string, Quest*>::iterator it = this->questMap_.begin(); it != this->questMap_.end(); it++)
        {
            if(it->second->getParentQuest() == NULL && !it->second->isInactive(player))
                numQuests++;
        }
        return numQuests;
    }

    Quest* QuestManager::getParentQuest(PlayerInfo* player, int index)
    {
        for(std::map<std::string, Quest*>::iterator it = this->questMap_.begin(); it != this->questMap_.end(); it++)
        {
            if(it->second->getParentQuest() == NULL && !it->second->isInactive(player) && index-- == 0)
                return it->second;
        }
        return NULL;
    }

    int QuestManager::getNumSubQuests(Quest* quest, PlayerInfo* player)
    {
        std::list<Quest*> quests = quest->getSubQuestList();
        int numQuests = 0;
        for(std::list<Quest*>::iterator it = quests.begin(); it != quests.end(); it++)
        {
            if(!(*it)->isInactive(player))
                numQuests++;
        }
        return numQuests;
    }

    Quest* QuestManager::getSubQuest(Quest* quest, PlayerInfo* player, int index)
    {
        std::list<Quest*> quests = quest->getSubQuestList();
        for(std::list<Quest*>::iterator it = quests.begin(); it != quests.end(); it++)
        {
            if(!(*it)->isInactive(player) && index-- == 0)
                return *it;
        }
        return NULL;
    }

    int QuestManager::getNumHints(Quest* quest, PlayerInfo* player)
    {
        std::list<QuestHint*> hints = quest->getHintsList();
        int numHints = 0;
        for(std::list<QuestHint*>::iterator it = hints.begin(); it != hints.end(); it++)
        {
            if((*it)->isActive(player))
                numHints++;
        }
        return numHints;
    }

    QuestHint* QuestManager::getHints(Quest* quest, PlayerInfo* player, int index)
    {
        std::list<QuestHint*> hints = quest->getHintsList();
        for(std::list<QuestHint*>::iterator it = hints.begin(); it != hints.end(); it++)
        {
            if((*it)->isActive(player) && index-- == 0)
                return *it;
        }
        return NULL;
    }

    QuestDescription* QuestManager::getDescription(Quest* item)
    {
        return item->getDescription();
    }

    QuestDescription* QuestManager::getDescription(QuestHint* item)
    {
        return item->getDescription();
    }

    /**
    @brief
        Retrieve the player for a certain GUI.
    @param guiName
        The name of the GUI the player is retrieved for.
    @return
        Returns the player.
    @todo
        This very well might be outdated. So: Check if still needed, and update if necessary.
    */
    PlayerInfo* QuestManager::retrievePlayer(const std::string & guiName)
    {
        PlayerInfo* player = GUIManager::getInstance().getPlayer(guiName);
        if(player == NULL)
        {
            COUT(1) << "Error: GUIOverlay with name '" << guiName << "' has no player." << std::endl;
            return NULL;
        }

        return player;
    }

}
