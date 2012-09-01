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
#include "util/SubString.h"

#include "interfaces/NotificationListener.h"

namespace orxonox
{
    
    /*static*/ const std::string NotificationListener::ALL("all");
    /*static*/ const std::string NotificationListener::NONE("none");
    
    // Commands
    /*static*/ const std::string NotificationListener::COMMAND_CLEAR("clear");
    /*static*/ const std::string NotificationListener::COMMAND_NONE("none");
    
    registerStaticNetworkFunction(NotificationListener::sendHelper);
    
    NotificationListener::NotificationListener()
    {
        RegisterRootObject(NotificationListener);
    }

    /**
    @brief
        Helper method to send both notifications and commands over the network.
    @param message
        The message/command that should be sent.
    @param sender
        The sender that sent the notification/command.
    @param sendMode
        The mode in which the notification/command is sent, can be 'local' to send the notification to the client where this function is executed, 'network' if the notification is to be sent to the client with the specified clientID, or 'broadcast' if the notification should be sent to all hosts.
    @param clientId
        The id of the client the notification/command should be sent to.
    @param isCommand
        Whether the message is a notification or a command.
    @param messageType
        The type of the notification, can be either 'info' or 'important'.
    */
    /*static*/ void NotificationListener::sendNetworkHelper(const std::string& message, const std::string& sender, notificationSendMode::Value sendMode, unsigned int clientId, bool isCommand, notificationMessageType::Value messageType)
    {
        // If we're in standalone mode or we're already no the right client we create and send the notification/command.
        if(GameMode::isStandalone() || sendMode == notificationSendMode::local || (sendMode ==  notificationSendMode::network && Host::getPlayerID() == clientId))
        {
            sendHelper(message, sender, isCommand, messageType);
        }
        // If we're on the server (and the server is not the intended recipient of the notification/command) we send it over the network.
        else if(GameMode::isServer() && sendMode == notificationSendMode::network && Host::getPlayerID() != clientId)
        {
            callStaticNetworkFunction(NotificationListener::sendHelper, clientId, message, sender, isCommand, (unsigned int)messageType);
        }
        else if(GameMode::isServer() && sendMode == notificationSendMode::broadcast)
        {
            // TODO: Works as intended?
            callStaticNetworkFunction(NotificationListener::sendHelper, NETWORK_PEER_ID_BROADCAST, message, sender, isCommand, (unsigned int)messageType);
        }
    }

    /**
    @brief
        Helper method to register a notification/execute a command with all NotificationListeners after it has been sent over the network.
    @param message
        The notification/command to be sent/executed.
    @param sender
        The sender that sent the notification/command.
    @param isCommand
        Whether the message is a command or a notification.
    @param messageType
        The type of the notification.
    */
    /*static*/ void NotificationListener::sendHelper(const std::string& message, const std::string& sender, bool isCommand, unsigned int messageType)
    {
        // Iterate through all NotificationListeners and notify them by calling the method they overloaded.
        for(ObjectList<NotificationListener>::iterator it = ObjectList<NotificationListener>::begin(); it != ObjectList<NotificationListener>::end(); ++it)
        {
            // If the notification is a message.
            if(!isCommand)
                it->registerNotification(message, sender, notificationMessageType::Value(messageType));

            // If the notification is a command.
            if(isCommand)
            {
                notificationCommand::Value command = str2Command(message);
                if(command != notificationCommand::none)
                    it->executeCommand(command, sender);
            }
        }
    }

    /**
    @brief
        Helper method. Converts a string into the enum for a command.
    @param string
        The string to be converted.
    @return
        Returns the corresponding enum, notificationCommand::none if the command doesn't exist.
    */
    /*static*/ notificationCommand::Value NotificationListener::str2Command(const std::string& string)
    {
        notificationCommand::Value command = notificationCommand::none;

        if(string == NotificationListener::COMMAND_CLEAR)
            command = notificationCommand::clear;

        return command;
    }

    /**
    @brief
        Helper method. Converts a command enum into its corresponding string.
    @param command
        The command to be converted.
    @return
        Returns the corresponding string.
    */
    /*static*/ const std::string& NotificationListener::command2Str(notificationCommand::Value command)
    {
        switch(command)
        {
            case notificationCommand::clear:
                return NotificationListener::COMMAND_CLEAR;
            default:
                return NotificationListener::COMMAND_NONE;
        }
    }
    
}