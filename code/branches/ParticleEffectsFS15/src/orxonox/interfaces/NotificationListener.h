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
    @file NotificationListener.h
    @brief Definition of the NotificationListener class.
    @ingroup Notifications
*/

#ifndef _NotificationListener_H__
#define _NotificationListener_H__

#include "OrxonoxPrereqs.h"

#include <ctime>
#include <set>
#include <string>

#include "util/StringUtils.h"

#include "core/class/OrxonoxInterface.h"

namespace orxonox
{
    // TODO: Document.
    namespace notificationMessageType
    {
        enum Value {
            info,
            important
        };
    }
    
    namespace notificationSendMode
    {
        enum Value {
            local,
            network,
            broadcast
        };
    }
    
    namespace notificationCommand
    {
        enum Value {
            none,
            clear
        };
    }

    // TODO: Update doc.
    /**
    @brief
        NotificationListener interface.

        The NotificationListener interface (or more precisely abstract class) presents a means of being informed when a new @ref orxonox::Notification "Notification" is sent.
        The NotificationListener can be used to send a new notification message (with NotificationListener::sendNotification() ) or a new notification command (with NotificationListener::sendCommand() ). Each NotificationListener is then informed about the new @ref orxonox::Notification "Notification" and can take appropriate action. Currently the only NotificationListener ist the @ref orxonox::NotificationManager "NotificationManager" singleton.

        When inheriting from a NotificationListener it is important to provide an appropriate implementation of registerNotification() and executeCommand().

    @author
        Damian 'Mozork' Frick
        
    @ingroup Notifications
    @todo Consistent terminology between message, notification and command.
    */
    class _OrxonoxExport NotificationListener : virtual public OrxonoxInterface
    {
        public:
            NotificationListener();
            virtual ~NotificationListener() {}

            /**
            @brief Sends a Notification with the specified message to the specified client from the specified sender.
            @param message The message that should be sent.
            @param sender The sender that sent the notification. Default is 'none'.
            @param messageType The type of the message, can be either 'info' or 'important'. Default is 'info'.
            @param sendMode The mode in which the notification is sent, can be 'local' to send the notification to the client where this function is executed, 'network' if the notification is to be sent to the client with the specified clientID, or 'broadcast' if the notification should be sent to all hosts. Default is notificationSendMode::local.
            @param clientId The id of the client the notification should be sent to. Default is 0.
            */
            static void sendNotification(const std::string& message, const std::string& sender = NotificationListener::NONE, notificationMessageType::Value messageType = notificationMessageType::info, notificationSendMode::Value sendMode = notificationSendMode::local, unsigned int clientId = 0)
                { NotificationListener::sendNetworkHelper(message, sender, sendMode, clientId, false, messageType); }
            /**
            @brief Sends a specified command to the specified client from the specified sender.
            @param command The command that should be sent (and later executed).
            @param sender The sender that sent the notification. Default is 'none'.
            @param sendMode The mode in which the command is sent, can be 'local' to send the command to the client where this function is executed, 'network' if the command is to be sent to the client with the specified clientID, or 'broadcast' if the command should be sent to all hosts. Default is notificationSendMode::local.
            @param clientId The id of the client the command should be sent to. Default is 0.
            */
            static void sendCommand(const std::string& command, const std::string& sender = NotificationListener::NONE, notificationSendMode::Value sendMode = notificationSendMode::local, unsigned int clientId = 0)
                { NotificationListener::sendNetworkHelper(command, sender, sendMode, clientId, true); }

            static void sendHelper(const std::string& message, const std::string& sender, bool isCommand = false, unsigned int messageMode = 0); // Helper method to register a notification/execute a command with all NotificationListeners after it has been sent over the network.

            //TODO: Make protected?
            
            /**
            @brief Registers a notification with the NotificationListener.
                   This needs to be overloaded by each class inheriting from NotificationListener.
            @param message The notification's message.
            @param sender The sender of the notification.
            @param type The type of the notification.
            @return Returns true if the notification was successfully registered, false if not.
            */
            virtual bool registerNotification(const std::string& message, const std::string& sender, notificationMessageType::Value type)
                { return false; }
            /**
            @brief Executes a command with the NotificationListener
                   This needs to be overloaded by each class inheriting from NotificationListener.
            @param command The command to be executed.
            @param sender The sender of the command.
            @return Returns true if the command was successfully executed, false if not.
            */
            virtual bool executeCommand(notificationCommand::Value command, const std::string& sender) { return false; }

        public:
            
            static const std::string ALL; //!< Static string to indicate a sender that sends to all NotificationQueues.
            static const std::string NONE; //!< Static string to indicate a sender that sends to no specific NotificationQueues.
            
            //! Commands
            static const std::string COMMAND_CLEAR;
            static const std::string COMMAND_NONE;
            
        protected:
            static void sendNetworkHelper(const std::string& message, const std::string& sender, notificationSendMode::Value sendMode, unsigned int clientId, bool isCommand = false, notificationMessageType::Value messageType = notificationMessageType::info); // Helper method to send both notifications and commands over the network.

            static notificationCommand::Value str2Command(const std::string& string); // Helper method. Converts a string into the enum for a command.
            static const std::string& command2Str(notificationCommand::Value command); // Helper method. Converts a command enum into its corresponding string.
    };
}

#endif /* _NotificationListener_H__ */
