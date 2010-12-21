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

#ifndef _InputHandler_H__
#define _InputHandler_H__

#include "InputPrereqs.h"
#include "util/Math.h"

namespace orxonox
{
    namespace ButtonEvent
    {
        //! Helper enum to deploy events with the help of templates
        enum Value
        {
            Press,
            Release,
            Hold
        };

        //! Enables function overloading with integer values
        template <ButtonEvent::Value Event>
        struct EnumToType { };
        typedef EnumToType<Press>   TPress;
        typedef EnumToType<Release> TRelease;
        typedef EnumToType<Hold>    THold;
    }

    namespace KeyboardModifier
    {
        //! Keyboard modifiers (shift, ctrl and alt)
        enum Enum
        {
            Shift = 0x0000001,
            Ctrl  = 0x0000010,
            Alt   = 0x0000100
        };
    }

    //! Event argument for key events
    class _CoreExport KeyEvent
    {
    public:
        KeyEvent(const OIS::KeyEvent& evt)
            : key_(static_cast<KeyCode::ByEnum>(evt.key))
            , text_(evt.text)
            , modifiers_(0)
        { }
        KeyEvent(KeyCode::ByEnum key, unsigned int text, int modifiers)
            : key_(key)
            , text_(text)
            , modifiers_(modifiers)
        { }
        bool operator==(const KeyEvent& rhs) const
            { return rhs.key_ == key_; }
        bool operator!=(const KeyEvent& rhs) const
            { return rhs.key_ != key_; }
        void setModifiers(int modifiers)
            { modifiers_ = modifiers; }

        bool isModifierDown(KeyboardModifier::Enum modifier) const
            { return static_cast<KeyboardModifier::Enum>(modifier & modifiers_); }
        KeyCode::ByEnum getKeyCode() const
            { return key_; }
        unsigned int getText() const { return text_; }

    private:
        KeyCode::ByEnum key_;
        unsigned int text_;
        int modifiers_;
    };

    /**
    @brief
        Base class for all input handlers like KeyBinder, InputBuffer, etc.

        Derive from this class if you wish to receive input events.
        But keep in mind that this is pointless without first having an InputState.
    @note
        The definitions for the button events with the weird arguments are simply
        to avoid redundant code in the input devices.
    */
    class _CoreExport InputHandler
    {
    public:
        virtual ~InputHandler() { }

        template<class T> void buttonEvent(unsigned int device, T button, ButtonEvent::TPress)
            { this->buttonPressed(button); }
        template<class T> void buttonEvent(unsigned int device, T button, ButtonEvent::TRelease)
            { this->buttonReleased(button); }
        template<class T> void buttonEvent(unsigned int device, T button, ButtonEvent::THold)
            { this->buttonHeld(button); }

        virtual void buttonPressed (const KeyEvent& evt) { }
        virtual void buttonReleased(const KeyEvent& evt) { }
        virtual void buttonHeld    (const KeyEvent& evt) { }

        virtual void buttonPressed (MouseButtonCode::ByEnum button) { }
        virtual void buttonReleased(MouseButtonCode::ByEnum button) { }
        virtual void buttonHeld    (MouseButtonCode::ByEnum button) { }
        virtual void mouseMoved    (IntVector2 abs, IntVector2 rel, IntVector2 clippingSize) { }
        virtual void mouseScrolled (int abs, int rel) { }

        virtual void buttonPressed (unsigned int joyStick, JoyStickButtonCode::ByEnum button) { }
        virtual void buttonReleased(unsigned int joyStick, JoyStickButtonCode::ByEnum button) { }
        virtual void buttonHeld    (unsigned int joyStick, JoyStickButtonCode::ByEnum button) { }
        virtual void axisMoved     (unsigned int joyStick, unsigned int axis, float value){ }

        virtual void keyboardUpdated(float dt) { }
        virtual void mouseUpdated   (float dt) { }
        virtual void joyStickUpdated(unsigned int joyStick, float dt) { }

        virtual void allDevicesUpdated(float dt) { }

        //! Use this input handler if you want to occupy a device in an input state.
        static InputHandler EMPTY;
    };

    template<> inline void InputHandler::buttonEvent<JoyStickButtonCode::ByEnum>(unsigned int device, JoyStickButtonCode::ByEnum button, ButtonEvent::TPress)
        { this->buttonPressed(device - InputDeviceEnumerator::FirstJoyStick, button); }
    template<> inline void InputHandler::buttonEvent<JoyStickButtonCode::ByEnum>(unsigned int device, JoyStickButtonCode::ByEnum button, ButtonEvent::TRelease)
        { this->buttonReleased(device - InputDeviceEnumerator::FirstJoyStick, button); }
    template<> inline void InputHandler::buttonEvent<JoyStickButtonCode::ByEnum>(unsigned int device, JoyStickButtonCode::ByEnum button, ButtonEvent::THold)
        { this->buttonHeld(device - InputDeviceEnumerator::FirstJoyStick, button); }
}

#endif /* _InputHandler_H__ */
