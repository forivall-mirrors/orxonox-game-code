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
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See thes
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
    @brief Implementation of the NotificationDispatcher class.
*/

#include "NotificationDispatcher.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/EventIncludes.h"
#include "Notification.h"
#include "NotificationManager.h"
#include "interfaces/PlayerTrigger.h"
#include "infos/PlayerInfo.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{

    CreateUnloadableFactory(NotificationDispatcher);

    /**
    @brief
        Default constructor. Initializes the object.
    */
    NotificationDispatcher::NotificationDispatcher(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(NotificationDispatcher);

        this->sender_ = NotificationManager::NONE;
    }

    /**
    @brief
        Destructor.
    */
    NotificationDispatcher::~NotificationDispatcher()
    {

    }

    /**
    @brief
        Method for creating a NotificationDispatcher object through XML.
    */
    void NotificationDispatcher::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(NotificationDispatcher, XMLPort, xmlElement, mode);

        XMLPortEventSink(NotificationDispatcher, BaseObject, "trigger", trigger, xmlElement, mode); //TODO: Change BaseObject to MultiTrigger as soon as MultiTrigger is the base of all triggers.
    }

    void NotificationDispatcher::XMLEventPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(NotificationDispatcher, XMLEventPort, xmlElement, mode);

        XMLPortEventState(NotificationDispatcher, BaseObject, "trigger", trigger, xmlElement, mode);
    }

    /**
    @brief
        Dispatches a Notification with a message supplied by the createNotificationMessage() method, which can be overloaded.
    @param clientId
        The id of the client the notification should be dispatched to.
    */
    void NotificationDispatcher::dispatch(unsigned int clientId)
    {
        const std::string message = this->createNotificationMessage();
        Notification* notification = new Notification(this, message);

        notification->send(clientId, this->getSender());
    }

    /**
    @brief
        Is called when the NotificationDispatcher is triggered.
    @param triggered
        Whether it has been triggered or untriggered. The NotificationDispatcher only reacts to the first kind of events.
    @return
        Returns true if the NotificationDispatcher was successfully triggered.
    */
    bool NotificationDispatcher::trigger(bool triggered, BaseObject* trigger)
    {
        if(!triggered || !this->isActive()) // If the NotificationDispatcher is inactive it cannot be executed.
            return false;

        COUT(4) << "NotificationDispatcher (&" << this << ") triggered." << std::endl;

        PlayerTrigger* pTrigger = orxonox_cast<PlayerTrigger*>(trigger);
        Pawn* pawn = NULL;

        // If the trigger is a PlayerTrigger.
        if(pTrigger != NULL)
        {
            if(!pTrigger->isForPlayer())  //!< The PlayerTrigger is not exclusively for Pawns which means we cannot extract one.
                return false;
            else
                pawn = pTrigger->getTriggeringPlayer();
        }
        else
            return false;

        if(pawn == NULL)
        {
            COUT(4) << "The QuestEffectBeacon was triggered by an entity other than a Pawn. (" << trigger->getIdentifier()->getName() << ")" << std::endl;
            return false;
        }

        //! Extract the PlayerInfo from the Pawn.
        PlayerInfo* player = pawn->getPlayer();

        if(player == NULL)
        {
            COUT(3) << "The PlayerInfo* is NULL." << std::endl;
            return false;
        }

        this->dispatch(player->getClientID());

        return true;
    }

}
