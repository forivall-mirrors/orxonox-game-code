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
    @file QuestListener.h
    @brief Definition of the QuestListener class.
*/

#ifndef _QuestListener_H__
#define _QuestListener_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include <list>

#include "core/BaseObject.h"

#include "Quest.h"

namespace questListenerMode
{

    //! The mode of the QuestListener.
    enum Enum
    {
        all,
        start,
        fail,
        complete
    };

}

namespace orxonox {

    /**
    @brief
        Provides a way to react to the starting, completing and failing of Quests.
        
        The XML representation goes as follows:
        You can use the QuestListener as if it were a Trigger or EventListener, that fires an Event when the status (depending on the set mode) of the given Quest changes.
        
        <BaseObject> // The object that should react to the status change of a Quest.
            <events>
                <function> // Where function is the method of the object that schould be executed. Normally this would be visibility or activity.
                    <QuestListener questId="someQuestId" mode="someMode" /> // Where someQuestId is the identifier for the Quest the QuestListener is reacting to, and someMode is the kind of status change the QUestListener reacts to (all, start, complete or fail).
                </function>
            </events>
        </BaseObject>
    @author
	Damian 'Mozork' Frick
    */
    class _OrxonoxExport QuestListener : public BaseObject
    {
	public:
	    QuestListener(BaseObject* creator);
	    virtual ~QuestListener();
	    
	    virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a QuestListener object through XML.
	    
	    static void advertiseStatusChange(std::list<QuestListener*> & listeners, const std::string & status); //!< Makes all QuestListener in the list aware that a certain status change has occured.
	    
	    bool setQuestId(const std::string & id); //!< Sets the questId of the Quest the QuestListener reacts to.
	    bool setMode(const std::string & mode); //!< Sets the mode of the QuestListener.
	    
	    const std::string getMode(void); //!< Get the mode of the QuestListener.
	    
	    /**
	    @brief Get the questId of the Quest the QuestListener reacts to.
	    @return Returns the questId of the Quest the QuestListener reacts to.
	    */
	    inline const std::string & getQuestId(void)
                { return this->quest_->getId(); }
	    
	    bool execute(void); //!< Executes the QuestListener, resp. fires an Event.
	    
	private:
            questListenerMode::Enum mode_; //!< The mode of the QuestListener.
            Quest* quest_; //!< A pointer to the Quest the QuestListener is reacting to.
    
    };

}

#endif /* _QuestListener_H__ */
