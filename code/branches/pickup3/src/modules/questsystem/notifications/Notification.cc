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
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See thes
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
    @brief Implementation of the Notification class.
*/

#include "Notification.h"

#include "core/CoreIncludes.h"
#include "NotificationManager.h"

namespace orxonox
{

    /**
    @brief
        Default constructor. Initializes the object.
    */
    Notification::Notification(BaseObject* creator) : BaseObject(creator)
    {
        RegisterObject(Notification);
        this->initialize();
    }

    /**
    @brief
        Constructor. Creates a Notification with the input message.
    @param message
        The message of the Notification.
    */
    Notification::Notification(const std::string & message) : BaseObject(NULL)
    {
        this->message_ = message;
    }

    /**
    @brief
        Destructor.
    */
    Notification::~Notification()
    {
    }

    /**
    @brief
        Registers the object and sets some default values.
    */
    void Notification::initialize(void)
    {
        this->message_.clear();
        this->sender_ = NotificationManager::NONE;
        this->sent_ = false;
    }

    /**
    @brief
        Sends the Notification to the Notificationmanager, with sender NetificationManager::NONE.
    @return
        Returns true if successful.
    */
    bool Notification::send(void)
    {
        return this->send(NotificationManager::NONE);
    }

    /**
    @brief
        Sends the Notification to the Notificationmanager, which then in turn distributes it to the different NotificationQueues.
    @param sender
        The sender the Notification was sent by. Used by the NotificationManager to distributes the notification to the correct NotificationQueues.
    @return
        Returns true if successful.
    */
    bool Notification::send(const std::string & sender)
    {
        this->sender_ = sender;
        bool successful = NotificationManager::getInstance().registerNotification(this);
        if(!successful)
            return false;
        this->sent_ = true;

        COUT(3) << "Notification \"" << this->getMessage() << "\" sent." << std::endl;

        return true;
    }

    /**
    @brief
        Sets the message of the notification.
    @param message
        The message to be set.
    @return
        Returns true if successful.
    */
    bool Notification::setMessage(const std::string & message)
    {
        if(this->isSent()) //!< The message cannot be changed if the message has already been sent.
            return false;
        this->message_ = message;
        return true;
    }

}
