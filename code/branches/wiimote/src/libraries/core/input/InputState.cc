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
#include "core/command/Functor.h"

namespace orxonox
{
    //! Sets priority of it's a high priority and resizes the handler list
    InputState::InputState(const std::string& name, bool bAlwaysGetsInput, bool bTransparent, InputStatePriority priority)
        : name_(name)
        , bAlwaysGetsInput_(bAlwaysGetsInput)
        , bTransparent_(bTransparent)
        , exclusiveMouse_(dontcare)
        , bExpired_(true)
        , handlers_(2)
        , joyStickHandlerAll_(0)
        , enterFunctor_(0)
        , leaveFunctor_(0)
    {
        if (priority >= InputStatePriority::HighPriority || priority == InputStatePriority::Empty)
            priority_ = priority;
        else
            priority_ = 0;

        handlers_.resize(InputDeviceEnumerator::FirstJoyStick + this->getJoyStickList().size(), NULL);
    }

    bool InputState::isInputDeviceEnabled(unsigned int device)
    {
        if (device < handlers_.size())
        {	orxout() << "Test we made it into the if clause" << std::endl;
            return handlers_[device] != NULL;
        }
        else
            return false;
    }

    //! Called by JoyStickQuantityListener upon joy stick adding/removal
    void InputState::JoyStickQuantityChanged(const std::vector<JoyStick*>& joyStickList)
    {
        unsigned int oldSize = handlers_.size();
        handlers_.resize(InputDeviceEnumerator::FirstJoyStick + joyStickList.size(), NULL);

        for (unsigned int i = oldSize; i < handlers_.size(); ++i)
            handlers_[i] = joyStickHandlerAll_;

        bExpired_ = true;
    }

    bool InputState::setJoyStickHandler(InputHandler* handler, unsigned int joyStick)
    {
        unsigned device = joyStick + firstJoyStickIndex_s;
        if (joyStick >= handlers_.size() - device)
            return false;

        handlers_[device] = handler;
        bExpired_ = true;
        return true;
    }

    void InputState::setJoyStickHandler(InputHandler* handler)
    {
        joyStickHandlerAll_ = handler;
        for (unsigned int i = firstJoyStickIndex_s; i < handlers_.size(); ++i)
            handlers_[i] = handler;
        bExpired_ = true;
    }

    void InputState::setHandler(InputHandler* handler)
    {
        setKeyHandler(handler);
        setMouseHandler(handler);
        setJoyStickHandler(handler);
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
