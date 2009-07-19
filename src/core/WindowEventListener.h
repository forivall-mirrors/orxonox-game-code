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

#ifndef _WindowEventListener_H__
#define _WindowEventListener_H__

#include "CorePrereqs.h"
#include "OrxonoxClass.h"

namespace orxonox
{
    //! Interface for receiving window events like resize, moved and focusChanged
    class _CoreExport WindowEventListener : virtual public OrxonoxClass
    {
        friend class OgreWindowEventListener;

        protected:
            WindowEventListener();
            virtual ~WindowEventListener() { }

            //! Returns the current render window width
            unsigned int getWindowWidth() const { return windowWidth_s; }
            //! Returns the current render window height
            unsigned int getWindowHeight() const { return windowHeight_s; }

        private:
            //! Window has been moved
            virtual void windowMoved() { }

            //! Window has resized
            virtual void windowResized(unsigned int newWidth, unsigned int newHeight) { }

            //! Window has lost/gained focus
            virtual void windowFocusChanged() { }

            static void moveWindow();
            static void resizeWindow(unsigned int newWidth, unsigned int newHeight);
            static void changeWindowFocus();

            //! Static variable that holds the latest distributed information
            static unsigned int windowWidth_s;
            static unsigned int windowHeight_s;
    };
}

#endif /* _WindowEventListener_H__ */
