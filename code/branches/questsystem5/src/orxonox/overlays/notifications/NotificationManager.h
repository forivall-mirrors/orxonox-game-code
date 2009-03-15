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
*/

#ifndef _NotificationManager_H__
#define _NotificationManager_H__

#include "OrxonoxPrereqs.h"

#include "core/BaseObject.h"

#include <map>
#include <string>
#include <ctime>

#include "NotificationOverlay.h"

namespace orxonox
{

    /**
    @brief
        The Singleton NotificationManager functions as a gateway between Notifications and NotificationQueues.
        It receives, organizes Notifications and the redistributes them to the specific NotificationQueues.
    @author
        Damian 'Mozork' Frick
    */
    class _OrxonoxExport NotificationManager : public BaseObject
    {
        public:
            NotificationManager();
            virtual ~NotificationManager();
	        
            static const std::string ALL;
            static const std::string NONE;
         
            static NotificationManager & getInstance(); //! Returns a reference to the single instance of the NotificationManager.

            //TDO: Visibility?
            bool registerNotification(Notification* notification); //!< Registers a Notification within the NotificationManager.
            bool registerQueue(NotificationQueue* queue); //!< Registers a NotificationQueue within the NotificationManager.
            
            std::multimap<std::time_t,Notification*>* getNotifications(NotificationQueue* queue, const std::time_t & timeFrameStart, const std::time_t & timeFrameEnd); //!< Returns the Notifications for a specific NotificationQueue in a specified timeframe.
            
            /**
            @brief Fetches the Notifications for a specific NotificationQueue starting at a specified time.
            @param queue The NotificationQueue the Notifications are fetched for.
            @param timeFrameStart The start time the Notifications are fetched from.
            @return Returns a time-ordered list of Notifications.
            */
            std::multimap<std::time_t,Notification*>* getNotifications(NotificationQueue* queue, const std::time_t & timeFrameStart)
                { return this->getNotifications(queue, timeFrameStart, std::time(0)); }
            /**
            @brief Fetches the Notifications for a specific NotificationQueue starting at a specified timespan before now.
            @param queue The NotificationQueue the Notifications are fetched for.
            @param timeDelay The timespan.
            @return Returns a time-ordered list of Notifications.
            */
            std::multimap<std::time_t,Notification*>* getNotifications(NotificationQueue* queue, int timeDelay)
                { return this->getNotifications(queue, std::time(0)-timeDelay, std::time(0)); }
     
        private:
            static NotificationManager* singletonRef_s;

            int highestIndex_; //!< This variable holds the highest index (resp. key) in notificationLists_s, to secure that  no key appears twice.
        
            std::multimap<std::time_t,Notification*> allNotificationsList_; //!< Container where all notifications are stored (together with their respecive timestamps).
            std::map<NotificationQueue*,int> queueList_; //!< Container where all NotificationQueues are stored with a number as identifier.
            std::map<int,std::multimap<std::time_t,Notification*>*> notificationLists_; //!< Container where all Notifications, for each identifier (associated with a NotificationQueue), are stored.
            

    };

}

#endif /* _NotificationManager_H__ */
