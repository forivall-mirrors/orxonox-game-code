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
 @brief Different implementations of input processing.
 */

#ifndef _InputHandler_H__
#define _InputHandler_H__

#include <OIS/OIS.h>

#include "CorePrereqs.h"

namespace orxonox
{
  /**
    @brief Captures mouse and keyboard input and distributes it to the
    GUI.
  */
  class _CoreExport InputHandlerGUI
        : public OIS::KeyListener, public OIS::MouseListener
  {
  public:
    InputHandlerGUI ();
    ~InputHandlerGUI();

  private:
    // input events
		bool mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseMoved   (const OIS::MouseEvent &arg);
		bool keyPressed   (const OIS::KeyEvent   &arg);
		bool keyReleased  (const OIS::KeyEvent   &arg);
  };


  /**
    @brief Captures mouse and keyboard input while in the actual game mode.
    Manages the key bindings.
  */
  class _CoreExport InputHandlerGame
        : public OIS::KeyListener, public OIS::MouseListener
  {
  public:
    InputHandlerGame ();
    ~InputHandlerGame();

  private:
    // input events
		bool mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseMoved   (const OIS::MouseEvent &arg);
		bool keyPressed   (const OIS::KeyEvent   &arg);
		bool keyReleased  (const OIS::KeyEvent   &arg);
  };

}

#endif /* _InputHandler_H__ */
