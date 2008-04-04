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

#ifndef _InputManager_H__
#define _InputManager_H__

#include <OIS/OIS.h>

#include "CorePrereqs.h"
#include "core/Tickable.h"
#include "InputEvent.h"

namespace orxonox
{
  /**
    @brief Designates the way input is handled currently.
    IM_GUI:      All the OIS input events are passed to CEGUI
    IM_KEYBOARD: Only keyboard input is captured and passed to the InputBuffer
    IM_INGAME:   Normal game mode. Key bindings and mouse are active.
  */
  enum InputMode
  {
    IM_GUI      = 0,
    IM_KEYBOARD = 1,
    IM_INGAME   = 2,
  };

  /**
    @brief Captures and distributes mouse and keyboard input.
    It resolves the key bindings to InputEvents which can be heard by
    implementing the InputEventListener interface.
  */
  class _CoreExport InputManager
        : public Tickable, public OIS::KeyListener, public OIS::MouseListener
  {
  public:
    bool initialise(size_t windowHnd, int windowWidth, int windowHeight);
    void destroyDevices();
    void tick(float dt);
    void setWindowExtents(int width, int height);

    // Temporary solutions. Will be removed soon!
    OIS::Mouse    *getMouse()    { return this->mouse_   ; }
    OIS::Keyboard *getKeyboard() { return this->keyboard_; }

    static InputManager* getSingleton();
    static void destroySingleton();

  private:
    // don't mess with a Singleton
    InputManager ();
    InputManager (const InputManager&);
    InputManager& operator=(const InputManager& instance);
    ~InputManager();

    void callListeners(InputEvent &evt);

    // input events
		bool mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseMoved   (const OIS::MouseEvent &arg);
		bool keyPressed   (const OIS::KeyEvent   &arg);
		bool keyReleased  (const OIS::KeyEvent   &arg);

    OIS::InputManager *inputSystem_;    //!< OIS input manager
    OIS::Keyboard     *keyboard_;       //!< OIS mouse
    OIS::Mouse        *mouse_;          //!< OIS keyboard

    /** denotes the maximum number of different keys there are in OIS.
        256 should be ok since the highest number in the enum is 237. */
    static const int numberOfKeys_ = 256;
    //! Array of input events for every pressed key
    InputEvent bindingsKeyPressed_[numberOfKeys_];
    //! Array of input events for every released key
    InputEvent bindingsKeyReleased_[numberOfKeys_];

    /** denotes the maximum number of different buttons there are in OIS.
        16 should be ok since the highest number in the enum is 7. */
    static const int numberOfButtons_ = 16;
    //! Array of input events for every pressed key
    InputEvent bindingsButtonPressed_[numberOfButtons_];
    //! Array of input events for every released key
    InputEvent bindingsButtonReleased_[numberOfButtons_];

    //! Pointer to the instance of the singleton
    static InputManager *singletonRef_s;
  };
}

#endif /* _InputManager_H__ */
