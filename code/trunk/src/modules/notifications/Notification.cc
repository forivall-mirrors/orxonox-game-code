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
#include "network/NetworkFunction.h"
#include "NotificationManager.h"

namespace orxonox
{

    CreateUnloadableFactory(Notification);

    registerMemberNetworkFunction(Notification, sendHelper);

    /**
    @brief
        Default constructor. Initializes the object.
    @param creator
        The object that created this Notification 
    */
    Notification::Notification(BaseObject* creator) : BaseObject(creator), Synchronisable(creator)
    {
        RegisterObject(Notification);
        this->initialize();
        this->registerVariables();
    }

    /**
    @brief
        Constructor. Creates a Notification with the input message.
    @param creator
        The creator.
    @param message
        The message of the Notification.
    */
    Notification::Notification(BaseObject* creator, const std::string & message) : BaseObject(creator), Synchronisable(creator)
    {
        RegisterObject(Notification);
        this->initialize();
        this->message_ = message;
        this->registerVariables();
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

    void Notification::registerVariables(void)
    {
        registerVariable(this->message_);
        registerVariable(this->sender_);
        registerVariable(this->sent_);
    }

    /**
    @brief
        Sends the Notification to the Notificationmanager, which then in turn distributes it to the different NotificationQueues.
    @param clientID
        The id of the client that this Notification is sent to.
    @param sender
        The sender the Notification was sent by. Used by the NotificationManager to distributes the notification to the correct NotificationQueues.
    @return
        Returns true if successful.
    */
    bool Notification::send(unsigned int clientId, const std::string & sender = NotificationManager::NONE)
    {

        if(GameMode::isStandalone())
        {
            this->sendHelper(sender);
        }
        else
        {
            callMemberNetworkFunction(Notification, sendHelper, this->getObjectID(), clientId, sender);
        }

        return true;
    }

    bool Notification::sendHelper(const std::string& sender)
    {
        if(this->isSent()) //TODO: Needed?
            return false;

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
