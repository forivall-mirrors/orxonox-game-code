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
    Implementation of the SimpleInputState class.
*/

#include "SimpleInputState.h"
#include "core/Executor.h"

namespace orxonox
{
    SimpleInputState::SimpleInputState()
        : keyHandler_(0)
        , mouseHandler_(0)
        , joyStickHandlerAll_(0)
    {
    }

    void SimpleInputState::numberOfJoySticksChanged(unsigned int n)
    {
        unsigned int oldSize = joyStickHandler_.size();
        joyStickHandler_.resize(n);

        if (n > oldSize)
        {
            // we have to add the handler in joyStickHandlerAll_ to the joyStickHandler_[>n]
            for (unsigned int i = oldSize; i < n; ++i)
            {
                joyStickHandler_[i] = joyStickHandlerAll_;
            }
        }
        update();
    }

    void SimpleInputState::keyPressed(const KeyEvent& evt)
    {
        if (keyHandler_)
            keyHandler_->keyPressed(evt);
    }

    /**
    @brief
        Adds a joy stick handler.
    @param handler
        Pointer to the handler object.
    @param joyStickID
        ID of the joy stick
    @return
        True if added, false otherwise.
    */
    bool SimpleInputState::setJoyStickHandler(JoyStickHandler* handler, unsigned int joyStickID)
    {
        if (joyStickID >= joyStickHandler_.size())
            return false;

        joyStickHandler_[joyStickID] = handler;
        update();
        return true;
    }

    /**
    @brief
        Adds a joy stick handler.
    @param handler
        Pointer to the handler object.
    @return
        True if added, false if handler already existed.
    */
    bool SimpleInputState::setJoyStickHandler(JoyStickHandler* handler)
    {
        if (handler == joyStickHandlerAll_)
            return false;

        joyStickHandlerAll_ = handler;
        for (unsigned int iJoyStick = 0; iJoyStick < joyStickHandler_.size(); ++iJoyStick)
            setJoyStickHandler(handler, iJoyStick);
        update();
        return true;
    }

    /**
    @brief
        Adds a handler of any kind. dynamic_cast determines to which list it is added.
    @param handler
        Pointer to the handler object.
    @return
        True if added, false if handler already existed.
    */
    bool SimpleInputState::setHandler(InputHandler* handler)
    {
        setKeyHandler(dynamic_cast<KeyHandler*>(handler));
        setMouseHandler(dynamic_cast<MouseHandler*>(handler));
        return setJoyStickHandler(dynamic_cast<JoyStickHandler*>(handler));
    }

    void SimpleInputState::update()
    {
        // we can use a set to have a list of unique pointers (an object can implement all 3 handlers)
        std::set<InputHandler*> tempSet;
        if (keyHandler_)
            tempSet.insert(keyHandler_);
        if (mouseHandler_)
            tempSet.insert(mouseHandler_);
        for (unsigned int iJoyStick = 0; iJoyStick < joyStickHandler_.size(); iJoyStick++)
            if (joyStickHandler_[iJoyStick])
                tempSet.insert(joyStickHandler_[iJoyStick]);

        // copy the content of the map back to the actual vector
        allHandlers_.clear();
        for (std::set<InputHandler*>::const_iterator itHandler = tempSet.begin();
            itHandler != tempSet.end(); itHandler++)
            allHandlers_.push_back(*itHandler);

        // update the deviceEnabled options
        setInputDeviceEnabled(InputDevice::Keyboard, (keyHandler_ != 0));
        setInputDeviceEnabled(InputDevice::Mouse, (mouseHandler_ != 0));
        for (unsigned int i = 0; i < joyStickHandler_.size(); ++i)
            setInputDeviceEnabled(2 + i, (joyStickHandler_[i] != 0));

        // inform InputManager that there might be changes in EMPTY_HANDLER situation
        bHandlersChanged_ = true;
    }

    void SimpleInputState::onEnter()
    {
        if (executorOnEnter_)
            (*executorOnEnter_)();
    }

    void SimpleInputState::onLeave()
    {
        if (executorOnLeave_)
            (*executorOnLeave_)();
    }
}
