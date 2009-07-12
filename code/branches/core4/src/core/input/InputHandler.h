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
    Declarations of various interface classes for the input management.
*/

#ifndef _InputHandler_H__
#define _InputHandler_H__

#include "InputPrereqs.h"
#include "util/Math.h"

namespace orxonox
{
    /**
    @brief
    */
    class _CoreExport InputHandler
    {
    public:
        virtual ~InputHandler() { }

        template<class T> void buttonEvent(unsigned int device, const T& button, ButtonEvent::TPress)
            { this->buttonPressed(button); }
        template<class T> void buttonEvent(unsigned int device, const T& button, ButtonEvent::TRelease)
            { this->buttonReleased(button); }
        template<class T> void buttonEvent(unsigned int device, const T& button, ButtonEvent::THold)
            { this->buttonHeld(button); }
        void buttonEvent(unsigned int device, JoyStickButtonCode::ByEnum button, ButtonEvent::TPress)
            { this->buttonPressed(device - InputDeviceEnumerator::FirstJoyStick, button); }
        void buttonEvent(unsigned int device, JoyStickButtonCode::ByEnum button, ButtonEvent::TRelease)
            { this->buttonReleased(device - InputDeviceEnumerator::FirstJoyStick, button); }
        void buttonEvent(unsigned int device, JoyStickButtonCode::ByEnum button, ButtonEvent::THold)
            { this->buttonHeld(device - InputDeviceEnumerator::FirstJoyStick, button); }

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

        static InputHandler EMPTY;
    };
}

#endif /* _InputHandler_H__ */
