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
    @brief Implementation of the NotificationQueue class.
*/

#include "NotificationQueue.h"

#include <sstream>

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "NotificationOverlay.h"
#include "NotificationManager.h"

namespace orxonox
{

    CreateFactory(NotificationQueue);

    const std::string NotificationQueue::DEFAULT_FONT("VeraMono");
    const Vector2 NotificationQueue::DEFAULT_POSITION(0.0,0.0);
    const float NotificationQueue::DEFAULT_FONT_SIZE = 0.025f;

    /**
    @brief
        Constructor. Creates and initializes the object.
    */
    NotificationQueue::NotificationQueue(BaseObject* creator) : OverlayGroup(creator)
    {
        RegisterObject(NotificationQueue);
        this->initialize();
    }

    /**
    @brief
        Destructor.
    */
    NotificationQueue::~NotificationQueue()
    {
        this->targets_.clear();
        this->clear();
    }

    /**
    @brief
        Initializes the object.
        Registers the object, initializes variables, sets default values and registers the queue with the NotificationManager.
    */
    void NotificationQueue::initialize(void)
    {
        this->size_ = 0;
        this->tickTime_ = 0.0;

        NotificationManager::getInstance().registerListener(this);
    }

    /**
    @brief
        Sets the defaults.
    */
    void NotificationQueue::setDefaults(void)
    {
        this->setMaxSize(DEFAULT_SIZE);
        this->setNotificationLength(DEFAULT_LENGTH);
        this->setDisplayTime(DEFAULT_DISPLAY_TIME);
        this->setPosition(DEFAULT_POSITION);

        this->setTargets(NotificationManager::ALL);

        this->setFontSize(DEFAULT_FONT_SIZE);
        this->setFont(DEFAULT_FONT);
    }

    /**
    @brief
        Method for creating a NotificationQueue object through XML.
    */
    void NotificationQueue::XMLPort(Element& xmlElement, XMLPort::Mode mode)
    {
        SUPER(NotificationQueue, XMLPort, xmlElement, mode);

        this->setDefaults();

        XMLPortParam(NotificationQueue, "maxSize", setMaxSize, getMaxSize, xmlElement, mode);
        XMLPortParam(NotificationQueue, "notificationLength", setNotificationLength, getNotificationLength, xmlElement, mode);
        XMLPortParam(NotificationQueue, "displayTime", setDisplayTime, getDisplayTime, xmlElement, mode);
        XMLPortParam(NotificationQueue, "targets", setTargets, getTargets, xmlElement, mode);
        XMLPortParam(NotificationQueue, "font", setFont, getFont, xmlElement, mode);
        XMLPortParam(NotificationQueue, "fontSize", setFontSize, getFontSize, xmlElement, mode);
        XMLPortParam(NotificationQueue, "position", setPosition, getPosition, xmlElement, mode);

        COUT(3) << "NotificationQueue created." << std::endl;
    }

    /**
    @brief
        Updates the queue from time to time.
    @param dt
        The time interval that has passed since the last tick.
    */
    void NotificationQueue::tick(float dt)
    {
        this->tickTime_ += dt; //!< Add the time interval that has passed to the time counter.
        if(this->tickTime_ >= 1.0) //!< If the time counter is greater than 1s all Notifications that have expired are removed, if it is smaller we wait to the next tick.
        {
            this->timeLimit_.time = std::time(0)-this->displayTime_; //!< Container containig the current time.

            std::multiset<NotificationOverlayContainer*, NotificationOverlayContainerCompare>::iterator it;
            it = this->containers_.begin();
            while(it != this->containers_.upper_bound(&this->timeLimit_)) //!< Iterate through all elements whose creation time is smaller than the current time minus the display time.
            {
                this->removeContainer(*it);
                this->scroll(Vector2(0.0f,-(1.1f*this->getFontSize())));
                it = this->containers_.begin(); //TODO: Needed?
            }

            this->tickTime_ = 0.0f; //!< Reset time counter.
        }
    }

