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
         and redirects it to handlers.
 */

#include "InputManager.h"
#include "util/Convert.h"
#include "CoreIncludes.h"
#include "ConfigValueIncludes.h"
#include "Debug.h"
#include "InputBuffer.h"
#include "KeyBinder.h"
#include "CommandExecutor.h"
#include "ConsoleCommand.h"
#include "Shell.h"

namespace orxonox
{
  SetConsoleCommandShortcut(InputManager, keyBind);
  SetConsoleCommandShortcut(InputManager, storeKeyStroke);
  SetConsoleCommandShortcut(InputManager, calibrate);

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
      joySticksSize_(0),
      keyBinder_(0), keyDetector_(0), buffer_(0), calibratorCallback_(0),
      state_(IS_UNINIT), stateRequest_(IS_UNINIT), savedState_(IS_UNINIT),
      keyboardModifiers_(0)
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
  bool InputManager::_initialise(const size_t windowHnd, int windowWidth, int windowHeight,
        bool createKeyboard, bool createMouse, bool createJoySticks)
  {
    if (state_ == IS_UNINIT)
    {
      CCOUT(3) << "Initialising Input System..." << std::endl;
      CCOUT(ORX_DEBUG) << "Initialising OIS components..." << std::endl;

      OIS::ParamList paramList;
      std::ostringstream windowHndStr;

      // Fill parameter list
      windowHndStr << (unsigned int)windowHnd;
      paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
      //paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
      //paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
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
      if (mouse_)
      {
        //// hack the mouse position
        //((OIS::MouseState&)mouse_->getMouseState()).X.abs = windowWidth/2;
        //((OIS::MouseState&)mouse_->getMouseState()).Y.abs = windowHeight/2;
        setWindowExtents(windowWidth, windowHeight);
      }

      state_ = IS_NONE;
      CCOUT(ORX_DEBUG) << "Initialising OIS components done." << std::endl;

      // InputManager holds the input buffer --> create one and add it.
      buffer_ = new InputBuffer();
      addKeyHandler(buffer_, "buffer");
      Shell::getInstance().setInputBuffer(buffer_);

      keyBinder_ = new KeyBinder();
      keyBinder_->loadBindings();
      addKeyHandler(keyBinder_, "keybinder");
      addMouseHandler(keyBinder_, "keybinder");
      addJoyStickHandler(keyBinder_, "keybinder");

      keyDetector_ = new KeyDetector();
      keyDetector_->loadBindings();
      addKeyHandler(keyDetector_, "keydetector");
      addMouseHandler(keyDetector_, "keydetector");
      addJoyStickHandler(keyDetector_, "keydetector");

      calibratorCallback_ = new CalibratorCallback();
      addKeyHandler(calibratorCallback_, "calibratorcallback");

      setConfigValues();

      CCOUT(ORX_DEBUG) << "Initialising complete." << std::endl;
    }
    else
    {
      CCOUT(ORX_WARNING) << "Warning: OIS compoments already initialised, skipping" << std::endl;
    }
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
        CCOUT(ORX_DEBUG) << "Created OIS mouse" << std::endl;
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
    if (joySticksSize_ > 0)
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
          joySticks_.push_back(stig);
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
    joySticksSize_ = joySticks_.size();
    activeJoyStickHandlers_.resize(joySticksSize_);
    joyStickButtonsDown_.resize(joySticksSize_);
    povStates_.resize(joySticksSize_);
    sliderStates_.resize(joySticksSize_);
    joySticksCalibration_.resize(joySticksSize_);
    for (unsigned int iJoyStick = 0; iJoyStick < joySticksSize_; iJoyStick++)
    {
      // reset the calibration with default values
      for (unsigned int i = 0; i < 24; i++)
      {
        joySticksCalibration_[iJoyStick].negativeCoeff[i] = 1.0f/32767.0f;
        joySticksCalibration_[iJoyStick].positiveCoeff[i] = 1.0f/32768.0f;
        joySticksCalibration_[iJoyStick].zeroStates[i] = 0;
      }
    }
    return success;
  }

  /**
    @brief Sets the configurable values. Use keybindings.ini as file..
  */
  void InputManager::setConfigValues()
  {
    if (joySticksSize_)
    {
      std::vector<MultiTypeMath> coeffPos;
      std::vector<MultiTypeMath> coeffNeg;
      std::vector<MultiTypeMath> zero;
      coeffPos.resize(24);
      coeffNeg.resize(24);
      zero.resize(24);
      for (unsigned int i = 0; i < 24; i++)
      {
        coeffPos[i] =  1.0f/32767.0f;
        coeffNeg[i] =  1.0f/32768.0f;
        zero[i]     =  0;
      }

      ConfigValueContainer* cont = getIdentifier()->getConfigValueContainer("CoeffPos");
      if (!cont)
      {
          cont = new ConfigValueContainer(CFT_Keybindings, getIdentifier(), "CoeffPos", coeffPos);
          getIdentifier()->addConfigValueContainer("CoeffPos", cont);
      }
      cont->getValue(&coeffPos);

      cont = getIdentifier()->getConfigValueContainer("CoeffNeg");
      if (!cont)
      {
          cont = new ConfigValueContainer(CFT_Keybindings, getIdentifier(), "CoeffNeg", coeffNeg);
          getIdentifier()->addConfigValueContainer("CoeffNeg", cont);
      }
      cont->getValue(&coeffNeg);

      cont = getIdentifier()->getConfigValueContainer("Zero");
      if (!cont)
      {
          cont = new ConfigValueContainer(CFT_Keybindings, getIdentifier(), "Zero", zero);
          getIdentifier()->addConfigValueContainer("Zero", cont);
      }
      cont->getValue(&zero);

      // copy values to our own variables
      for (unsigned int i = 0; i < 24; i++)
      {
        joySticksCalibration_[0].positiveCoeff[i] = coeffPos[i];
        joySticksCalibration_[0].negativeCoeff[i] = coeffNeg[i];
        joySticksCalibration_[0].zeroStates[i]    = zero[i];
      }
    }
  }

