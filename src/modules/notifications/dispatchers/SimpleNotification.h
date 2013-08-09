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
    @file SimpleNotification.h
    @brief Declaration of the SimpleNotification class.
    @ingroup NotificationDispatchers
*/

#ifndef _SimpleNotification_H__
#define _SimpleNotification_H__

#include "notifications/NotificationsPrereqs.h"

#include "notifications/NotificationDispatcher.h"
#include <string>

namespace orxonox {

    /**
    @brief
        The SimpleNotification class enables the sending of (in XML) predefined Notifications upon some kind of triggering event.

        In use it would like this:
        @code
        <SimpleNotification message="some message..." sender="me">
            <events>
                <trigger>
                    <PlayerTrigger />
                </trigger>
            </events>
        </SimpleNotification>
        @endcode
        For more information on what can be used for @code <PlayerTrigger /> @endcode see the @ref orxonox::NotificationDispatcher "NotificationDispatcher" documentation.

    @author
        Damian 'Mozork' Frick

    @ingroup NotificationDispatchers
    */
    class _NotificationsExport SimpleNotification : public NotificationDispatcher
    {
        public:
            SimpleNotification(Context* context); //!< Default Constructor.
            virtual ~SimpleNotification(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a SimpleNotification object through XML.

            /**
            @brief Get the message, that is displayed.
            @return Returns the message, that is displayed.
            */
            const std::string& getMessage(void)
                { return this->message_; }

        protected:
            /**
            @brief Creates the notification message that should be sent upon the SimpleNotification triggering.
            @return Returns the notification message.
            */
            virtual const std::string& createNotificationMessage(void)
                { return this->message_; }

        private:
            std::string message_; //!< The message that is displayed.

            /**
            @brief Sets the message that is to be displayed.
            @param message The message to be displayed.
            */
            void setMessage(const std::string& message)
                { this->message_ = message; }
    };

}

#endif // _SimpleNotification_H__
