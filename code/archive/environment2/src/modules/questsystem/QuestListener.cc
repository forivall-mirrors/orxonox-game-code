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

#include "QuestListener.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

#include "Quest.h"
#include "QuestManager.h"

namespace orxonox
{
    CreateFactory(QuestListener);

    // Initialization of the static variables for the modes as strings.
    /*static*/ const std::string QuestListener::ALL = "all";
    /*static*/ const std::string QuestListener::START = "start";
    /*static*/ const std::string QuestListener::FAIL = "fail";
    /*static*/ const std::string QuestListener::COMPLETE = "complete";

    /**
    @brief
        Constructor. Registers the object and initializes variables.
    */
    QuestListener::QuestListener(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(QuestListener);

        this->mode_ = QuestListenerMode::All;
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

        if(this->quest_ != NULL)
            this->quest_->addListener(this); // Adds the QuestListener to the Quests list of listeners.

        COUT(4) << "QuestListener created for quest: {" << this->quest_->getId() << "} with mode '" << this->getMode() << "'." << std::endl;
    }

    /**
    @brief
        Makes all QuestListener in the list aware that a certain status change has occurred and executes them if the status change affects them.
    @param listeners
        The list of QuestListeners that have to be made aware of the status change.
    @param status
        The status that has changed. Can be 'start' (if the Quest was started), 'complete' (if the Quest was completed) or 'fail' (if the Quest was failed).
    */
    /* static */ void QuestListener::advertiseStatusChange(std::list<QuestListener*> & listeners, const std::string & status)
    {
        for (std::list<QuestListener*>::iterator it = listeners.begin(); it != listeners.end(); ++it) // Iterate through all QuestListeners
        {
            QuestListener* listener = *it;
            if(listener->getMode() == status || listener->getMode() == QuestListener::ALL) // Check whether the status change affects the give QuestListener.
                listener->execute();
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
        this->quest_ = QuestManager::getInstance().findQuest(id); // Find the Quest corresponding to the given questId.

        if(this->quest_ == NULL) // If there is no such Quest.
        {
            ThrowException(Argument, "This is bad! The QuestListener has not found a Quest with a corresponding id..");
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
        if(mode == QuestListener::ALL)
            this->mode_ = QuestListenerMode::All;
        else if(mode == QuestListener::START)
            this->mode_ = QuestListenerMode::Start;
        else if(mode == QuestListener::FAIL)
            this->mode_ = QuestListenerMode::Fail;
        else if(mode == QuestListener::COMPLETE)
            this->mode_ = QuestListenerMode::Complete;
        else
        {
            COUT(2) << "QuestListener with invalid mode '" << mode << "' created. Mode set to 'all'." << std::endl;
            this->mode_ = QuestListenerMode::All;
            return false;
        }

        return true;
    }

    /**
    @brief
        Get the mode of the QuestListener.
    @return
        Return the mode of the QuestListener. Can be either 'all', 'start', 'fail' or 'complete'.
    */
    std::string QuestListener::getMode(void)
    {
        switch(this->mode_)
        {
            case QuestListenerMode::All:
                return QuestListener::ALL;
            case QuestListenerMode::Start:
                return QuestListener::START;
            case QuestListenerMode::Fail:
                return QuestListener::FAIL;
            case QuestListenerMode::Complete:
                return QuestListener::COMPLETE;
            default: // This will never happen.
                return QuestListener::ALL;
        }
    }

    /**
    @brief
        Get the questId of the Quest the QuestListener reacts to.
    @return
        Returns the questId of the Quest the QuestListener reacts to.
    */
    const std::string & QuestListener::getQuestId(void)
    {
        return this->quest_->getId();
    }

    /**
    @brief
        Executes the QuestListener, resp. fires an Event.
    @return
        Returns true if successful.
    */
    bool QuestListener::execute()
    {
        this->fireEvent(true);
        return true;
    }

}
