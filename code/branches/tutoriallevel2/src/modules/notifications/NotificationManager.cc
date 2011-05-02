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
#include "core/GameMode.h"
#include "core/GUIManager.h"
#include "core/LuaState.h"
#include "util/Convert.h"
#include "util/ScopedSingletonManager.h"

#include "interfaces/NotificationListener.h"

#include "Notification.h"
#include "NotificationQueue.h"

#include "ToluaBindNotifications.h"

namespace orxonox
{

    // Register tolua_open function when loading the library.
    DeclareToluaInterface(Notifications);

    ManageScopedSingleton(NotificationManager, ScopeID::Root, false);

    // Setting console command to enter the edit mode.
    SetConsoleCommand("enterEditMode", &NotificationManager::enterEditMode);

    /**
    @brief
        Constructor. Registers the Object.
    */
    NotificationManager::NotificationManager()
    {
        RegisterRootObject(NotificationManager);

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

        // Destroys all Notifications.
        for(std::multimap<std::time_t, Notification*>::iterator it = this->allNotificationsList_.begin(); it!= this->allNotificationsList_.end(); it++)
            it->second->destroy();
        this->allNotificationsList_.clear();

        COUT(3) << "NotificationManager destroyed." << std::endl;
    }

    /**
    @brief
        Is called before the object is destroyed.
    */
    void NotificationManager::preDestroy(void)
    {
        // Destroys all NotificationQueues that have been registered with the NotificationManager.
        std::map<const std::string, NotificationQueue*>::iterator it = this->queues_.begin();
        while(it != this->queues_.end())
        {
            it->second->destroy(true);
            it = this->queues_.begin();
        }

        this->queues_.clear();
    }
    
    bool NotificationManager::registerNotification(const std::string& message, const std::string& sender)
    {
        Notification* notification = new Notification(message, sender);
        return this->registerNotification(notification);
    }

    /**
    @brief
        Registers a Notification within the NotificationManager and makes sure that the Notification is sent to all the NotificationQueues associated with its sender.
    @param notification
        The Notification to be registered.
    @return
        Returns true if successful.
    */
    bool NotificationManager::registerNotification(Notification* notification)
    {
        assert(notification);

        std::time_t time = std::time(0); // Get current time.

        // Add the Notification to the list that holds all Notifications.
        this->allNotificationsList_.insert(std::pair<std::time_t, Notification*>(time, notification));

        if(notification->getSender() == NotificationListener::NONE) // If the sender has no specific name, then the Notification is only added to the list of all Notifications.
            return true;

        bool all = false;
        if(notification->getSender() == NotificationListener::ALL) // If all are the sender, then the Notifications is added to every NotificationQueue.
            all = true;

        // Insert the Notification in all NotificationQueues that have its sender as target.
        for(std::map<const std::string, NotificationQueue*>::iterator it = this->queues_.begin(); it != this->queues_.end(); it++) // Iterate through all NotificationQueues.
        {
            const std::set<std::string>& set = it->second->getTargetsSet();
            bool bAll = set.find(NotificationListener::ALL) != set.end();
            // If either the Notification has as sender 'all', the NotificationQueue displays all Notifications or the NotificationQueue has the sender of the Notification as target.
            if(all || bAll || set.find(notification->getSender()) != set.end())
            {
                if(!bAll)
                    this->notificationLists_[it->second->getName()]->insert(std::pair<std::time_t, Notification*>(time, notification)); // Insert the Notification in the notifications list of the current NotificationQueue.
                it->second->update(notification, time); // Update the NotificationQueue.
            }
        }

        COUT(4) << "Notification (&" << notification << ") registered with the NotificationManager." << std::endl;

        return true;
    }

    /**
    @brief
        Unregisters a Notification within the NotificationManager for a given NotificationQueue.
    @param notification
        A pointer to the Notification to be unregistered.
    @param queue
        A pointer to the NotificationQueue the Notification is unregistered for.
    */
    void NotificationManager::unregisterNotification(Notification* notification, NotificationQueue* queue)
    {
        assert(notification);
        assert(queue);

        // Remove the Notification from the list of Notifications of the input NotificationQueue.
        this->removeNotification(notification, *(this->notificationLists_.find(queue->getName())->second));

        COUT(4) << "Notification (&" << notification << ") unregistered with the NotificationManager from NotificationQueue " << queue->getName() << "." << std::endl;
    }

