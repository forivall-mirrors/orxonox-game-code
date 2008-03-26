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
 *      Some guy writing the example code from Ogre
 *
 */

/**
 @file
 @brief Implementation of a little Input handler that distributes everything
        coming from OIS.
 */

#include "OrxonoxStableHeaders.h"

#include "core/CoreIncludes.h"
#include "Orxonox.h"
#include "InputEventListener.h"
#include "InputHandler.h"

namespace orxonox
{
  /**
    @brief Constructor only resets the pointer values to 0.
  */
  InputHandler::InputHandler()
  {
    //RegisterObject(InputHandler);
    this->mouse_       = 0;
    this->keyboard_    = 0;
    this->inputSystem_ = 0;
  }

  /**
    @brief The one instance of the InputHandler is stored in this function.
    @return The pointer to the only instance of the InputHandler
  */
  InputHandler *InputHandler::getSingleton()
  {
    static InputHandler theOnlyInstance;
    return &theOnlyInstance;
  }

  /**
    @brief Creates the OIS::InputMananger, the keyboard and the mouse and
           assigns the key bindings.
    @param windowHnd The window handle of the render window
    @param windowWidth The width of the render window
    @param windowHeight The height of the render window
  */
  void InputHandler::initialise(size_t windowHnd, int windowWidth, int windowHeight)
  {
    if (!inputSystem_)
    {
      // Setup basic variables
      OIS::ParamList paramList;
      std::ostringstream windowHndStr;

      // Fill parameter list
      windowHndStr << (unsigned int)windowHnd;
      paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

#if defined OIS_LINUX_PLATFORM
      paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

      // Create inputsystem
      inputSystem_ = OIS::InputManager::createInputSystem(paramList);

      // If possible create a buffered keyboard
      if (inputSystem_->numKeyboards() > 0)
      {
        keyboard_ = static_cast<OIS::Keyboard*>(inputSystem_->createInputObject(OIS::OISKeyboard, true));
        keyboard_->setEventCallback(this);
      }

      // If possible create a buffered mouse
      if (inputSystem_->numMice() > 0 )
      {
        mouse_ = static_cast<OIS::Mouse*>(inputSystem_->createInputObject(OIS::OISMouse, true));
        mouse_->setEventCallback(this);

        // Set mouse region
        this->setWindowExtents(windowWidth, windowHeight);
      }
    }

    // load the key bindings
    InputEvent empty = {0, false, 0, 0, 0};
    for (int i = 0; i < this->numberOfKeys_; i++)
      this->bindingsKeyPressed_[i] = empty;

    //assign 'abort' to the escape key
    this->bindingsKeyPressed_[(int)OIS::KC_ESCAPE].id = 1;
  }

  /**
    @brief Destroys all the created input devices.
  */
  void InputHandler::destroy()
  {
    this->inputSystem_->destroyInputObject(this->mouse_);
    this->inputSystem_->destroyInputObject(this->keyboard_);
    OIS::InputManager::destroyInputSystem(this->inputSystem_);

    //TODO: If the InputHandler has been destroyed, how does it know?
  }

  /**
    @brief Updates the InputHandler
    @param dt Delta time
  */
  void InputHandler::tick(float dt)
  {
    // capture all the input. That calls the event handlers.
    if (mouse_)
      mouse_->capture();

    if (keyboard_)
      keyboard_->capture();
  }

  /**
    @brief Adjusts the mouse window metrics.
    This method has to be called every time the size of the window changes.
    @param width The new width of the render window
    @param height the new height of the render window
  */
  void InputHandler::setWindowExtents(int width, int height)
  {
    // Set mouse region (if window resizes, we should alter this to reflect as well)
    const OIS::MouseState &mouseState = mouse_->getMouseState();
    mouseState.width  = width;
    mouseState.height = height;
  }

  /**
    @brief Calls all the objects from classes that derive from InputEventListener.
    @param evt The input event that occured.
  */
  inline void InputHandler::callListeners(orxonox::InputEvent &evt)
  {
    for (Iterator<InputEventListener> it = ObjectList<InputEventListener>::start(); it; )
    {
      if (it->bActive_)
        (it++)->eventOccured(evt);
      else
        it++;
    }
  }

  /**
    @brief Event handler for the keyPressed Event.
    @param e Event information
  */
  bool InputHandler::keyPressed(const OIS::KeyEvent &e)
  {
    callListeners(this->bindingsKeyPressed_[(int)e.key]);
    return true;
  }

  /**
    @brief Event handler for the keyReleased Event.
    @param e Event information
  */
  bool InputHandler::keyReleased(const OIS::KeyEvent &e)
  {
    return true;
  }

  /**
    @brief Event handler for the mouseMoved Event.
    @param e Event information
  */
  bool InputHandler::mouseMoved(const OIS::MouseEvent &e)
  {
    return true;
  }

  /**
    @brief Event handler for the mousePressed Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool InputHandler::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    return true;
  }

  /**
    @brief Event handler for the mouseReleased Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool InputHandler::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    return true;
  }

}
