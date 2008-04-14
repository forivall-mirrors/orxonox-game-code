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
 @brief Implementation of a little Input handler that distributes everything
        coming from OIS.
 */

#include "core/CoreIncludes.h"
#include "core/Debug.h"
#include "InputEventListener.h"
#include "InputHandler.h"
#include "InputManager.h"

namespace orxonox
{
  /**
    @brief The reference to the singleton
  */
  //InputManager* InputManager::singletonRef_s = 0;

  /**
    @brief Constructor only resets the pointer values to 0.
  */
  InputManager::InputManager() :
      inputSystem_(0), keyboard_(0), mouse_(0),
      currentMode_(IM_UNINIT), setMode_(IM_UNINIT),
      handlerGUI_(0), handlerBuffer_(0), handlerGame_(0)
  {
  }

  /**
    @brief Destructor only called at the end of the program
  */
  InputManager::~InputManager()
  {
    this->destroy();
  }

  /**
    @brief The one instance of the InputManager is stored in this function.
    @return A reference to the only instance of the InputManager
  */
  InputManager& InputManager::getSingleton()
  {
    static InputManager theOnlyInstance;
    return theOnlyInstance;
  }

  /**
    @brief Creates the OIS::InputMananger, the keyboard and the mouse and
           assigns the key bindings.
    @param windowHnd The window handle of the render window
    @param windowWidth The width of the render window
    @param windowHeight The height of the render window
  */
  bool InputManager::initialise(size_t windowHnd, int windowWidth, int windowHeight)
  {
    if (!this->inputSystem_)
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

      try
      {
        // Create inputsystem
        inputSystem_ = OIS::InputManager::createInputSystem(paramList);
        COUT(ORX_DEBUG) << "*** InputManager: Created OIS input system" << std::endl;

        // create a keyboard. If none are available the exception is caught.
        keyboard_ = static_cast<OIS::Keyboard*>(inputSystem_->createInputObject(OIS::OISKeyboard, true));
        COUT(ORX_DEBUG) << "*** InputManager: Created OIS mouse" << std::endl;

        // create a mouse. If none are available the exception is caught.
        mouse_ = static_cast<OIS::Mouse*>(inputSystem_->createInputObject(OIS::OISMouse, true));
        COUT(ORX_DEBUG) << "*** InputManager: Created OIS keyboard" << std::endl;

        // Set mouse region
        this->setWindowExtents(windowWidth, windowHeight);
      }
      catch (OIS::Exception ex)
      {
        // something went wrong with the initialisation
        COUT(ORX_ERROR) << "Error: Failed creating an input system/keyboard/mouse. Message: \"" << ex.eText << "\"" << std::endl;
        this->inputSystem_ = 0;
        return false;
      }
    }

    // create the handlers
    this->handlerGUI_ = new InputHandlerGUI();
    this->handlerGame_ = new InputHandlerGame();
    this->handlerGame_->loadBindings();

    /*COUT(ORX_DEBUG) << "*** InputManager: Loading key bindings..." << std::endl;
    // load the key bindings
    InputEvent empty = {0, false, 0, 0, 0};
    for (int i = 0; i < this->numberOfKeys_; i++)
      this->bindingsKeyPressed_[i] = empty;

    //assign 'abort' to the escape key
    this->bindingsKeyPressed_[(int)OIS::KC_ESCAPE].id = 1;
    COUT(ORX_DEBUG) << "*** InputManager: Loading done." << std::endl;*/

    return true;
  }

  /**
    @brief Destroys all the created input devices and handlers.
  */
  void InputManager::destroy()
  {
    COUT(ORX_DEBUG) << "*** InputManager: Destroying ..." << std::endl;
    if (this->mouse_)
      this->inputSystem_->destroyInputObject(mouse_);
    if (this->keyboard_)
      this->inputSystem_->destroyInputObject(keyboard_);
    if (this->inputSystem_)
      OIS::InputManager::destroyInputSystem(this->inputSystem_);

    this->mouse_         = 0;
    this->keyboard_      = 0;
    this->inputSystem_   = 0;

    if (this->handlerBuffer_)
      delete this->handlerBuffer_;
    if (this->handlerGame_)
      delete this->handlerGame_;
    if (this->handlerGUI_)
      delete this->handlerGUI_;

    this->handlerBuffer_ = 0;
    this->handlerGame_   = 0;
    this->handlerGUI_    = 0;

    COUT(ORX_DEBUG) << "*** InputManager: Destroying done." << std::endl;
  }

  /**
    @brief Updates the InputManager
    @param dt Delta time
  */
  void InputManager::tick(float dt)
  {
    // reset the game if it has changed
    if (this->currentMode_ != this->setMode_)
    {
      switch (this->setMode_)
      {
      case IM_GUI:
        this->mouse_->setEventCallback(this->handlerGUI_);
        this->keyboard_->setEventCallback(this->handlerGUI_);
        break;
      case IM_INGAME:
        this->mouse_->setEventCallback(this->handlerGame_);
        this->keyboard_->setEventCallback(this->handlerGame_);
        break;
      case IM_KEYBOARD:
        this->mouse_->setEventCallback(this->handlerGame_);
        this->keyboard_->setEventCallback(this->handlerBuffer_);
        break;
      case IM_UNINIT:
        this->mouse_->setEventCallback(0);
        this->keyboard_->setEventCallback(0);
        break;
      }
      this->currentMode_ = this->setMode_;
    }

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
  void InputManager::setWindowExtents(int width, int height)
  {
    // Set mouse region (if window resizes, we should alter this to reflect as well)
    const OIS::MouseState &mouseState = mouse_->getMouseState();
    mouseState.width  = width;
    mouseState.height = height;
  }

  /**
    @brief Sets the input mode to either GUI, inGame or Buffer
    @param mode The new input mode
    @remark Only has an affect if the mode actually changes
  */
  void InputManager::setInputMode(InputMode mode)
  {
    this->setMode_ = mode;
  }

  /**
    @brief Returns the current input handling method
    @return The current input mode.
  */
  InputMode InputManager::getInputMode()
  {
    return this->currentMode_;
  }

}
