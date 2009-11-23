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

#ifndef _Core_Mouse_H__
#define _Core_Mouse_H__

#include "InputPrereqs.h"

#include "InputDevice.h"
#include "core/WindowEventListener.h"

namespace orxonox
{
    //! Template parameter collection for the base class
    struct MouseTraits
    {
        typedef Mouse DeviceClass;
        typedef OIS::Mouse OISDeviceClass;
        typedef MouseButtonCode::ByEnum ButtonType;
        typedef MouseButtonCode::ByEnum ButtonTypeParam;
        static const OIS::Type OISDeviceValue = OIS::OISMouse;
    };

    /**
    @brief
        Wraps around an OIS::Mouse and forwards the input events to
        a list of input states.
    */
    class _CoreExport Mouse
        : public InputDeviceTemplated<MouseTraits>
        , public OIS::MouseListener
        , public WindowEventListener
    {
        friend class InputDeviceTemplated<MouseTraits>;
        //! Super class alias
        typedef InputDeviceTemplated<MouseTraits> super;

    public:
        //! Only sets the clipping size. Initialising is done in the base class.
        Mouse(unsigned int id, OIS::InputManager* oisInputManager);
        ~Mouse() { }

#ifdef ORXONOX_PLATFORM_LINUX
        // TODO: Make this a feature rather than a hack
        void grab();
        void ungrab();
#endif

    private:
        //! OIS event handler
        bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
        {
            super::buttonPressed(static_cast<MouseButtonCode::ByEnum>(id));
            return true;
        }

        //! OIS event handler
        bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
        {
            super::buttonReleased(static_cast<MouseButtonCode::ByEnum>(id));
            return true;
        }

        bool mouseMoved(const OIS::MouseEvent &arg);

        void windowResized(unsigned int newWidth, unsigned int newHeight);

        // Returns the class name as string
        static std::string getClassNameImpl() { return "Mouse"; }
    };
}

#endif /* _Core_Mouse_H__ */
