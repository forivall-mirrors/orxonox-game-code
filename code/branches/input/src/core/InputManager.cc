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

#include "InputManager.h"
#include "CoreIncludes.h"
#include "Debug.h"
#include "InputBuffer.h"
#include "ConsoleCommand.h"
#include "util/Convert.h"

namespace orxonox
{
  /**
    @brief Constructor only resets the pointer values to 0.
  */
  InputManager::InputManager() :
      inputSystem_(0), keyboard_(0), mouse_(0),
      state_(IS_UNINIT), stateRequest_(IS_UNINIT)
  {
    RegisterObject(InputManager);
  }

  /**
    @brief The one instance of the InputManager is stored in this function.
    @return A reference to the only instance of the InputManager
  */
  InputManager& InputManager::_getSingleton()
  {
    static InputManager theOnlyInstance;
    return theOnlyInstance;
  }

  /**
    @brief Destructor only called at the end of the program
  */
  InputManager::~InputManager()
  {
    this->_destroy();
  }

  /**
    @brief Creates the OIS::InputMananger, the keyboard and the mouse and
           assigns the key bindings.
    @param windowHnd The window handle of the render window
    @param windowWidth The width of the render window
    @param windowHeight The height of the render window
  */
  bool InputManager::_initialise(size_t windowHnd, int windowWidth, int windowHeight)
  {
    if (!inputSystem_)
    {
      // Setup basic variables
      OIS::ParamList paramList;
      std::ostringstream windowHndStr;

      // Fill parameter list
      windowHndStr << (unsigned int)windowHnd;
      paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

//#if defined OIS_LINUX_PLATFORM
//      paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
//#endif

      try
      {
        // Create inputsystem
        inputSystem_ = OIS::InputManager::createInputSystem(paramList);
        COUT(ORX_DEBUG) << "*** InputManager: Created OIS input system" << std::endl;

        // create a keyboard. If none are available the exception is caught.
        keyboard_ = static_cast<OIS::Keyboard*>(inputSystem_->createInputObject(OIS::OISKeyboard, true));
        COUT(ORX_DEBUG) << "*** InputManager: Created OIS mouse" << std::endl;

        //TODO: check for already pressed keys

        // create a mouse. If none are available the exception is caught.
        mouse_ = static_cast<OIS::Mouse*>(inputSystem_->createInputObject(OIS::OISMouse, true));
        COUT(ORX_DEBUG) << "*** InputManager: Created OIS keyboard" << std::endl;

        // Set mouse region
        setWindowExtents(windowWidth, windowHeight);

        this->state_ = IS_NONE;
      }
      catch (OIS::Exception ex)
      {
        // something went wrong with the initialisation
        COUT(ORX_ERROR) << "Error: Failed creating an input system/keyboard/mouse. Message: \"" << ex.eText << "\"" << std::endl;
        inputSystem_ = 0;
        return false;
      }
    }

    keyboard_->setEventCallback(this);
    mouse_->setEventCallback(this);

    addKeyListener(new InputBuffer(), "buffer");

    _loadBindings();

    COUT(ORX_DEBUG) << "*** InputManager: Loading done." << std::endl;

    return true;
  }

  void InputManager::_loadBindings()
  {
    for (int i = 0; i < numberOfKeys_s; i++)
    {
      // simply write the key number (i) in the string
      this->bindingsKeyPress_[i] = "";
      this->bindingsKeyRelease_[i] = "";
    }
    this->bindingsKeyPress_[OIS::KC_NUMPADENTER] = "activateConsole";
    this->bindingsKeyPress_[OIS::KC_ESCAPE] = "exit";
    this->bindingsKeyHold_[OIS::KC_U] = "exec disco.txt";
  }

  /**
    @brief Destroys all the created input devices and handlers.
  */
  void InputManager::_destroy()
  {
    COUT(ORX_DEBUG) << "*** InputManager: Destroying ..." << std::endl;
    if (mouse_)
      inputSystem_->destroyInputObject(mouse_);
    if (keyboard_)
      inputSystem_->destroyInputObject(keyboard_);
    if (inputSystem_)
      OIS::InputManager::destroyInputSystem(inputSystem_);

    mouse_         = 0;
    keyboard_      = 0;
    inputSystem_   = 0;

    OIS::KeyListener* buffer = keyListeners_["buffer"];
    if (buffer)
    {
      this->removeKeyListener("buffer");
      delete buffer;
      buffer = 0;
    }

    COUT(ORX_DEBUG) << "*** InputManager: Destroying done." << std::endl;
  }

  /**
    @brief Updates the InputManager
    @param dt Delta time
  */
  void InputManager::tick(float dt)
  {
    if (state_ == IS_UNINIT)
      return;

    // reset the game if it has changed
    if (state_ != stateRequest_)
    {
      if (stateRequest_ != IS_CUSTOM)
        _setDefaultState();

      switch (stateRequest_)
      {
      case IS_NORMAL:
        break;
      case IS_GUI:
        //FIXME: do stuff
        break;
      case IS_CONSOLE:
        if (this->keyListeners_.find("buffer") != this->keyListeners_.end())
          this->activeKeyListeners_.push_back(this->keyListeners_["buffer"]);
        this->bDefaultKeyInput = false;
        break;
      case IS_CUSTOM:
        // don't do anything
        break;
      }
      state_ = stateRequest_;
    }

    // capture all the input. That calls the event handlers.
    if (mouse_)
      mouse_->capture();
    if (keyboard_)
      keyboard_->capture();
  }

