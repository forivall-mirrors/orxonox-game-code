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
    @file QuestManager.cc
    @brief
    Implementation of the QuestManager class.
*/

#include "OrxonoxStableHeaders.h"
#include "QuestManager.h"

#include "core/CoreIncludes.h"

#include "util/Exception.h"
#include "Quest.h"
#include "QuestHint.h"

namespace orxonox {

    //! All Quests registered by their id's.
    std::map<std::string, Quest*> QuestManager::questMap_s;
    //! All QuestHints registered by their id's.
    std::map<std::string, QuestHint*> QuestManager::hintMap_s;

    /**
    @brief
        Constructor. Registers the object.
    */
    QuestManager::QuestManager(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(QuestManager);
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
        result = questMap_s.insert( std::pair<std::string,Quest*>(quest->getId(),quest) ); //!< Inserting the Quest.

        if(result.second) //!< If inserting was a success.
        {
            COUT(3) << "Quest with questId {" << quest->getId() << "} successfully inserted." << std::endl;
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
        result = hintMap_s.insert ( std::pair<std::string,QuestHint*>(hint->getId(),hint) ); //!< Inserting the QuestHSint.

        if(result.second) //!< If inserting was a success.
        {
            COUT(3) << "QuestHint with hintId {" << hint->getId() << "} successfully inserted." << std::endl;
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
        if(!QuestItem::isId(questId)) //!< Check vor validity of the given id.
        {
            ThrowException(Argument, "Invalid questId.");
        }

        Quest* quest;
        std::map<std::string, Quest*>::iterator it = questMap_s.find(questId);
        if (it != questMap_s.end()) //!< If the Quest is registered.
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
        if(!QuestItem::isId(hintId)) //!< Check vor validity of the given id.
        {
            ThrowException(Argument, "Invalid hintId.");
        }

        QuestHint* hint;
        std::map<std::string, QuestHint*>::iterator it = hintMap_s.find(hintId);
        if (it != hintMap_s.end()) //!< If the QuestHint is registered.
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


}
