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
#include "core/XMLPort.h"
#include "util/SubString.h"

namespace orxonox
{

    CreateFactory(NotificationQueue);
    
    /**
    @brief
        Default constructor. Registers and initializes the object.
    @param creator
        The creator of the NotificationQueue.
    */
    NotificationQueue::NotificationQueue(BaseObject* creator) : BaseObject(creator), Synchronisable(creator), registered_(false)
    {
        RegisterObject(NotificationQueue);

        this->size_ = 0;
        this->tickTime_ = 0.0f;
        this->maxSize_ = NotificationQueue::DEFAULT_SIZE;
        this->displayTime_ = NotificationQueue::DEFAULT_DISPLAY_TIME;

        this->creationTime_ = std::time(0);
        
        this->registerVariables();
    }

    // TODO move to docu.
    /**
    @brief
        Constructor. Registers and initializes the object.
    @param creator
        The creator of the NotificationQueue
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

    /**
    @brief
        Destructor.
    */
    NotificationQueue::~NotificationQueue()
    {
        this->targets_.clear();

        if(this->isRegistered()) // If the NotificationQueue is registered.
        {
            this->clear(true);

            // Unregister with the NotificationManager.
            NotificationManager::getInstance().unregisterQueue(this);
        }
    }

    /**
    @brief
        Is called when the name of the NotificationQueue has changed.
        Clears and re-creates the NotificationQueue.
    */
    void NotificationQueue::changedName(void)
    {
        SUPER(NotificationQueue, changedName);

        if(this->isRegistered())
            this->clear();
            
        this->create();

        this->targetsChanged();
        this->maxSizeChanged();
        this->displayTimeChanged();
    }

