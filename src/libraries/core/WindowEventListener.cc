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

#include "WindowEventListener.h"
#include "CoreIncludes.h"

namespace orxonox
{
    unsigned int WindowEventListener::windowWidth_s  = 0;
    unsigned int WindowEventListener::windowHeight_s = 0;

    RegisterAbstractClass(WindowEventListener).inheritsFrom(Class(Listable));

    WindowEventListener::WindowEventListener()
    {
        RegisterObject(WindowEventListener);
    }

    //! Calls all registered objects
    /*static*/ void WindowEventListener::moveWindow()
    {
        for (ObjectList<WindowEventListener>::iterator it = ObjectList<WindowEventListener>::begin(); it; ++it)
            it->windowMoved();
    }

    //! Calls all registered objects and sets the static variables
    /*static*/ void WindowEventListener::resizeWindow(unsigned int newWidth, unsigned int newHeight)
    {
        windowWidth_s = newWidth;
        windowHeight_s = newHeight;
        for (ObjectList<WindowEventListener>::iterator it = ObjectList<WindowEventListener>::begin(); it; ++it)
            it->windowResized(newWidth, newHeight);
    }

    //! Calls all registered objects
    /*static*/ void WindowEventListener::changeWindowFocus(bool bFocus)
    {
        for (ObjectList<WindowEventListener>::iterator it = ObjectList<WindowEventListener>::begin(); it; ++it)
            it->windowFocusChanged(bFocus);
    }
}
