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
    @file NotificationQueue.cc
    @brief Implementation of the NotificationQueue class.
*/

#include "NotificationQueue.h"

#include <map>
#include <sstream>

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/GUIManager.h"
#include "core/LuaState.h"
#include "util/Convert.h"
#include "util/SubString.h"

#include "Notification.h"

namespace orxonox
{

    /**
    @brief
        Constructor. Creates and initializes the object.
    @param name
        The name of the new NotificationQueue. It needs to be unique
    @param senders
        The senders that are targets of this NotificationQueue, i.e. the names of senders whose Notifications this NotificationQueue displays.
        The senders need to be seperated by commas.
    @param size
        The size (the maximum number of displayed Notifications) of this NotificationQueue.
    @param displayTime
        The time during which a Notification is (at most) displayed.
    */
    NotificationQueue::NotificationQueue(const std::string& name, const std::string& senders, unsigned int size, unsigned int displayTime)
    {
        this->registered_ = false;

        RegisterRootObject(NotificationQueue);

        // Initialize.
        this->size_ = 0;
        this->tickTime_ = 0.0f;

        // Sets the input values.
        this->setTargets(senders);
        this->name_ = name;
        this->maxSize_ = size;
        this->setDisplayTime(displayTime);

        // Register the NotificationQueue with the NotificationManager.
        bool queueRegistered = NotificationManager::getInstance().registerQueue(this);
        this->registered_ = true;
        if(!queueRegistered) // If the registration has failed.
        {
            this->registered_ = false;
            COUT(1) << "Error: NotificationQueue '" << this->getName() << "' could not be registered." << std::endl;
            return;
        }

        this->create(); // Creates the NotificationQueue in lua.

        // Register the NotificationQueue as NotificationListener with the NotificationManager.
        bool listenerRegistered = NotificationManager::getInstance().registerListener(this);
        if(!listenerRegistered) // If the registration has failed.
        {
            this->registered_ = false;
            // Remove the NotificationQueue in lua.
            if(GameMode::showsGraphics())
                GUIManager::getInstance().getLuaState()->doString("NotificationLayer.removeQueue(\"" + this->getName() +  "\")");
            NotificationManager::getInstance().unregisterQueue(this);
            COUT(1) << "Error: NotificationQueue '" << this->getName() << "' could not be registered." << std::endl;
            return;
        }

        COUT(3) << "NotificationQueue '" << this->getName() << "' created." << std::endl;
    }

    /**
    @brief
        Destructor.
    */
    NotificationQueue::~NotificationQueue()
    {
        this->targets_.clear();

        if(this->registered_) // If the NotificationQueue is registered.
        {
            this->clear(true);

            // Unregister with the NotificationManager.
            NotificationManager::getInstance().unregisterListener(this);
            NotificationManager::getInstance().unregisterQueue(this);
        }
    }

    /**
    @brief
        Destroys the NotificationQueue.
        Used in lua and NotificationManager.
    @param noGraphics
        If this is set to true (false is default), then the queue is not removed in lua. This is used to destroy the queue, after the GUIManager has been destroyed.
    */
    void NotificationQueue::destroy(bool noGraphics)
    {
        // Remove the NotificationQueue in lua.
        if(GameMode::showsGraphics() && !noGraphics)
            GUIManager::getInstance().getLuaState()->doString("NotificationLayer.removeQueue(\"" + this->getName() +  "\")");

        COUT(3) << "NotificationQueue '" << this->getName() << "' destroyed." << std::endl;

        this->OrxonoxClass::destroy();
    }

