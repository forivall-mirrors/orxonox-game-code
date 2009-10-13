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

#include "Mouse.h"

#include <ois/OISMouse.h>
#include "core/ConsoleCommand.h"
#include "core/CoreIncludes.h"
#include "InputState.h"

#ifdef ORXONOX_PLATFORM_LINUX
// include this as last, X11 seems to define some macros...
#include <ois/linux/LinuxMouse.h>
#endif

namespace orxonox
{
    Mouse::Mouse(unsigned int id, OIS::InputManager* oisInputManager)
        : super(id, oisInputManager)
    {
        RegisterRootObject(Mouse);
        this->windowResized(this->getWindowWidth(), this->getWindowHeight());

#ifdef ORXONOX_PLATFORM_LINUX
        // Mouse grab console command
        this->getIdentifier()->addConsoleCommand(createConsoleCommand(createFunctor(&Mouse::grab, this), "grab"), false);
        // Mouse ungrab console command
        this->getIdentifier()->addConsoleCommand(createConsoleCommand(createFunctor(&Mouse::ungrab, this), "ungrab"), false);
#endif
    }

    //! OIS event handler
    bool Mouse::mouseMoved(const OIS::MouseEvent &e)
    {
        // check for actual moved event
        if (e.state.X.rel != 0 || e.state.Y.rel != 0)
        {
            IntVector2 abs(e.state.X.abs, e.state.Y.abs);
            IntVector2 rel(e.state.X.rel, e.state.Y.rel);
            IntVector2 clippingSize(e.state.width, e.state.height);
            for (unsigned int i = 0; i < inputStates_.size(); ++i)
                inputStates_[i]->mouseMoved(abs, rel, clippingSize);
        }

        // check for mouse scrolled event
        if (e.state.Z.rel != 0)
        {
            for (unsigned int i = 0; i < inputStates_.size(); ++i)
                inputStates_[i]->mouseScrolled(e.state.Z.abs, e.state.Z.rel);
        }

        return true;
    }

    void Mouse::windowResized(unsigned int newWidth, unsigned int newHeight)
    {
        oisDevice_->getMouseState().width  = newWidth;
        oisDevice_->getMouseState().height = newHeight;
    }

#ifdef ORXONOX_PLATFORM_LINUX
    void Mouse::grab()
    {
        OIS::LinuxMouse* linuxMouse = dynamic_cast<OIS::LinuxMouse*>(oisDevice_);
        assert(linuxMouse);
        linuxMouse->grab(true);
    }

    void Mouse::ungrab()
    {
        OIS::LinuxMouse* linuxMouse = dynamic_cast<OIS::LinuxMouse*>(oisDevice_);
        assert(linuxMouse);
        linuxMouse->grab(false);
    }
#endif
}
