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
    @file NotificationManager.h
    @brief Definition of the NotificationManager class.
    @ingroup Notifications
*/

#ifndef _NotificationManager_H__
#define _NotificationManager_H__

#include "notifications/NotificationsPrereqs.h"

#include <ctime>
#include <map>
#include <string>

#include "core/class/OrxonoxClass.h"
#include "util/Singleton.h"
#include "interfaces/NotificationListener.h"

namespace orxonox // tolua_export
{ // tolua_export

    /**
    @brief
        A Notification represents a short message used to inform the player about something that just happened. With the @ref orxonox::NotificationManager "NotificationManager" a Notification can be sent from any part of orxonox and is then displayed by the proper @ref orxonox::NotificationQueue "NotificationQueue(s)" (depending on which senders the specific @ref orxonox::NotificationQueue "NotificationQueues" accepts).

        A Notification is just a data structure that is used internally by the Notifications module.

    @author
        Damian 'Mozork' Frick

    @ingroup Notifications
    */
    class _NotificationsExport Notification
    {
        public:
            Notification(const std::string& message, const std::string& sender, notificationMessageType::Value type);
            virtual ~Notification();

            /**
            @brief Destroys the Notification.
            */
            void destroy(void)
                { delete this; }

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

            /**
            @brief Get the type of the Notification.
            @return Returns an enum with the type of the Notification.
            */
            inline notificationMessageType::Value getType(void) const
                { return this->type_; }

        private:
            std::string message_; //!< The Notification message.
            std::string sender_; //!< The sender of the notification.
            notificationMessageType::Value type_; //!< The type of the notification.

            void initialize(void); //!< Registers the object and sets some default values.

    };

    /**
    @brief
        The Singleton NotificationManager is a NotificationListener and functions as a gateway between @ref orxonox::Notification "Notifications" and @ref orxonox::NotificationQueue "NotificationQueues".
        It receives, organizes @ref orxonox::Notification "Notifications" and the redistributes them to the specific @ref orxonox::NotificationQueue "NotificationQueues".
        It also works as a liaison between the @ref orxonox::NotificationQueue "NotificationQueues" and the GUI that displays notification, called NotificationLayer.

    @author
        Damian 'Mozork' Frick

    @ingroup Notifications
    */
    class _NotificationsExport NotificationManager // tolua_export
        : public Singleton<NotificationManager>, public NotificationListener
    { // tolua_export
            friend class Singleton<NotificationManager>;
        public:
            NotificationManager();
            virtual ~NotificationManager();

            virtual void preDestroy(void); // Is called before the object is destroyed.

            /**
            @brief Get the instance of the NotificationManager Singleton.
            @return Returns a reference to the NotificationManager.
            */
            static NotificationManager& getInstance() { return Singleton<NotificationManager>::getInstance(); } // tolua_export

            virtual bool registerNotification(const std::string& message, const std::string& sender, notificationMessageType::Value type);
            virtual bool executeCommand(notificationCommand::Value command, const std::string& sender);

            bool registerNotification(Notification* notification); // Registers a Notification within the NotificationManager.
            void unregisterNotification(Notification* notification, NotificationQueue* queue); // Unregisters a Notification within the NotificationManager for a given NotificationQueue.

            void getNotifications(NotificationQueue* queue, std::multimap<std::time_t, Notification*>* map, const std::time_t & timeFrameStart, const std::time_t & timeFrameEnd); // Fetches the Notifications for a specific NotificationQueue in a specified timeframe and stores them in the input map.

            /**
            @brief Fetches the Notifications for a specific NotificationQueue in a timeframe from (now-timeDelay) to now and stores them in the input map.
            @param queue The NotificationQueue the Notifications are fetched for.
            @param map A pointer to a multimap, in which the notifications are stored. The map needs to have been allocated.
            @param timeDelay The timespan.
            @return Returns true if successful.
            */
            void getNotifications(NotificationQueue* queue, std::multimap<std::time_t, Notification*>* map, int timeDelay)
                { this->getNotifications(queue, map, std::time(0)-timeDelay, std::time(0)); }

            void getNewestNotifications(NotificationQueue* queue, std::multimap<std::time_t, Notification*>* map, int numberOfNotifications); // Fetches the newest Notifications for a specific NotificationQueue and stores them in the input map.

            bool registerQueue(NotificationQueue* queue); // Registers a NotificationQueue.
            void unregisterQueue(NotificationQueue* queue); // Unregisters a NotificationQueue.

            void loadQueues(void); // tolua_export // Loads all the NotificationQueues that should exist.

            NotificationQueue* getQueue(const std::string & name); // Get the NotificationQueue with the input name.

        private:
            static NotificationManager* singletonPtr_s;

            std::multimap<std::time_t, Notification*> allNotificationsList_; //!< Container where all Notifications are stored.
            std::map<const std::string, std::multimap<std::time_t, Notification*>*> notificationLists_; //!< Container where all Notifications, for each identifier (associated with a NotificationQueue), are stored.

            std::map<const std::string, NotificationQueue*> queues_; //!< The list of NotificationQueues created by the NotificationManager.

            bool removeNotification(Notification* notification, std::multimap<std::time_t, Notification*>& map); // Helper method that removes an input Notification form an input map.
            
            // Commands
            bool commandClear(const std::string& sender); // The clear command. Clears all NotificationQueues that have its sender as a target.

    }; // tolua_export

} // tolua_export

#endif /* _NotificationManager_H__ */
