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
    @file QuestListener.cc
    @brief Implementation of the QuestListener class.
*/

#include "OrxonoxStableHeaders.h"
#include "QuestListener.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Quest.h"
#include "QuestManager.h"

namespace orxonox {

    CreateFactory(QuestListener);

    /**
    @brief
        Constructor. Registers the object and initializes variables.
    */
    QuestListener::QuestListener(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(QuestListener);
        
        this->mode_ = questListenerMode::all;
        this->quest_ = NULL;
    }
    
    /**
    @brief
        Destructor.
    */
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
        XMLPortParam(QuestListener, "mode", setMode, getMode, xmlelement, mode);

        this->quest_->addListener(this); //!< Adds the QuestListener to the Quests list of listeners.
        
        COUT(3) << "QuestListener created for quest: {" << this->quest_->getId() << "} with mode '" << this->getMode() << "'." << std::endl;
    }
    
    /**
    @brief
        Makes all QuestListener in the list aware that a certain status change has occured and executes them if the status change affects them.
    @param listeners
        The list of QuestListeners that have to be made aware of the status change.
    @param status
        The status that has changed. Can be 'start' (if the Quest was started), 'complete' (if the Quest was completed) or 'fail' (if the Quest was failed).
    */
    /* static */ void QuestListener::advertiseStatusChange(std::list<QuestListener*> & listeners, const std::string & status)
    {
        for (std::list<QuestListener*>::iterator it = listeners.begin(); it != listeners.end(); ++it) //!< Iterate through all QuestListeners
        {
            QuestListener* listener = *it;
            if(listener->getMode() == status || listener->getMode() == "all") //!< Check whether the status change affects the give QuestListener.
            {
                listener->execute();
            }
        }
    }
    
    /**
    @brief
        Sets the questId of the Quest the QuestListener reacts to.
    @param id
        The questId of the Quest the QUestListener reacts to.
    @return
        Returns true if successful.
    */
    bool QuestListener::setQuestId(const std::string & id)
    {
        this->quest_ = QuestManager::findQuest(id); //!< Find the Quest corresponding to the given questId.
        
        if(this->quest_ == NULL) //!< If there is no such Quest.
        {
            COUT(1) << "This is bad! The QuestListener has not found a Quest qith a corresponding id." << std::endl; //TDO Throw a damn Exception!
            return false;
        }
        
        return true;
    }
    
    /**
    @brief
        Sets the mode of the QuestListener.
    @param mode
        The mode to be set. Can be eighter 'all', 'start', 'fail' or 'complete'.
    @return
        Returns true if successful.
    */
    bool QuestListener::setMode(const std::string & mode)
    {
        if(mode == "all")
        {
            this->mode_ = questListenerMode::all;
        }
        else if(mode == "start")
        {
            this->mode_ = questListenerMode::start;
        }
        else if(mode == "fail")
        {
            this->mode_ = questListenerMode::fail;
        }
        else if(mode == "complete")
        {
            this->mode_ = questListenerMode::complete;
        }
        else
        {
            COUT(2) << "QuestListener with invalid mode '" << mode << "' created. Mode set to 'all'." << std::endl;
	    this->mode_ = questListenerMode::all;
	    return false;
        }
        
        return true;
    }
    
    /**
    @brief
        Get the mode of the QuestListener.
    @return
        Return the mode of the QuestListener. Can be eighter 'all', 'start', 'fail' or 'complete'.
    */
    const std::string QuestListener::getMode(void)
    {
        if(this->mode_ == questListenerMode::all)
        {
            return "all";
        }
        else if(this->mode_ == questListenerMode::start)
        {
            return "start";
        }
        else if(this->mode_ == questListenerMode::fail)
        {
            return "fail";
        }
        else if(this->mode_ == questListenerMode::complete)
        {
            return "complete";
        }
        else
        {
            COUT(1) << "An unforseen, never to happen, Error has occured. This is impossible!" << std::endl;
	    return "";
        }
    }

    /**
    @brief
        Executes the QuestListener, resp. fires an Event.
    @return
        Returns true if successful.
    */
    bool QuestListener::execute()
    {
        this->fireEvent(true); //TDO This' right?
        return true;
    }

}
