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
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "util/tribool.h"
#include "InputHandler.h"
#include "InputManager.h"
#include "JoyStickQuantityListener.h"

#define INPUT_STATE_PUSH_CALL(deviceIndex, functionName, ...) \
    InputManager::getInstance().pushCall(boost::function<void ()>(boost::bind(&InputHandler::functionName, handlers_[deviceIndex], __VA_ARGS__)))

namespace orxonox
{
    /**
    @brief
        InputStates allow you to customise the input event targets at runtime.

        The general idea is a stack: Every activated InputState will be pushed on
        that stack and only the top one gets the input events. This is done for
        every device (keyboard, mouse, all joy sticks) separately to allow
        for instance keyboard input capturing for the console while you can still
        steer a ship with the mouse.
        There are two exceptions to this behaviour though:
        - If an InputState is created with the 'Transparent' parameter on, the
          state will not prevent input from getting to the state below it on the stack.
          This can be useful for instance if you need to deploy input to multiple
          handlers: Simply create two InputStates and make the high priority one transparent.
        - If an InputState is created with the 'AlwaysGetsInput' parameter on, then
          the state will always receive input as long as it is activated.
        - Note: If you mark an InputState with both parameters on, then it will
          not influence only other InputState at all.

    @par Priorities
        Every InputState has a priority when on the stack, but mostly this
        priority is dynamic (InputStatePriority::Dynamic) which means that a state
        pushed onto the stack will simply have a higher priority than the top one.
        This behaviour really only applies to normal states that don't have
        a high priority (InputStatePriority::HighPriority). These 'special' ones
        are used for features like the KeyDetector or the console. Use with care!

    @par Exclusive/Non-Exclusive mouse Mode
        You can select a specific mouse mode that tells whether the application
        should have exclusive access to it or not.
        When in non-exclusive mode, you can move the mouse out of the window
        like with any other normal window (only for windowed mode!).
        The setting is dictated by the topmost InputState that gets mouse events.
    */
    class _CoreExport InputState : public JoyStickQuantityListener
    {
        friend class InputManager;

        //! Marks the index in the handler vector for the keyboard handler
        static const InputDeviceEnumerator::Value keyboardIndex_s = InputDeviceEnumerator::Keyboard;
        //! Marks the index in the handler vector for the mouse handler
        static const InputDeviceEnumerator::Value mouseIndex_s = InputDeviceEnumerator::Mouse;
        //! Marks the index in the handler vector for the first joy stick handler
        static const InputDeviceEnumerator::Value firstJoyStickIndex_s = InputDeviceEnumerator::FirstJoyStick;

    public:
        //! Sets the keyboard event handler (overwrites if there already was one!)
        void setKeyHandler     (InputHandler* handler)
            { handlers_[keyboardIndex_s] = handler; bExpired_ = true; }
        //! Sets the mouse event handler (overwrites if there already was one!)
        void setMouseHandler   (InputHandler* handler)
            { handlers_[mouseIndex_s]    = handler; bExpired_ = true; }
        /**
        @brief
            Sets the joy stick event handler for one specific joy stick (overwrites if there already was one!)
        @return
            Returns false if the specified device was not found
        */
        bool setJoyStickHandler(InputHandler* handler, unsigned int joyStick);
        //! Sets the joy stick event handler for all joy sticks (overwrites if there already was one!)
        void setJoyStickHandler(InputHandler* handler);
        //! Sets an InputHandler to be used for all devices
        void setHandler        (InputHandler* handler);

        void setMouseExclusive(tribool value) { exclusiveMouse_ = value; this->bExpired_ = true; }
        tribool getMouseExclusive() const { return exclusiveMouse_; }

        //! Returns the name of the state (which is unique!)
        const std::string& getName() const { return name_; }
        //! Returns the priority of the state (which is unique if != 0)
        int getPriority()            const { return priority_; }

        //! Tells whether there a handler installed for a specific device
        bool isInputDeviceEnabled(unsigned int device);

        //! Returns true if the handler situation has changed
        bool hasExpired()      { return this->bExpired_; }
        //! Call this if you have applied the changes resulting from changed handlers
        void resetExpiration() { bExpired_ = false; }

        //! Updates one specific device handler with deviceUpdated
        void update(float dt, unsigned int device);
        //! Updates all handlers with allDevicesUpdated
        void update(float dt);

        //! Generic function that distributes all 9 button events
        template <typename EventType, class ButtonTypeParam>
        void buttonEvent(unsigned int device, ButtonTypeParam button);

