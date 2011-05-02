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
    @file NotificationListener.cc
    @brief Implementation of the NotificationListener class.
*/

#include "core/CoreIncludes.h"
#include "network/Host.h"
#include "network/NetworkFunction.h"

#include "interfaces/NotificationListener.h"

namespace orxonox
{
    
    const std::string NotificationListener::ALL("all");
    const std::string NotificationListener::NONE("none");
    
    registerStaticNetworkFunction(NotificationListener::sendNotificationHelper);
    
    
    NotificationListener::NotificationListener()
    {
        RegisterRootObject(NotificationListener);
    }

    /**
    @brief
        Sends a Notification with the specified message to the specified client from the specified sender.
    @param message
        The message that should be sent.
    @param sender
        The sender that sent the notification.
    @param messageMode
        The mode of the message, can be either 'message' or 'command'.
    @param sendMode
        The mode in which the notification is sent, can be 'local' to send the notification to the client where this function is executed, 'network' if the notification is to be sent to the client with the specified clientID, or 'broadcast' if the notification should be sent to all hosts.
    @param clientId
        The id of the client the notification should be sent to.
    */
    /*static*/ void NotificationListener::sendNotification(const std::string& message, const std::string& sender, notificationMessageMode::Value messageMode, notificationSendMode::Value sendMode, unsigned int clientId)
    {
        // If we're in standalone mode or we're already no the right client we create and send the Notification.
        if(GameMode::isStandalone() || sendMode == notificationSendMode::local || (sendMode ==  notificationSendMode::network && Host::getPlayerID() == clientId))
        {
            sendNotificationHelper(message, sender, messageMode);
        }
        // If we're on the server (and the server is not the intended recipient of the Notification) we send it over the network.
        else if(GameMode::isServer() && sendMode == notificationSendMode::network && Host::getPlayerID() != clientId)
        {
            callStaticNetworkFunction(NotificationListener::sendNotificationHelper, clientId, message, sender, (unsigned int)messageMode);
        }
        else if(GameMode::isServer() && sendMode == notificationSendMode::broadcast)
        {
            // TODO: Works as intended?
            callStaticNetworkFunction(NotificationListener::sendNotificationHelper, NETWORK_PEER_ID_BROADCAST, message, sender, (unsigned int)messageMode);
        }
    }
    
    /*static*/ void NotificationListener::sendNotificationHelper(const std::string& message, const std::string& sender, unsigned int messageMode)
    {
        // Iterate through all NotificationListeners and notify them by calling the method they overloaded.
        for(ObjectList<NotificationListener>::iterator it = ObjectList<NotificationListener>::begin(); it != ObjectList<NotificationListener>::end(); ++it)
        {
            if(messageMode == 0 && it->registerNotification(message, sender))
                COUT(3) << "Notification \"" << message << "\" sent." << std::endl;
            
            if(messageMode == 1)
                it->executeCommand(message, sender);
        }
    }
    
}