    /**
    @brief
        Helper method that removes an input Notification form an input map.
    @param notification
        A pointer to the Notification to be removed.
    @param map
        The map the Notification should be removed from.
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
        Fetches the Notifications for a specific NotificationQueue in a specified timeframe and stores them in the input map.
    @param queue
        The NotificationQueue the Notifications are fetched for.
    @param map
        A pointer to a multimap, in which the notifications are stored. The map needs to have been allocated.
    @param timeFrameStart
        The start time of the timeframe.
    @param timeFrameEnd
        The end time of the timeframe.
    @return
        Returns true if successful.
    */
    void NotificationManager::getNotifications(NotificationQueue* queue, std::multimap<std::time_t,Notification*>* map, const std::time_t & timeFrameStart, const std::time_t & timeFrameEnd)
    {
        assert(queue);
        assert(map);

        std::multimap<std::time_t, Notification*>* notifications = this->notificationLists_[queue->getName()]; // All the Notifications for the input NotificationQueue.

        std::multimap<std::time_t,Notification*>::iterator it, itLowest, itHighest;
        // Iterators pointing to the bounds specified by the specified start and end times of the time frame.
        itLowest = notifications->lower_bound(timeFrameStart);
        itHighest = notifications->upper_bound(timeFrameEnd);

        for(it = itLowest; it != itHighest; it++) // Iterate through the Notifications from the start of the time frame to the end of it.
            map->insert(std::pair<std::time_t, Notification*>(it->first, it->second)); // Add the found Notifications to the map.
    }

    /**
    @brief
        Fetches the newest Notifications for a specific NotificationQueue and stores them in the input map.
    @param queue
        The NotificationQueue the Notifications are fetched for.
    @param map
        A pointer to a multimap, in which the notifications are stored. The map needs to have been allocated.
    @param numberOfNotifications
        The number of newest Notifications to be got.
    @return
        Returns true if successful.
    */
    void NotificationManager::getNewestNotifications(NotificationQueue* queue, std::multimap<std::time_t, Notification*>* map, int numberOfNotifications)
    {
        assert(queue);
        assert(map);

        std::multimap<std::time_t, Notification*>* notifications = this->notificationLists_[queue->getName()]; // All the Notifications for the input NotificationQueue.

        if(!notifications->empty()) // If the list of Notifications is not empty.
        {
            std::multimap<std::time_t,Notification*>::iterator it = notifications->end();
            for(int i = 0; i < numberOfNotifications; i++) // Iterate through the Notifications from the newest until we have the specified number of notifications.
            {
                it--;
                map->insert(std::pair<std::time_t, Notification*>(it->first, it->second)); // Add the found Notifications to the map.
                if(it == notifications->begin())
                    break;
            }
        }
    }

    /**
    @brief
        Enters the edit mode of the NotificationLayer.
    */
    void NotificationManager::enterEditMode(void)
    {
        if(GameMode::showsGraphics())
        {
            GUIManager::getInstance().hideGUI("NotificationLayer");
            GUIManager::getInstance().showGUI("NotificationLayer", false, false);
            GUIManager::getInstance().getLuaState()->doString("NotificationLayer.enterEditMode()");
        }
    }

