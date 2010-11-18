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
    @file Notification.h
    @brief Definition of the Notification class.
    @ingroup Notifications
*/

#ifndef _Notification_H__
#define _Notification_H__

#include "notifications/NotificationsPrereqs.h"

#include <string>
#include "core/OrxonoxClass.h"

namespace orxonox
{

    /**
    @brief
        A Notification represents a short message used to inform the player about something that just happened. With the @ref orxonox::NotificationManager "NotificationManager" a Notification can be sent from any part of orxonox and is then displayed by the proper @ref orxonox::NotificationQueue "NotificationQueue(s)" (depending on which senders the specific @ref orxonox::NotificationQueue "NotificationQueues" accepts).

        A Notification is just a datastructure that is used internally by the Notifications module.

    @author
        Damian 'Mozork' Frick

    @ingroup Notifications
    */
    class _NotificationsExport Notification : public OrxonoxClass
    {
        public:
            Notification(const std::string& message, const std::string& sender);
            virtual ~Notification();

            /**
            @brief Get the message of the Notification.
            @return Returns the message of the Notification.
            */
            inline const std::string & getMessage(void) const
                { return this->message_; }

            /**
            @brief Get the sender of the Notification.
            @return Returns the sender of the Notification.
            */
            inline const std::string & getSender(void) const
                { return this->sender_; }

        private:
            std::string message_; //!< The Notification message.
            std::string sender_; //!< The sender of the notification.

            void initialize(void); //!< Registers the object and sets some default values.
            void registerVariables(void) {}

    };

}

#endif /* _Notification_H__ */
