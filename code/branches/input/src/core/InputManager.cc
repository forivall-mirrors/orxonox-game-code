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
  @brief Implementation of the InputManager that captures all the input from OIS
         and redirects it to listeners if necessary.
 */

#include "InputManager.h"
#include "CoreIncludes.h"
#include "Debug.h"
#include "InputBuffer.h"
#include "ConsoleCommand.h"
#include "util/Convert.h"

namespace orxonox
{
  // ###############################
  // ###    Internal Methods     ###
  // ###############################

  /**
    @brief Constructor only sets member fields to initial zero values
           and registers the class in the class hierarchy.
  */
  InputManager::InputManager() :
      inputSystem_(0), keyboard_(0), mouse_(0),
      state_(IS_UNINIT), stateRequest_(IS_UNINIT)
  {
    // overwrite every key binding with ""
    _clearBindings();
    _setNumberOfJoysticks(0);

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
    @brief Destructor only called at the end of the program, after main.
  */
  InputManager::~InputManager()
  {
    this->_destroy();
  }

  /**
    @brief Creates the OIS::InputMananger, the keyboard, the mouse and
           the joysticks and assigns the key bindings.
    @param windowHnd The window handle of the render window
    @param windowWidth The width of the render window
    @param windowHeight The height of the render window
  */
  bool InputManager::_initialise(size_t windowHnd, int windowWidth, int windowHeight,
        bool createKeyboard, bool createMouse, bool createJoySticks)
  {
    if (state_ == IS_UNINIT)
    {
      CCOUT(ORX_DEBUG) << "Initialising OIS components..." << std::endl;

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
        inputSystem_ = OIS::InputManager::createInputSystem(paramList);
        CCOUT(ORX_DEBUG) << "Created OIS input system" << std::endl;
      }
      catch (OIS::Exception ex)
      {
        CCOUT(ORX_ERROR) << "Error: Failed creating an OIS input system."
            << "OIS message: \"" << ex.eText << "\"" << std::endl;
        inputSystem_ = 0;
        return false;
      }

      if (createKeyboard)
        _initialiseKeyboard();

      if (createMouse)
        _initialiseMouse();

      if (createJoySticks)
        _initialiseJoySticks();

      // Set mouse/joystick region
      setWindowExtents(windowWidth, windowHeight);

      this->state_ = IS_NONE;
      CCOUT(ORX_DEBUG) << "Initialising OIS components done." << std::endl;
    }
    else
    {
      CCOUT(ORX_WARNING) << "Warning: OIS compoments already initialised, skipping" << std::endl;
    }

    // InputManager holds the input buffer --> create one and add it.
    addKeyListener(new InputBuffer(), "buffer");

    // Read all the key bindings and assign them
    if (!_loadBindings())
      return false;

    CCOUT(ORX_DEBUG) << "Initialising complete." << std::endl;
    return true;
  }

  /**
    @brief Creates a keyboard and sets the event handler.
  */
  void InputManager::_initialiseKeyboard()
  {
    try
    {
#if (OIS_VERSION >> 8) == 0x0100
    if (inputSystem_->numKeyboards() > 0)
#elif (OIS_VERSION >> 8) == 0x0102
    if (inputSystem_->getNumberOfDevices(OIS::OISKeyboard) > 0)
#endif
      {
        keyboard_ = (OIS::Keyboard*)inputSystem_->createInputObject(OIS::OISKeyboard, true);
        // register our listener in OIS.
        keyboard_->setEventCallback(this);
        // note: OIS will not detect keys that have already been down when the keyboard was created.
        CCOUT(ORX_DEBUG) << "Created OIS keyboard" << std::endl;
      }
      else
        CCOUT(ORX_WARNING) << "Warning: No keyboard found!" << std::endl;
    }
    catch (OIS::Exception ex)
    {
      CCOUT(ORX_WARNING) << "Warning: Failed to create an OIS keyboard\n"
          << "OIS error message: \"" << ex.eText << "\"" << std::endl;
      keyboard_ = 0;
    }
  }

