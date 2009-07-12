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
#include <boost/foreach.hpp>
#include "InputState.h"
#include "core/ConsoleCommand.h"

// HACK (include this as last, X11 seems to define some macros...)
#ifdef ORXONOX_PLATFORM_LINUX
#  include <ois/linux/LinuxMouse.h>
#endif

namespace orxonox
{
    Mouse::Mouse(unsigned int id, unsigned int windowWidth, unsigned int windowHeight)
        : super(id)
    {
        this->setMouseClipping(windowWidth, windowHeight);
        // HACK:
        instancePointer_s = this;
    }

    void Mouse::setMouseClipping(unsigned int width, unsigned int height)
    {
        oisDevice_->getMouseState().width  = width;
        oisDevice_->getMouseState().height = height;
    }

    unsigned int Mouse::getClippingWidth() const
    {
        return oisDevice_->getMouseState().width;
    }

    unsigned int Mouse::getClippingHeight() const
    {
        return oisDevice_->getMouseState().height;
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
            BOOST_FOREACH(InputState* state, inputStates_)
                state->mouseMoved(abs, rel, clippingSize);
        }

        // check for mouse scrolled event
        if (e.state.Z.rel != 0)
        {
            BOOST_FOREACH(InputState* state, inputStates_)
                state->mouseScrolled(e.state.Z.abs, e.state.Z.rel);
        }

        return true;
    }

    // ############################################################
    // #####                   ugly hacks                     #####
    // ##########                                        ##########
    // ############################################################

    // HACK:
    SetConsoleCommand(Mouse, setMouseClipping_s, false);
#ifdef ORXONOX_PLATFORM_LINUX
    SetConsoleCommand(Mouse, grabMouse, true);
    SetConsoleCommand(Mouse, ungrabMouse, true);
#endif
    Mouse* Mouse::instancePointer_s = NULL;

#ifdef ORXONOX_PLATFORM_LINUX
    void Mouse::grabMouse()
    {
        OIS::LinuxMouse* linuxMouse = dynamic_cast<OIS::LinuxMouse*>(instancePointer_s->oisDevice_);
        assert(linuxMouse);
        linuxMouse->grab(true);
    }

    void Mouse::ungrabMouse()
    {
        OIS::LinuxMouse* linuxMouse = dynamic_cast<OIS::LinuxMouse*>(instancePointer_s->oisDevice_);
        assert(linuxMouse);
        linuxMouse->grab(false);
    }
#endif
}
