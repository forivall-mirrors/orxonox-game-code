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
#include "core/CoreIncludes.h"
#include "core/command/ConsoleCommandIncludes.h"
#include "InputState.h"

#ifdef ORXONOX_PLATFORM_LINUX
// include this as last, X11 seems to define some macros...
#include <ois/linux/LinuxMouse.h>
#undef Success
#endif

namespace orxonox
{
#ifdef ORXONOX_PLATFORM_LINUX
    static const std::string __CC_Mouse_name = "Mouse";
    static const std::string __CC_grab_name = "grab";
    static const std::string __CC_ungrab_name = "ungrab";

    SetConsoleCommand(__CC_Mouse_name, __CC_grab_name,   &Mouse::grab);
    SetConsoleCommand(__CC_Mouse_name, __CC_ungrab_name, &Mouse::ungrab);
#endif

    Mouse::Mouse(unsigned int id, OIS::InputManager* oisInputManager)
        : super(id, oisInputManager)
    {
        RegisterObject(Mouse);
        this->windowResized(this->getWindowWidth(), this->getWindowHeight());

#ifdef ORXONOX_PLATFORM_LINUX
        ModifyConsoleCommand(__CC_Mouse_name, __CC_grab_name).setObject(this);
        ModifyConsoleCommand(__CC_Mouse_name, __CC_ungrab_name).setObject(this);
#endif
    }

    Mouse::~Mouse()
    {
#ifdef ORXONOX_PLATFORM_LINUX
        ModifyConsoleCommand(__CC_Mouse_name, __CC_grab_name).setObject(0);
        ModifyConsoleCommand(__CC_Mouse_name, __CC_ungrab_name).setObject(0);
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
