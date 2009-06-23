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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
@file
@brief
    Implementation of the different input handlers.
*/

#include "KeyDetector.h"

#include "util/Debug.h"
#include "core/CoreIncludes.h"
#include "Button.h"

namespace orxonox
{
    /**
    @brief
        Constructor
    */
    KeyDetector::KeyDetector()
    {
        RegisterObject(KeyDetector);
    }

    /**
    @brief
        Destructor
    */
    KeyDetector::~KeyDetector()
    {
    }

    /**
    @brief
        Assigns all the buttons 'command' plus the button's name.
    */
    void KeyDetector::setCallbackCommand(const std::string& command)
    {
        callbackCommand_ = command;
        clearBindings();
        for (std::map<std::string, Button*>::const_iterator it = allButtons_.begin(); it != allButtons_.end(); ++it)
        {
            it->second->bindingString_ = callbackCommand_ + it->second->name_;
            it->second->parse();
        }
    }

    void KeyDetector::JoyStickDeviceNumberChanged(unsigned int value)
    {
        KeyBinder::JoyStickDeviceNumberChanged(value);
        setCallbackCommand(callbackCommand_);
    }
}
