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
  // ###############################

  /**
    @brief Constructor only sets member fields to initial zero values
           and registers the class in the class hierarchy.
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
    @brief Destructor only called at the end of the program, after main.
  */
  InputManager::~InputManager()
  {
    _destroy();
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

      state_ = IS_NONE;
      CCOUT(ORX_DEBUG) << "Initialising OIS components done." << std::endl;
    }
    else
    {
      CCOUT(ORX_WARNING) << "Warning: OIS compoments already initialised, skipping" << std::endl;
    }

    // InputManager holds the input buffer --> create one and add it.
    addKeyListener(new InputBuffer(), "buffer");

    KeyBinder* binder = new KeyBinder();
    binder->loadBindings();
    addKeyListener(binder, "keybinder");
    addMouseListener(binder, "keybinder");

    // Read all the key bindings and assign them
    //if (!_loadBindings())
    //  return false;

    CCOUT(ORX_DEBUG) << "Initialising complete." << std::endl;
    return true;
  }

  /**
    @brief Creates a keyboard and sets the event handler.
    @return False if keyboard stays uninitialised, true otherwise.
  */
  bool InputManager::_initialiseKeyboard()
  {
    if (keyboard_ != 0)
    {
      CCOUT(2) << "Warning: Keyboard already initialised, skipping." << std::endl;
      return true;
    }
    try
    {
      if (inputSystem_->getNumberOfDevices(OIS::OISKeyboard) > 0)
      {
        keyboard_ = (OIS::Keyboard*)inputSystem_->createInputObject(OIS::OISKeyboard, true);
        // register our listener in OIS.
        keyboard_->setEventCallback(this);
        // note: OIS will not detect keys that have already been down when the keyboard was created.
        CCOUT(ORX_DEBUG) << "Created OIS keyboard" << std::endl;
        return true;
      }
      else
      {
        CCOUT(ORX_WARNING) << "Warning: No keyboard found!" << std::endl;
        return false;
      }
    }
    catch (OIS::Exception ex)
    {
      // TODO: Test this output regarding formatting
      CCOUT(ORX_WARNING) << "Warning: Failed to create an OIS keyboard\n"
          << "OIS error message: \"" << ex.eText << "\"" << std::endl;
      keyboard_ = 0;
      return false;
    }
  }

  /**
    @brief Creates a mouse and sets the event handler.
    @return False if mouse stays uninitialised, true otherwise.
  */
  bool InputManager::_initialiseMouse()
  {
    if (mouse_ != 0)
    {
      CCOUT(2) << "Warning: Mouse already initialised, skipping." << std::endl;
      return true;
    }
    try
    {
      if (inputSystem_->getNumberOfDevices(OIS::OISMouse) > 0)
      {
        mouse_ = static_cast<OIS::Mouse*>(inputSystem_->createInputObject(OIS::OISMouse, true));
        // register our listener in OIS.
        mouse_->setEventCallback(this);
        CCOUT(ORX_DEBUG) << "Created OIS keyboard" << std::endl;
        return true;
      }
      else
      {
        CCOUT(ORX_WARNING) << "Warning: No mouse found!" << std::endl;
        return false;
      }
    }
    catch (OIS::Exception ex)
    {
      CCOUT(ORX_WARNING) << "Warning: Failed to create an OIS mouse\n"
          << "OIS error message: \"" << ex.eText << "\"" << std::endl;
      mouse_ = 0;
      return false;
    }
  }

  /**
    @brief Creates all joy sticks and sets the event handler.
    @return False joy stick stay uninitialised, true otherwise.
  */
  bool InputManager::_initialiseJoySticks()
  {
    if (joySticks_.size() > 0)
    {
      CCOUT(2) << "Warning: Joy sticks already initialised, skipping." << std::endl;
      return true;
    }
    bool success = false;
    if (inputSystem_->getNumberOfDevices(OIS::OISJoyStick) > 0)
    {
      for (int i = 0; i < inputSystem_->getNumberOfDevices(OIS::OISJoyStick); i++)
      {
        try
        {
          OIS::JoyStick* stig = static_cast<OIS::JoyStick*>(inputSystem_->createInputObject(OIS::OISJoyStick, true));
          joySticks_[stig->getID()] = stig;
          // register our listener in OIS.
          stig->setEventCallback(this);
          CCOUT(ORX_DEBUG) << "Created OIS joy stick with ID " << stig->getID() << std::endl;
          success = true;
        }
        catch (OIS::Exception ex)
        {
          CCOUT(ORX_WARNING) << "Warning: Failed to create OIS joy number" << i << "\n"
              << "OIS error message: \"" << ex.eText << "\"" << std::endl;
        }
      }
    }
    else
    {
      CCOUT(ORX_WARNING) << "Warning: Joy stick support requested, but no joy stick was found" << std::endl;
      return false;
    }
    return success;
  }

  /**
    @brief Destroys all the created input devices and sets the InputManager to construction state.
  */
  void InputManager::_destroy()
  {
    CCOUT(ORX_DEBUG) << "Destroying ..." << std::endl;

    if (state_ != IS_UNINIT)
    {
      if (listenersKey_.find("buffer") != listenersKey_.end())
        delete listenersKey_["buffer"];

      if (listenersKey_.find("keybinder") != listenersKey_.end())
        delete listenersKey_["keybinder"];

      listenersKeyActive_.clear();
      listenersMouseActive_.clear();
      listenersJoySticksActive_.clear();
      listenersKey_.clear();
      listenersMouse_.clear();
      listenersJoySticks_.clear();

      keysDown_.clear();
      mouseButtonsDown_.clear();
      joySticksButtonsDown_.clear();

      _destroyKeyboard();
      _destroyMouse();
      _destroyJoySticks();

      // inputSystem_ can never be 0, or else the code is mistaken
      OIS::InputManager::destroyInputSystem(inputSystem_);
      inputSystem_ = 0;

      state_ = IS_UNINIT;
    }
    else
      CCOUT(ORX_WARNING) << "Warning: Cannot be destroyed, since not initialised." << std::endl;

    CCOUT(ORX_DEBUG) << "Destroying done." << std::endl;
  }

  /**
    @brief Destroys the keyboard and sets it to 0.
  */
  void InputManager::_destroyKeyboard()
  {
    if (keyboard_)
      // inputSystem_ can never be 0, or else the code is mistaken
      inputSystem_->destroyInputObject(keyboard_);
    keyboard_ = 0;
    CCOUT(ORX_DEBUG) << "Keyboard destroyed." << std::endl;
  }

  /**
    @brief Destroys the mouse and sets it to 0.
  */
  void InputManager::_destroyMouse()
  {
    if (mouse_)
      // inputSystem_ can never be 0, or else the code is mistaken
      inputSystem_->destroyInputObject(mouse_);
    mouse_ = 0;
    CCOUT(ORX_DEBUG) << "Mouse destroyed." << std::endl;
  }

  /**
    @brief Destroys all the joy sticks and resizes the lists to 0.
  */
  void InputManager::_destroyJoySticks()
  {
    if (joySticks_.size() > 0)
    {
      // note: inputSystem_ can never be 0, or else the code is mistaken
      for (std::map<int, OIS::JoyStick*>::iterator itstick = joySticks_.begin(); itstick != joySticks_.end(); itstick++)
      {
        if ((*itstick).second != 0)
          inputSystem_->destroyInputObject((*itstick).second);
      }
      joySticks_.clear();
    }
    CCOUT(ORX_DEBUG) << "Joy sticks destroyed." << std::endl;
  }


  // #################################
  // ### Private Interface Methods ###
  // #################################
  // #################################

  /**
    @brief Updates the InputManager. Tick is called by Orxonox.
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
      {
        listenersKeyActive_.clear();
        listenersMouseActive_.clear();
        listenersJoySticksActive_.clear();

        switch (stateRequest_)
        {
        case IS_NORMAL:
          // normal play mode
          if (listenersKey_.find("keybinder") != listenersKey_.end())
            listenersKeyActive_.push_back(listenersKey_["keybinder"]);
          if (listenersMouse_.find("keybinder") != listenersMouse_.end())
            listenersMouseActive_.push_back(listenersMouse_["keybinder"]);
          if (listenersJoySticks_.find("keybinder") != listenersJoySticks_.end())
          {
            for (std::map<int, OIS::JoyStick*>::const_iterator it = joySticks_.begin();
                  it != joySticks_.end(); it++)
              listenersJoySticksActive_[(*it).first].push_back(listenersJoySticks_["keybinder"]);
          }
          break;

        case IS_GUI:
          // FIXME: do stuff
          break;

        case IS_CONSOLE:
          if (listenersMouse_.find("keybinder") != listenersMouse_.end())
            listenersMouseActive_.push_back(listenersMouse_["keybinder"]);
          if (listenersJoySticks_.find("keybinder") != listenersJoySticks_.end())
          {
            for (std::map<int, OIS::JoyStick*>::const_iterator it = joySticks_.begin();
                  it != joySticks_.end(); it++)
              listenersJoySticksActive_[(*it).first].push_back(listenersJoySticks_["keybinder"]);
          }

          if (listenersKey_.find("buffer") != listenersKey_.end())
            listenersKeyActive_.push_back(listenersKey_["buffer"]);
          else
          {
            // someone fiddled with the InputBuffer
            CCOUT(2) << "Error: Cannot redirect input to console, InputBuffer instance missing." << std::endl;
            if (listenersKey_.find("keybinder") != listenersKey_.end())
              listenersKeyActive_.push_back(listenersKey_["keybinder"]);
            else
              // this is bad
              CCOUT(2) << "Error: Cannot reactivate key binder: not found!" << std::endl;
          }
          break;

        default:
          break;
        }
        state_ = stateRequest_;
      }
    }

    // Capture all the input. This calls the event handlers in InputManager.
    if (mouse_)
      mouse_->capture();
    if (keyboard_)
      keyboard_->capture();


    // call all the listeners for the held key events
    for (std::list<OIS::KeyCode>::const_iterator itKey = keysDown_.begin();
          itKey != keysDown_.end(); itKey++)
    {
      OIS::KeyEvent keyArg(keyboard_, *itKey, 0);
      for (std::list<KeyHandler*>::const_iterator itKeyHandler = listenersKeyActive_.begin();
            itKeyHandler != listenersKeyActive_.end(); itKeyHandler++)
      {
        (*itKeyHandler)->keyHeld(keyArg);
      }
    }

    // call all the listeners for the held mouse button events
    for (std::list<OIS::MouseButtonID>::const_iterator itMouseButton = mouseButtonsDown_.begin();
          itMouseButton != mouseButtonsDown_.end(); itMouseButton++)
    {
      OIS::MouseEvent mouseButtonArg(mouse_, mouse_->getMouseState());
      for (std::list<MouseHandler*>::const_iterator itMouseHandler = listenersMouseActive_.begin();
            itMouseHandler != listenersMouseActive_.end(); itMouseHandler++)
      {
        (*itMouseHandler)->mouseHeld(mouseButtonArg, *itMouseButton);
      }
    }

    // call all the listeners for the held joy stick button events
    for (std::map<int, std::list <int> >::const_iterator itJoyStick = joySticksButtonsDown_.begin();
          itJoyStick != joySticksButtonsDown_.end(); itJoyStick++)
    {
      int id = (*itJoyStick).first;
      for (std::list<int>::const_iterator itJoyStickButton = (*itJoyStick).second.begin();
            itJoyStickButton != (*itJoyStick).second.end(); itJoyStickButton++)
      {
        OIS::JoyStickEvent joyStickButtonArg(joySticks_[id], joySticks_[id]->getJoyStickState());
        for (std::list<JoyStickHandler*>::const_iterator itJoyStickHandler = listenersJoySticksActive_[id].begin();
              itJoyStickHandler != listenersJoySticksActive_[id].end(); itJoyStickHandler++)
        {
          (*itJoyStickHandler)->buttonHeld(joyStickButtonArg, *itJoyStickButton);
        }
      }
    }
  }


  /**
    @brief Event handler for the keyPressed Event.
    @param e Event information
  */
  bool InputManager::keyPressed(const OIS::KeyEvent &e)
  {
    // check whether the key is already in the list (can happen when focus was lost)
    for (std::list<OIS::KeyCode>::iterator it = keysDown_.begin(); it != keysDown_.end(); it++)
    {
      if (*it == e.key)
      {
        keysDown_.erase(it);
        break;
      }
    }
    keysDown_.push_back(e.key);

    for (std::list<KeyHandler*>::const_iterator it = listenersKeyActive_.begin(); it != listenersKeyActive_.end(); it++)
      (*it)->keyPressed(e);

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

    for (std::list<KeyHandler*>::const_iterator it = listenersKeyActive_.begin(); it != listenersKeyActive_.end(); it++)
      (*it)->keyReleased(e);

    return true;
  }

  /**
    @brief Event handler for the mouseMoved Event.
    @param e Event information
  */
  bool InputManager::mouseMoved(const OIS::MouseEvent &e)
  {
    for (std::list<MouseHandler*>::const_iterator it = listenersMouseActive_.begin(); it != listenersMouseActive_.end(); it++)
      (*it)->mouseMoved(e);

    return true;
  }

  /**
    @brief Event handler for the mousePressed Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool InputManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    // check whether the button is already in the list (can happen when focus was lost)
    for (std::list<OIS::MouseButtonID>::iterator it = mouseButtonsDown_.begin(); it != mouseButtonsDown_.end(); it++)
    {
      if (*it == id)
      {
        mouseButtonsDown_.erase(it);
        break;
      }
    }
    mouseButtonsDown_.push_back(id);

    for (std::list<MouseHandler*>::const_iterator it = listenersMouseActive_.begin(); it != listenersMouseActive_.end(); it++)
      (*it)->mousePressed(e, id);

    return true;
  }

  /**
    @brief Event handler for the mouseReleased Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool InputManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    // remove the button from the keysDown_ list
    for (std::list<OIS::MouseButtonID>::iterator it = mouseButtonsDown_.begin(); it != mouseButtonsDown_.end(); it++)
    {
      if (*it == id)
      {
        mouseButtonsDown_.erase(it);
        break;
      }
    }

    for (std::list<MouseHandler*>::const_iterator it = listenersMouseActive_.begin(); it != listenersMouseActive_.end(); it++)
      (*it)->mouseReleased(e, id);

    return true;
  }

  bool InputManager::buttonPressed(const OIS::JoyStickEvent &arg, int button)
  {
    int devID = arg.device->getID();

    // check whether the button is already in the list (can happen when focus was lost)
    std::list<int>& buttonsDownList = joySticksButtonsDown_[devID];
    for (std::list<int>::iterator it = buttonsDownList.begin(); it != buttonsDownList.end(); it++)
    {
      if (*it == button)
      {
        buttonsDownList.erase(it);
        break;
      }
    }
    joySticksButtonsDown_[devID].push_back(button);

    std::list<JoyStickHandler*>::iterator end = listenersJoySticksActive_[devID].end();
    for (std::list<JoyStickHandler*>::const_iterator it = listenersJoySticksActive_[devID].begin(); it != end; it++)
      (*it)->buttonPressed(arg, button);

    return true;
  }

  bool InputManager::buttonReleased(const OIS::JoyStickEvent &arg, int button)
  {
    int devID = arg.device->getID();

    // remove the button from the joySticksButtonsDown_ list
    std::list<int>& buttonsDownList = joySticksButtonsDown_[devID];
    for (std::list<int>::iterator it = buttonsDownList.begin(); it != buttonsDownList.end(); it++)
    {
      if (*it == button)
      {
        buttonsDownList.erase(it);
        break;
      }
    }

    std::list<JoyStickHandler*>::const_iterator end = listenersJoySticksActive_[devID].end();
    for (std::list<JoyStickHandler*>::const_iterator it = listenersJoySticksActive_[devID].begin(); it != end; it++)
      (*it)->buttonReleased(arg, button);

    return true;
  }

  bool InputManager::axisMoved(const OIS::JoyStickEvent &arg, int axis)
  {
    int devID = arg.device->getID();
    std::list<JoyStickHandler*>::const_iterator end = listenersJoySticksActive_[devID].end();
    for (std::list<JoyStickHandler*>::const_iterator it = listenersJoySticksActive_[devID].begin(); it != end; it++)
      (*it)->axisMoved(arg, axis);

    return true;
  }

  bool InputManager::sliderMoved(const OIS::JoyStickEvent &arg, int id)
  {
    int devID = arg.device->getID();
    std::list<JoyStickHandler*>::const_iterator end = listenersJoySticksActive_[devID].end();
    for (std::list<JoyStickHandler*>::const_iterator it = listenersJoySticksActive_[devID].begin(); it != end; it++)
      (*it)->sliderMoved(arg, id);

    return true;
  }

  bool InputManager::povMoved(const OIS::JoyStickEvent &arg, int id)
  {
    int devID = arg.device->getID();
    std::list<JoyStickHandler*>::const_iterator end = listenersJoySticksActive_[devID].end();
    for (std::list<JoyStickHandler*>::const_iterator it = listenersJoySticksActive_[devID].begin(); it != end; it++)
      (*it)->povMoved(arg, id);

    return true;
  }


  // ################################
  // ### Static Interface Methods ###
  // ################################
  // ################################

  bool InputManager::initialise(size_t windowHnd, int windowWidth, int windowHeight,
    bool createKeyboard, bool createMouse, bool createJoySticks)
  {
    return _getSingleton()._initialise(windowHnd, windowWidth, windowHeight,
          createKeyboard, createMouse, createJoySticks);
  }

  bool InputManager::initialiseKeyboard()
  {
    return _getSingleton()._initialiseKeyboard();
  }

  bool InputManager::initialiseMouse()
  {
    return _getSingleton()._initialiseMouse();
  }

  bool InputManager::initialiseJoySticks()
  {
    return _getSingleton()._initialiseJoySticks();
  }


  void InputManager::destroy()
  {
    _getSingleton()._destroy();
  }

  void InputManager::destroyKeyboard()
  {
    return _getSingleton()._destroyKeyboard();
  }

  void InputManager::destroyMouse()
  {
    return _getSingleton()._destroyMouse();
  }

  void InputManager::destroyJoySticks()
  {
    return _getSingleton()._destroyJoySticks();
  }


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


  // ###### KeyHandler ######

  /**
    @brief Adds a new key listener.
    @param listener Pointer to the listener object.
    @param name Unique name of the listener.
    @return True if added, false if name already existed.
  */
  bool InputManager::addKeyListener(KeyHandler* listener, const std::string& name)
  {
    if (_getSingleton().listenersKey_.find(name) == _getSingleton().listenersKey_.end())
    {
      _getSingleton().listenersKey_[name] = listener;
      return true;
    }
    else
      return false;
  }

  /**
    @brief Removes a Key Listener from the list.
    @param name Unique name of the listener.
    @return True if removal was successful, false if name was not found.
  */
  bool InputManager::removeKeyListener(const std::string &name)
  {
    disableKeyListener(name);
    std::map<std::string, KeyHandler*>::iterator it = _getSingleton().listenersKey_.find(name);
    if (it != _getSingleton().listenersKey_.end())
    {
      _getSingleton().listenersKey_.erase(it);
      return true;
    }
    else
      return false;
  }

  /**
    @brief Returns the pointer to a listener.
    @param name Unique name of the listener.
    @return Pointer to the instance, 0 if name was not found.
  */
  KeyHandler* InputManager::getKeyListener(const std::string& name)
  {
    std::map<std::string, KeyHandler*>::iterator it = _getSingleton().listenersKey_.find(name);
    if (it != _getSingleton().listenersKey_.end())
    {
      return (*it).second;
    }
    else
      return 0;
  }

  /**
    @brief Enables a specific key listener that has already been added.
    @param name Unique name of the listener.
    @return False if name was not found, true otherwise.
  */
  bool InputManager::enableKeyListener(const std::string& name)
  {
    // get pointer from the map with all stored listeners
    std::map<std::string, KeyHandler*>::const_iterator mapIt = _getSingleton().listenersKey_.find(name);
    if (mapIt == _getSingleton().listenersKey_.end())
      return false;
    // see whether the listener is already in the list
    for (std::list<KeyHandler*>::iterator it = _getSingleton().listenersKeyActive_.begin();
          it != _getSingleton().listenersKeyActive_.end(); it++)
    {
      if ((*it) == (*mapIt).second)
        return true;
    }
    _getSingleton().listenersKeyActive_.push_back((*mapIt).second);
    return true;
  }

  /**
    @brief Disables a specific key listener.
    @param name Unique name of the listener.
    @return False if name was not found, true otherwise.
  */
  bool InputManager::disableKeyListener(const std::string &name)
  {
    // get pointer from the map with all stored listeners
    std::map<std::string, KeyHandler*>::const_iterator mapIt = _getSingleton().listenersKey_.find(name);
    if (mapIt == _getSingleton().listenersKey_.end())
      return false;
    // look for the listener in the list
    for (std::list<KeyHandler*>::iterator it = _getSingleton().listenersKeyActive_.begin();
          it != _getSingleton().listenersKeyActive_.end(); it++)
    {
      if ((*it) == (*mapIt).second)
      {
        _getSingleton().listenersKeyActive_.erase(it);
        return true;
      }
    }
    return true;
  }


  // ###### MouseHandler ######
  /**
    @brief Adds a new mouse listener.
    @param listener Pointer to the listener object.
    @param name Unique name of the listener.
    @return True if added, false if name already existed.
  */
  bool InputManager::addMouseListener(MouseHandler* listener, const std::string& name)
  {
    if (_getSingleton().listenersMouse_.find(name) == _getSingleton().listenersMouse_.end())
    {
      _getSingleton().listenersMouse_[name] = listener;
      return true;
    }
    else
      return false;
  }

  /**
    @brief Removes a Mouse Listener from the list.
    @param name Unique name of the listener.
    @return True if removal was successful, false if name was not found.
  */
  bool InputManager::removeMouseListener(const std::string &name)
  {
    disableMouseListener(name);
    std::map<std::string, MouseHandler*>::iterator it = _getSingleton().listenersMouse_.find(name);
    if (it != _getSingleton().listenersMouse_.end())
    {
      _getSingleton().listenersMouse_.erase(it);
      return true;
    }
    else
      return false;
  }

  /**
    @brief Returns the pointer to a listener.
    @param name Unique name of the listener.
    @return Pointer to the instance, 0 if name was not found.
  */
  MouseHandler* InputManager::getMouseListener(const std::string& name)
  {
    std::map<std::string, MouseHandler*>::iterator it = _getSingleton().listenersMouse_.find(name);
    if (it != _getSingleton().listenersMouse_.end())
    {
      return (*it).second;
    }
    else
      return 0;
  }

  /**
    @brief Enables a specific mouse listener that has already been added.
    @param name Unique name of the listener.
    @return False if name was not found, true otherwise.
  */
  bool InputManager::enableMouseListener(const std::string& name)
  {
    // get pointer from the map with all stored listeners
    std::map<std::string, MouseHandler*>::const_iterator mapIt = _getSingleton().listenersMouse_.find(name);
    if (mapIt == _getSingleton().listenersMouse_.end())
      return false;
    // see whether the listener is already in the list
    for (std::list<MouseHandler*>::iterator it = _getSingleton().listenersMouseActive_.begin();
          it != _getSingleton().listenersMouseActive_.end(); it++)
    {
      if ((*it) == (*mapIt).second)
        return true;
    }
    _getSingleton().listenersMouseActive_.push_back((*mapIt).second);
    return true;
  }

  /**
    @brief Disables a specific mouse listener.
    @param name Unique name of the listener.
    @return False if name was not found, true otherwise.
  */
  bool InputManager::disableMouseListener(const std::string &name)
  {
    // get pointer from the map with all stored listeners
    std::map<std::string, MouseHandler*>::const_iterator mapIt = _getSingleton().listenersMouse_.find(name);
    if (mapIt == _getSingleton().listenersMouse_.end())
      return false;
    // look for the listener in the list
    for (std::list<MouseHandler*>::iterator it = _getSingleton().listenersMouseActive_.begin();
          it != _getSingleton().listenersMouseActive_.end(); it++)
    {
      if ((*it) == (*mapIt).second)
      {
        _getSingleton().listenersMouseActive_.erase(it);
        return true;
      }
    }
    return true;
  }


  // ###### JoyStickHandler ######

  /**
    @brief Adds a new joy stick listener.
    @param listener Pointer to the listener object.
    @param name Unique name of the listener.
    @return True if added, false if name already existed.
  */
  bool InputManager::addJoyStickListener(JoyStickHandler* listener, const std::string& name)
  {
    if (_getSingleton().listenersJoySticks_.find(name) == _getSingleton().listenersJoySticks_.end())
    {
      _getSingleton().listenersJoySticks_[name] = listener;
      return true;
    }
    else
      return false;
  }

  /**
    @brief Removes a JoyStick Listener from the list.
    @param name Unique name of the listener.
    @return True if removal was successful, false if name was not found.
  */
  bool InputManager::removeJoyStickListener(const std::string &name)
  {
    for (std::map<int, OIS::JoyStick*>::iterator itstick = _getSingleton().joySticks_.begin(); itstick != _getSingleton().joySticks_.end(); itstick++)
      disableJoyStickListener(name, (*itstick).first);

    std::map<std::string, JoyStickHandler*>::iterator it = _getSingleton().listenersJoySticks_.find(name);
    if (it != _getSingleton().listenersJoySticks_.end())
    {
      _getSingleton().listenersJoySticks_.erase(it);
      return true;
    }
    else
      return false;
  }

  /**
    @brief Returns the pointer to a listener.
    @param name Unique name of the listener.
    @return Pointer to the instance, 0 if name was not found.
  */
  JoyStickHandler* InputManager::getJoyStickListener(const std::string& name)
  {
    std::map<std::string, JoyStickHandler*>::iterator it = _getSingleton().listenersJoySticks_.find(name);
    if (it != _getSingleton().listenersJoySticks_.end())
    {
      return (*it).second;
    }
    else
      return 0;
  }

  /**
    @brief Enables a specific joy stick listener that has already been added.
    @param name Unique name of the listener.
    @return False if name or id was not found, true otherwise.
  */
  bool InputManager::enableJoyStickListener(const std::string& name, const int id)
  {
    // get pointer from the map with all stored listeners
    std::map<std::string, JoyStickHandler*>::const_iterator listenerIt = _getSingleton().listenersJoySticks_.find(name);
    if (listenerIt == _getSingleton().listenersJoySticks_.end())
      return false;

    // check for existance of the ID
    std::map<int, OIS::JoyStick*>::const_iterator joyStickIt = _getSingleton().joySticks_.find(id);
    if (joyStickIt == _getSingleton().joySticks_.end())
      return false;

    // see whether the listener is already in the list
    for (std::list<JoyStickHandler*>::iterator it = _getSingleton().listenersJoySticksActive_[id].begin();
          it != _getSingleton().listenersJoySticksActive_[id].end(); it++)
    {
      if ((*it) == (*listenerIt).second)
        return true;
    }
    _getSingleton().listenersJoySticksActive_[id].push_back((*listenerIt).second);
    return true;
  }

  /**
    @brief Disables a specific joy stick listener.
    @param name Unique name of the listener.
    @return False if name or id was not found, true otherwise.
  */
  bool InputManager::disableJoyStickListener(const std::string &name, int id)
  {
    // get pointer from the map with all stored listeners
    std::map<std::string, JoyStickHandler*>::const_iterator listenerIt = _getSingleton().listenersJoySticks_.find(name);
    if (listenerIt == _getSingleton().listenersJoySticks_.end())
      return false;

    // check for existance of the ID
    std::map<int, OIS::JoyStick*>::const_iterator joyStickIt = _getSingleton().joySticks_.find(id);
    if (joyStickIt == _getSingleton().joySticks_.end())
      return false;

    // look for the listener in the list
    for (std::list<JoyStickHandler*>::iterator it = _getSingleton().listenersJoySticksActive_[id].begin();
          it != _getSingleton().listenersJoySticksActive_[id].end(); it++)
    {
      if ((*it) == (*listenerIt).second)
      {
        _getSingleton().listenersJoySticksActive_[id].erase(it);
        return true;
      }
    }
    return true;
  }

}
