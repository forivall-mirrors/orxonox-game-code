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
#include "network/Host.h"
#include "network/NetworkFunction.h"
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

    ManageScopedSingleton(NotificationManager, ScopeID::Root, false);

    // Setting console command to enter the edit mode.
    SetConsoleCommand("enterEditMode", &NotificationManager::enterEditMode);

    registerStaticNetworkFunction(NotificationManager::sendNotification);

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

    /**
    @brief
        Sends a Notification with the specified message to the specified client from the specified sender.
    @param message
        The message that should be sent.
    @param clientId
        The id of the client the notification should be sent to.
    @param sender
        The sender that sent the notification.
    @param isLocal
        If this is set to true (false is default), then the Notification is sent to the client where this function is executed, meaning the Notification is sent locally.
    */
    /*static*/ void NotificationManager::sendNotification(const std::string& message, unsigned int clientId, const std::string& sender, bool isLocal)
    {
        // If we're in standalone mode or we're already no the right client we create and send the Notification.
        if(GameMode::isStandalone() || isLocal || Host::getPlayerID() == clientId)
        {
            Notification* notification = new Notification(message, sender);
            if(NotificationManager::getInstance().registerNotification(notification))
                COUT(3) << "Notification \"" << notification->getMessage() << "\" sent." << std::endl;
        }
        // If we're on the server (and the server is not the intended recipient of the Notification) we send it over the network.
        else if(GameMode::isServer())
        {
            callStaticNetworkFunction(NotificationManager::sendNotification, clientId, message, clientId, sender);
        }
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
        assert(notification);

        std::time_t time = std::time(0); // Get current time.

        // Add the Notification to the list that holds all Notifications.
        this->allNotificationsList_.insert(std::pair<std::time_t, Notification*>(time, notification));

        if(notification->getSender() == NotificationManager::NONE) // If the sender has no specific name, then the Notification is only added to the list of all Notifications.
            return true;

        bool all = false;
        if(notification->getSender() == NotificationManager::ALL) // If all are the sender, then the Notifications is added to every NotificationListener.
            all = true;

        // Insert the Notification in all NotificationListeners that have its sender as target.
        for(std::map<NotificationListener*, unsigned int>::iterator it = this->listenerList_.begin(); it != this->listenerList_.end(); it++) // Iterate through all NotificationListeners.
        {
            const std::set<std::string>& set = it->first->getTargetsSet();
            bool bAll = set.find(NotificationManager::ALL) != set.end();
            // If either the Notification has as sender 'all', the NotificationListener displays all Notifications or the NotificationListener has the sender of the Notification as target.
            if(all || bAll || set.find(notification->getSender()) != set.end())
            {
                if(!bAll)
                    this->notificationLists_[it->second]->insert(std::pair<std::time_t, Notification*>(time, notification)); // Insert the Notification in the notifications list of the current NotificationListener.
                it->first->update(notification, time); // Update the NotificationListener.
            }
        }

        COUT(4) << "Notification (&" << notification << ") registered with the NotificationManager." << std::endl;

        return true;
    }

    /**
    @brief
        Unregisters a Notification within the NotificationManager for a given NotificationListener.
    @param notification
        A pointer to the Notification to be unregistered.
    @param listener
        A pointer to the NotificationListener the Notification is unregistered for.
    */
    void NotificationManager::unregisterNotification(Notification* notification, NotificationListener* listener)
    {
        assert(notification);
        assert(listener);

        // Remove the Notification from the list of Notifications of the input NotificationListener.
        this->removeNotification(notification, *(this->notificationLists_.find(this->listenerList_.find(listener)->second)->second));

        COUT(4) << "Notification (&" << notification << ") unregistered with the NotificationManager from listener (&" << listener << ")" << std::endl;
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
        Registers a NotificationListener within the NotificationManager.
    @param listener
        The NotificationListener to be registered.
    @return
        Returns true if successful.  Fales if the NotificationListener is already registered.
    */
    bool NotificationManager::registerListener(NotificationListener* listener)
    {
        assert(listener);

        // If the NotificationListener is already registered.
        if(this->listenerList_.find(listener) != this->listenerList_.end())
            return false;

        this->highestIndex_ += 1;
        unsigned int index = this->highestIndex_; // An identifier that identifies each registered NotificationListener uniquely.

        this->listenerList_[listener] = index; // Add the NotificationListener to the list of NotificationListeners.

        const std::set<std::string>& set = listener->getTargetsSet();

        // If all senders are the target of the NotificationListener, then the list of Notifications for that specific NotificationListener is the same as the list of all Notifications.
        bool bAll = set.find(NotificationManager::ALL) != set.end();
        std::multimap<std::time_t, Notification*>* map = NULL;
        if(bAll)
            this->notificationLists_[index] = &this->allNotificationsList_;
        // Else a new list (resp. multimap) is created and added to the list of Notification lists for NotificationListeners.
        else
        {
            this->notificationLists_[index] = new std::multimap<std::time_t, Notification*>;
            map = this->notificationLists_[index];
        }

        // Iterate through all Notifications to determine whether any of them should belong to the newly registered NotificationListener.
        for(std::multimap<std::time_t, Notification*>::iterator it = this->allNotificationsList_.begin(); it != this->allNotificationsList_.end(); it++)
        {
            if(!bAll && set.find(it->second->getSender()) != set.end()) // Checks whether the listener has the sender of the current Notification as target.
                map->insert(std::pair<std::time_t, Notification*>(it->first, it->second));
        }

        listener->update(); // Update the listener.

        COUT(4) << "NotificationListener registered with the NotificationManager." << std::endl;

        return true;
    }

    /**
    @brief
        Unregisters a NotificationListener within the NotificationManager.
    @param listener
        The NotificationListener to be unregistered.
    */
    void NotificationManager::unregisterListener(NotificationListener* listener)
    {
        assert(listener);

        unsigned int identifier = this->listenerList_.find(listener)->second;
        std::multimap<std::time_t, Notification*>* map = this->notificationLists_.find(identifier)->second;

        // If the map is not the map of all Notifications, make sure all Notifications are unregistered.
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

        COUT(4) << "NotificationListener '" << identifier << "' unregistered with the NotificationManager." << std::endl;

        // Remove the NotificationListener from the list of NotificationListeners.
        this->listenerList_.erase(listener);
        // Remove the Notifications list that was associated with the input NotificationListener.
        this->notificationLists_.erase(identifier);
    }

    /**
    @brief
        Fetches the Notifications for a specific NotificationListener in a specified timeframe and stores them in the input map.
    @param listener
        The NotificationListener the Notifications are fetched for.
    @param map
        A pointer to a multimap, in which the notifications are stored. The map needs to have been allocated.
    @param timeFrameStart
        The start time of the timeframe.
    @param timeFrameEnd
        The end time of the timeframe.
    @return
        Returns true if successful.
    */
    void NotificationManager::getNotifications(NotificationListener* listener, std::multimap<std::time_t,Notification*>* map, const std::time_t & timeFrameStart, const std::time_t & timeFrameEnd)
    {
        assert(listener);
        assert(map);

        std::multimap<std::time_t, Notification*>* notifications = this->notificationLists_[this->listenerList_[listener]]; // All the Notifications for the input NotificationListener.

        std::multimap<std::time_t,Notification*>::iterator it, itLowest, itHighest;
        // Iterators pointing to the bounds specified by the specified start and end times of the time frame.
        itLowest = notifications->lower_bound(timeFrameStart);
        itHighest = notifications->upper_bound(timeFrameEnd);

        for(it = itLowest; it != itHighest; it++) // Iterate through the Notifications from the start of the time frame to the end of it.
            map->insert(std::pair<std::time_t, Notification*>(it->first, it->second)); // Add the found Notifications to the map.
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
        This makes sure that the NotificationQueue can be attained through lua by name. It also makes sure that the NotificationQueue is destroyed upon destruction of the NotificationManager.
    @param queue
        A pointer to the NotificationQueue to be registered.
    @return
        Returns true if successful. If e.g. the a NotificationQueue with that name already exists this returns false.
    */
    bool NotificationManager::registerQueue(NotificationQueue* queue)
    {
        COUT(4) << "NotificationQueue '" << queue->getName() << "' registered with the NotificationManager." << std::endl;
        return this->queues_.insert(std::pair<const std::string, NotificationQueue*>(queue->getName(), queue)).second;
    }

    /**
    @brief
        Unregisters a NotificationQueue.
    @param queue
        A pointer to the NotificationQueue to be unregistered.
    */
    void NotificationManager::unregisterQueue(NotificationQueue* queue)
    {
        COUT(4) << "NotificationQueue '" << queue->getName() << "' unregistered with the NotificationManager." << std::endl;
        this->queues_.erase(queue->getName());
    }

    /**
    @brief
        Loads all the NotificationQueues that should exist.
    */
    void NotificationManager::loadQueues(void)
    {
        new NotificationQueue("all");
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
