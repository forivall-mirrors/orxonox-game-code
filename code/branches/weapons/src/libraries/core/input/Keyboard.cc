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

#include "Keyboard.h"
#include "InputState.h"

namespace orxonox
{
    //! OIS event handler
    bool Keyboard::keyPressed(const OIS::KeyEvent& arg)
    {
        // update modifiers
        switch (arg.key)
        {
            case OIS::KC_RMENU:
            case OIS::KC_LMENU:
                modifiers_ |= KeyboardModifier::Alt;   // alt key
                break;
            case OIS::KC_RCONTROL:
            case OIS::KC_LCONTROL:
                modifiers_ |= KeyboardModifier::Ctrl;  // ctrl key
                break;
            case OIS::KC_RSHIFT:
            case OIS::KC_LSHIFT:
                modifiers_ |= KeyboardModifier::Shift; // shift key
                break;
            case OIS::KC_TAB:
                // Do not distribute the alt+tab event (messes with the operating system)
                if ((modifiers_ & KeyboardModifier::Alt) != 0)
                    return true;
            default:;
        }

        KeyEvent evt(static_cast<KeyCode::ByEnum>(arg.key), Keyboard::getKeyText(arg), 0);
        super::buttonPressed(evt);
        return true;
    }

    //! OIS event handler
    bool Keyboard::keyReleased(const OIS::KeyEvent& arg)
    {
        // update modifiers
        switch (arg.key)
        {
            case OIS::KC_RMENU:
            case OIS::KC_LMENU:
                modifiers_ &= ~KeyboardModifier::Alt;   // alt key
                break;
            case OIS::KC_RCONTROL:
            case OIS::KC_LCONTROL:
                modifiers_ &= ~KeyboardModifier::Ctrl;  // ctrl key
                break;
            case OIS::KC_RSHIFT:
            case OIS::KC_LSHIFT:
                modifiers_ &= ~KeyboardModifier::Shift; // shift key
                break;
            default:;
        }

        KeyEvent evt(static_cast<KeyCode::ByEnum>(arg.key), Keyboard::getKeyText(arg), 0);
        super::buttonReleased(evt);
        return true;
    }

    /// A map which returns the corresponding chars for some key codes
    unsigned int Keyboard::getKeyText(const OIS::KeyEvent& arg)
    {
        switch (arg.key)
        {
            case OIS::KC_NUMPAD0:     return static_cast<unsigned int>('0');
            case OIS::KC_NUMPAD1:     return static_cast<unsigned int>('1');
            case OIS::KC_NUMPAD2:     return static_cast<unsigned int>('2');
            case OIS::KC_NUMPAD3:     return static_cast<unsigned int>('3');
            case OIS::KC_NUMPAD4:     return static_cast<unsigned int>('4');
            case OIS::KC_NUMPAD5:     return static_cast<unsigned int>('5');
            case OIS::KC_NUMPAD6:     return static_cast<unsigned int>('6');
            case OIS::KC_NUMPAD7:     return static_cast<unsigned int>('7');
            case OIS::KC_NUMPAD8:     return static_cast<unsigned int>('8');
            case OIS::KC_NUMPAD9:     return static_cast<unsigned int>('9');
            case OIS::KC_DECIMAL:     return static_cast<unsigned int>('.');
            case OIS::KC_DIVIDE:      return static_cast<unsigned int>('/');
            case OIS::KC_NUMPADENTER: return static_cast<unsigned int>('\n');
            default:                  return arg.text;
        }
    }
}
