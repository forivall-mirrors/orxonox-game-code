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

/**
 @file
 @brief Different definitions of input processing.
 */

#ifndef _InputHandler_H__
#define _InputHandler_H__

#include "CorePrereqs.h"

#include <string>
#include <OIS/OIS.h>

#include "InputEvent.h"

namespace orxonox
{
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

    bool loadBindings();

  private:
    // input events
		bool mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseMoved   (const OIS::MouseEvent &arg);
		bool keyPressed   (const OIS::KeyEvent   &arg);
		bool keyReleased  (const OIS::KeyEvent   &arg);

    // temporary hack
    void callListeners(InputEvent &evt);

    /** denotes the maximum number of different keys there are in OIS.
        256 should be ok since the highest number in the enum is 237. */
    static const int numberOfKeys_s = 256;
    //! Array of input events for every pressed key
    std::string bindingsKeyPressed_[numberOfKeys_s];
    //! Array of input events for every released key
    std::string bindingsKeyReleased_[numberOfKeys_s];

    /** denotes the maximum number of different buttons there are in OIS.
        16 should be ok since the highest number in the enum is 7. */
    static const int numberOfButtons_s = 16;
    //! Array of input events for every pressed key
    std::string bindingsButtonPressed_[numberOfButtons_s];
    //! Array of input events for every released key
    std::string bindingsButtonReleased_[numberOfButtons_s];

  };


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

}

#endif /* _InputHandler_H__ */
