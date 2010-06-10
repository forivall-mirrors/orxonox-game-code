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
    @brief Implementation of the NotificationOverlay class.
*/

#include "NotificationOverlay.h"

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "Notification.h"
#include "NotificationQueue.h"

namespace orxonox
{

    /**
    @brief
        Constructor. Intializes the class.
    */
    NotificationOverlay::NotificationOverlay(BaseObject* creator) : OverlayText(creator)
    {
        RegisterObject(NotificationOverlay);
        this->initialize();
    }

    /**
    @brief
        Constructor. Initializes the class creates a graphical representation of the input Notification for the input Queue.
    @param queue
        A pointer to the queue the NotificatonOverlay belongs to.
    @param notification
        A pointer to the Notification represented by this overlay.
    @throws Argument
        Throws an Argument-Exception if either no Notification or no NotificationQueue were input.
    */
    NotificationOverlay::NotificationOverlay(NotificationQueue* queue, Notification* notification) : OverlayText(NULL)
    {
        this->initialize();

        if(notification == NULL || queue == NULL) //!> If either notification or queue are not given an Exception is thrown.
        {
            ThrowException(Argument, "There were NULL-Pointer arguments in NotificationOverlay creation.");
        }

        this->queue_ = queue;
        this->defineOverlay();

        this->processNotification(notification);
    }

    /**
    @brief
        Initializes and Registers the object.
    */
    void NotificationOverlay::initialize(void)
    {
        this->queue_ = NULL;
    }

    /**
    @brief
        Set some Overlay-specific values.
    */
    void NotificationOverlay::defineOverlay(void)
    {
        this->setFont(this->queue_->getFont());
        this->setTextSize(this->queue_->getFontSize());

        this->setPosition(this->queue_->getPosition());
    }

    /**
    @brief
        Destructor.
    */
    NotificationOverlay::~NotificationOverlay()
    {
    }

    /**
    @brief
        Processes the input notification, resp. sees to it. that the NotificationOverlay displays the Notification message.
    @param notification
        A pointer to the notification that should be processed.
    @return
        Returns true if successful.
    */
    bool NotificationOverlay::processNotification(Notification* notification)
    {
        if(notification == NULL)
            return false;
        this->setCaption(clipMessage(notification->getMessage()));
        this->notification_ = notification;
        return true;
    }

    /**
    @brief
        Clips the input message so that it meets the requirements for the maximal length of Notifications given by the NotificationQueue.
    */
    std::string NotificationOverlay::clipMessage(const std::string & message)
    {
        if(message.length() <= static_cast<unsigned int>(this->queue_->getNotificationLength())) //!< If the message is not too long.
            return message;
        return message.substr(0, this->queue_->getNotificationLength());
    }

}
