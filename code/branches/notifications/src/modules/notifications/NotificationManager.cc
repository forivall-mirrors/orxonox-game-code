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

#include "NotificationManager.h"

#include "core/command/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "core/GUIManager.h"
#include "core/LuaState.h"
#include "util/ScopedSingletonManager.h"
#include "interfaces/NotificationListener.h"
#include "Notification.h"
#include "NotificationQueue.h"

#include "ToluaBindNotifications.h"

namespace orxonox
{

    const std::string NotificationManager::ALL("all");
    const std::string NotificationManager::NONE("none");

    // Register tolua_open function when loading the library.
    DeclareToluaInterface(Notifications);

    ManageScopedSingleton(NotificationManager, ScopeID::Graphics, false);

    //TODO: Make work.
    SetConsoleCommand("enterEditMode", &NotificationManager::enterEditMode);

    /**
    @brief
        Constructor. Registers the Object.
    */
    NotificationManager::NotificationManager()
    {
        RegisterRootObject(NotificationManager);

        this->highestIndex_ = 0;

        ModifyConsoleCommand("enterEditMode").setObject(this);
        
        COUT(3) << "NotificatioManager created." << std::endl;
    }

    /**
    @brief
        Destructor.
    */
    NotificationManager::~NotificationManager()
    {
        ModifyConsoleCommand("enterEditMode").setObject(NULL);

        for(std::multimap<std::time_t, Notification*>::iterator it = this->allNotificationsList_.begin(); it != this->allNotificationsList_.end(); it++)
            it->second->destroy();
        
        COUT(3) << "NotificationManager destroyed." << std::endl;
    }

    void NotificationManager::preDestroy(void)
    {
        for(std::map<const std::string, NotificationQueue*>::iterator it = this->queues_.begin(); it != this->queues_.end(); )
        {
            NotificationQueue* queue = (*it).second;
            it++;
            queue->destroy();
        }
        this->queues_.clear();
    }

    /**
    @brief
        Registers a Notification within the NotificationManager and makes sure that the Notification is sent to all the NotificationListeners associated with its sender.
    @param notification
        The Notification to be registered.
    @return
        Returns true if successful.
    */
    bool NotificationManager::registerNotification(Notification* notification)
    {
        if(notification == NULL) // A NULL-Notification cannot be registered.
            return false;

        std::time_t time = std::time(0); //TODO: Doesn't this expire? //!< Get current time.

        this->allNotificationsList_.insert(std::pair<std::time_t, Notification*>(time, notification));

        if(notification->getSender() == NotificationManager::NONE) // If the sender has no specific name, then the Notification is only added to the list of all Notifications.
            return true;

        bool all = false;
        if(notification->getSender() == NotificationManager::ALL) // If all are the sender, then the Notifications is added to every NotificationListener.
            all = true;

        // Insert the notification in all listeners that have its sender as target.
        for(std::map<NotificationListener*,int>::iterator it = this->listenerList_.begin(); it != this->listenerList_.end(); it++) // Iterate through all listeners.
        {
            std::set<std::string, NotificationListenerStringCompare> set = it->first->getTargetsSet();
            bool bAll = set.find(NotificationManager::ALL) != set.end();
            if(all || bAll || set.find(notification->getSender()) != set.end()) //TODO: Make sure this works.
            {
                if(!bAll)
                {
                    this->notificationLists_[it->second]->insert(std::pair<std::time_t, Notification*>(time, notification)); // Insert the Notification in the Notifications list of the current NotificationListener.
                }
                it->first->update(notification, time); // Update the listener.
                std::map<Notification*, unsigned int>::iterator counterIt = this->listenerCounter_.find(notification);
                if(counterIt == this->listenerCounter_.end())
                    this->listenerCounter_[notification] = 1;
                else
                    this->listenerCounter_[notification] = counterIt->second + 1;
            }
        }

        COUT(4) << "Notification (&" << notification << ") registered with the NotificationManager." << std::endl;

        return true;
    }

    /**
    @brief
        Unregisters a Notification within the NotificationManager.
    @param notification
        A pointer to the Notification to be unregistered.
    @param listener
        A pointer to the NotificationListener the Notification is unregistered for.
    */
    void NotificationManager::unregisterNotification(Notification* notification, NotificationListener* listener)
    {
        assert(notification);
        assert(listener);

        // If the Notification was removed from the list of Notifications of the input NotificationListener, the counter for the Notification of the number of NotificationListeners it is present in is decremented.
        if(this->removeNotification(notification, *(this->notificationLists_.find(this->listenerList_.find(listener)->second)->second)))
            this->listenerCounter_[notification] = this->listenerCounter_[notification] - 1;

        COUT(4) << "Notification (&" << notification << ") unregistered with the NotificationManager from listener (&" << listener << ")" << std::endl;
    }

    /**
    @brief
        Helper method that removes an input notification form an input map.
    @param notification
        A pointer to the notification to be removed.
    @param map
        The map the notification should be removed from.
    @return
        Returns true if successful.
    */
    bool NotificationManager::removeNotification(Notification* notification, std::multimap<std::time_t, Notification*>& map)
    {
        // Iterates through all items in the map until the Notification is found.
        //TODO: Do more efficiently?
        for(std::multimap<std::time_t, Notification*>::iterator it = map.begin(); it != map.end(); it++)
        {
            if(it->second == notification)
            {
                map.erase(it);
                return true;
            }
        }
        return false;
    }