  /**
    @brief Destroys all the created input devices and sets the InputManager to construction state.
  */
  void InputManager::_destroy()
  {
    if (state_ != IS_UNINIT)
    {
      CCOUT(ORX_DEBUG) << "Destroying ..." << std::endl;

      if (buffer_)
        delete buffer_;

      if (keyBinder_)
        delete keyBinder_;

      if (keyDetector_)
        delete keyDetector_;

      if (calibratorCallback_)
        delete calibratorCallback_;

      keyHandlers_.clear();
      mouseHandlers_.clear();
      joyStickHandlers_.clear();

      _destroyKeyboard();
      _destroyMouse();
      _destroyJoySticks();

      activeHandlers_.clear();

      // inputSystem_ can never be 0, or else the code is mistaken
      OIS::InputManager::destroyInputSystem(inputSystem_);
      inputSystem_ = 0;

      state_ = IS_UNINIT;
      CCOUT(ORX_DEBUG) << "Destroying done." << std::endl;
    }
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
    activeKeyHandlers_.clear();
    keysDown_.clear();
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
    activeMouseHandlers_.clear();
    mouseButtonsDown_.clear();
    CCOUT(ORX_DEBUG) << "Mouse destroyed." << std::endl;
  }

  /**
    @brief Destroys all the joy sticks and resizes the lists to 0.
  */
  void InputManager::_destroyJoySticks()
  {
    if (joySticksSize_ > 0)
    {
      // note: inputSystem_ can never be 0, or else the code is mistaken
      for (unsigned int i = 0; i < joySticksSize_; i++)
        if (joySticks_[i] != 0)
          inputSystem_->destroyInputObject(joySticks_[i]);

      joySticks_.clear();
      joySticksSize_ = 0;
      activeJoyStickHandlers_.clear();
      joyStickButtonsDown_.clear();
      povStates_.clear();
      sliderStates_.clear();
      joySticksCalibration_.clear();
    }
    CCOUT(ORX_DEBUG) << "Joy sticks destroyed." << std::endl;
  }

  void InputManager::_saveState()
  {
    savedHandlers_.activeHandlers_ = activeHandlers_;
    savedHandlers_.activeJoyStickHandlers_ = activeJoyStickHandlers_;
    savedHandlers_.activeKeyHandlers_ = activeKeyHandlers_;
    savedHandlers_.activeMouseHandlers_ = activeMouseHandlers_;
  }

  void InputManager::_restoreState()
  {
    activeHandlers_ = savedHandlers_.activeHandlers_;
    activeJoyStickHandlers_ = savedHandlers_.activeJoyStickHandlers_;
    activeKeyHandlers_ = savedHandlers_.activeKeyHandlers_;
    activeMouseHandlers_ = savedHandlers_.activeMouseHandlers_;
  }

