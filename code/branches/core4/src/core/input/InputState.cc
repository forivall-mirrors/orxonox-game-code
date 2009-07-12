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

#include "InputState.h"
#include "core/Functor.h"

namespace orxonox
{
    InputState::InputState()
        : priority_(0)
        , bAlwaysGetsInput_(false)
        , bTransparent_(false)
        , bExpired_(true)
        , handlers_(2)
        , joyStickHandlerAll_(0)
        , enterFunctor_(0)
        , leaveFunctor_(0)
    {
    }

    bool InputState::isInputDeviceEnabled(unsigned int device)
    {
        if (device < handlers_.size())
            return handlers_[device] != NULL;
        else
            return false;
    }

    void InputState::JoyStickQuantityChanged(unsigned int n)
    {
        unsigned int oldSize = handlers_.size();
        handlers_.resize(InputDeviceEnumerator::FirstJoyStick + n, NULL);

        for (unsigned int i = oldSize; i < handlers_.size(); ++i)
            handlers_[i] = joyStickHandlerAll_;

        bExpired_ = true;
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
    bool InputState::setJoyStickHandler(InputHandler* handler, unsigned int joyStick)
    {
        unsigned device = joyStick + firstJoyStickIndex_s;
        if (joyStick >= handlers_.size() - device)
            return false;

        handlers_[device] = handler;
        bExpired_ = true;
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
    bool InputState::setJoyStickHandler(InputHandler* handler)
    {
        joyStickHandlerAll_ = handler;
        for (unsigned int i = firstJoyStickIndex_s; i < handlers_.size(); ++i)
            handlers_[i] = handler;
        bExpired_ = true;
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
    bool InputState::setHandler(InputHandler* handler)
    {
        setKeyHandler(handler);
        setMouseHandler(handler);
        return setJoyStickHandler(handler);
    }

    void InputState::entered()
    {
        if (enterFunctor_)
            (*enterFunctor_)();
            
    }

    void InputState::left()
    {
        if (leaveFunctor_)
            (*leaveFunctor_)();
    }
}