    /**
    @brief
        Creates the NotificationQueue in lua.
    */
    void NotificationQueue::create(void)
    {
        if(GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString("NotificationLayer.createQueue(\"" + this->getName() +  "\", " + multi_cast<std::string>(this->getMaxSize()) + ")");
    }

    /**
    @brief
        Updates the queue from time to time.
    @param dt
        The time interval that has passed since the last tick.
    */
    void NotificationQueue::tick(float dt)
    {
        this->tickTime_ += dt; // Add the time interval that has passed to the time counter.
        if(this->tickTime_ >= 1.0) // If the time counter is greater than 1s all Notifications that have expired are removed, if it is smaller we wait to the next tick.
        {
            this->timeLimit_.time = std::time(0)-this->displayTime_; // Container containig the current time.

            std::multiset<NotificationContainer*, NotificationContainerCompare>::iterator it = this->ordering_.begin();
            // Iterate through all elements whose creation time is smaller than the current time minus the display time.
            while(it != this->ordering_.upper_bound(&this->timeLimit_))
            {
                this->remove(it); // Remove the Notifications that have expired.
                it = this->ordering_.begin();
            }

            this->tickTime_ = this->tickTime_ - (int)this->tickTime_; // Reset time counter.
        }
    }

    /**
    @brief
        Updates the NotificationQueue.
        Updates by clearing the queue and requesting all relevant Notifications from the NotificationManager and inserting them into the queue.
    */
    void NotificationQueue::update(void)
    {
        this->clear();

        std::multimap<std::time_t, Notification*>* notifications = new std::multimap<std::time_t, Notification*>;
        // Get the Notifications sent in the interval from now to now minus the display time.
        NotificationManager::getInstance().getNotifications(this, notifications, this->displayTime_);

        if(!notifications->empty())
        {
            // Add all Notifications.
            for(std::multimap<std::time_t, Notification*>::iterator it = notifications->begin(); it != notifications->end(); it++)
                this->push(it->second, it->first);
        }

        delete notifications;

        COUT(4) << "NotificationQueue '" << this->getName() << "' updated." << std::endl;
    }

    /**
    @brief
        Updates the NotificationQueue by adding an new Notification.
    @param notification
        Pointer to the Notification.
    @param time
        The time the Notification was sent.
    */
    void NotificationQueue::update(Notification* notification, const std::time_t & time)
    {
        assert(notification);

        this->push(notification, time);

        COUT(4) << "NotificationQueue '" << this->getName() << "' updated. A new Notification has been added." << std::endl;
    }

    /**
    @brief
        Adds (pushes) a Notification to the NotificationQueue.
        It inserts it into the storage containers, creates a corresponding container and pushes the Notification message to the GUI.
    @param notification
        The Notification to be pushed.
    @param time
        The time when the Notification has been sent.
    */
    void NotificationQueue::push(Notification* notification, const std::time_t & time)
    {
        assert(notification);

        NotificationContainer* container = new NotificationContainer;
        container->notification = notification;
        container->time = time;

        // If the maximum size of the NotificationQueue has been reached the last (least recently added) Notification is removed.
        if(this->getSize() >= this->getMaxSize())
            this->pop();

        this->size_++;

        this->ordering_.insert(container);
        // Insert the Notification at the begin of the list (vector, actually).
        this->notifications_.insert(this->notifications_.begin(), container);

        // Push the Notification to the GUI.
        if(GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString("NotificationLayer.pushNotification(\"" + this->getName() + "\", \"" + notification->getMessage() + "\")");

        COUT(5) << "Notification \"" << notification->getMessage() << "\" pushed to NotificationQueue '" << this->getName() << "'" << endl;
    }

    /**
    @brief
        Removes (pops) the least recently added Notification form the NotificationQueue.
    */
    void NotificationQueue::pop(void)
    {
        NotificationContainer* container = this->notifications_.back();
        // Get all the NotificationContainers that were sent the same time the NotificationContainer we want to pop was sent.
        std::pair<std::multiset<NotificationContainer*, NotificationContainerCompare>::iterator, std::multiset<NotificationContainer*, NotificationContainerCompare>::iterator> iterators = this->ordering_.equal_range(container);

        // Iterate through all suspects and remove the container as soon as we find it.
        for(std::multiset<NotificationContainer*, NotificationContainerCompare>::iterator it = iterators.first; it != iterators.second; it++)
        {
            if(container == *it)
            {
                COUT(5) << "Notification \"" << (*it)->notification->getMessage() << "\" popped from NotificationQueue '" << this->getName() << "'" << endl;
                this->ordering_.erase(it);
                break;
            }
        }
        this->notifications_.pop_back();

        this->size_--;

        delete container;

        // Pops the Notification from the GUI.
        if(GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString("NotificationLayer.popNotification(\"" + this->getName() + "\")");
    }

    /**
    @brief
        Removes the Notification that is stored in the input NotificationContainer.
    @param containerIterator
        An iterator to the NotificationContainer to be removed.
    */
    void NotificationQueue::remove(const std::multiset<NotificationContainer*, NotificationContainerCompare>::iterator& containerIterator)
    {
        std::vector<NotificationContainer*>::iterator it = std::find(this->notifications_.begin(), this->notifications_.end(), *containerIterator);
        // Get the index at which the Notification is.
        std::vector<NotificationContainer*>::difference_type index = it - this->notifications_.begin ();

        COUT(5) << "Notification \"" << (*it)->notification->getMessage() << "\" removed from NotificationQueue '" << this->getName() << "'" << endl;

        this->ordering_.erase(containerIterator);
        this->notifications_.erase(it);

        this->size_--;

        delete *containerIterator;

        // Removes the Notification from the GUI.
        if(GameMode::showsGraphics())
            GUIManager::getInstance().getLuaState()->doString("NotificationLayer.removeNotification(\"" + this->getName() + "\", " + multi_cast<std::string>(index) + ")");
    }

    /**
    @brief
        Clears the NotificationQueue by removing all NotificationContainers.
    @param noGraphics
        If this is eset to true the GUI is not informed of the clearing of the NotificationQueue. This is needed only internally.
    */
    void NotificationQueue::clear(bool noGraphics)
    {
        COUT(4) << "Clearing NotificationQueue " << this->getName() << "." << endl;
        this->ordering_.clear();
        // Delete all NotificationContainers in the list.
        for(std::vector<NotificationContainer*>::iterator it = this->notifications_.begin(); it != this->notifications_.end(); it++)
            delete *it;

        this->notifications_.clear();
        this->size_ = 0;

        // Clear the NotificationQueue in the GUI.
        if(GameMode::showsGraphics() && !noGraphics)
            GUIManager::getInstance().getLuaState()->doString("NotificationLayer.clearQueue(\"" + this->getName() + "\")");
    }

    /**
    @brief
        Sets the name of the NotificationQueue.
    @param name
        The name to be set.
    */
    void NotificationQueue::setName(const std::string& name)
    {
        this->name_ = name;
    }

    /**
    @brief
        Sets the maximum number of displayed Notifications.
    @param size
        The size to be set.
    */
    void NotificationQueue::setMaxSize(unsigned int size)
    {
        if(this->maxSize_ == size)
            return;

        this->maxSize_ = size;

        if(this->registered_)
            this->update();
    }

    /**
    @brief
        Sets the maximum number of seconds a Notification is displayed.
    @param time
        The number of seconds the Notifications is displayed.
    @return
        Returns true if successful.
    */
    void NotificationQueue::setDisplayTime(unsigned int time)
    {
        if(this->displayTime_ == time)
            return;

        this->displayTime_ = time;

        if(this->registered_)
            this->update();
    }

    /**
    @brief
        Produces all targets of the NotificationQueue concatinated as string, with commas (',') as seperators.
    @return
        Returns the targets as a string.
    */
    const std::string& NotificationQueue::getTargets(void) const
    {
        std::stringstream stream;
        bool first = true;
        // Iterate through the set of targets.
        for(std::set<std::string>::const_iterator it = this->targets_.begin(); it != this->targets_.end(); it++)
        {
            if(!first)
                stream << ", ";
            else
                first = false;
            stream << *it;
        }

        return *(new std::string(stream.str()));
    }

    /**
    @brief
        Sets the targets of the NotificationQueue.
        The targets are the senders whose Notifications are displayed in this queue.
    @param targets
        Accepts a string of targets, each seperated by commas (','), spaces are ignored.
    */
    void NotificationQueue::setTargets(const std::string & targets)
    {
        this->targets_.clear();

        SubString string = SubString(targets, ",", " ", false);
        for(unsigned int i = 0; i < string.size(); i++)
            this->targets_.insert(string[i]);

        if(this->registered_)
        {
            NotificationManager::getInstance().unregisterListener(this);
            NotificationManager::getInstance().registerListener(this);
        }
    }

}

