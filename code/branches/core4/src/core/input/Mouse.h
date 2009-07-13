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

namespace orxonox
{
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
    {
        friend class InputDeviceTemplated<MouseTraits>;
        //! Super class alias
        typedef InputDeviceTemplated<MouseTraits> super;

    public:
        Mouse(unsigned int id, unsigned int windowWidth, unsigned int windowHeight);
        ~Mouse() { }

        /**
        @brief
            Adjusts the mouse window metrics.

            This method has to be called every time the size of the window changes.
        */
        void setMouseClipping(unsigned int width, unsigned int height);
        unsigned int getClippingWidth() const;
        unsigned int getClippingHeight() const;

        // HACK!
        static void setMouseClipping_s(unsigned int width, unsigned int height)
            { instancePointer_s->setMouseClipping(width, height); }
        void setConfigValues() { }
#ifdef ORXONOX_PLATFORM_LINUX
        // HACK!
        static void grabMouse();
        static void ungrabMouse();
#endif

    private:
        // TODO: Do we need to reset the mouse position?
        void clearBuffersImpl() { }

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

        static std::string getClassNameImpl() { return "Mouse"; }

        // HACK:
        static Mouse* instancePointer_s;
    };
}

#endif /* _Core_Mouse_H__ */
