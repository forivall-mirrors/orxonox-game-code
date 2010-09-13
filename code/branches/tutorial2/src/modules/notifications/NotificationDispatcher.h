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
    @brief Definition of the NotificationDispatcher class.
*/

#ifndef _NotificationDispatcher_H__
#define _NotificationDispatcher_H__

#include "notifications/NotificationsPrereqs.h"

#include <string>
#include "core/BaseObject.h"

namespace orxonox
{

    /**
    @brief
        A NotificationDispatcher is an entity that, upon being triggered, dispatches (or sends) a specified Notification.
    @author
        Damian 'Mozork' Frick
    */
    class _NotificationsExport NotificationDispatcher : public BaseObject
    {
        public:
            NotificationDispatcher(BaseObject* creator); //!< Default constructor. Initializes the object.
            virtual ~NotificationDispatcher(); //!< Destructor.

            virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode); //!< Method for creating a NotificationDispatcher object through XML.
            virtual void XMLEventPort(Element& xmlelement, XMLPort::Mode mode);

            /**
            @brief Get the sender of the Notification dispatched by this NotificationDispatcher.
            @return Returns the name of the sender.
            */
            const std::string& getSender(void)
                { return this->sender_; }

            void dispatch(unsigned int clientId); //!< Dispatches a specific Notification.
            bool trigger(bool triggered, BaseObject* trigger); //!< Is called when the NotificationDispatcher is triggered.

        protected:
            std::string sender_; //!< The name of the sender of the Notification dispatched by this NotificationDispatcher.

            /**
            @brief Set the sender of the Notification dispatched by this NotificationDispatcher.
            @param sender The name of the sender.
            */
            void setSender(const std::string& sender)
                { this->sender_ = sender; }

            /**
            @brief Creates the notification message that should be sent upon the NotificationDispatcher triggering.
                   This method can be overloaded to customize the NotificationDispatcher.
            @return Returns the notification message.
            */
            virtual const std::string& createNotificationMessage(void)
                { return *(new std::string("")); }

    };

}

#endif /* _NotificationDispatcher_H__ */