  void InputManager::_setDefaultState()
  {
    this->activeJoyStickListeners_.clear();
    this->activeKeyListeners_.clear();
    this->activeMouseListeners_.clear();
    this->bDefaultKeyInput      = true;
    this->bDefaultMouseInput    = true;
    this->bDefaultJoyStickInput = true;
  }


  /**
    @brief Event handler for the keyPressed Event.
    @param e Event information
  */
  bool InputManager::keyPressed(const OIS::KeyEvent &e)
  {
    this->keysDown_.push_back(e.key);

    if (this->bDefaultKeyInput)
    {
      // find the appropriate key binding
      std::string cmdStr = bindingsKeyPress_[int(e.key)];
      if (cmdStr != "")
      {
        CommandExecutor::execute(cmdStr);
        COUT(3) << "Executing command: " << cmdStr << std::endl;
      }
    }
    else
    {
      for (std::list<OIS::KeyListener*>::const_iterator it = activeKeyListeners_.begin(); it != activeKeyListeners_.end(); it++)
        (*it)->keyPressed(e);
    }
    return true;
  }

  /**
    @brief Event handler for the keyReleased Event.
    @param e Event information
  */
  bool InputManager::keyReleased(const OIS::KeyEvent &e)
  {
    // remove the key from the keysDown_ list
    for (std::list<OIS::KeyCode>::iterator it = keysDown_.begin(); it != keysDown_.end(); it++)
    {
      if (*it == e.key)
      {
        keysDown_.erase(it);
        break;
      }
    }

    if (this->bDefaultKeyInput)
    {
      // find the appropriate key binding
      std::string cmdStr = bindingsKeyRelease_[int(e.key)];
      if (cmdStr != "")
      {
        CommandExecutor::execute(cmdStr);
        COUT(3) << "Executing command: " << cmdStr << std::endl;
      }
    }
    else
    {
      for (std::list<OIS::KeyListener*>::const_iterator it = activeKeyListeners_.begin(); it != activeKeyListeners_.end(); it++)
        (*it)->keyReleased(e);
    }
    return true;
  }

  /**
    @brief Event handler for the mouseMoved Event.
    @param e Event information
  */
  bool InputManager::mouseMoved(const OIS::MouseEvent &e)
  {
    return true;
  }

  /**
    @brief Event handler for the mousePressed Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool InputManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    return true;
  }

  /**
    @brief Event handler for the mouseReleased Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool InputManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    return true;
  }

  bool InputManager::buttonPressed(const OIS::JoyStickEvent &arg, int button)
  {
    return true;
  }

  bool InputManager::buttonReleased(const OIS::JoyStickEvent &arg, int button)
  {
    return true;
  }

  bool InputManager::axisMoved(const OIS::JoyStickEvent &arg, int axis)
  {
    return true;
  }

  bool InputManager::sliderMoved(const OIS::JoyStickEvent &arg, int id)
  {
    return true;
  }

  bool InputManager::povMoved(const OIS::JoyStickEvent &arg, int id)
  {
    return true;
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
    const OIS::MouseState &mouseState = _getSingleton().mouse_->getMouseState();
    mouseState.width  = width;
    mouseState.height = height;
  }

  /**
    @brief Sets the input mode to either GUI, inGame or Buffer
    @param mode The new input mode
    @remark Only has an affect if the mode actually changes
  */
  void InputManager::setInputState(const InputState state)
  {
    _getSingleton().stateRequest_ = state;
  }

  /**
    @brief Returns the current input handling method
    @return The current input mode.
  */
  InputManager::InputState InputManager::getInputState()
  {
    return _getSingleton().state_;
  }

  void InputManager::destroy()
  {
    _getSingleton()._destroy();
  }

  bool InputManager::initialise(size_t windowHnd, int windowWidth, int windowHeight)
  {
    return _getSingleton()._initialise(windowHnd, windowWidth, windowHeight);
  }

  bool InputManager::addKeyListener(OIS::KeyListener *listener, const std::string& name)
  {
    if (_getSingleton().keyListeners_.find(name) == _getSingleton().keyListeners_.end())
    {
      _getSingleton().keyListeners_[name] = listener;
      return true;
    }
    else
      return false;
  }

  bool InputManager::removeKeyListener(const std::string &name)
  {
    std::map<std::string, OIS::KeyListener*>::iterator it = _getSingleton().keyListeners_.find(name);
    if (it != _getSingleton().keyListeners_.end())
    {
      _getSingleton().keyListeners_.erase(it);
      return true;
    }
    else
      return false;
  }

  OIS::KeyListener* InputManager::getKeyListener(const std::string& name)
  {
    std::map<std::string, OIS::KeyListener*>::iterator it = _getSingleton().keyListeners_.find(name);
    if (it != _getSingleton().keyListeners_.end())
    {
      return (*it).second;
    }
    else
      return 0;
  }

}