  void InputManager::_updateTickables()
  {
    // we can use a map to have a list of unique pointers (an object can implement all 3 handlers)
    std::map<InputTickable*, HandlerState> tempSet;
    for (unsigned int iHandler = 0; iHandler < activeKeyHandlers_.size(); iHandler++)
      tempSet[activeKeyHandlers_[iHandler]].joyStick = true;
    for (unsigned int iHandler = 0; iHandler < activeMouseHandlers_.size(); iHandler++)
      tempSet[activeMouseHandlers_[iHandler]].mouse = true;
    for (unsigned int iJoyStick  = 0; iJoyStick < joySticksSize_; iJoyStick++)
      for (unsigned int iHandler = 0; iHandler  < activeJoyStickHandlers_[iJoyStick].size(); iHandler++)
        tempSet[activeJoyStickHandlers_[iJoyStick][iHandler]].joyStick = true;

    // copy the content of the map back to the actual vector
    activeHandlers_.clear();
    for (std::map<InputTickable*, HandlerState>::const_iterator itHandler = tempSet.begin();
        itHandler != tempSet.end(); itHandler++)
      activeHandlers_.push_back(std::pair<InputTickable*, HandlerState>((*itHandler).first, (*itHandler).second));
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

    if (state_ != stateRequest_)
    {
      InputState sr = stateRequest_;
      switch (sr)
      {
      case IS_NORMAL:
        activeKeyHandlers_.clear();
        activeMouseHandlers_.clear();
        for (unsigned int i = 0; i < joySticksSize_; i++)
          activeJoyStickHandlers_[i].clear();

        // normal play mode
        // note: we assume that the handlers exist since otherwise, something's wrong anyway.
        enableKeyHandler("keybinder");
        enableMouseHandler("keybinder");
        enableJoyStickHandler("keybinder", 0);
        stateRequest_ = IS_NORMAL;
        state_ = IS_NORMAL;
        break;

      case IS_GUI:
        state_ = IS_GUI;
        break;

      case IS_CONSOLE:
        activeKeyHandlers_.clear();
        activeMouseHandlers_.clear();
        for (unsigned int i = 0; i < joySticksSize_; i++)
          activeJoyStickHandlers_[i].clear();

        enableMouseHandler("keybinder");
        enableJoyStickHandler("keybinder", 0);
        enableKeyHandler("buffer");
        stateRequest_ = IS_CONSOLE;
        state_ = IS_CONSOLE;
        break;

      case IS_DETECT:
        savedState_ = state_;
        _saveState();

        activeKeyHandlers_.clear();
        activeMouseHandlers_.clear();
        for (unsigned int i = 0; i < joySticksSize_; i++)
          activeJoyStickHandlers_[i].clear();

        enableKeyHandler("keydetector");
        enableMouseHandler("keydetector");
        enableJoyStickHandler("keydetector", 0);

        stateRequest_ = IS_DETECT;
        state_ = IS_DETECT;
        break;

      case IS_NODETECT:
        _restoreState();
        keysDown_.clear();
        mouseButtonsDown_.clear();
        joyStickButtonsDown_[0].clear();
        state_ = IS_NODETECT;
        stateRequest_ = savedState_;
        break;

      case IS_CALIBRATE:
        if (joySticksSize_)
        {
          savedState_ = _getSingleton().state_;
          for (unsigned int i = 0; i < 24; i++)
          {
            marginalsMax_[i] = INT_MIN;
            marginalsMin_[i] = INT_MAX;
          }
          COUT(0) << "Move all joy stick axes in all directions a few times. "
            << "Then put all axes in zero state and hit enter." << std::endl;

          savedState_ = state_;
          _saveState();

          activeKeyHandlers_.clear();
          activeMouseHandlers_.clear();
          for (unsigned int i = 0; i < joySticksSize_; i++)
            activeJoyStickHandlers_[i].clear();

          enableKeyHandler("calibratorcallback");
          stateRequest_ = IS_CALIBRATE;
          state_ = IS_CALIBRATE;
        }
        else
        {
          COUT(3) << "Connot calibrate, no joy stick found!" << std::endl;
          stateRequest_ = state_;
        }
        break;

      case IS_NOCALIBRATE:
        _completeCalibration();
        _restoreState();
        keyBinder_->resetJoyStickAxes();
        state_ = IS_NOCALIBRATE;
        stateRequest_ = savedState_;
        break;

      case IS_NONE:
        activeKeyHandlers_.clear();
        activeMouseHandlers_.clear();
        for (unsigned int i = 0; i < joySticksSize_; i++)
          activeJoyStickHandlers_[i].clear();
        state_ = IS_NONE;

      default:
        break;
      }
    }

    // Capture all the input. This calls the event handlers in InputManager.
    if (mouse_)
      mouse_->capture();
    if (keyboard_)
      keyboard_->capture();
    for (unsigned  int i = 0; i < joySticksSize_; i++)
      joySticks_[i]->capture();

    if (state_ != IS_CALIBRATE)
    {
      // call all the handlers for the held key events
      for (unsigned int iKey = 0; iKey < keysDown_.size(); iKey++)
        for (unsigned int iHandler = 0; iHandler < activeKeyHandlers_.size(); iHandler++)
          activeKeyHandlers_[iHandler]->keyHeld(KeyEvent(keysDown_[iKey], keyboardModifiers_));

      // call all the handlers for the held mouse button events
      for (unsigned int iButton = 0; iButton < mouseButtonsDown_.size(); iButton++)
        for (unsigned int iHandler = 0; iHandler < activeMouseHandlers_.size(); iHandler++)
          activeMouseHandlers_[iHandler]->mouseButtonHeld(mouseButtonsDown_[iButton]);

      // call all the handlers for the held joy stick button events
      for (unsigned int iJoyStick  = 0; iJoyStick < joySticksSize_; iJoyStick++)
        for (unsigned int iButton   = 0; iButton   < joyStickButtonsDown_[iJoyStick].size(); iButton++)
          for (unsigned int iHandler = 0; iHandler  < activeJoyStickHandlers_[iJoyStick].size(); iHandler++)
            activeJoyStickHandlers_[iJoyStick][iHandler]->joyStickButtonHeld(iJoyStick, joyStickButtonsDown_[iJoyStick][iButton]);
    }

    // call the ticks for the handlers (need to be treated specially)
    for (unsigned int iHandler = 0; iHandler < activeHandlers_.size(); iHandler++)
      activeHandlers_[iHandler].first->tickInput(dt, activeHandlers_[iHandler].second);
  }

  void InputManager::_completeCalibration()
  {
    for (unsigned int i = 0; i < 24; i++)
    {
      // positive coefficient
      if (marginalsMax_[i] == INT_MIN)
        marginalsMax_[i] =  32767;
      // coefficients
      if (marginalsMax_[i] - joySticksCalibration_[0].zeroStates[i])
        joySticksCalibration_[0].positiveCoeff[i] =  1.0f/(marginalsMax_[i] - joySticksCalibration_[0].zeroStates[i]);
      else
        joySticksCalibration_[0].positiveCoeff[i] =  1.0f;

      // config value
      ConfigValueContainer* cont = getIdentifier()->getConfigValueContainer("CoeffPos");
      assert(cont);
      cont->set(i, joySticksCalibration_[0].positiveCoeff[i]);

      // negative coefficient
      if (marginalsMin_[i] == INT_MAX)
        marginalsMin_[i] = -32768;
      // coefficients
      if (marginalsMin_[i] - joySticksCalibration_[0].zeroStates[i])
        joySticksCalibration_[0].negativeCoeff[i] = -1.0f/(marginalsMin_[i] - joySticksCalibration_[0].zeroStates[i]);
      else
        joySticksCalibration_[0].negativeCoeff[i] =  1.0f;
      // config value
      cont = getIdentifier()->getConfigValueContainer("CoeffNeg");
      assert(cont);
      cont->set(i, joySticksCalibration_[0].negativeCoeff[i]);

      // zero states
      if (i < 8)
      {
        if (!(i & 1))
          joySticksCalibration_[0].zeroStates[i] = joySticks_[0]->getJoyStickState().mSliders[i/2].abX;
        else
          joySticksCalibration_[0].zeroStates[i] = joySticks_[0]->getJoyStickState().mSliders[i/2].abY;
      }
      else
      {
        if (i - 8 < joySticks_[0]->getJoyStickState().mAxes.size())
          joySticksCalibration_[0].zeroStates[i] = joySticks_[0]->getJoyStickState().mAxes[i - 8].abs;
        else
          joySticksCalibration_[0].zeroStates[i] = 0;
      }
      // config value
      cont = getIdentifier()->getConfigValueContainer("Zero");
      assert(cont);
      cont->set(i, joySticksCalibration_[0].zeroStates[i]);
    }
  }

