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

#ifndef _Core_Keyboard_H__
#define _Core_Keyboard_H__

#include "InputPrereqs.h"
#include "InputDevice.h"

namespace orxonox
{
    /**
    @brief
        Wraps around an OIS::Mouse and forwards the input events to
        a list of input states.

        It also saves the state of the keyboard modifiers (like shift, etc.)
    */
    class _CoreExport Keyboard
        : public InputDeviceTemplated<KeyboardTraits>
        , public OIS::KeyListener
    {
        friend InputDeviceTemplated<KeyboardTraits>;
        //! Super class alias
        typedef InputDeviceTemplated<KeyboardTraits> super;

    public:
        Keyboard(unsigned int id) : super(id), modifiers_(0) { }
        ~Keyboard() { }

    private:
        // TODO: Do we need to reset the modifiers?
        void clearBuffersImpl() { }
        //! Translates the KeyHandle to a KeyEvent
        KeyEvent& getButtonEventArg(KeyEvent& button) { button.setModifiers(modifiers_); return button; }

        bool keyPressed(const OIS::KeyEvent& arg);
        bool keyReleased(const OIS::KeyEvent& arg);

        //! Bit mask representing keyboard modifiers
        int modifiers_;
    };
}

#endif /* _Core_Keyboard_H__ */