  /**
    @brief Creates a mouse and sets the event handler.
  */
  void InputManager::_initialiseMouse()
  {
    try
    {
#if (OIS_VERSION >> 8) == 0x0100
    if (inputSystem_->numMice() > 0)
#elif (OIS_VERSION >> 8) == 0x0102
    if (inputSystem_->getNumberOfDevices(OIS::OISMouse) > 0)
#endif
      {
        mouse_ = static_cast<OIS::Mouse*>(inputSystem_->createInputObject(OIS::OISMouse, true));
        // register our listener in OIS.
        mouse_->setEventCallback(this);
        CCOUT(ORX_DEBUG) << "Created OIS keyboard" << std::endl;
      }
      else
        CCOUT(ORX_WARNING) << "Warning: No mouse found!" << std::endl;
    }
    catch (OIS::Exception ex)
    {
      CCOUT(ORX_WARNING) << "Warning: Failed to create an OIS mouse\n"
          << "OIS error message: \"" << ex.eText << "\"" << std::endl;
      mouse_ = 0;
    }
  }

  /**
    @brief Creates all joy sticks and sets the event handler.
  */
  void InputManager::_initialiseJoySticks()
  {
#if (OIS_VERSION >> 8) == 0x0100
    if (inputSystem_->numJoySticks() > 0)
    {
      _setNumberOfJoysticks(inputSystem_->numJoySticks());
#elif (OIS_VERSION >> 8) == 0x0102
    if (inputSystem_->getNumberOfDevices(OIS::OISJoyStick) > 0)
    {
      _setNumberOfJoysticks(inputSystem_->getNumberOfDevices(OIS::OISJoyStick));
#endif
      for (std::vector<OIS::JoyStick*>::iterator it = joySticks_.begin(); it != joySticks_.end(); it++)
      {
        try
        {
          *it = static_cast<OIS::JoyStick*>(inputSystem_->createInputObject(OIS::OISJoyStick, true));
          // register our listener in OIS.
          (*it)->setEventCallback(this);
          CCOUT(ORX_DEBUG) << "Created OIS joy stick with ID " << (*it)->getID() << std::endl;
        }
        catch (OIS::Exception ex)
        {
          CCOUT(ORX_WARNING) << "Warning: Failed to create OIS joy stick with ID" << (*it)->getID() << "\n"
              << "OIS error message: \"" << ex.eText << "\"" << std::endl;
          (*it) = 0;
        }
      }
    }
    else
      CCOUT(ORX_WARNING) << "Warning: Joy stick support requested, but no joy stick was found" << std::endl;
  }

  /**
    @brief Resizes all lists related to joy sticks and sets joy stick bindings to "".
    @param size Number of joy sticks available.
  */
  void InputManager::_setNumberOfJoysticks(int size)
  {
    this->bindingsJoyStickButtonHold_   .resize(size);
    this->bindingsJoyStickButtonPress_  .resize(size);
    this->bindingsJoyStickButtonRelease_.resize(size);
    this->bJoyStickButtonBindingsActive_.resize(size);
    this->joyStickButtonsDown_          .resize(size);
    this->joySticks_                    .resize(size);
    for (int j = 0; j < size; j++)
    {
      bindingsJoyStickButtonPress_  [j].resize(numberOfJoyStickButtons_s);
      bindingsJoyStickButtonRelease_[j].resize(numberOfJoyStickButtons_s);
      bindingsJoyStickButtonHold_   [j].resize(numberOfJoyStickButtons_s);
      for (int i = 0; i < numberOfJoyStickButtons_s; i++)
      {
        this->bindingsJoyStickButtonPress_  [j][i] = "";
        this->bindingsJoyStickButtonRelease_[j][i] = "";
        this->bindingsJoyStickButtonHold_   [j][i] = "";
      }
    }
  }

  /**
    @brief Loads the key and button bindings.
  */
  bool InputManager::_loadBindings()
  {
    CCOUT(ORX_DEBUG) << "Loading key bindings..." << std::endl;

    // clear all the bindings at first.
    _clearBindings();

    // TODO: Insert the code to load the bindings from file.
    this->bindingsKeyPress_[OIS::KC_NUMPADENTER] = "activateConsole";
    this->bindingsKeyPress_[OIS::KC_ESCAPE] = "exit";
    this->bindingsKeyHold_[OIS::KC_U] = "exec disco.txt";

    CCOUT(ORX_DEBUG) << "Loading key bindings done." << std::endl;
    return true;
  }

  /**
    @brief Overwrites all bindings with ""
  */
  void InputManager::_clearBindings()
  {
    for (int i = 0; i < numberOfKeys_s; i++)
    {
      this->bindingsKeyPress_  [i] = "";
      this->bindingsKeyRelease_[i] = "";
      this->bindingsKeyHold_   [i] = "";
    }
    for (int i = 0; i < numberOfMouseButtons_s; i++)
    {
      this->bindingsMouseButtonPress_  [i] = "";
      this->bindingsMouseButtonRelease_[i] = "";
      this->bindingsMouseButtonHold_   [i] = "";
    }
    for (unsigned int j = 0; j < joySticks_.size(); j++)
    {
      for (int i = 0; i < numberOfJoyStickButtons_s; i++)
      {
        this->bindingsJoyStickButtonPress_  [j][i] = "";
        this->bindingsJoyStickButtonRelease_[j][i] = "";
        this->bindingsJoyStickButtonHold_   [j][i] = "";
      }
    }
  }

  /**
    @brief Destroys all the created input devices and sets the InputManager to construction state.
  */
  void InputManager::_destroy()
  {
    CCOUT(ORX_DEBUG) << "Destroying ..." << std::endl;

    if (state_ != IS_UNINIT)
    {
      this->listenersKeyActive_.clear();
      this->listenersMouseActive_.clear();
      this->listenersJoySticksActive_.clear();
      this->listenersKey_.clear();
      this->listenersMouse_.clear();
      this->listenersJoySticks_.clear();

      this->keysDown_.clear();
      this->mouseButtonsDown_.clear();

      _clearBindings();

      if (keyboard_)
        inputSystem_->destroyInputObject(keyboard_);
      keyboard_ = 0;

      if (mouse_)
        inputSystem_->destroyInputObject(mouse_);
      mouse_ = 0;

      if (joySticks_.size() > 0)
      {
        for (unsigned int i = 0; i < joySticks_.size(); i++)
        {
          if (joySticks_[i] != 0)
            inputSystem_->destroyInputObject(joySticks_[i]);
        }
        _setNumberOfJoysticks(0);
      }

      if (inputSystem_)
        OIS::InputManager::destroyInputSystem(inputSystem_);
      inputSystem_ = 0;

      if (listenersKey_.find("buffer") != listenersKey_.end())
        delete listenersKey_["buffer"];

      this->state_ = IS_UNINIT;
    }
    else
      CCOUT(ORX_WARNING) << "Warning: Cannot be destroyed, since not initialised." << std::endl;

    CCOUT(ORX_DEBUG) << "Destroying done." << std::endl;
  }


  // ###############################
  // ###  Public Member Methods  ###
  // ###############################

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
      switch (stateRequest_)
      {
      case IS_NORMAL:
        this->listenersKeyActive_.clear();
        this->listenersMouseActive_.clear();
        this->listenersJoySticksActive_.clear();
        this->bKeyBindingsActive_            = true;
        this->bMouseButtonBindingsActive_    = true;
        //this->bJoySticksButtonBindingsActive_ = true;
        break;

      case IS_GUI:
        // FIXME: do stuff
        break;

      case IS_CONSOLE:
        this->listenersKeyActive_.clear();
        this->listenersMouseActive_.clear();
        //this->listenersJoyStickActive_.clear();
        this->bKeyBindingsActive_            = false;
        this->bMouseButtonBindingsActive_    = true;
        //this->bJoyStickButtonBindingsActive_ = true;
        if (listenersKey_.find("buffer") != listenersKey_.end())
          listenersKeyActive_.push_back(listenersKey_["buffer"]);
        else
        {
          // someone fiddled with the InputBuffer
          CCOUT(2) << "Error: Cannot redirect input to console, InputBuffer instance missing." << std::endl;
          this->bKeyBindingsActive_ = true;
        }
        break;

      case IS_NONE:
        this->listenersKeyActive_.clear();
        this->listenersMouseActive_.clear();
        //this->listenersJoyStickActive_.clear();
        this->bKeyBindingsActive_            = false;
        this->bMouseButtonBindingsActive_    = false;
        //this->bJoyStickButtonBindingsActive_ = false;
        break;

      case IS_CUSTOM:
        // don't do anything
        break;
      }
      state_ = stateRequest_;
    }

