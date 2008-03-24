/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

/**
 @file
 @brief Implementation of a little Input handler that distributes everything
        coming from OIS.
 */

#ifndef _InputHandler_H__
#define _InputHandler_H__

#include <OIS/OISPrereqs.h>
#include <OIS/OISMouse.h>
#include <OIS/OISKeyboard.h>

#include "OrxonoxPrereqs.h"
//#include "core/OrxonoxClass.h"
#include "core/Tickable.h"

namespace orxonox
{
  class _OrxonoxExport InputHandler
        : public Tickable,
          public OIS::KeyListener, public OIS::MouseListener
  {
  public:

    void initialise(size_t windowHnd, int windowWidth, int windowHeight);
    void tick(float dt);
    void setWindowExtents(int width, int height);

    OIS::Mouse    *getMouse()    { return this->mouse_   ; }
    OIS::Keyboard *getKeyboard() { return this->keyboard_; }

    // input events
		bool mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseMoved   (const OIS::MouseEvent &arg);
		bool keyPressed   (const OIS::KeyEvent   &arg);
		bool keyReleased  (const OIS::KeyEvent   &arg);

    static InputHandler* getSingleton();

    void setConfigValues();

  private:
    // don't mess with a Singleton
    InputHandler ();
    InputHandler (const InputHandler&) { }
    ~InputHandler() { }

    OIS::InputManager *inputSystem_;
    OIS::Keyboard     *keyboard_;
    OIS::Mouse        *mouse_;

    // Key bindings
    int codeFire_;
    OIS::KeyCode codeMoveForward_;

  };
}

#endif /* _InputHandler_H__ */