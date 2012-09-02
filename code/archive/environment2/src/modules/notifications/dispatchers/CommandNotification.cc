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
    @file CommandNotification.cc
    @brief Implementation of the CommandNotification class.
*/

#include "CommandNotification.h"

#include "core/CoreIncludes.h"
#include "core/XMLPort.h"
#include "core/input/KeyBinderManager.h"
#include "core/input/KeyBinder.h"
#include "util/SubString.h"

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
        this->registerVariables();
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
        Register some variables for synchronisation.
    */
    void CommandNotification::registerVariables(void)
    {
        registerVariable(this->command_, VariableDirection::ToClient);
        registerVariable(this->preMessage_, VariableDirection::ToClient);
        registerVariable(this->postMessage_, VariableDirection::ToClient);
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
        stream << this->bindingNiceifyer(KeyBinderManager::getInstance().getCurrent()->getBinding(this->getCommand()));
        stream << this->getPostMessage();
        return *(new std::string(stream.str()));
    }

    /**
    @brief
        Transforms the input binding into a human readable form.
    @param binding
        The binding to be transformed
    @return
        Returns a human readable version of the input binding.
    */
    //TODO: Move to KeyBinderManager...
    const std::string& CommandNotification::bindingNiceifyer(const std::string& binding)
    {
        SubString string = SubString(binding, ".");
        std::string name;
        std::string group;
        switch(string.size())
        {
            case 0:
                return binding;
            case 1:
                return binding;
            case 2:
                group = string[0];
            default:
                name = string.subSet(1, string.size()).join(".");
        }

        std::stringstream stream;
        if(group.compare("Keys") == 0)
            stream << "Key " << name.substr(3);
        else if(group.compare("MouseButtons") == 0)
            stream << "Mouse " << name;
        else if(group.compare("JoyStickButtons") == 0)
            stream << "Joystick " << name;
        else if(group.compare("JoyStickAxes") == 0)
            stream << "Joystick Axis" << name.substr(5, 6) << name.substr(name.find("Axis")+6);
        else if(group.compare("MouseAxes") == 0)
            stream << "Mouse " << name.substr(1,3) << " " << name.substr(0, 1) << "-Axis";
        else
            return binding;

        return *(new std::string(stream.str()));
    }
    
}
