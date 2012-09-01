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
    @file NotificationDispatcher.h
    @brief Definition of the NotificationDispatcher class.
    @ingroup Notifications
*/

#ifndef _NotificationDispatcher_H__
#define _NotificationDispatcher_H__

#include "notifications/NotificationsPrereqs.h"

#include <string>

#include "core/BaseObject.h"
#include "network/synchronisable/Synchronisable.h"

namespace orxonox
{

    /**
    @brief
        A NotificationDispatcher is an entity that, upon being triggered, dispatches (or sends) a specified @ref orxonox::Notification "Notification".

        There are two parameter to be set:
        - The @b sender . The sender specifies the part of Orxonox the sent @ref orxonox::Notification "Notification" comes from. The default value is set by the classes implementing NotificationDispatcher.
        - The @b broadcast . Specifies whether messages are broadcast (i.e. sent to all clients) or just sent to a specific player.

        Its standard usage is:
        @code
        <NotificationDispatcher sender="me">
            <events>
                <trigger>
                    <PlayerTrigger />
                </trigger>
            </event>
        </NotificationDispatcher>
        @endcode
        But keep in mind, that NotificationDispatcher is an abstract class.
        Also in this example @ref orxonox::PlayerTrigger "PlayerTrigger" stands for any event that is caused by a @ref orxonox::PlayerTrigger "PlayerTrigger", so instead of @ref orxonox::PlayerTrigger "PlayerTrigger", there could be a @ref orxonox::DistanceTrigger "DistanceTrigger", or a @ref orxonox::DistanceMultiTrigger "DistanceMutliTrigger", or even an @ref orxonox::EventListener "EventListener" that waits for an event coming from any kind of @ref orxonox::PlayerTrigger "PlayerTrigger".
        If the NotificationDispatcher is not set to broadcast only events caused by @ref orxonox::PlayerTrigger "PlayerTriggers" trigger a message since the information obtained by the @ref orxonox::PlayerTrigger "PlayerTrigger" is used to identify the client to which the message should be sent.

    @author
        Damian 'Mozork' Frick

    @ingroup Notifications
    */
    class _NotificationsExport NotificationDispatcher : public BaseObject, public Synchronisable
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
            const std::string& getSender(void) const
                { return this->sender_; }
            /**
            @brief Set the sender of the Notification dispatched by this NotificationDispatcher.
            @param sender The name of the sender.
            */
            void setSender(const std::string& sender)
                { this->sender_ = sender; }

            /**
            @brief Check whether the NotificationDispatcher is set to broadcast.
            @return Returns true if the NotificationDispatcher is set to broadcast.
            */
            bool isBroadcasting(void) const
                { return this->bBroadcast_; }
            /**
            @brief Set the NotificationDispatcher to broadcast.
            @param broadcast Whether the NotificationDispatcher is set to broadcast or singlecast.
            */
            void setBroadcasting(bool v)
                { this->bBroadcast_ = v; }

            void broadcast(void); //!< Broadcasts a specific Notification.
            void broadcastHelper(void); //!< Helper function for broadcast.
            void dispatch(unsigned int clientId); //!< Dispatches a specific Notification to a given client.
            bool trigger(bool triggered, BaseObject* trigger); //!< Is called when the NotificationDispatcher is triggered.

        protected:
            std::string sender_; //!< The name of the sender of the Notification dispatched by this NotificationDispatcher.
            bool bBroadcast_; //!< Whether the NotificationDispatcher is broadcasting.

            void registerVariables(void); //!< Register some variables for synchronisation.

            /**
            @brief Creates the notification message that should be sent upon the NotificationDispatcher triggering.
                   This method can be overloaded to customize the NotificationDispatcher.
            @return Returns the notification message.
            */
            virtual const std::string& createNotificationMessage(void)
                { return BLANKSTRING; }

    };

}

#endif /* _NotificationDispatcher_H__ */
