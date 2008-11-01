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

#include "QuestManager.h"
#include "Quest.h"
#include "QuestHint.h"

namespace orxonox {

    std::map<std::string, Quest*> QuestManager::questMap_;
    std::map<std::string, QuestHint*> QuestManager::hintMap_;

    QuestManager::QuestManager(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(QuestManager);
    }


    QuestManager::~QuestManager()
    {

    }

    /**
    @brief
        Registers a quest with the QuestManager to make it globally accessable.
    @param quest
        The quest that is to be registered.
    @return
        Returns true if successful, false if not.
    */
    bool QuestManager::registerQuest(Quest* quest)
    {
        if(quest == NULL)
        {
            COUT(2) << "Registration of Quest in QuestManager failed, because inserted Quest-pointer was NULL." << std::endl;
            return false;
        }

        std::pair<std::map<std::string, Quest*>::iterator,bool> ret;
        ret = questMap_.insert( std::pair<std::string,Quest*>(quest->getId(),quest) );

        if(ret.second)
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
    @param hint
        The QuestHint to be registered.
    @return
        Returns true if successful, false if not.
    */
    bool QuestManager::registerHint(QuestHint* hint)
    {
        if(hint == NULL)
        {
            COUT(2) << "Registration of QuestHint in QuestManager failed, because inserted QuestHint-pointer was NULL." << std::endl;
            return false;
        }

        std::pair<std::map<std::string, QuestHint*>::iterator,bool> ret;
        ret = hintMap_.insert ( std::pair<std::string,QuestHint*>(hint->getId(),hint) );

        if(ret.second)
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
        Finds a quest with the given id.
    @param questId
        The id of the quest sought for.
    @return
        Returns a reference to the quest with the input id.
        Returns NULL if there is no quest with the given questId.
    @throws
        Throws an exception if the given questId is invalid.
    */
    Quest* QuestManager::findQuest(const std::string & questId)
    {
        if(!QuestItem::isId(questId))
        {
            ThrowException(Argument, "Invalid questId.");
        }

        Quest* quest;
        std::map<std::string, Quest*>::iterator it = questMap_.find(questId);
        if (it != questMap_.end())
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
        Finds a hint with the given id.
    @param hintId
        The id of the hint sought for.
    @return
        Returns a reference to the hint with the input id.
        Returns NULL if there is no hint with the given hintId.
    @throws
        Throws an exception if the given hintId is invalid.
    */
    QuestHint* QuestManager::findHint(const std::string & hintId)
    {
        if(!QuestItem::isId(hintId))
        {
            ThrowException(Argument, "Invalid hintId.");
        }

        QuestHint* hint;
        std::map<std::string, QuestHint*>::iterator it = hintMap_.find(hintId);
        if (it != hintMap_.end())
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
