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
    @file NotificationManager.cc
    @brief Implementation of the NotificationManager class.
*/

#include "OrxonoxStableHeaders.h"
#include "NotificationManager.h"

#include "core/CoreIncludes.h"

#include <set>

#include "Notification.h"
#include "NotificationQueue.h"

namespace orxonox
{

    const std::string NotificationManager::ALL = "all";
    const std::string NotificationManager::NONE = "none";

    NotificationManager* NotificationManager::singletonRef_s = NULL;

    /**
    @brief
        Constructor. Registers the Object.
    */
    NotificationManager::NotificationManager() : BaseObject(this)
    {
        RegisterRootObject(NotificationManager);

        assert(singletonRef_s == 0);
        singletonRef_s = this;

        this->highestIndex_ = 0;
    }

    /**
    @brief
        Destructor.
    */
    NotificationManager::~NotificationManager()
    {
    }

    /**
    @brief
        Returns the current (and single) instance of the NotificationManager. Creates one, if there isn't one to begin with.
    @return
        Returns a reference to the single instance of the NotificationManager.
    */
    /*static*/ NotificationManager & NotificationManager::getInstance()
    {
        assert(singletonRef_s);
        return *singletonRef_s;
    }
    
    /**
    @brief
        Registers a Notification within the NotificationManager and makes sure that the Notification is displayed in all the NotificationQueues associated with its sender.
    @param notification
        The Notification to be registered.
    @return
        Returns true if successful.
    */
    bool NotificationManager::registerNotification(Notification* notification)
    {
    
        if(notification == NULL) //!< A NULL-Notification cannot be registered.
            return false;
        
        std::time_t time = std::time(0); //TDO: Doesn't this expire? //!< Get current time.
        
        this->allNotificationsList_.insert(std::pair<std::time_t,Notification*>(time,notification));
        
        if(notification->getSender() == NONE) //!< If the sender has no specific name, then the Notification is only added to the list of all Notifications.
            return true;
        
        bool all = false;
        if(notification->getSender() == ALL) //!< If all are the sender, then the Notifications is added to every NotificationQueue.
            all = true;
        
        //!< Insert the notification in all queues that have its sender as target.
        for(std::map<NotificationQueue*,int>::iterator it = this->queueList_.begin(); it != this->queueList_.end(); it++) //!< Iterate through all queues.
        {
            std::set<std::string> set = it->first->getTargetsSet();
            if(all || set.find(notification->getSender()) != set.end() || set.find(ALL) != set.end()) //TDO: Make sure this works.
            {
                this->notificationLists_[it->second]->insert(std::pair<std::time_t,Notification*>(time,notification)); //!< Insert the Notification in the Notifications list of the current NotificationQueue.
                it->first->update(notification, time); //!< Update the queue.
            }
        }
        
        COUT(3) << "Notification registered with the NotificationManager." << std::endl;
        
        return true;
    }
    
    /**
    @brief
        Registers a NotificationQueue within the NotificationManager.
    @param queue
        The NotificationQueue to be registered.
    @return
        Returns true if successful.
    */
    bool NotificationManager::registerQueue(NotificationQueue* queue)
    {
        this->highestIndex_ += 1;
        int index = this->highestIndex_;
        
        this->queueList_[queue] = index; //!< Add the NotificationQueue to the list of queues.
        
        std::set<std::string> set = queue->getTargetsSet(); //TDO: Works this?
        
        //! If all senders are the target of the queue, then the list of notification for that specific queue is te same as the list of all Notifications.
        if(set.find(ALL) != set.end())
        {
            this->notificationLists_[index] = &this->allNotificationsList_;
            COUT(3) << "NotificationQueue registered with the NotificationManager." << std::endl;
            return true;
        }
        
        this->notificationLists_[index] = new std::multimap<std::time_t,Notification*>;
        std::multimap<std::time_t,Notification*> map = *this->notificationLists_[index];
        
        //! Iterate through all Notifications to determine whether any of them should belong to the newly registered NotificationQueue.
        for(std::multimap<std::time_t,Notification*>::iterator it = this->allNotificationsList_.begin(); it != this->allNotificationsList_.end(); it++)
        {
            if(set.find(it->second->getSender()) != set.end()) //!< Checks whether the overlay has the sender of the current notification as target.
            {
                map.insert(std::pair<std::time_t,Notification*>(it->first, it->second));
            }
        }
        
        queue->update(); //!< Update the queue.

        COUT(3) << "NotificationQueue registered with the NotificationManager." << std::endl;
        
        return true;
    }
    
    /**
    @brief
        Fetches the Notifications for a specific NotificationQueue in a specified timeframe.
    @param queue
        The NotificationQueue the Notifications are fetched for.
    @param timeFrameStart
        The start time of the timeframe.
    @param timeFrameEnd
        The end time of the timeframe.
    @return
        Returns a time-ordered list of Notifications.
    @todo
        Make sure the map is deleted.
    */
    std::multimap<std::time_t,Notification*>* NotificationManager::getNotifications(NotificationQueue* queue, const std::time_t & timeFrameStart, const std::time_t & timeFrameEnd)
    {
        std::multimap<std::time_t,Notification*>* notifications = this->notificationLists_[this->queueList_[queue]];
        
        if(notifications == NULL)
            return NULL;
    
        std::multimap<std::time_t,Notification*>::iterator it, itLowest, itHighest;
        itLowest = notifications->lower_bound(timeFrameStart);
        itHighest = notifications->upper_bound(timeFrameStart);
        
        std::multimap<std::time_t,Notification*>* map = new std::multimap<std::time_t,Notification*>();
        
        for(it = itLowest; it != itHighest; it++)
        {
            map->insert(std::pair<std::time_t,Notification*>(it->first,it->second));
        }
        
        if(map->size() == 0)
        {
            delete map;
            return NULL;
        }
        
        return map;
    }

}