        //! Event handler
        void mouseMoved(IntVector2 abs, IntVector2 rel, IntVector2 clippingSize);
        //! Event handler
        void mouseScrolled(int abs, int rel);
        //! Event handler
        void joyStickAxisMoved(unsigned int device, unsigned int axis, float value);

        // Functors
        //! Called when the state is being activated (even if it doesn't get any events afterwards!)
        void entered();
        //! Called upon deactivation of the state
        void left();
        //! Sets a functor to be called upon activation of the state
        void setEnterFunctor(const FunctorPtr& functor) { this->enterFunctor_ = functor; }
        //! Sets a functor to be called upon deactivation of the state
        void setLeaveFunctor(const FunctorPtr& functor) { this->leaveFunctor_ = functor; }

    private:
        InputState(const std::string& name, bool bAlwaysGetsInput, bool bTransparent, InputStatePriority priority);
        ~InputState() { }

        void JoyStickQuantityChanged(const std::vector<JoyStick*>& joyStickList);

        //! Sets the priority (only to be used by the InputManager!)
        void setPriority(int priority) { priority_ = priority; }

        const std::string           name_;                  //!< Name of the state
        const bool                  bAlwaysGetsInput_;      //!< See class declaration for explanation
        const bool                  bTransparent_;          //!< See class declaration for explanation
        tribool                     exclusiveMouse_;        //!< See class declaration for explanation
        int                         priority_;              //!< Current priority (might change)
        bool                        bExpired_;              //!< See hasExpired()
        std::vector<InputHandler*>  handlers_;              //!< Vector with all handlers where the index is the device ID
        //! Handler to be used for all joy sticks (needs to be saved in case another joy stick gets attached)
        InputHandler*               joyStickHandlerAll_;
        FunctorPtr                  enterFunctor_;          //!< Functor to be executed on enter
        FunctorPtr                  leaveFunctor_;          //!< Functor to be executed on leave
    };

    ORX_FORCEINLINE void InputState::update(float dt)
    {
        for (unsigned int i = 0; i < handlers_.size(); ++i)
            if (handlers_[i] != NULL)
                INPUT_STATE_PUSH_CALL(i, allDevicesUpdated, dt);
    }

    ORX_FORCEINLINE void InputState::update(float dt, unsigned int device)
    {
        switch (device)
        {
        case InputDeviceEnumerator::Keyboard:
            if (handlers_[keyboardIndex_s] != NULL)
                INPUT_STATE_PUSH_CALL(keyboardIndex_s, keyboardUpdated, dt);
            break;

        case InputDeviceEnumerator::Mouse:
            if (handlers_[mouseIndex_s] != NULL)
                INPUT_STATE_PUSH_CALL(mouseIndex_s, mouseUpdated, dt);
            break;

        default: // joy sticks
            if (handlers_[device] != NULL)
                INPUT_STATE_PUSH_CALL(device, joyStickUpdated, device - firstJoyStickIndex_s, dt);
            break;
        }
    }

    template <typename EventType, class ButtonTypeParam>
    ORX_FORCEINLINE void InputState::buttonEvent(unsigned int device, ButtonTypeParam button)
    {
        assert(device < handlers_.size());
        if (handlers_[device] != NULL)
        {
            // We have to store the function pointer to tell the compiler about its actual type because of overloading
            void (InputHandler::*function)(unsigned int, ButtonTypeParam, EventType) = &InputHandler::buttonEvent<ButtonTypeParam>;
            InputManager::getInstance().pushCall(boost::function<void ()>(boost::bind(function, handlers_[device], device, button, EventType())));
        }
    }

    ORX_FORCEINLINE void InputState::mouseMoved(IntVector2 abs, IntVector2 rel, IntVector2 clippingSize)
    {
        if (handlers_[mouseIndex_s] != NULL)
            INPUT_STATE_PUSH_CALL(mouseIndex_s, mouseMoved, abs, rel, clippingSize);
    }

    ORX_FORCEINLINE void InputState::mouseScrolled(int abs, int rel)
    {
        if (handlers_[mouseIndex_s] != NULL)
            INPUT_STATE_PUSH_CALL(mouseIndex_s, mouseScrolled, abs, rel);
    }

    ORX_FORCEINLINE void InputState::joyStickAxisMoved(unsigned int device, unsigned int axis, float value)
    {
        assert(device < handlers_.size());
        if (handlers_[device] != NULL)
            INPUT_STATE_PUSH_CALL(device, axisMoved, device - firstJoyStickIndex_s, axis, value);
    }
}

#endif /* _InputState_H__ */