  // ###### Key Events ######

  /**
    @brief Event handler for the keyPressed Event.
    @param e Event information
  */
  bool InputManager::keyPressed(const OIS::KeyEvent &e)
  {
    // check whether the key already is in the list (can happen when focus was lost)
    unsigned int iKey = 0;
    while (iKey < keysDown_.size() && keysDown_[iKey].key != (KeyCode::Enum)e.key)
      iKey++;
    if (iKey == keysDown_.size())
      keysDown_.push_back(Key(e));

    // update modifiers
    if(e.key == OIS::KC_RMENU    || e.key == OIS::KC_LMENU)
      keyboardModifiers_ |= KeyboardModifier::Alt;   // alt key
    if(e.key == OIS::KC_RCONTROL || e.key == OIS::KC_LCONTROL)
      keyboardModifiers_ |= KeyboardModifier::Ctrl;  // ctrl key
    if(e.key == OIS::KC_RSHIFT   || e.key == OIS::KC_LSHIFT)
      keyboardModifiers_ |= KeyboardModifier::Shift; // shift key

    for (unsigned int i = 0; i < activeKeyHandlers_.size(); i++)
      activeKeyHandlers_[i]->keyPressed(KeyEvent(e, keyboardModifiers_));

    return true;
  }

  /**
    @brief Event handler for the keyReleased Event.
    @param e Event information
  */
  bool InputManager::keyReleased(const OIS::KeyEvent &e)
  {
    // remove the key from the keysDown_ list
    for (unsigned int iKey = 0; iKey < keysDown_.size(); iKey++)
    {
      if (keysDown_[iKey].key == (KeyCode::Enum)e.key)
      {
        keysDown_.erase(keysDown_.begin() + iKey);
        break;
      }
    }

    // update modifiers
    if(e.key == OIS::KC_RMENU    || e.key == OIS::KC_LMENU)
      keyboardModifiers_ &= ~KeyboardModifier::Alt;   // alt key
    if(e.key == OIS::KC_RCONTROL || e.key == OIS::KC_LCONTROL)
      keyboardModifiers_ &= ~KeyboardModifier::Ctrl;  // ctrl key
    if(e.key == OIS::KC_RSHIFT   || e.key == OIS::KC_LSHIFT)
      keyboardModifiers_ &= ~KeyboardModifier::Shift; // shift key

    for (unsigned int i = 0; i < activeKeyHandlers_.size(); i++)
      activeKeyHandlers_[i]->keyReleased(KeyEvent(e, keyboardModifiers_));

    return true;
  }


  // ###### Mouse Events ######

  /**
    @brief Event handler for the mouseMoved Event.
    @param e Event information
  */
  bool InputManager::mouseMoved(const OIS::MouseEvent &e)
  {
    // check for actual moved event
    if (e.state.X.rel != 0 || e.state.Y.rel != 0)
    {
      for (unsigned int i = 0; i < activeMouseHandlers_.size(); i++)
        activeMouseHandlers_[i]->mouseMoved(IntVector2(e.state.X.abs, e.state.Y.abs),
            IntVector2(e.state.X.rel, e.state.Y.rel), IntVector2(e.state.width, e.state.height));
    }

    // check for mouse scrolled event
    if (e.state.Z.rel != 0)
    {
      for (unsigned int i = 0; i < activeMouseHandlers_.size(); i++)
        activeMouseHandlers_[i]->mouseScrolled(e.state.Z.abs, e.state.Z.rel);
    }

    return true;
  }

  /**
    @brief Event handler for the mousePressed Event.
    @param e Event information
    @param id The ID of the mouse button
  */
  bool InputManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
  {
    // check whether the button already is in the list (can happen when focus was lost)
    unsigned int iButton = 0;
    while (iButton < mouseButtonsDown_.size() && mouseButtonsDown_[iButton] != (MouseButton::Enum)id)
      iButton++;
    if (iButton == mouseButtonsDown_.size())
      mouseButtonsDown_.push_back((MouseButton::Enum)id);

    for (unsigned int i = 0; i < activeMouseHandlers_.size(); i++)
      activeMouseHandlers_[i]->mouseButtonPressed((MouseButton::Enum)id);

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
    for (unsigned int iButton = 0; iButton < mouseButtonsDown_.size(); iButton++)
    {
      if (mouseButtonsDown_[iButton] == (MouseButton::Enum)id)
      {
        mouseButtonsDown_.erase(mouseButtonsDown_.begin() + iButton);
        break;
      }
    }

    for (unsigned int i = 0; i < activeMouseHandlers_.size(); i++)
      activeMouseHandlers_[i]->mouseButtonReleased((MouseButton::Enum)id);

    return true;
  }


