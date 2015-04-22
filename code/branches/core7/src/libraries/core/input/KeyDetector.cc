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

#include "KeyDetector.h"

#include "util/ScopedSingletonManager.h"
#include "core/CoreIncludes.h"
#include "core/command/ConsoleCommandIncludes.h"
#include "Button.h"
#include "InputManager.h"
#include "InputState.h"

namespace orxonox
{
    ManageScopedSingleton(KeyDetector, ScopeID::Graphics, false);

    static const std::string __CC_KeyDetector_callback_name = "KeyDetectorKeyPressed";
    DeclareConsoleCommand(__CC_KeyDetector_callback_name, &prototype::void__string).hide();

    RegisterAbstractClass(KeyDetector).inheritsFrom<KeyBinder>();

    KeyDetector::KeyDetector()
        : KeyBinder("")
    {
        RegisterObject(KeyDetector);

        ModifyConsoleCommand(__CC_KeyDetector_callback_name).setFunction(&KeyDetector::callback, this);

        this->assignCommands();

        inputState_ = InputManager::getInstance().createInputState("detector", false, false, InputStatePriority::Detector);
        // Create a callback to avoid buttonHeld events after the key has been detected
        inputState_->setLeaveFunctor(createFunctor(&InputManager::clearBuffers, &InputManager::getInstance()));
        inputState_->setHandler(this);
    }

    KeyDetector::~KeyDetector()
    {
        inputState_->setHandler(NULL);
        InputManager::getInstance().destroyState("detector");
        ModifyConsoleCommand(__CC_KeyDetector_callback_name).resetFunction();
    }

    void KeyDetector::assignCommands()
    {
        // Assign every button/axis the same command, but with its name as argument
        for (std::map<std::string, Button*>::const_iterator it = allButtons_.begin(); it != allButtons_.end(); ++it)
            it->second->parse(__CC_KeyDetector_callback_name + ' ' + it->second->groupName_ + "." + it->second->name_);
    }

    void KeyDetector::callback(const std::string& name)
    {
        // Call the registered function
        if (this->callbackFunction_)
            (*this->callbackFunction_)(name);
    }

    void KeyDetector::JoyStickQuantityChanged(const std::vector<JoyStick*>& joyStickList)
    {
        KeyBinder::JoyStickQuantityChanged(joyStickList);
        this->assignCommands();
    }
}
