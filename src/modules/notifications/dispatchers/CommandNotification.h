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
    @file CommandNotification.h
    @brief Definition of the CommandNotification class.
    @ingroup NotificationDispatchers
*/

#ifndef _CommandNotification_H__
#define _CommandNotification_H__

#include "notifications/NotificationsPrereqs.h"

#include <string>
#include "notifications/NotificationDispatcher.h"

namespace orxonox {

    /**
    @brief
        This class implements a method of displaying a Notification with information to an input command and the key the command is mapped to.
        The message that is displayed is a string made out of the concatenation of the preMessage, the key the specified command is mapped to and the postMessage.

        In use it would like this:
        @code
        <CommandNotification preMessage="Please press " command="someCommand" postMessage=" to do something." sender="me">
            <events>
                <trigger>
                    <PlayerTrigger />
                </trigger>
            </events>
        </CommandNotification>
        @endcode
        Upon being triggered this would display the @ref orxonox::Notification "Notification" "Please press {the binding of the specified command} to do something".
        For more information on what can be used for @code <PlayerTrigger /> @endcode see the @ref orxonox::NotificationDispatcher "NotificationDispatcher" documentation.

    @author
        Damian 'Mozork' Frick

    @ingroup NotificationDispatchers
    */
    class _NotificationsExport CommandNotification : public NotificationDispatcher
    {

        public:
            CommandNotification(Context* context); //!< Default Constructor.
            virtual ~CommandNotification(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a CommandNotification object through XML.

            /**
            @brief Get the command, whose key is displayed.
            @return Returns the name of the command.
            */
            const std::string& getCommand(void)
                { return this->command_; }
            /**
            @brief Get the preMessage, the first part of the displayed message.
            @return Returns the preMessage.
            */
            const std::string& getPreMessage(void)
                { return this->preMessage_; }
            /**
            @brief Get the postMessage, the last part of the displayed message.
            @return Returns the postMessage.
            */
            const std::string& getPostMessage(void)
                { return this->postMessage_; }

        protected:
            virtual const std::string& createNotificationMessage(void); //!< Composes the Notification of the preMessage the name of the key the command is mapped to and the postMessage.

        private:
            std::string command_; //!< The name of the command.
            std::string preMessage_; //!< The first part of the displayed message.
            std::string postMessage_; //!< The last part of the displayed message.

            void registerVariables(void); //!< Register some variables for synchronisation.

            /**
            @brief Set the command, whose key is displayed.
            @param command The name of the command.
            */
            void setCommand(const std::string& command)
                { this->command_ = command; }
            /**
            @brief Set the preMessage, the first part of the displayed message.
            @param message The preMessage.
            */
            void setPreMessage(const std::string& message)
                { this->preMessage_ = message; }
            /**
            @brief Set the postMessage, the last part of the displayed message.
            @param message The postMessage.
            */
            void setPostMessage(const std::string& message)
                 { this->postMessage_ = message; }

            const std::string& bindingNiceifyer(const std::string& binding); //!< Transforms the input binding into a human readable form.

    };

}

#endif // _CommandNotification_H__