    /**
    @brief
        Updates the NotificationQueue.
        Updates by clearing the queue and requesting all relevant Notifications from the NotificationManager and inserting the in the queue.
    */
    void NotificationQueue::update(void)
    {
        this->clear();

        std::multimap<std::time_t,Notification*>* notifications = new std::multimap<std::time_t,Notification*>;
        if(!NotificationManager::getInstance().getNotifications(this, notifications, this->displayTime_)) //!< Get the Notifications sent in the interval form now to minus the display time.
        {
            COUT(1) << "NotificationQueue update failed due to undetermined cause." << std::endl;
            return;
        }

        if(notifications->empty())
            return;

        for(std::multimap<std::time_t,Notification*>::iterator it = notifications->begin(); it != notifications->end(); it++) //!> Add all Notifications.
        {
            this->addNotification(it->second, it->first);
        }

        delete notifications;

        COUT(3) << "NotificationQueue updated." << std::endl;
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
        this->addNotification(notification, time);

        std::multiset<NotificationOverlayContainer*, NotificationOverlayContainerCompare>::iterator it;
        while(this->getSize() > this->getMaxSize())
        {
            it = this->containers_.begin();
            this->removeContainer(*it);
            this->scroll(Vector2(0.0f,-(1.1f*this->getFontSize())));
        }

        COUT(3) << "NotificationQueue updated. A new Notifications has been added." << std::endl;
    }

    /**
    @brief
        Sets the maximum number of displayed Notifications.
    @param size
        The size to be set.
    @return
        Returns true if successful.
    */
    bool NotificationQueue::setMaxSize(int size)
    {
        if(size < 0)
            return false;
        this->maxSize_ = size;
        this->update();
        return true;
    }

    /**
    @brief
        Sets the maximum number of characters a Notification message displayed by this queue is allowed to have.
    @param length
        The length to be set.
    @return
        Returns true if successful.
    */
    bool NotificationQueue::setNotificationLength(int length)
    {
        if(length < 0)
            return false;
        this->notificationLength_ = length;
        this->update();
        return true;
    }

    /**
    @brief
        Sets the maximum number of seconds a Notification is displayed.
    @param time
        The number of seconds the Notifications is displayed.
    @return
        Returns true if successful.
    */
    bool NotificationQueue::setDisplayTime(int time)
    {
        if(time < 0)
            return false;
        this->displayTime_ = time;
        this->update();
        return true;
    }

    /**
    @brief
        Produces all targets concatinated as string, with kommas (',') as seperators.
    @param string
        Pointer to a string which will be used by the method to fill with the concatination of the targets.
    @return
        Returns true if successful.
    */
    bool NotificationQueue::getTargets(std::string* string) const
    {
        if(string == NULL)
        {
            COUT(4) << "Input string must have memory allocated." << std::endl;
            return false;
        }
        string->clear();
        bool first = true;
        for(std::set<std::string>::const_iterator it = this->targets_.begin(); it != this->targets_.end(); it++) //!< Iterate through the set of targets.
        {
            if(!first)
            {
                *string += ',';
            }
            else
            {
                first = false;
            }
            *string += *it;
        }

        return true;
    }

    /**
    @brief
        Sets the targets of the queue.
        The targets are the senders whose Notifications are displayed in this queue.
    @param targets
        Accepts a string of targets, each seperated by commas (','), spaces are ignored.
    @return
        Returns true if successful.
    */
    bool NotificationQueue::setTargets(const std::string & targets)
    {
        this->targets_.clear();

        std::string* pTemp;
        unsigned int index = 0;
        while( index < targets.size() ) //!< Go through the string, character by character until the end is reached.
        {
            pTemp = new std::string();
            while(index < targets.size() && targets[index] != ',' && targets[index] != ' ')
            {
                *pTemp += targets[index];
                index++;
            }
            index++;
            this->targets_.insert(*pTemp);
        }

        return true;
    }

