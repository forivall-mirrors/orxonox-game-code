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
    IM_UNINIT   = 3,
  };

  /**
    @brief Captures and distributes mouse and keyboard input.
    It resolves the key bindings to InputEvents which can be heard by
    implementing the InputEventListener interface.
  */
  class _CoreExport InputManager
        : public Tickable
  {
  public:
    bool initialise(size_t windowHnd, int windowWidth, int windowHeight);
    void destroyDevices();
    void tick(float dt);
    void setWindowExtents(int width, int height);
    void setInputMode(InputMode mode);
    InputMode getInputMode();

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

    OIS::InputManager *inputSystem_;    //!< OIS input manager
    OIS::Keyboard     *keyboard_;       //!< OIS mouse
    OIS::Mouse        *mouse_;          //!< OIS keyboard

    InputMode          currentMode_;    //!< Input mode currently used
    InputMode          setMode_;        //!< Input mode that has been set lately
    InputHandlerGUI   *handlerGUI_;     //!< Handles the input if in GUI mode
    // FIXME: insert the InputBuffer once merged with core2
    InputHandlerGUI   *handlerBuffer_;  //!< Handles the input if in Buffer mode
    InputHandlerGame  *handlerGame_;    //!< Handles the input if in Game mode

    //! Pointer to the instance of the singleton
    static InputManager *singletonRef_s;
  };
}

#endif /* _InputManager_H__ */
