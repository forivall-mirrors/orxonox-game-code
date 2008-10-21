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
#include "QuestManager.h"

namespace orxonox {

    QuestManager::QuestManager() : OrxonoxClass()
    {
        RegisterRootObject(QuestManager);
    }
    
    
    QuestManager::~QuestManager()
    {
        
    }

    /**
    @brief
        Registers a quest with the QuestManager to make it globally accessable.
    @param
        The quest that is to be registered.
    */
    static void QuestManager::registerQuest(Quest & quest)
    {
        this->questMap_.insert ( pair<std::string,Quest>(quest.getId(),quest) );
    }
    
    /**
    @brief
        Registers a QuestHint with the QuestManager to make it globally accessable.
    @param
        The QuestHint to be registered.
    */
    static void QuestManager::registerHint(QuestHint & hint)
    {
        this->hintMap_.insert ( pair<std::string,Hint>(hint.getId(),hint) );
    }
    
    /**
    @brief
        Finds a quest with the given id.
    @param questId
        The id of the quest sought for.
    @return
        Returns a reference to the quest with the input id.
    */
    static Quest & QuestManager::findQuest(const std::string & questId) const
    {
        Quest* quest;
        std::map<std::string, Quest>::iterator it = this->questMap_.find(questId);
	if (it != this->questMap_.end())
	{
	    quest = &(it->second);
	}
	else
	{
	   //TDO: Exception???
	   quest = NULL;
	   COUT(2) << "The quest with id {" << questId << "} is nowhere to be found." << std::endl;
	}
	
	return *quest;

    }
    
    /**
    @brief
        Finds a hint with the given id.
    @param hintId
        The id of the hint sought for.
    @return
        Returns a reference to the hint with the input id.
    */
    static QuestHint & QuestManager::findHint(const std::string & hintId) const
    {
        QuestHint* hint;
        std::map<std::string, QuestHint>::iterator it = this->hintMap_.find(hintId);
	if (it != this->hintMap_.end())
	{
	    hint = &(it->second);
	}
	else
	{
	   //TDO: Exception???
	   hint = NULL;
	   COUT(2) << "The hint with id {" << hintId << "} is nowhere to be found." << std::endl;
	}
	
	return hint;

    }
    

}