  // ###### Joy Stick Events ######

  inline unsigned int InputManager::_getJoystick(const OIS::JoyStickEvent& arg)
  {
    // use the device to identify which one called the method
    OIS::JoyStick* joyStick = (OIS::JoyStick*)arg.device;
    unsigned int iJoyStick = 0;
    while (joySticks_[iJoyStick] != joyStick)
    {
      iJoyStick++;
      if (iJoyStick == joySticksSize_)
      {
        CCOUT(3) << "Unknown joystick fired an event. This means there is a bug somewhere! Aborting." << std::endl;
        abort();
      }
    }
    return iJoyStick;
  }

  bool InputManager::buttonPressed(const OIS::JoyStickEvent &arg, int button)
  {
    unsigned int iJoyStick = _getJoystick(arg);

    // check whether the button already is in the list (can happen when focus was lost)
    std::vector<int>& buttonsDown = joyStickButtonsDown_[iJoyStick];
    unsigned int iButton = 0;
    while (iButton < buttonsDown.size() && buttonsDown[iButton] != button)
      iButton++;
    if (iButton == buttonsDown.size())
      buttonsDown.push_back(button);

    for (unsigned int iHandler = 0; iHandler < activeJoyStickHandlers_[iJoyStick].size(); iHandler++)
      activeJoyStickHandlers_[iJoyStick][iHandler]->joyStickButtonPressed(iJoyStick, button);

    return true;
  }

  bool InputManager::buttonReleased(const OIS::JoyStickEvent &arg, int button)
  {
    unsigned int iJoyStick = _getJoystick(arg);

    // remove the button from the joyStickButtonsDown_ list
    std::vector<int>& buttonsDown = joyStickButtonsDown_[iJoyStick];
    for (unsigned int iButton = 0; iButton < buttonsDown.size(); iButton++)
    {
      if (buttonsDown[iButton] == button)
      {
        buttonsDown.erase(buttonsDown.begin() + iButton);
        break;
      }
    }

    for (unsigned int iHandler = 0; iHandler < activeJoyStickHandlers_[iJoyStick].size(); iHandler++)
      activeJoyStickHandlers_[iJoyStick][iHandler]->joyStickButtonReleased(iJoyStick, button);

    return true;
  }

  void InputManager::_fireAxis(unsigned int iJoyStick, int axis, int value)
  {
    if (state_ == IS_CALIBRATE)
    {
      if (value > marginalsMax_[axis])
        marginalsMax_[axis] = value;
      if (value < marginalsMin_[axis])
        marginalsMin_[axis] = value;
    }
    else
    {
      float fValue = value - joySticksCalibration_[iJoyStick].zeroStates[axis];
      if (fValue > 0.0f)
        fValue *= joySticksCalibration_[iJoyStick].positiveCoeff[axis];
      else
        fValue *= joySticksCalibration_[iJoyStick].negativeCoeff[axis];

      for (unsigned int iHandler = 0; iHandler < activeJoyStickHandlers_[iJoyStick].size(); iHandler++)
        activeJoyStickHandlers_[iJoyStick][iHandler]->joyStickAxisMoved(iJoyStick, axis, fValue);
    }
  }

  bool InputManager::axisMoved(const OIS::JoyStickEvent &arg, int axis)
  {
    //if (arg.state.mAxes[axis].abs > 10000 || arg.state.mAxes[axis].abs < -10000)
    //{ CCOUT(3) << "axis " << axis << " moved" << arg.state.mAxes[axis].abs << std::endl;}

    unsigned int iJoyStick = _getJoystick(arg);

    // keep in mind that the first 8 axes are reserved for the sliders
    _fireAxis(iJoyStick, axis + 8, arg.state.mAxes[axis].abs);

    return true;
  }

  bool InputManager::sliderMoved(const OIS::JoyStickEvent &arg, int id)
  {
    //if (arg.state.mSliders[id].abX > 10000 || arg.state.mSliders[id].abX < -10000)
    //{CCOUT(3) << "slider " << id << " moved" << arg.state.mSliders[id].abX << std::endl;}
    //CCOUT(3) << arg.state.mSliders[id].abX << "\t |" << arg.state.mSliders[id].abY << std::endl;

    unsigned int iJoyStick = _getJoystick(arg);

    if (sliderStates_[iJoyStick].sliderStates[id].x != arg.state.mSliders[id].abX)
      _fireAxis(iJoyStick, id * 2, arg.state.mSliders[id].abX);
    else if (sliderStates_[iJoyStick].sliderStates[id].y != arg.state.mSliders[id].abY)
      _fireAxis(iJoyStick, id * 2 + 1, arg.state.mSliders[id].abY);

    return true;
  }

  bool InputManager::povMoved(const OIS::JoyStickEvent &arg, int id)
  {
    unsigned int iJoyStick = _getJoystick(arg);

    // translate the POV into 8 simple buttons
    int lastState = povStates_[iJoyStick][id];
    if (lastState & OIS::Pov::North)
      buttonReleased(arg, 32 + id * 4 + 0);
    if (lastState & OIS::Pov::South)
      buttonReleased(arg, 32 + id * 4 + 1);
    if (lastState & OIS::Pov::East)
      buttonReleased(arg, 32 + id * 4 + 2);
    if (lastState & OIS::Pov::West)
      buttonReleased(arg, 32 + id * 4 + 3);

    povStates_[iJoyStick].povStates[id] = arg.state.mPOV[id].direction;

    int currentState = povStates_[iJoyStick][id];
    if (currentState & OIS::Pov::North)
      buttonPressed(arg, 32 + id * 4 + 0);
    if (currentState & OIS::Pov::South)
      buttonPressed(arg, 32 + id * 4 + 1);
    if (currentState & OIS::Pov::East)
      buttonPressed(arg, 32 + id * 4 + 2);
    if (currentState & OIS::Pov::West)
      buttonPressed(arg, 32 + id * 4 + 3);

    return true;
  }

