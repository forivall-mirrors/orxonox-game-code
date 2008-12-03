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

#include "OrxonoxStableHeaders.h"
#include "QuestListener.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Quest.h"
#include "QuestManager.h"

namespace orxonox {

    CreateFactory(QuestListener);

    QuestListener::QuestListener(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(QuestListener);
    }
    
    QuestListener::~QuestListener()
    {
    }
    
    /**
    @brief
        Method for creating a Quest object through XML.
    */
    void QuestListener::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(QuestListener, XMLPort, xmlelement, mode);

        XMLPortParam(QuestListener, "questId", setQuestId, getQuestId, xmlelement, mode);
        XMLPortParam(QuestListener, "questStatus", setQuestStatus, getQuestStatus, xmlelement, mode);

        this->quest_->addListener(this);
        
        COUT(3) << "QuestListener created for quest: {" << this->quest_->getId() << "} and status '" << this->status_ << "'." << std::endl;
    }
    
    void QuestListener::advertiseStatusChange(std::list<QuestListener*> & listeners, const std::string & status)
    {
        for (std::list<QuestListener*>::iterator it = listeners.begin(); it != listeners.end(); ++it)
        {
            QuestListener* listener = *it;
            if(listener->getQuestStatus() == status)
            {
                listener->execute();
            }
        }
    }
    
    bool QuestListener::execute()
    {
        this->fireEvent(true); //TDO This' right?
        
        COUT(3) << "QuestListener fired Event, expect for something to happen." << std::endl; //TDO remove
        
        return true;
    }
    
    bool QuestListener::setQuestId(const std::string & id)
    {
        this->quest_ = QuestManager::findQuest(id);
        if(this->quest_ == NULL)
        {
            COUT(1) << "This is bad!" << std::endl; //TDO Find a better way.
            return false;
        }
        
        return true;
    }
    
    bool QuestListener::setQuestStatus(const std::string & status)
    {
        if(status == "start")
        {
            this->status_ = questListenerStatus::start;
            return true;
        }
        else if(status == "fail")
        {
            this->status_ = questListenerStatus::fail;
            return true;
        }
        else if(status == "complete")
        {
            this->status_ = questListenerStatus::complete;
            return true;
        }
        else
        {
            COUT(2) << "QuestListener with invalid status '" << status << "' created. Status set to 'start'." << std::endl;
	    this->status_ = questListenerStatus::start;
	    return false;
        }
    }
    
    const std::string QuestListener::getQuestStatus(void)
    {
        if(this->status_ == questListenerStatus::start)
        {
            return "start";
        }
        else if(this->status_ == questListenerStatus::fail)
        {
            return "fail";
        }
        else if(this->status_ == questListenerStatus::complete)
        {
            return "complete";
        }
        else
        {
            COUT(1) << "An unforseen, never to happen, Error has occured. This is impossible!" << std::endl;
	    return "";
        }
    }


}