    // Capture all the input. This calls the event handlers in InputManager.
    if (mouse_)
      mouse_->capture();
    if (keyboard_)
      keyboard_->capture();
  }

  /*void InputManager::_setDefaultState()
  {
    this->listenersKeyActive_.clear();
    this->listenersMouseActive_.clear();
    this->listenersJoyStickActive_.clear();
    this->bKeyBindingsActive_            = true;
    this->bMouseButtonBindingsActive_    = true;
    this->bJoyStickButtonBindingsActive_ = true;
  }*/


  /**
    @brief Event handler for the keyPressed Event.
    @param e Event information
  */
  bool InputManager::keyPressed(const OIS::KeyEvent &e)
  {
    this->keysDown_.push_back(e.key);

    if (this->bKeyBindingsActive_)
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
      for (std::list<OIS::KeyListener*>::const_iterator it = listenersKeyActive_.begin(); it != listenersKeyActive_.end(); it++)
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

    if (this->bKeyBindingsActive_)
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
      for (std::list<OIS::KeyListener*>::const_iterator it = listenersKeyActive_.begin(); it != listenersKeyActive_.end(); it++)
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


  // ################################
  // ### Static Interface Methods ###
  // ################################

  /**
    @brief Adjusts the mouse window metrics.
    This method has to be called every time the size of the window changes.
    @param width The new width of the render window
    @param height the new height of the render window
  */
  void InputManager::setWindowExtents(int width, int height)
  {
    if (_getSingleton().mouse_)
    {
      // Set mouse region (if window resizes, we should alter this to reflect as well)
      const OIS::MouseState &mouseState = _getSingleton().mouse_->getMouseState();
      mouseState.width  = width;
      mouseState.height = height;
    }
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

  bool InputManager::initialise(size_t windowHnd, int windowWidth, int windowHeight,
    bool createKeyboard, bool createMouse, bool createJoySticks)
  {
    return _getSingleton()._initialise(windowHnd, windowWidth, windowHeight,
          createKeyboard, createMouse, createJoySticks);
  }

  bool InputManager::addKeyListener(OIS::KeyListener *listener, const std::string& name)
  {
    if (_getSingleton().listenersKey_.find(name) == _getSingleton().listenersKey_.end())
    {
      _getSingleton().listenersKey_[name] = listener;
      return true;
    }
    else
      return false;
  }

  bool InputManager::removeKeyListener(const std::string &name)
  {
    std::map<std::string, OIS::KeyListener*>::iterator it = _getSingleton().listenersKey_.find(name);
    if (it != _getSingleton().listenersKey_.end())
    {
      _getSingleton().listenersKey_.erase(it);
      return true;
    }
    else
      return false;
  }

  OIS::KeyListener* InputManager::getKeyListener(const std::string& name)
  {
    std::map<std::string, OIS::KeyListener*>::iterator it = _getSingleton().listenersKey_.find(name);
    if (it != _getSingleton().listenersKey_.end())
    {
      return (*it).second;
    }
    else
      return 0;
  }

}