    /**
    @brief
        Registers a NotificationListener within the NotificationManager.
    @param listener
        The NotificationListener to be registered.
    @return
        Returns true if successful.
    */
    bool NotificationManager::registerListener(NotificationListener* listener)
    {
        this->highestIndex_ += 1;
        int index = this->highestIndex_;

        this->listenerList_[listener] = index; // Add the NotificationListener to the list of listeners.

        std::set<std::string, NotificationListenerStringCompare> set = listener->getTargetsSet();

        // If all senders are the target of the listener, then the list of notification for that specific listener is te same as the list of all Notifications.
        bool bAll = set.find(NotificationManager::ALL) != set.end();
        std::multimap<std::time_t, Notification*>* map;
        if(bAll)
            this->notificationLists_[index] = &this->allNotificationsList_;
        else
        {
            this->notificationLists_[index] = new std::multimap<std::time_t, Notification*>;
            map = this->notificationLists_[index];
        }

        // Iterate through all Notifications to determine whether any of them should belong to the newly registered NotificationListener.
        for(std::multimap<std::time_t, Notification*>::iterator it = this->allNotificationsList_.begin(); it != this->allNotificationsList_.end(); it++)
        {
            if(bAll || set.find(it->second->getSender()) != set.end()) // Checks whether the listener has the sender of the current notification as target.
            {
                if(!bAll)
                    map->insert(std::pair<std::time_t, Notification*>(it->first, it->second));
                std::map<Notification*, unsigned int>::iterator counterIt = this->listenerCounter_.find(it->second);
                if(counterIt == this->listenerCounter_.end())
                    this->listenerCounter_[it->second] = 1;
                else
                    this->listenerCounter_[it->second] = counterIt->second + 1;
            }
        }

        listener->update(); // Update the listener.

        COUT(4) << "NotificationListener registered with the NotificationManager." << std::endl;

        return true;
    }

    /**
    @brief
        Unregisters a NotificationListener withing the NotificationManager.
    */
    void NotificationManager::unregisterListener(NotificationListener* listener)
    {
        assert(listener);

        int identifier = this->listenerList_.find(listener)->second;
        std::multimap<std::time_t, Notification*>* map = this->notificationLists_.find(identifier)->second;

        // If the map is not the map of all notifications, make sure all Notifications are removed and delete it.
        std::multimap<std::time_t, Notification*>::iterator it = map->begin();
        if(map != &this->allNotificationsList_)
        {
            while(it != map->end())
            {
                this->unregisterNotification(it->second, listener);
                it = map->begin();
            }
            delete map;
        }

        this->listenerList_.erase(listener);
        this->notificationLists_.erase(identifier);

        COUT(4) << "NotificationListener unregistered with the NotificationManager." << std::endl;
    }

    /**
    @brief
        Fetches the Notifications for a specific NotificationListener in a specified timeframe.
    @param listener
        The NotificationListener the Notifications are fetched for.
    @param map
        A multimap, in which the notifications are stored.
    @param timeFrameStart
        The start time of the timeframe.
    @param timeFrameEnd
        The end time of the timeframe.
    @return
        Returns true if successful.
    */
    bool NotificationManager::getNotifications(NotificationListener* listener, std::multimap<std::time_t,Notification*>* map, const std::time_t & timeFrameStart, const std::time_t & timeFrameEnd)
    {
        if(listener == NULL || map == NULL)
            return false;

        std::multimap<std::time_t, Notification*>* notifications = this->notificationLists_[this->listenerList_[listener]]; // The Notifications for the input NotificationListener.

        if(notifications == NULL) // Returns false, if there are no Notifications.
            return false;

        std::multimap<std::time_t,Notification*>::iterator it, itLowest, itHighest;
        itLowest = notifications->lower_bound(timeFrameStart);
        itHighest = notifications->upper_bound(timeFrameEnd);

        for(it = itLowest; it != itHighest; it++) // Iterate through the Notifications from the start of the time Frame to the end of it.
            map->insert(std::pair<std::time_t, Notification*>(it->first,it->second)); // Add the found Notifications to the map.

        return true;
    }

    void NotificationManager::loadQueues(void)
    {
        new NotificationQueue("all");
    }

    void NotificationManager::createQueue(const std::string& name)
    {
        new NotificationQueue(name);
    }

    NotificationQueue* NotificationManager::getQueue(const std::string & name)
    {
        std::map<const std::string, NotificationQueue*>::iterator it = this->queues_.find(name);
        if(it == this->queues_.end())
            return NULL;

        return (*it).second;
    }

    bool NotificationManager::registerQueue(NotificationQueue* queue)
    {
        return this->queues_.insert(std::pair<const std::string, NotificationQueue*>(queue->getName(), queue)).second;
    }
    
    void NotificationManager::unregisterQueue(NotificationQueue* queue)
    {
        this->queues_.erase(queue->getName());
    }

    void NotificationManager::enterEditMode(void)
    {
        GUIManager::getInstance().hideGUI("NotificationLayer");
        GUIManager::getInstance().showGUI("NotificationLayer", false, false);
        GUIManager::getInstance().getLuaState()->doString("NotificationLayer.enterEditMode()");
    }

}
