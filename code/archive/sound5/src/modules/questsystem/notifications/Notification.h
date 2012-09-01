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
    @file
    @brief Definition of the Notification class.
*/

#ifndef _Notification_H__
#define _Notification_H__

#include "questsystem/QuestsystemPrereqs.h"

#include <string>
#include "core/BaseObject.h"

namespace orxonox
{

    /**
    @brief
        A Notification is a short message used to inform the player about something that just happened. A Notification can be sent from any part of orxonox and is then displayed in the proper NotificationQueue (depending on which senders the specific NotificationQueue accepts).
    @author
        Damian 'Mozork' Frick
    */
    class _QuestsystemExport Notification : public BaseObject
    {
        public:
            Notification(BaseObject* creator);
            Notification(const std::string & message);
            virtual ~Notification();

            bool send(void); //!< Sends the Notification to the Notificationmanager, with sender NotificationManager::NONE;
            bool send(const std::string & sender); //!< Sends the Notification to the Notificationmanager.

            /**
            @brief Checks whether the Notification was sent.
            @return Returns true if the Notification was sent already.
            */
            inline bool isSent(void) const
                { return this->sent_; }
            /**
            @brief Returns the message of the Notification.
            @return Returns the message of the Notification.
            */
            inline const std::string & getMessage(void) const
                { return this->message_; }

            inline const std::string & getSender(void) const
                { return this->sender_; }

            bool setMessage(const std::string & message); //!< Sets the message of the notification.

        private:
            std::string message_; //!< The Notification message.
            std::string sender_; //!< The sender of the notification.
            bool sent_; //!< Whether Notification has been sent, if so it cannot be changed.

            void initialize(void);

    };

}

#endif /* _Notification_H__ */
