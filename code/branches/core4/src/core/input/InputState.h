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

#ifndef _InputState_H__
#define _InputState_H__

#include "InputPrereqs.h"

#include <cassert>
#include <string>
#include <vector>

#include "InputHandler.h"
#include "JoyStickQuantityListener.h"

namespace orxonox
{
    class _CoreExport InputState : public JoyStickQuantityListener
    {
        friend class InputManager;

        static const InputDeviceEnumerator::Value keyboardIndex_s      = InputDeviceEnumerator::Keyboard;
        static const InputDeviceEnumerator::Value mouseIndex_s         = InputDeviceEnumerator::Mouse;
        static const InputDeviceEnumerator::Value firstJoyStickIndex_s = InputDeviceEnumerator::FirstJoyStick;

    public:
        void setKeyHandler     (InputHandler* handler)
            { handlers_[keyboardIndex_s] = handler; bExpired_ = true; }
        void setMouseHandler   (InputHandler* handler)
            { handlers_[mouseIndex_s]    = handler; bExpired_ = true; }
        bool setJoyStickHandler(InputHandler* handler, unsigned int joyStick);
        bool setJoyStickHandler(InputHandler* handler);
        bool setHandler        (InputHandler* handler);

        const std::string& getName() const { return name_; }
        int getPriority()            const { return priority_; }

        bool isInputDeviceEnabled(unsigned int device);

        bool hasExpired()      { return this->bExpired_; }
        void resetExpiration() { bExpired_ = false; }

        void update(float dt, unsigned int device);
        void update(float dt);

        template <typename EventType, class Traits>
        void buttonEvent(unsigned int device, const typename Traits::ButtonTypeParam button);

        void mouseMoved(IntVector2 abs, IntVector2 rel, IntVector2 clippingSize);
        void mouseScrolled(int abs, int rel);
        void joyStickAxisMoved(unsigned int device, unsigned int axis, float value);

        // Functors
        void entered();
        void left();
        void setEnterFunctor(Functor* functor) { this->enterFunctor_ = functor; }
        void setLeaveFunctor(Functor* functor) { this->leaveFunctor_ = functor; }

    private:
        InputState();
        ~InputState() { }

        void JoyStickQuantityChanged(unsigned int n);

        void setName(const std::string& name) { name_ = name; }
        void setPriority(int priority)        { priority_ = priority; }

        std::string                 name_;
        int                         priority_;
        bool                        bAlwaysGetsInput_;
        bool                        bTransparent_;
        bool                        bExpired_;
        std::vector<InputHandler*>  handlers_;
        InputHandler*               joyStickHandlerAll_;
        Functor*                    enterFunctor_;
        Functor*                    leaveFunctor_;
    };

    inline void InputState::update(float dt)
    {
        for (unsigned int i = 0; i < handlers_.size(); ++i)
            if (handlers_[i] != NULL)
                handlers_[i]->allDevicesUpdated(dt);
    }

    inline void InputState::update(float dt, unsigned int device)
    {
        switch (device)
        {
        case InputDeviceEnumerator::Keyboard:
            if (handlers_[keyboardIndex_s] != NULL)
                handlers_[keyboardIndex_s]->keyboardUpdated(dt);
            break;

        case InputDeviceEnumerator::Mouse:
            if (handlers_[mouseIndex_s] != NULL)
                handlers_[mouseIndex_s]->mouseUpdated(dt);
            break;

        default: // joy sticks
            if (handlers_[device] != NULL)
                handlers_[device]->joyStickUpdated(device - firstJoyStickIndex_s, dt);
            break;
        }
    }

    template <typename EventType, class Traits>
    inline void InputState::buttonEvent(unsigned int device, const typename Traits::ButtonTypeParam button)
    {
        assert(device < handlers_.size());
        if (handlers_[device] != NULL)
            handlers_[device]->buttonEvent(device, button, EventType());
    }

    inline void InputState::mouseMoved(IntVector2 abs, IntVector2 rel, IntVector2 clippingSize)
    {
        if (handlers_[mouseIndex_s] != NULL)
            handlers_[mouseIndex_s]->mouseMoved(abs, rel, clippingSize);
    }

    inline void InputState::mouseScrolled(int abs, int rel)
    {
        if (handlers_[mouseIndex_s] != NULL)
            handlers_[mouseIndex_s]->mouseScrolled(abs, rel);
    }

    inline void InputState::joyStickAxisMoved(unsigned int device, unsigned int axis, float value)
    {
        assert(device < handlers_.size());
        if (handlers_[device] != NULL)
            handlers_[device]->axisMoved(device - firstJoyStickIndex_s, axis, value);
    }
}

#endif /* _InputState_H__ */
