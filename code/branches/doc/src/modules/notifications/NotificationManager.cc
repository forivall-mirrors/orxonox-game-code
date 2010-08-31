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
    @brief Implementation of the NotificationManager class.
*/

#include "NotificationManager.h"

#include <set>

#include "util/ScopedSingletonManager.h"
#include "core/CoreIncludes.h"
#include "Notification.h"
#include "interfaces/NotificationListener.h"

namespace orxonox
{

    const std::string NotificationManager::ALL("all");
    const std::string NotificationManager::NONE("none");

    ManageScopedSingleton(NotificationManager, ScopeID::Root, false);

    /**
    @brief
        Constructor. Registers the Object.
    */
    NotificationManager::NotificationManager()
    {
        RegisterRootObject(NotificationManager);

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
        Registers a Notification within the NotificationManager and makes sure that the Notification is sent to all the NotificationListeners associated with its sender.
    @param notification
        The Notification to be registered.
    @return
        Returns true if successful.
    */
    bool NotificationManager::registerNotification(Notification* notification)
    {

        if(notification == NULL) //!< A NULL-Notification cannot be registered.
            return false;

        std::time_t time = std::time(0); //TODO: Doesn't this expire? //!< Get current time.

        this->allNotificationsList_.insert(std::pair<std::time_t,Notification*>(time,notification));

        if(notification->getSender() == NONE) //!< If the sender has no specific name, then the Notification is only added to the list of all Notifications.
            return true;

        bool all = false;
        if(notification->getSender() == ALL) //!< If all are the sender, then the Notifications is added to every NotificationListener.
            all = true;

        //!< Insert the notification in all listeners that have its sender as target.
        for(std::map<NotificationListener*,int>::iterator it = this->listenerList_.begin(); it != this->listenerList_.end(); it++) //!< Iterate through all listeners.
        {
            std::set<std::string> set = it->first->getTargetsSet();
            if(all || set.find(notification->getSender()) != set.end() || set.find(ALL) != set.end()) //TODO: Make sure this works.
            {
                this->notificationLists_[it->second]->insert(std::pair<std::time_t,Notification*>(time,notification)); //!< Insert the Notification in the Notifications list of the current NotificationListener.
                it->first->update(notification, time); //!< Update the listener.
                std::map<Notification*, unsigned int>::iterator counterIt = this->listenerCounter_.find(notification);
                if(counterIt == this->listenerCounter_.end())
                    this->listenerCounter_[notification] = 1;
                else
                    this->listenerCounter_[notification] = counterIt->second + 1;
            }
        }

        COUT(4) << "Notification registered with the NotificationManager." << std::endl;

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

        // If the Notification is no longer present in any of the NotificationListeners it can be removed from the map of all Notifications and be destroyed.
        if(this->listenerCounter_[notification] == (unsigned int) 0)
        {
            this->removeNotification(notification, this->allNotificationsList_);
            this->listenerCounter_.erase(notification);
            notification->destroy();
        }

        COUT(4) << "Notification unregistered with the NotificationManager." << std::endl;
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

        this->listenerList_[listener] = index; //!< Add the NotificationListener to the list of listeners.

        std::set<std::string> set = listener->getTargetsSet(); //TODO: Does this work?

        //! If all senders are the target of the listener, then the list of notification for that specific listener is te same as the list of all Notifications.
        if(set.find(ALL) != set.end())
        {
            this->notificationLists_[index] = &this->allNotificationsList_;
            COUT(4) << "NotificationListener registered with the NotificationManager." << std::endl;
            return true;
        }

        this->notificationLists_[index] = new std::multimap<std::time_t,Notification*>;
        std::multimap<std::time_t,Notification*> map = *this->notificationLists_[index];

        //! Iterate through all Notifications to determine whether any of them should belong to the newly registered NotificationListener.
        for(std::multimap<std::time_t,Notification*>::iterator it = this->allNotificationsList_.begin(); it != this->allNotificationsList_.end(); it++)
        {
            if(set.find(it->second->getSender()) != set.end()) //!< Checks whether the overlay has the sender of the current notification as target.
            {
                map.insert(std::pair<std::time_t, Notification*>(it->first, it->second));
                std::map<Notification*, unsigned int>::iterator counterIt = this->listenerCounter_.find(it->second);
                if(counterIt == this->listenerCounter_.end())
                    this->listenerCounter_[it->second] = 1;
                else
                    this->listenerCounter_[it->second] = counterIt->second + 1;
            }
        }

        listener->update(); //!< Update the listener.

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

        // Make sure all Notifications are removed.
        std::multimap<std::time_t, Notification*>::iterator it = map->begin();
        while(it != map->end())
        {
            this->unregisterNotification(it->second, listener);
            it = map->begin();
        }

        this->listenerList_.erase(listener);
        this->notificationLists_.erase(identifier);

        // If the map is not the map of all notifications, delete it.
        if(map != &this->allNotificationsList_)
            delete map;

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

        std::multimap<std::time_t,Notification*>* notifications = this->notificationLists_[this->listenerList_[listener]]; //!< The Notifications for the input NotificationListener.

        if(notifications == NULL) //!< Returns NULL, if there are no Notifications.
            return true;

        std::multimap<std::time_t,Notification*>::iterator it, itLowest, itHighest;
        itLowest = notifications->lower_bound(timeFrameStart);
        itHighest = notifications->upper_bound(timeFrameStart);

        for(it = itLowest; it != itHighest; it++) //!< Iterate through the Notifications from the start of the time Frame to the end of it.
        {
            map->insert(std::pair<std::time_t,Notification*>(it->first,it->second)); //!< Add the found Notifications to the map.
        }

        return true;
    }

}