    /**
    @brief
        Sets the font size.
    @param size
        The font size.
    @return
        Returns true if successful.
    */
    bool NotificationQueue::setFontSize(float size)
    {
        if(size <= 0)
            return false;
        this->fontSize_ = size;
        for (std::map<Notification*, NotificationOverlayContainer*>::iterator it = this->overlays_.begin(); it != this->overlays_.end(); it++) //!< Set the font size for each overlay.
        {
            it->second->overlay->setFontSize(size);
        }
        return true;
    }

    /**
    @brief
        Sets the font.
    @param font
        The font.
    @return
        Returns true if successful.
    */
    bool NotificationQueue::setFont(const std::string & font)
    {
        this->font_ = font;
        for (std::map<Notification*, NotificationOverlayContainer*>::iterator it = this->overlays_.begin(); it != this->overlays_.end(); it++) //!< Set the font for each overlay.
        {
            it->second->overlay->setFont(font);
        }
        return true;
    }

    /**
    @brief
        Scrolls the NotificationQueue, meaning all NotificationOverlays are moved the input vector.
    @param pos
        The vector the NotificationQueue is scrolled.
    */
    void NotificationQueue::scroll(const Vector2 pos)
    {
        for (std::map<Notification*, NotificationOverlayContainer*>::iterator it = this->overlays_.begin(); it != this->overlays_.end(); ++it) //!< Scroll each overlay.
        {
            it->second->overlay->scroll(pos);
        }
    }

    /**
    @brief
        Aligns all the Notifications to the position of the NotificationQueue.
    */
    void NotificationQueue::positionChanged(void)
    {
        int counter = 0;
        for (std::multiset<NotificationOverlayContainer*, NotificationOverlayContainerCompare>::iterator it = this->containers_.begin(); it != this->containers_.end(); it++) //!< Set the position for each overlay.
        {
            (*it)->overlay->setPosition(this->getPosition());
            (*it)->overlay->scroll(Vector2(0.0f,(1.1f*this->getFontSize())*counter));
            counter++;
        }
    }

    /**
    @brief
        Adds a Notification, to the queue.
        It inserts it into the storage containers, creates an corresponding overlay and a container.
    @param notification
        The Notification.
    @param time
        The time.
    */
    void NotificationQueue::addNotification(Notification* notification, const std::time_t & time)
    {
        NotificationOverlayContainer* container = new NotificationOverlayContainer;
        container->overlay = new NotificationOverlay(this, notification);
        container->notification = notification;
        container->time = time;
        std::string timeString = std::ctime(&time);
        timeString.erase(timeString.length()-1);
        std::ostringstream stream;
        stream << reinterpret_cast<unsigned long>(notification);
        const std::string& addressString = stream.str();
        container->name = "NotificationOverlay(" + timeString + ")&" + addressString;

        this->containers_.insert(container);
        this->overlays_[notification] = container;
        this->addElement(container->overlay);
        this->size_= this->size_+1;

        container->overlay->scroll(Vector2(0.0f,(1.1f*this->getFontSize())*(this->getSize()-1)));
    }

    /**
    @brief
        Removes a container from the queue.
    @param container
        A pointer to the container.
    @return
        Returns true if successful.
    */
    bool NotificationQueue::removeContainer(NotificationOverlayContainer* container)
    {
        if(this->size_ == 0) //!< You cannot remove anything if the queue is empty.
            return false;

        this->removeElement(container->overlay);
        this->containers_.erase(container);
        this->overlays_.erase(container->notification);
        container->overlay->destroy();
        delete container;
        this->size_= this->size_-1;

        return true;
    }

    /**
    @brief
        Clears the queue by removing all containers.
    */
    void NotificationQueue::clear(void)
    {
        std::multiset<NotificationOverlayContainer*, NotificationOverlayContainerCompare>::iterator it = this->containers_.begin();
        while(it != this->containers_.end())
        {
            this->removeContainer(*it);
            it = this->containers_.begin(); //TODO: Needed?
        }
    }

}
