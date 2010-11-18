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
        if (arg.key == OIS::KC_RMENU    || arg.key == OIS::KC_LMENU)
            modifiers_ |= KeyboardModifier::Alt;   // alt key
        if (arg.key == OIS::KC_RCONTROL || arg.key == OIS::KC_LCONTROL)
            modifiers_ |= KeyboardModifier::Ctrl;  // ctrl key
        if (arg.key == OIS::KC_RSHIFT   || arg.key == OIS::KC_LSHIFT)
            modifiers_ |= KeyboardModifier::Shift; // shift key

        // Do not distribute the alt+tab event (messes with the operating system)
        if ((modifiers_ & KeyboardModifier::Alt) != 0 && arg.key == OIS::KC_TAB)
            return true;

        KeyEvent evt(arg);
        super::buttonPressed(evt);
        return true;
    }

    //! OIS event handler
    bool Keyboard::keyReleased(const OIS::KeyEvent& arg)
    {
        // update modifiers
        if (arg.key == OIS::KC_RMENU    || arg.key == OIS::KC_LMENU)
            modifiers_ &= ~KeyboardModifier::Alt;   // alt key
        if (arg.key == OIS::KC_RCONTROL || arg.key == OIS::KC_LCONTROL)
            modifiers_ &= ~KeyboardModifier::Ctrl;  // ctrl key
        if (arg.key == OIS::KC_RSHIFT   || arg.key == OIS::KC_LSHIFT)
            modifiers_ &= ~KeyboardModifier::Shift; // shift key

        KeyEvent evt(arg);
        super::buttonReleased(evt);
        return true;
    }
}