    /**
    @brief
        Registers a NotificationQueue.
        This makes sure that the NotificationQueue can be accessed through lua by name. It also makes sure that the NotificationQueue is destroyed upon destruction of the NotificationManager.
    @param queue
        A pointer to the NotificationQueue to be registered.
    @return
        Returns true if successful. If e.g. the a NotificationQueue with that name already exists this returns false.
    */
    bool NotificationManager::registerQueue(NotificationQueue* queue)
    {
        assert(queue);

        // If the NotificationQueue is already registered.
        if(this->queues_.find(queue->getName()) != this->queues_.end())
            return false;

        this->queues_.insert(std::pair<const std::string, NotificationQueue*>(queue->getName(), queue)); // Add the NotificationQueue to the list of NotificationQueues.

        const std::set<std::string>& set = queue->getTargetsSet();

        // If all senders are the target of the NotificationQueue, then the list of Notifications for that specific NotificationQueue is the same as the list of all Notifications.
        bool bAll = set.find(NotificationListener::ALL) != set.end();
        std::multimap<std::time_t, Notification*>* map = NULL;
        if(bAll)
            this->notificationLists_[queue->getName()] = &this->allNotificationsList_;
        // Else a new list (resp. multimap) is created and added to the list of Notification lists for NotificationQueues.
        else
        {
            this->notificationLists_[queue->getName()] = new std::multimap<std::time_t, Notification*>;
            map = this->notificationLists_[queue->getName()];
        }

        // Iterate through all Notifications to determine whether any of them should belong to the newly registered NotificationQueue.
        for(std::multimap<std::time_t, Notification*>::iterator it = this->allNotificationsList_.begin(); it != this->allNotificationsList_.end(); it++)
        {
            if(!bAll && set.find(it->second->getSender()) != set.end()) // Checks whether the listener has the sender of the current Notification as target.
                map->insert(std::pair<std::time_t, Notification*>(it->first, it->second));
        }

        queue->update(); // Update the queue.
        
        COUT(4) << "NotificationQueue '" << queue->getName() << "' registered with the NotificationManager." << std::endl;
        return true;
    }

    /**
    @brief
        Unregisters a NotificationQueue.
    @param queue
        A pointer to the NotificationQueue to be unregistered.
    */
    void NotificationManager::unregisterQueue(NotificationQueue* queue)
    {
        assert(queue);

        std::multimap<std::time_t, Notification*>* map = this->notificationLists_.find(queue->getName())->second;

        // If the map is not the map of all Notifications, make sure all Notifications are unregistered.
        std::multimap<std::time_t, Notification*>::iterator it = map->begin();
        if(map != &this->allNotificationsList_)
        {
            while(it != map->end())
            {
                this->unregisterNotification(it->second, queue);
                it = map->begin();
            }
            delete map;
        }

        // Remove the NotificationQueue from the list of NotificationQueues.
        this->queues_.erase(queue->getName());
        // Remove the Notifications list that was associated with the input NotificationQueue.
        this->notificationLists_.erase(queue->getName());
        
        COUT(4) << "NotificationQueue '" << queue->getName() << "' unregistered with the NotificationManager." << std::endl;
    }

    /**
    @brief
        Loads all the NotificationQueues that should exist.
    */
    void NotificationManager::loadQueues(void)
    {
        NotificationQueue* allQueue = new NotificationQueue("all");
        GUIManager::getInstance().getLuaState()->doString("NotificationLayer.resizeQueue(\"all\", 0.5, 0, " + multi_cast<std::string>(allQueue->getMaxSize()) + ")");
        GUIManager::getInstance().getLuaState()->doString("NotificationLayer.moveQueue(\"all\", 0, 10, 0.3, 0)");

        NotificationQueue* infoQueue = new NotificationQueue("info", NotificationListener::ALL, 1, -1);
        GUIManager::getInstance().getLuaState()->doString("NotificationLayer.changeQueueFont(\"info\", 24, \"FFFFFF00\")");
        GUIManager::getInstance().getLuaState()->doString("NotificationLayer.resizeQueue(\"info\", 0.6, 0, " + multi_cast<std::string>(infoQueue->getMaxSize()) + ")");
        GUIManager::getInstance().getLuaState()->doString("NotificationLayer.moveQueue(\"info\", 0.2, 0, 0.8, 0)");
    }

    /**
    @brief
        Creates a new NotificationQueue.
        This is used in lua.
    @param name
        The name of the new NotificationQueue.
    */
    void NotificationManager::createQueue(const std::string& name)
    {
        new NotificationQueue(name);
    }

    /**
    @brief
        Get the NotificationQueue with the input name.
    @param name
        The name of the NotificationQueue.
    @return
        Returns a pointer to the NotificationQueue with the input name. Returns NULL if no NotificationQueue with such a name exists.
    */
    NotificationQueue* NotificationManager::getQueue(const std::string & name)
    {
        std::map<const std::string, NotificationQueue*>::iterator it = this->queues_.find(name);
        // Returns NULL if no such NotificationQueue exists.
        if(it == this->queues_.end())
            return NULL;

        return (*it).second;
    }

}