    /**
    @brief
        Creates the NotificationQueue.
    */
    void NotificationQueue::create(void)
    {
        // Register the NotificationQueue with the NotificationManager.
        bool queueRegistered = NotificationManager::getInstance().registerQueue(this);
        this->registered_ = true;
        if(!queueRegistered) // If the registration has failed.
        {
            this->registered_ = false;
            orxout(internal_error, context::notifications) << "NotificationQueue '" << this->getName() << "' could not be registered." << endl;
            return;
        }

        orxout(internal_info, context::notifications) << "NotificationQueue '" << this->getName() << "' created." << endl;
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
        if(this->displayTime_ != INF && this->tickTime_ >= 1.0) // If the time counter is greater than 1 s all Notifications that have expired are removed, if it is smaller we wait to the next tick.
        {
            this->timeLimit_.time = std::time(0)-this->displayTime_; // Container containing the current time.

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

    void NotificationQueue::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(NotificationQueue, XMLPort, xmlelement, mode);

        XMLPortParam(NotificationQueue, "targets", setTargets, getTargets, xmlelement, mode).defaultValues(NotificationListener::ALL);
        XMLPortParam(NotificationQueue, "size", setMaxSize, getMaxSize, xmlelement, mode);
        XMLPortParam(NotificationQueue, "displayTime", setDisplayTime, getDisplayTime, xmlelement, mode);
    }
    
    
    /**
    @brief
        Registers Variables to be Synchronised.
        Registers Variables which have to be synchronised to the network system.
      */
    void NotificationQueue::registerVariables()
    {
        registerVariable( this->name_, VariableDirection::ToClient, new NetworkCallback<NotificationQueue>(this, &NotificationQueue::changedName));
        registerVariable( this->maxSize_, VariableDirection::ToClient, new NetworkCallback<NotificationQueue>(this, &NotificationQueue::maxSizeChanged));
        registerVariable( this->targets_, VariableDirection::ToClient, new NetworkCallback<NotificationQueue>(this, &NotificationQueue::targetsChanged));
        registerVariable( this->displayTime_, VariableDirection::ToClient, new NetworkCallback<NotificationQueue>(this, &NotificationQueue::displayTimeChanged));
    }

    /**
    @brief
        Updates the NotificationQueue.
        Updates by clearing the queue and requesting all relevant Notifications from the NotificationManager and inserting them into the queue.
        This is called by the NotificationManager when the Notifications have changed so much, that the NotificationQueue may have to re-initialize his operations.
    */
    void NotificationQueue::update(void)
    {
        this->clear();

        std::multimap<std::time_t, Notification*>* notifications = new std::multimap<std::time_t, Notification*>;
        // Get the Notifications sent in the interval from now to now minus the display time.
        if(this->displayTime_ == INF)
            NotificationManager::getInstance().getNewestNotifications(this, notifications, this->getMaxSize());
        else
            NotificationManager::getInstance().getNotifications(this, notifications, this->displayTime_);

        if(!notifications->empty())
        {
            // Add all Notifications that have been created after this NotificationQueue was created.
            for(std::multimap<std::time_t, Notification*>::iterator it = notifications->begin(); it != notifications->end(); it++)
            {
                if(it->first >= this->creationTime_)
                    this->push(it->second, it->first);
            }
        }

        delete notifications;

        orxout(verbose, context::notifications) << "NotificationQueue '" << this->getName() << "' updated." << endl;
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

        orxout(verbose, context::notifications) << "NotificationQueue '" << this->getName() << "' updated. A new Notification has been added." << endl;
    }

    /**
    @brief
        Adds (pushes) a Notification to the NotificationQueue.
        It inserts it into the storage containers, creates a corresponding container and pushes the notification message to the GUI.
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

        // Inform that a Notification was pushed.
        this->notificationPushed(notification);

        orxout(verbose_more, context::notifications) << "Notification \"" << notification->getMessage() << "\" pushed to NotificationQueue '" << this->getName() << "'" << endl;
        orxout(internal_info, context::notifications) << "NotificationQueue \"" << this->getName() << "\": " << notification->getMessage() << endl;
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
                orxout(verbose_more, context::notifications) << "Notification \"" << (*it)->notification->getMessage() << "\" popped from NotificationQueue '" << this->getName() << "'" << endl;
                this->ordering_.erase(it);
                break;
            }
        }
        this->notifications_.pop_back();

        this->size_--;

        delete container;

        // Inform that a Notification was popped.
        this->notificationPopped();
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

        orxout(verbose_more, context::notifications) << "Notification \"" << (*it)->notification->getMessage() << "\" removed from NotificationQueue '" << this->getName() << "'" << endl;

        this->ordering_.erase(containerIterator);
        this->notifications_.erase(it);

        this->size_--;

        delete *containerIterator;

        // TODO: index automatically cast?
        // Inform that a Notification was removed.
        this->notificationRemoved(index);
    }

    /**
    @brief
        Clears the NotificationQueue by removing all NotificationContainers.
    @param noGraphics
        If this is set to true the GUI is not informed of the clearing of the NotificationQueue. This is needed only internally.
    */
    void NotificationQueue::clear(bool noGraphics)
    {
        orxout(verbose, context::notifications) << "Clearing NotificationQueue " << this->getName() << "." << endl;
        this->ordering_.clear();
        // Delete all NotificationContainers in the list.
        for(std::vector<NotificationContainer*>::iterator it = this->notifications_.begin(); it != this->notifications_.end(); it++)
            delete *it;

        this->notifications_.clear();
        this->size_ = 0;
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

        if(size == 0)
        {
            orxout(internal_warning, context::notifications) << "Trying to set maximal size of NotificationQueue '" << this->getName() << "' to 0. Ignoring..." << endl;
            return;
        }
        
        this->maxSize_ = size;
        this->maxSizeChanged();
    }

    /**
    @brief
        Is called when the maximum number of displayed Notifications has changed.
    */
    void NotificationQueue::maxSizeChanged(void)
    {
        if(this->isRegistered())
            this->update();
    }

    /**
    @brief
        Sets the maximum number of seconds a Notification is displayed.
    @param time
        The number of seconds a Notification is displayed.
    */
    void NotificationQueue::setDisplayTime(int time)
    {
        if(this->displayTime_ == time)
            return;

        if(time != NotificationQueue::INF && time <= 0)
        {
            orxout(internal_warning, context::notifications) << "Trying to set display time of NotificationQueue '" << this->getName() << "' to non-positive value. Ignoring..." << endl;
        }
            
        this->displayTime_ = time;
        this->displayTimeChanged();
    }

    /**
    @brief
        Is called when the maximum number of seconds a Notification is displayed has changed.
    */
    void NotificationQueue::displayTimeChanged(void)
    {
        if(this->isRegistered())
            this->update();
    }

    /**
    @brief
        Produces all targets of the NotificationQueue concatenated as string, with commas (',') as separators.
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
        Accepts a string of targets, each separated by commas (','), spaces are ignored.
    */
    void NotificationQueue::setTargets(const std::string & targets)
    {
        this->targets_.clear();

        SubString string = SubString(targets, ",", " ", false);
        for(unsigned int i = 0; i < string.size(); i++)
            this->targets_.insert(string[i]);

        this->targetsChanged();
    }

    /**
    @brief
        Is called when the NotificationQueue's targets have changed.
    */
    void NotificationQueue::targetsChanged(void)
    {
        // TODO: Why?
        if(this->isRegistered())
        {
            NotificationManager::getInstance().unregisterQueue(this);
            NotificationManager::getInstance().registerQueue(this);
        }
    }

    /**
    @brief
        Pops all Notifications from the NotificationQueue.
    @return
        Returns true if successful, false if not.
    */
    bool NotificationQueue::tidy(void)
    {
        while(this->size_ > 0)
            this->pop();
        return true;
    }

}

