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

#include "util/Singleton.h"
#include "core/OrxonoxClass.h"

namespace orxonox // tolua_export
{ // tolua_export

    /**
    @brief
        The Singleton NotificationManager functions as a gateway between @ref orxonox::Notification "Notifications" and @ref orxonox::NotificationListener "NotificationListeners".
        It receives, organizes @ref orxonox::Notification "Notifications" and the redistributes them to the specific @ref orxonox::NotificationListener "NotificationListeners".
        It also provides a static function to send @ref orxonox::Notification "Notifications" and works as a liaison between the @ref orxonox::NotificationQueue "NotificationQueues" and the GUI that displays notification, called NotificationLayer.

    @author
        Damian 'Mozork' Frick

    @ingroup Notifications
    */
    class _NotificationsExport NotificationManager // tolua_export
        : public Singleton<NotificationManager>, public OrxonoxClass
    { // tolua_export
            friend class Singleton<NotificationManager>;
        public:
            NotificationManager();
            virtual ~NotificationManager();

            virtual void preDestroy(void); //!< Is called before the object is destroyed.

            /**
            @brief Get the instance of the NotificationManager Singleton.
            @return Returns a reference to the NotificationManager.
            */
            static NotificationManager& getInstance() { return Singleton<NotificationManager>::getInstance(); } // tolua_export

            static const std::string ALL; //!< Static string to indicate a sender that sends to all NotificationListeners.
            static const std::string NONE; //!< Static string to indicare a sender that sends to no specific NotificationListener.

            //! Sends a Notification with the specified message to the specified client from the specified sender.
            static void sendNotification(const std::string& message, unsigned int clientId, const std::string& sender = NotificationManager::NONE, bool isLocal = false);

            bool registerNotification(Notification* notification); //!< Registers a Notification within the NotificationManager.
            void unregisterNotification(Notification* notification, NotificationListener* listener); //!< Unregisters a Notification within the NotificationManager for a given NotificationListener.
            bool registerListener(NotificationListener* listener); //!< Registers a NotificationListener within the NotificationManager.
            void unregisterListener(NotificationListener* listener); //!< Unregisters a NotificationListener withing the NotificationManager.

            void getNotifications(NotificationListener* listener, std::multimap<std::time_t, Notification*>* map, const std::time_t & timeFrameStart, const std::time_t & timeFrameEnd); //!< Fetches the Notifications for a specific NotificationListener in a specified timeframe and stores them in the input map.

            /**
            @brief Fetches the Notifications for a specific NotificationListener in a timeframe from now-timeDelay to now and stores them in the input map.
            @param listener The NotificationListener the Notifications are fetched for.
            @param map A pointer to a multimap, in which the notifications are stored. The map needs to have been allocated.
            @param timeDelay The timespan.
            @return Returns true if successful.
            */
            void getNotifications(NotificationListener* listener, std::multimap<std::time_t, Notification*>* map, int timeDelay)
                { this->getNotifications(listener, map, std::time(0)-timeDelay, std::time(0)); }

            void enterEditMode(void); //!< Enters the edit mode of the NotificationLayer.

            bool registerQueue(NotificationQueue* queue); //!< Registers a NotificationQueue.
            void unregisterQueue(NotificationQueue* queue); //!< Unregisters a NotificationQueue.

            // tolua_begin
            void loadQueues(void); //!< Loads all the NotificationQueues that should exist.
            void createQueue(const std::string& name); //!< Creates a new NotificationQueue.
            orxonox::NotificationQueue* getQueue(const std::string & name); //!< Get the NotificationQueue with the input name.
            // tolua_end

        private:
            static NotificationManager* singletonPtr_s;

            unsigned int highestIndex_; //!< This variable holds the highest index (resp. key) in notificationLists_s, to secure that no key appears twice.

            std::multimap<std::time_t, Notification*> allNotificationsList_; //!< Container where all Notifications are stored.
            std::map<NotificationListener*, unsigned int> listenerList_; //!< Container where all NotificationListeners are stored with a number as identifier.
            std::map<int,std::multimap<std::time_t, Notification*>*> notificationLists_; //!< Container where all Notifications, for each identifier (associated with a NotificationListener), are stored.

            std::map<const std::string, NotificationQueue*> queues_; //!< The list of NotificationQueues created by the NotificationManager.

            bool removeNotification(Notification* notification, std::multimap<std::time_t, Notification*>& map); //!< Helper method that removes an input Notification form an input map.

    }; // tolua_export

} // tolua_export

#endif /* _NotificationManager_H__ */
