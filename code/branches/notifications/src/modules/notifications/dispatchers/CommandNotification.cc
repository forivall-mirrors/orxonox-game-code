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
    @brief Implementation of the CommandNotification class.
*/

#include "CommandNotification.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/input/KeyBinderManager.h"
#include "core/input/KeyBinder.h"

#include <sstream>

namespace orxonox {

    CreateFactory(CommandNotification);

    /**
    @brief
        Default Constructor. Registers the object and initializes variables.
    */
    CommandNotification::CommandNotification(BaseObject* creator) : NotificationDispatcher(creator)
    {
        RegisterObject(CommandNotification);

        this->setSender("commandNotification");
    }

    /**
    @brief
        Destructor.
    */
    CommandNotification::~CommandNotification()
    {
        
    }

    /**
    @brief
        Method for creating a NotificationDispatcher object through XML.
    */
    void CommandNotification::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(CommandNotification, XMLPort, xmlelement, mode);

        XMLPortParam(CommandNotification, "command", setCommand, getCommand, xmlelement, mode);
        XMLPortParam(CommandNotification, "preMessage", setPreMessage, getPreMessage, xmlelement, mode);
        XMLPortParam(CommandNotification, "postMessage", setPostMessage, getPostMessage, xmlelement, mode);
    }

    /**
    @brief
        Composes the Notification of the preMessage the name of the key the command is mapped to and the postMessage.
    @return
        Returns the message to be sent in the Notification.
    */
    const std::string& CommandNotification::createNotificationMessage(void)
    {
        std::stringstream stream;
        stream << this->getPreMessage();
        //TODO: Add niceifyer.
        stream << KeyBinderManager::getInstance().getCurrent()->getBinding(this->getCommand());
        stream << this->getPostMessage();
        std::string* message = new std::string(stream.str());
        return *message;
    }
    
}
