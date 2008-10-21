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

#ifndef _QuestManager_H__
#define _QuestManager_H__

#include <map>
#include <string>

#include "core/OrxonoxClass.h"
#include "orxonox/objects/Quest.h"
#include "orxonox/objects/QuestHint.h"

namespace orxonox {


    //TDO: Needs to somehow be static.
    
    /**
    @brief
        Manages quest, by making them globally accessable.
    @author
        Damian 'Mozork' Frick
    */
    class QuestManager : virtual public OrxonoxClass
    {
    
	public:
	    QuestManager();
	    ~QuestManager();
	    
	    static void registerQuest(Quest & quest); //!< Registers a quest in the QuestManager.
	    static void registerHint(QuestHint & quest); //!< Registers a QuestHint in the QuestManager.
	    
	    static Quest & findQuest(const std::string & questId) const; //!< Returns the quest with the input id.
	    static QuestHint & findHint(const std::string & hintId) const; //!< Returns the QuestHint with the input id.
	    	    
	private:
	    std::map<std::string, Quest> questMap_;
	    std::map<std::string, QuestHint> hintMap_;
    
    };

}

#endif /* _QuestManager_H__ */