  /*bool InputManager::vector3Moved(const OIS::JoyStickEvent &arg, int id)
  {
    unsigned int iJoyStick = _getJoystick(arg);

    for (unsigned int iHandler = 0; iHandler < activeJoyStickHandlers_[iJoyStick].size(); iHandler++)
      activeJoyStickHandlers_[iJoyStick][iHandler]->joyStickVector3Moved(JoyStickState(arg.state, iJoyStick), id);

    return true;
  }*/


  // ################################
  // ### Static Interface Methods ###
  // ################################
  // ################################

  std::string InputManager::bindingCommmandString_s = "";

  bool InputManager::initialise(const size_t windowHnd, int windowWidth, int windowHeight,
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

  int InputManager::numberOfKeyboards()
  {
    if (_getSingleton().keyboard_ != 0)
      return 1;
    else
      return 0;
  }

  int InputManager::numberOfMice()
  {
    if (_getSingleton().mouse_ != 0)
      return 1;
    else
      return 0;
  }

  int InputManager::numberOfJoySticks()
  {
    return _getSingleton().joySticksSize_;
  }

  /*bool InputManager::isKeyDown(KeyCode::Enum key)
  {
    if (_getSingleton().keyboard_)
      return _getSingleton().keyboard_->isKeyDown((OIS::KeyCode)key);
    else
      return false;
  }*/

  /*bool InputManager::isModifierDown(KeyboardModifier::Enum modifier)
  {
    if (_getSingleton().keyboard_)
      return isModifierDown(modifier);
    else
      return false;
  }*/

  /*const MouseState InputManager::getMouseState()
  {
    if (_getSingleton().mouse_)
      return _getSingleton().mouse_->getMouseState();
    else
      return MouseState();
  }*/

  /*const JoyStickState InputManager::getJoyStickState(unsigned int ID)
  {
    if (ID < _getSingleton().joySticksSize_)
      return JoyStickState(_getSingleton().joySticks_[ID]->getJoyStickState(), ID);
    else
      return JoyStickState();
  }*/

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
  void InputManager::setWindowExtents(const int width, const int height)
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

  void InputManager::storeKeyStroke(const std::string& name)
  {
    setInputState(IS_NODETECT);
    COUT(0) << "Binding string \"" << bindingCommmandString_s << "\" on key '" << name << "'" << std::endl;
    CommandExecutor::execute("config KeyBinder " + name + " " + bindingCommmandString_s, false);
  }

  void InputManager::keyBind(const std::string& command)
  {
    bindingCommmandString_s = command;
    setInputState(IS_DETECT);
    COUT(0) << "Press any button/key or move a mouse/joystick axis" << std::endl;
  }

  void InputManager::calibrate()
  {
    _getSingleton().setInputState(IS_CALIBRATE);
  }

  // ###### KeyHandler ######

  /**
    @brief Adds a new key handler.
    @param handler Pointer to the handler object.
    @param name Unique name of the handler.
    @return True if added, false if name already existed.
  */
  bool InputManager::addKeyHandler(KeyHandler* handler, const std::string& name)
  {
    if (!handler)
      return false;
    if (_getSingleton().keyHandlers_.find(name) == _getSingleton().keyHandlers_.end())
    {
      _getSingleton().keyHandlers_[name] = handler;
      return true;
    }
    else
      return false;
  }

  /**
    @brief Removes a Key handler from the list.
    @param name Unique name of the handler.
    @return True if removal was successful, false if name was not found.
  */
  bool InputManager::removeKeyHandler(const std::string &name)
  {
    disableKeyHandler(name);
    std::map<std::string, KeyHandler*>::iterator it = _getSingleton().keyHandlers_.find(name);
    if (it != _getSingleton().keyHandlers_.end())
    {
      _getSingleton().keyHandlers_.erase(it);
      return true;
    }
    else
      return false;
  }

  /**
    @brief Returns the pointer to a handler.
    @param name Unique name of the handler.
    @return Pointer to the instance, 0 if name was not found.
  */
  KeyHandler* InputManager::getKeyHandler(const std::string& name)
  {
    std::map<std::string, KeyHandler*>::iterator it = _getSingleton().keyHandlers_.find(name);
    if (it != _getSingleton().keyHandlers_.end())
    {
      return (*it).second;
    }
    else
      return 0;
  }

  /**
    @brief Enables a specific key handler that has already been added.
    @param name Unique name of the handler.
    @return False if name was not found, true otherwise.
  */
  bool InputManager::enableKeyHandler(const std::string& name)
  {
    // get pointer from the map with all stored handlers
    std::map<std::string, KeyHandler*>::const_iterator mapIt = _getSingleton().keyHandlers_.find(name);
    if (mapIt == _getSingleton().keyHandlers_.end())
      return false;
    // see whether the handler already is in the list
    for (std::vector<KeyHandler*>::iterator it = _getSingleton().activeKeyHandlers_.begin();
          it != _getSingleton().activeKeyHandlers_.end(); it++)
    {
      if ((*it) == (*mapIt).second)
      {
        return true;
      }
    }
    _getSingleton().activeKeyHandlers_.push_back((*mapIt).second);
    _getSingleton().stateRequest_ = IS_CUSTOM;
    _getSingleton()._updateTickables();
    return true;
  }

  /**
    @brief Disables a specific key handler.
    @param name Unique name of the handler.
    @return False if name was not found, true otherwise.
  */
  bool InputManager::disableKeyHandler(const std::string &name)
  {
    // get pointer from the map with all stored handlers
    std::map<std::string, KeyHandler*>::const_iterator mapIt = _getSingleton().keyHandlers_.find(name);
    if (mapIt == _getSingleton().keyHandlers_.end())
      return false;
    // look for the handler in the list
    for (std::vector<KeyHandler*>::iterator it = _getSingleton().activeKeyHandlers_.begin();
          it != _getSingleton().activeKeyHandlers_.end(); it++)
    {
      if ((*it) == (*mapIt).second)
      {
        _getSingleton().activeKeyHandlers_.erase(it);
        _getSingleton().stateRequest_ = IS_CUSTOM;
        _getSingleton()._updateTickables();
        return true;
      }
    }
    return true;
  }

  /**
    @brief Checks whether a key handler is active
    @param name Unique name of the handler.
    @return False if key handler is not active or doesn't exist, true otherwise.
  */
  bool InputManager::isKeyHandlerActive(const std::string& name)
  {
    // get pointer from the map with all stored handlers
    std::map<std::string, KeyHandler*>::const_iterator mapIt = _getSingleton().keyHandlers_.find(name);
    if (mapIt == _getSingleton().keyHandlers_.end())
      return false;
    // see whether the handler already is in the list
    for (std::vector<KeyHandler*>::iterator it = _getSingleton().activeKeyHandlers_.begin();
          it != _getSingleton().activeKeyHandlers_.end(); it++)
    {
      if ((*it) == (*mapIt).second)
        return true;
    }
    return false;
  }


  // ###### MouseHandler ######
  /**
    @brief Adds a new mouse handler.
    @param handler Pointer to the handler object.
    @param name Unique name of the handler.
    @return True if added, false if name already existed.
  */
  bool InputManager::addMouseHandler(MouseHandler* handler, const std::string& name)
  {
    if (!handler)
      return false;
    if (_getSingleton().mouseHandlers_.find(name) == _getSingleton().mouseHandlers_.end())
    {
      _getSingleton().mouseHandlers_[name] = handler;
      return true;
    }
    else
      return false;
  }

  /**
    @brief Removes a Mouse handler from the list.
    @param name Unique name of the handler.
    @return True if removal was successful, false if name was not found.
  */
  bool InputManager::removeMouseHandler(const std::string &name)
  {
    disableMouseHandler(name);
    std::map<std::string, MouseHandler*>::iterator it = _getSingleton().mouseHandlers_.find(name);
    if (it != _getSingleton().mouseHandlers_.end())
    {
      _getSingleton().mouseHandlers_.erase(it);
      return true;
    }
    else
      return false;
  }

  /**
    @brief Returns the pointer to a handler.
    @param name Unique name of the handler.
    @return Pointer to the instance, 0 if name was not found.
  */
  MouseHandler* InputManager::getMouseHandler(const std::string& name)
  {
    std::map<std::string, MouseHandler*>::iterator it = _getSingleton().mouseHandlers_.find(name);
    if (it != _getSingleton().mouseHandlers_.end())
    {
      return (*it).second;
    }
    else
      return 0;
  }

  /**
    @brief Enables a specific mouse handler that has already been added.
    @param name Unique name of the handler.
    @return False if name was not found, true otherwise.
  */
  bool InputManager::enableMouseHandler(const std::string& name)
  {
    // get pointer from the map with all stored handlers
    std::map<std::string, MouseHandler*>::const_iterator mapIt = _getSingleton().mouseHandlers_.find(name);
    if (mapIt == _getSingleton().mouseHandlers_.end())
      return false;
    // see whether the handler already is in the list
    for (std::vector<MouseHandler*>::iterator it = _getSingleton().activeMouseHandlers_.begin();
          it != _getSingleton().activeMouseHandlers_.end(); it++)
    {
      if ((*it) == (*mapIt).second)
      {
        return true;
      }
    }
    _getSingleton().activeMouseHandlers_.push_back((*mapIt).second);
    _getSingleton().stateRequest_ = IS_CUSTOM;
    _getSingleton()._updateTickables();
    return true;
  }

  /**
    @brief Disables a specific mouse handler.
    @param name Unique name of the handler.
    @return False if name was not found, true otherwise.
  */
  bool InputManager::disableMouseHandler(const std::string &name)
  {
    // get pointer from the map with all stored handlers
    std::map<std::string, MouseHandler*>::const_iterator mapIt = _getSingleton().mouseHandlers_.find(name);
    if (mapIt == _getSingleton().mouseHandlers_.end())
      return false;
    // look for the handler in the list
    for (std::vector<MouseHandler*>::iterator it = _getSingleton().activeMouseHandlers_.begin();
          it != _getSingleton().activeMouseHandlers_.end(); it++)
    {
      if ((*it) == (*mapIt).second)
      {
        _getSingleton().activeMouseHandlers_.erase(it);
        _getSingleton().stateRequest_ = IS_CUSTOM;
        _getSingleton()._updateTickables();
        return true;
      }
    }
    return true;
  }

  /**
    @brief Checks whether a mouse handler is active
    @param name Unique name of the handler.
    @return False if key handler is not active or doesn't exist, true otherwise.
  */
  bool InputManager::isMouseHandlerActive(const std::string& name)
  {
    // get pointer from the map with all stored handlers
    std::map<std::string, MouseHandler*>::const_iterator mapIt = _getSingleton().mouseHandlers_.find(name);
    if (mapIt == _getSingleton().mouseHandlers_.end())
      return false;
    // see whether the handler already is in the list
    for (std::vector<MouseHandler*>::iterator it = _getSingleton().activeMouseHandlers_.begin();
          it != _getSingleton().activeMouseHandlers_.end(); it++)
    {
      if ((*it) == (*mapIt).second)
        return true;
    }
    return false;
  }


  // ###### JoyStickHandler ######

  /**
    @brief Adds a new joy stick handler.
    @param handler Pointer to the handler object.
    @param name Unique name of the handler.
    @return True if added, false if name already existed.
  */
  bool InputManager::addJoyStickHandler(JoyStickHandler* handler, const std::string& name)
  {
    if (!handler)
      return false;
    if (_getSingleton().joyStickHandlers_.find(name) == _getSingleton().joyStickHandlers_.end())
    {
      _getSingleton().joyStickHandlers_[name] = handler;
      return true;
    }
    else
      return false;
  }

  /**
    @brief Removes a JoyStick handler from the list.
    @param name Unique name of the handler.
    @return True if removal was successful, false if name was not found.
  */
  bool InputManager::removeJoyStickHandler(const std::string &name)
  {
    for (std::vector<OIS::JoyStick*>::iterator itstick = _getSingleton().joySticks_.begin();
          itstick != _getSingleton().joySticks_.end(); itstick++)
      disableJoyStickHandler(name, itstick - _getSingleton().joySticks_.begin());

    std::map<std::string, JoyStickHandler*>::iterator it = _getSingleton().joyStickHandlers_.find(name);
    if (it != _getSingleton().joyStickHandlers_.end())
    {
      _getSingleton().joyStickHandlers_.erase(it);
      return true;
    }
    else
      return false;
  }

  /**
    @brief Returns the pointer to a handler.
    @param name Unique name of the handler.
    @return Pointer to the instance, 0 if name was not found.
  */
  JoyStickHandler* InputManager::getJoyStickHandler(const std::string& name)
  {
    std::map<std::string, JoyStickHandler*>::iterator it = _getSingleton().joyStickHandlers_.find(name);
    if (it != _getSingleton().joyStickHandlers_.end())
    {
      return (*it).second;
    }
    else
      return 0;
  }

  /**
    @brief Enables a specific joy stick handler that has already been added.
    @param name Unique name of the handler.
    @return False if name or id was not found, true otherwise.
  */
  bool InputManager::enableJoyStickHandler(const std::string& name, unsigned int ID)
  {
    // get handler pointer from the map with all stored handlers
    std::map<std::string, JoyStickHandler*>::const_iterator handlerIt = _getSingleton().joyStickHandlers_.find(name);
    if (handlerIt == _getSingleton().joyStickHandlers_.end())
      return false;

    // check for existence of the ID
    if (ID >= _getSingleton().joySticksSize_)
      return false;

    // see whether the handler already is in the list
    for (std::vector<JoyStickHandler*>::iterator it = _getSingleton().activeJoyStickHandlers_[ID].begin();
          it != _getSingleton().activeJoyStickHandlers_[ID].end(); it++)
    {
      if ((*it) == (*handlerIt).second)
      {
        return true;
      }
    }
    _getSingleton().activeJoyStickHandlers_[ID].push_back((*handlerIt).second);
    _getSingleton().stateRequest_ = IS_CUSTOM;
    _getSingleton()._updateTickables();
    return true;
  }

  /**
    @brief Disables a specific joy stick handler.
    @param name Unique name of the handler.
    @return False if name or id was not found, true otherwise.
  */
  bool InputManager::disableJoyStickHandler(const std::string &name, unsigned int ID)
  {
    // get handler pointer from the map with all stored handlers
    std::map<std::string, JoyStickHandler*>::const_iterator handlerIt = _getSingleton().joyStickHandlers_.find(name);
    if (handlerIt == _getSingleton().joyStickHandlers_.end())
      return false;

    // check for existence of the ID
    if (ID >= _getSingleton().joySticksSize_)
      return false;

    // look for the handler in the list
    for (std::vector<JoyStickHandler*>::iterator it = _getSingleton().activeJoyStickHandlers_[ID].begin();
          it != _getSingleton().activeJoyStickHandlers_[ID].end(); it++)
    {
      if ((*it) == (*handlerIt).second)
      {
        _getSingleton().activeJoyStickHandlers_[ID].erase(it);
        _getSingleton().stateRequest_ = IS_CUSTOM;
        _getSingleton()._updateTickables();
        return true;
      }
    }
    return true;
  }

  /**
    @brief Checks whether a joy stick handler is active
    @param name Unique name of the handler.
    @return False if key handler is not active or doesn't exist, true otherwise.
  */
  bool InputManager::isJoyStickHandlerActive(const std::string& name, unsigned int ID)
  {
    // get handler pointer from the map with all stored handlers
    std::map<std::string, JoyStickHandler*>::const_iterator handlerIt = _getSingleton().joyStickHandlers_.find(name);
    if (handlerIt == _getSingleton().joyStickHandlers_.end())
      return false;

    // check for existence of the ID
    if (ID >= _getSingleton().joySticksSize_)
      return false;

    // see whether the handler already is in the list
    for (std::vector<JoyStickHandler*>::iterator it = _getSingleton().activeJoyStickHandlers_[ID].begin();
          it != _getSingleton().activeJoyStickHandlers_[ID].end(); it++)
    {
      if ((*it) == (*handlerIt).second)
        return true;
    }
    return false;
  }

}
