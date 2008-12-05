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

#ifndef _QuestListener_H__
#define _QuestListener_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include <list>

#include "core/BaseObject.h"
#include "Quest.h"

namespace questListenerStatus
{

    enum Enum
    {
        start,
        fail,
        complete
    };

}

namespace orxonox {

    /**
    @brief
        
    @author
	Damian 'Mozork' Frick
    */
    class _OrxonoxExport QuestListener : public BaseObject
    {
	public:
	    QuestListener(BaseObject* creator);
	    virtual ~QuestListener();
	    
	    virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a QuestListener object through XML.
	    
	    bool setQuestId(const std::string & id);
	    bool setQuestStatus(const std::string & status);
	    
	    const std::string getQuestStatus(void);
	    inline const std::string & getQuestId(void)
                { return this->quest_->getId(); }
                
	    static void advertiseStatusChange(std::list<QuestListener*> & listeners, const std::string & status);
	    
	    bool execute(void);
	    
	private:
            questListenerStatus::Enum status_;
            Quest* quest_;
    
    };

}

#endif /* _QuestListener_H__ */
