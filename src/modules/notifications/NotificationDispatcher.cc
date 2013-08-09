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
    @file NotificationDispatcher.cc
    @brief Implementation of the NotificationDispatcher class.
*/

#include "NotificationDispatcher.h"

#include "core/CoreIncludes.h"
#include "core/EventIncludes.h"
#include "core/XMLPort.h"
#include "network/NetworkFunction.h"
#include "network/Host.h"

#include "infos/PlayerInfo.h"
#include "interfaces/NotificationListener.h"
#include "interfaces/PlayerTrigger.h"
#include "worldentities/pawns/Pawn.h"

namespace orxonox
{

    CreateUnloadableFactory(NotificationDispatcher);

    registerMemberNetworkFunction(NotificationDispatcher, broadcastHelper);
    registerMemberNetworkFunction(NotificationDispatcher, dispatch);

    /**
    @brief
        Default constructor. Initializes the object.
    */
    NotificationDispatcher::NotificationDispatcher(Context* context) : BaseObject(context), Synchronisable(context)
    {
        RegisterObject(NotificationDispatcher);

        this->sender_ = NotificationListener::NONE;
        this->bBroadcast_ = false;
        this->registerVariables();
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
    void NotificationDispatcher::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(NotificationDispatcher, XMLPort, xmlelement, mode);

        XMLPortParam(NotificationDispatcher, "sender", setSender, getSender, xmlelement, mode);
        XMLPortParam(NotificationDispatcher, "broadcast", setBroadcasting, isBroadcasting, xmlelement, mode);

        XMLPortEventSink(NotificationDispatcher, BaseObject, "trigger", trigger, xmlelement, mode);
    }

    void NotificationDispatcher::XMLEventPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(NotificationDispatcher, XMLEventPort, xmlelement, mode);

        XMLPortEventState(NotificationDispatcher, BaseObject, "trigger", trigger, xmlelement, mode);
    }

    /**
    @brief
        Registers variables for synchronisation.
    */
    void NotificationDispatcher::registerVariables(void)
    {
        registerVariable(this->sender_, VariableDirection::ToClient);
    }

    /**
    @brief
        Broadcasts a specific Notification.
    */
    void NotificationDispatcher::broadcast(void)
    {
        // TODO: Needed?
        const std::string message = this->createNotificationMessage();
        NotificationListener::sendNotification(message, this->getSender(), notificationMessageType::info, notificationSendMode::local);

        // Broadcast
        if(!GameMode::isStandalone())
        {
            callMemberNetworkFunction(NotificationDispatcher, broadcastHelper, this->getObjectID(), NETWORK_PEER_ID_BROADCAST);
        }
    }

    /**
    @brief
        Helper function for broadcast.
    */
    void NotificationDispatcher::broadcastHelper(void)
    {
        this->dispatch(Host::getPlayerID());
    }

    /**
    @brief
        Dispatches a Notification with a message supplied by the createNotificationMessage() method, which can be overloaded.
    @param clientId
        The id of the client the notification should be dispatched to.
    */
    void NotificationDispatcher::dispatch(unsigned int clientId)
    {
        // We don't call sendNotification() directly on the server, because if might be necessary that createNotificationMessage() is executed on the client as the message may be client-specific.
        if(GameMode::isStandalone() || Host::getPlayerID() == clientId || this->getSyncMode() == ObjectDirection::None)
        {
            const std::string message = this->createNotificationMessage();
            // TODO: Make the type configurable.
            NotificationListener::sendNotification(message, this->getSender(), notificationMessageType::info, notificationSendMode::network, clientId);
        }
        else if(GameMode::isServer())
        {
            callMemberNetworkFunction(NotificationDispatcher, dispatch, this->getObjectID(), clientId, clientId);
        }
    }

    /**
    @brief
        Is called when the NotificationDispatcher is triggered.
    @param triggered
        Whether it has been triggered or untriggered. The NotificationDispatcher only reacts to the first kind of events.
    @param trigger
        The object that caused the event to be fired.
    @return
        Returns true if the NotificationDispatcher was successfully triggered.
    */
    bool NotificationDispatcher::trigger(bool triggered, BaseObject* trigger)
    {
        if(!triggered || !this->isActive()) // If the NotificationDispatcher is inactive it cannot be executed.
            return false;

        orxout(verbose, context::notifications) << "NotificationDispatcher (&" << this << ") triggered." << endl;

        // If the NotificationDispatcher is set to broadcast.
        if(this->isBroadcasting())
        {
            this->broadcast();
            return true;
        }

        PlayerTrigger* pTrigger = orxonox_cast<PlayerTrigger*>(trigger);
        PlayerInfo* player = NULL;

        // If the trigger is a PlayerTrigger.
        if(pTrigger != NULL)
        {
            if(!pTrigger->isForPlayer())  // The PlayerTrigger is not exclusively for Pawns which means we cannot extract one.
                return false;
            else
                player = pTrigger->getTriggeringPlayer();
        }
        else
            return false;

        if(player == NULL)
        {
            orxout(verbose, context::notifications) << "The NotificationDispatcher was triggered by an entity other than a Pawn. (" << trigger->getIdentifier()->getName() << ")" << endl;
            return false;
        }

        this->dispatch(player->getClientID());

        return true;
    }

}
