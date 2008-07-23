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
@brief
    Implementation of the InputManager that captures all the input from OIS
    and redirects it to handlers.
 */

#include "InputManager.h"

#include <climits>
#include <cassert>

#include "ois/OISException.h"
#include "ois/OISInputManager.h"

#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/Debug.h"
#include "core/CommandExecutor.h"
#include "core/ConsoleCommand.h"

#include "InputBuffer.h"
#include "KeyBinder.h"
#include "KeyDetector.h"
#include "CalibratorCallback.h"
#include "InputState.h"
#include "SimpleInputState.h"
#include "ExtendedInputState.h"

namespace orxonox
{
    SetConsoleCommandShortcut(InputManager, keyBind);
    SetConsoleCommandShortcut(InputManager, storeKeyStroke);
    SetConsoleCommandShortcut(InputManager, calibrate);

    std::string InputManager::bindingCommmandString_s = "";
    InputManager* InputManager::singletonRef_s = 0;

    using namespace InputDevice;

    // ############################################################
    // #####                  Initialisation                  #####
    // ##########                                        ##########
    // ############################################################

    /**
    @brief
        Constructor only sets member fields to initial zero values
        and registers the class in the class hierarchy.
    */
    InputManager::InputManager()
        : inputSystem_(0)
        , keyboard_(0)
        , mouse_(0)
        , joySticksSize_(0)
        , devicesNum_(0)
        , stateDetector_(0)
        , stateCalibrator_(0)
        , stateEmpty_(0)
        , keyboardModifiers_(0)
        , bCalibrating_(false)
    {
        RegisterRootObject(InputManager);

        assert(singletonRef_s == 0);
        singletonRef_s = this;
    }

    /**
    @brief
        Creates the OIS::InputMananger, the keyboard, the mouse and
        the joysticks and assigns the key bindings.
    @param windowHnd
        The window handle of the render window
    @param windowWidth
        The width of the render window
    @param windowHeight
        The height of the render window
    */
    bool InputManager::initialise(const size_t windowHnd, int windowWidth, int windowHeight,
                                   bool createKeyboard, bool createMouse, bool createJoySticks)
    {
        if (inputSystem_ == 0)
        {
            CCOUT(3) << "Initialising Input System..." << std::endl;
            CCOUT(4) << "Initialising OIS components..." << std::endl;

            OIS::ParamList paramList;
            std::ostringstream windowHndStr;

            // Fill parameter list
            windowHndStr << (unsigned int)windowHnd;
            paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
            //paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
            //paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
//#if defined OIS_LINUX_PLATFORM
            //paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
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

            // set all the std::vector list sizes now that the devices have been created
            _redimensionLists();

            // Set mouse/joystick region
            if (mouse_)
            {
                setWindowExtents(windowWidth, windowHeight);
            }

            CCOUT(ORX_DEBUG) << "Initialising OIS components done." << std::endl;

            setConfigValues();

            stateEmpty_ = createSimpleInputState("empty", -1);
            stateEmpty_->setHandler(new EmptyHandler());
            activeStates_[stateEmpty_->getPriority()] = stateEmpty_;

            stateDetector_ = createSimpleInputState("detector", 101);
            KeyDetector* temp = new KeyDetector();
            temp->loadBindings("storeKeyStroke");
            stateDetector_->setHandler(temp);

            stateCalibrator_ = createSimpleInputState("calibrator", 100);
            stateCalibrator_->setHandler(new EmptyHandler());
            InputBuffer* buffer = new InputBuffer();
            buffer->registerListener(this, &InputManager::_completeCalibration, '\r', true);
            stateCalibrator_->setKeyHandler(buffer);

            _updateActiveStates();

            CCOUT(3) << "Initialising complete." << std::endl;
        }
        else
        {
            CCOUT(2) << "Warning: OIS compoments already initialised, skipping" << std::endl;
        }
        return true;
    }

    /**
    @brief
        Creates a keyboard and sets the event handler.
    @return
        False if keyboard stays uninitialised, true otherwise.
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
            CCOUT(ORX_WARNING) << "Warning: Failed to create an OIS keyboard\n"
                << "OIS error message: \"" << ex.eText << "\"" << std::endl;
            keyboard_ = 0;
            return false;
        }
    }

    /**
    @brief
        Creates a mouse and sets the event handler.
    @return
        False if mouse stays uninitialised, true otherwise.
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
    @brief
        Creates all joy sticks and sets the event handler.
    @return
        False joy stick stay uninitialised, true otherwise.
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
                    OIS::JoyStick* stig = static_cast<OIS::JoyStick*>
                        (inputSystem_->createInputObject(OIS::OISJoyStick, true));
                    CCOUT(ORX_DEBUG) << "Created OIS joy stick with ID " << stig->getID() << std::endl;
                    joySticks_.push_back(stig);
                    // register our listener in OIS.
                    stig->setEventCallback(this);
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
    @brief
        Sets the size of all the different lists that are dependent on the number
        of joy stick devices created.
    @remarks
        No matter whether there are a mouse and/or keyboard, they will always
        occupy 2 places in the device number dependent lists.
    */
    void InputManager::_redimensionLists()
    {
        joySticksSize_ = joySticks_.size();
        devicesNum_ = 2 + joySticksSize_;
        joyStickButtonsDown_ .resize(joySticksSize_);
        povStates_           .resize(joySticksSize_);
        sliderStates_        .resize(joySticksSize_);
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

        // state management
        activeStatesTop_.resize(devicesNum_);

        // inform all states
        for (std::map<int, InputState*>::const_iterator it = inputStatesByPriority_.begin();
            it != inputStatesByPriority_.end(); ++it)
            (*it).second->setNumOfJoySticks(joySticksSize_);
    }

    /**
    @brief
        Sets the configurable values.
        This mainly concerns joy stick calibrations.
    */
    void InputManager::setConfigValues()
    {
        if (joySticksSize_ > 0)
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
                cont = new ConfigValueContainer(CFT_Settings, getIdentifier(), "CoeffPos", coeffPos);
                getIdentifier()->addConfigValueContainer("CoeffPos", cont);
            }
            cont->getValue(&coeffPos);

            cont = getIdentifier()->getConfigValueContainer("CoeffNeg");
            if (!cont)
            {
                cont = new ConfigValueContainer(CFT_Settings, getIdentifier(), "CoeffNeg", coeffNeg);
                getIdentifier()->addConfigValueContainer("CoeffNeg", cont);
            }
            cont->getValue(&coeffNeg);

            cont = getIdentifier()->getConfigValueContainer("Zero");
            if (!cont)
            {
                cont = new ConfigValueContainer(CFT_Settings, getIdentifier(), "Zero", zero);
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


    // ############################################################
    // #####                    Destruction                   #####
    // ##########                                        ##########
    // ############################################################

    /**
    @brief
        Destroys all the created input devices and states.
    */
    InputManager::~InputManager()
    {
        if (inputSystem_)
        {
            try
            {
                CCOUT(3) << "Destroying ..." << std::endl;

                // clear our own states
                stateEmpty_->removeAndDestroyAllHandlers();
                stateCalibrator_->removeAndDestroyAllHandlers();
                stateDetector_->removeAndDestroyAllHandlers();

                // kick all active states 'nicely'
                for (std::map<int, InputState*>::reverse_iterator rit = activeStates_.rbegin();
                    rit != activeStates_.rend(); ++rit)
                {
                    (*rit).second->onLeave();
                }
                activeStates_.clear();
                _updateActiveStates();

                // destroy all input states
                while (inputStatesByPriority_.size() > 0)
                {
                    _destroyState((*inputStatesByPriority_.rbegin()).second);
                }

                stateEmpty_ = 0;
                stateCalibrator_ = 0;
                stateDetector_ = 0;

                // destroy the devices
                _destroyKeyboard();
                _destroyMouse();
                _destroyJoySticks();

                // 0 joy sticks now
                _redimensionLists();

                OIS::InputManager::destroyInputSystem(inputSystem_);
                inputSystem_ = 0;

                CCOUT(3) << "Destroying done." << std::endl;
            }
            catch (OIS::Exception& ex)
            {
                CCOUT(1) << "An exception has occured while destroying:\n" << ex.what() << std::endl;
            }
        }
    }

    /**
    @brief
        Destroys the keyboard and sets it to 0.
    */
    void InputManager::_destroyKeyboard()
    {
        if (keyboard_)
            inputSystem_->destroyInputObject(keyboard_);
        keyboard_ = 0;
        keysDown_.clear();
        CCOUT(4) << "Keyboard destroyed." << std::endl;
    }

    /**
    @brief
        Destroys the mouse and sets it to 0.
    */
    void InputManager::_destroyMouse()
    {
        if (mouse_)
            inputSystem_->destroyInputObject(mouse_);
        mouse_ = 0;
        mouseButtonsDown_.clear();
        CCOUT(4) << "Mouse destroyed." << std::endl;
    }

    /**
    @brief
        Destroys all the joy sticks and resizes the lists to 0.
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
        }
        CCOUT(4) << "Joy sticks destroyed." << std::endl;
    }

    void InputManager::_destroyState(InputState* state)
    {
        assert(state);
        inputStatesByPriority_.erase(state->getPriority());
        inputStatesByName_.erase(state->getName());
        delete state;
    }


    // ############################################################
    // #####                  Runtime Methods                 #####
    // ##########                                        ##########
    // ############################################################

    /**
    @brief
        Updates the InputManager. Tick is called by the Core class.
    @param dt
        Delta time
    */
    void InputManager::tick(float dt)
    {
        if (inputSystem_ == 0)
            return;

        // check for states to leave (don't use unsigned int!)
        for (int i = stateLeaveRequests_.size() - 1; i >= 0; --i)
        {
            stateLeaveRequests_[i]->onLeave();
            // just to be sure that the state actually is registered
            assert(inputStatesByName_.find(stateLeaveRequests_[i]->getName()) != inputStatesByName_.end());
            
            activeStates_.erase(stateLeaveRequests_[i]->getPriority());
            _updateActiveStates();
            stateLeaveRequests_.pop_back();
        }


        // check for states to enter (don't use unsigned int!)
        for (int i = stateEnterRequests_.size() - 1; i >= 0; --i)
        {
            // just to be sure that the state actually is registered
            assert(inputStatesByName_.find(stateEnterRequests_[i]->getName()) != inputStatesByName_.end());
            
            activeStates_[stateEnterRequests_[i]->getPriority()] = stateEnterRequests_[i];
            _updateActiveStates();
            stateEnterRequests_[i]->onEnter();
            stateEnterRequests_.pop_back();
        }

        // Capture all the input. This calls the event handlers in InputManager.
        if (keyboard_)
            keyboard_->capture();
        if (mouse_)
            mouse_->capture();
        for (unsigned  int i = 0; i < joySticksSize_; i++)
            joySticks_[i]->capture();

        if (!bCalibrating_)
        {
            // call all the handlers for the held key events
            for (unsigned int iKey = 0; iKey < keysDown_.size(); iKey++)
                activeStatesTop_[Keyboard]->keyHeld(KeyEvent(keysDown_[iKey], keyboardModifiers_));

            // call all the handlers for the held mouse button events
            for (unsigned int iButton = 0; iButton < mouseButtonsDown_.size(); iButton++)
                activeStatesTop_[Mouse]->mouseButtonHeld(mouseButtonsDown_[iButton]);

            // call all the handlers for the held joy stick button events
            for (unsigned int iJoyStick  = 0; iJoyStick < joySticksSize_; iJoyStick++)
                for (unsigned int iButton   = 0; iButton   < joyStickButtonsDown_[iJoyStick].size(); iButton++)
                    activeStatesTop_[JoyStick0 + iJoyStick]
                        ->joyStickButtonHeld(iJoyStick, joyStickButtonsDown_[iJoyStick][iButton]);

            // tick the handlers for each active handler
            for (unsigned int i = 0; i < devicesNum_; ++i)
                activeStatesTop_[i]->tickInput(dt, i);

            // tick the handler with a general tick afterwards
            for (unsigned int i = 0; i < activeStatesTicked_.size(); ++i)
                activeStatesTicked_[i]->tickInput(dt);
        }
    }

    /**
    @brief
        Updates the currently active states (according to activeStates_) for each device.
        Also, a list of all active states (no duplicates!) is compiled for the general tick.
    */
    void InputManager::_updateActiveStates()
    {
        for (std::map<int, InputState*>::const_iterator it = activeStates_.begin(); it != activeStates_.end(); ++it)
            for (unsigned int i = 0; i < devicesNum_; ++i)
                if ((*it).second->isInputDeviceEnabled(i))
                    activeStatesTop_[i] = (*it).second;

        // update tickables (every state will only appear once)
        // Using a std::set to avoid duplicates
        std::set<InputState*> tempSet;
        for (unsigned int i = 0; i < devicesNum_; ++i)
            tempSet.insert(activeStatesTop_[i]);

        // copy the content of the set back to the actual vector
        activeStatesTicked_.clear();
        for (std::set<InputState*>::const_iterator it = tempSet.begin();it != tempSet.end(); ++it)
            activeStatesTicked_.push_back(*it);
    }

    /**
    @brief
        Processes the accumultated data for the joy stick calibration.
    */
    void InputManager::_completeCalibration()
    {
        for (unsigned int i = 0; i < 24; i++)
        {
            // positive coefficient
            if (marginalsMax_[i] == INT_MIN)
                marginalsMax_[i] =  32767;
            // coefficients
            if (marginalsMax_[i] - joySticksCalibration_[0].zeroStates[i])
            {
                joySticksCalibration_[0].positiveCoeff[i]
                    = 1.0f/(marginalsMax_[i] - joySticksCalibration_[0].zeroStates[i]);
            }
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
            {
                joySticksCalibration_[0].negativeCoeff[i] = -1.0f
                    / (marginalsMin_[i] - joySticksCalibration_[0].zeroStates[i]);
            }
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

        // restore old input state
        requestLeaveState("calibrator");
        bCalibrating_ = false;
    }


    // ############################################################
    // #####                    OIS events                    #####
    // ##########                                        ##########
    // ############################################################

    // ###### Key Events ######

    /**
    @brief
        Event handler for the keyPressed Event.
    @param e
        Event information
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

        activeStatesTop_[Keyboard]->keyPressed(KeyEvent(e, keyboardModifiers_));

        return true;
    }

    /**
    @brief
        Event handler for the keyReleased Event.
    @param e
        Event information
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

        activeStatesTop_[Keyboard]->keyReleased(KeyEvent(e, keyboardModifiers_));

        return true;
    }


    // ###### Mouse Events ######

    /**
    @brief
        Event handler for the mouseMoved Event.
    @param e
        Event information
    */
    bool InputManager::mouseMoved(const OIS::MouseEvent &e)
    {
        // check for actual moved event
        if (e.state.X.rel != 0 || e.state.Y.rel != 0)
        {
            activeStatesTop_[Mouse]->mouseMoved(IntVector2(e.state.X.abs, e.state.Y.abs),
                    IntVector2(e.state.X.rel, e.state.Y.rel), IntVector2(e.state.width, e.state.height));
        }

        // check for mouse scrolled event
        if (e.state.Z.rel != 0)
        {
            activeStatesTop_[Mouse]->mouseScrolled(e.state.Z.abs, e.state.Z.rel);
        }

        return true;
    }

    /**
    @brief
        Event handler for the mousePressed Event.
    @param e
        Event information
    @param id
        The ID of the mouse button
    */
    bool InputManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
    {
        // check whether the button already is in the list (can happen when focus was lost)
        unsigned int iButton = 0;
        while (iButton < mouseButtonsDown_.size() && mouseButtonsDown_[iButton] != (MouseButton::Enum)id)
            iButton++;
        if (iButton == mouseButtonsDown_.size())
            mouseButtonsDown_.push_back((MouseButton::Enum)id);

        activeStatesTop_[Mouse]->mouseButtonPressed((MouseButton::Enum)id);

        return true;
    }

    /**
    @brief
        Event handler for the mouseReleased Event.
    @param e
        Event information
    @param id
        The ID of the mouse button
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

        activeStatesTop_[Mouse]->mouseButtonReleased((MouseButton::Enum)id);

        return true;
    }


    // ###### Joy Stick Events ######

    /**
    @brief
        Returns the joy stick ID (orxonox) according to a OIS::JoyStickEvent
    */
    inline unsigned int InputManager::_getJoystick(const OIS::JoyStickEvent& arg)
    {
        // use the device to identify which one called the method
        OIS::JoyStick* joyStick = (OIS::JoyStick*)arg.device;
        unsigned int iJoyStick = 0;
        while (joySticks_[iJoyStick] != joyStick)
            iJoyStick++;
        // assert: Unknown joystick fired an event.
        assert(iJoyStick != joySticksSize_);
        return iJoyStick;
    }

    bool InputManager::buttonPressed(const OIS::JoyStickEvent &arg, int button)
    {
        unsigned int iJoyStick = _getJoystick(arg);

        // check whether the button already is in the list (can happen when focus was lost)
        std::vector<JoyStickButton::Enum>& buttonsDown = joyStickButtonsDown_[iJoyStick];
        unsigned int iButton = 0;
        while (iButton < buttonsDown.size() && buttonsDown[iButton] != button)
            iButton++;
        if (iButton == buttonsDown.size())
            buttonsDown.push_back((JoyStickButton::Enum)button);

        activeStatesTop_[2 + iJoyStick]->joyStickButtonPressed(iJoyStick, (JoyStickButton::Enum)button);

        return true;
    }

    bool InputManager::buttonReleased(const OIS::JoyStickEvent &arg, int button)
    {
        unsigned int iJoyStick = _getJoystick(arg);

        // remove the button from the joyStickButtonsDown_ list
        std::vector<JoyStickButton::Enum>& buttonsDown = joyStickButtonsDown_[iJoyStick];
        for (unsigned int iButton = 0; iButton < buttonsDown.size(); iButton++)
        {
            if (buttonsDown[iButton] == button)
            {
                buttonsDown.erase(buttonsDown.begin() + iButton);
                break;
            }
        }

        activeStatesTop_[2 + iJoyStick]->joyStickButtonReleased(iJoyStick, (JoyStickButton::Enum)button);

        return true;
    }

    /**
    @brief
        Calls the states for a particular axis with our enumeration.
        Used by OIS sliders and OIS axes.
    */
    void InputManager::_fireAxis(unsigned int iJoyStick, int axis, int value)
    {
        if (bCalibrating_)
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

            activeStatesTop_[2 + iJoyStick]->joyStickAxisMoved(iJoyStick, axis, fValue);
        }
    }

    bool InputManager::axisMoved(const OIS::JoyStickEvent &arg, int axis)
    {
        unsigned int iJoyStick = _getJoystick(arg);

        // keep in mind that the first 8 axes are reserved for the sliders
        _fireAxis(iJoyStick, axis + 8, arg.state.mAxes[axis].abs);

        return true;
    }

    bool InputManager::sliderMoved(const OIS::JoyStickEvent &arg, int id)
    {
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


    // ############################################################
    // #####         Other Public Interface Methods           #####
    // ##########                                        ##########
    // ############################################################

    /**
    @brief
        Adjusts the mouse window metrics.
        This method has to be called every time the size of the window changes.
    @param width
        The new width of the render window
    @param^height
        The new height of the render window
    */
    void InputManager::setWindowExtents(const int width, const int height)
    {
        if (mouse_)
        {
            // Set mouse region (if window resizes, we should alter this to reflect as well)
            mouse_->getMouseState().width  = width;
            mouse_->getMouseState().height = height;
        }
    }


    // ###### InputStates ######

    /**
    @brief
        Adds a new key handler.
    @param handler
        Pointer to the handler object.
    @param name
        Unique name of the handler.
    @param priority
        Unique integer number. Higher means more prioritised.
    @return
        True if added, false if name or priority already existed.
    */
    bool InputManager::_configureInputState(InputState* state, const std::string& name, int priority)
    {
        if (name == "")
            return false;
        if (!state)
            return false;
        if (inputStatesByName_.find(name) == inputStatesByName_.end())
        {
            if (inputStatesByPriority_.find(priority)
                == inputStatesByPriority_.end())
            {
                inputStatesByName_[name] = state;
                inputStatesByPriority_[priority] = state;
                state->setNumOfJoySticks(numberOfJoySticks());
                state->setName(name);
                state->setPriority(priority);
                return true;
            }
            else
            {
                COUT(2) << "Warning: Could not add an InputState with the same priority '"
                    << priority << "'." << std::endl;
                return false;
            }
        }
        else
        {
            COUT(2) << "Warning: Could not add an InputState with the same name '" << name << "'." << std::endl;
            return false;
        }
    }

    /**
    @brief
        Returns a new SimpleInputState and configures it first.
    */
    SimpleInputState* InputManager::createSimpleInputState(const std::string &name, int priority)
    {
        SimpleInputState* state = new SimpleInputState();
        if (_configureInputState(state, name, priority))
            return state;
        else
        {
            delete state;
            return 0;
        }
    }

    /**
    @brief
        Returns a new ExtendedInputState and configures it first.
    */
    ExtendedInputState* InputManager::createExtendedInputState(const std::string &name, int priority)
    {
        ExtendedInputState* state = new ExtendedInputState();
        if (_configureInputState(state, name, priority))
            return state;
        else
        {
            delete state;
            return 0;
        }
    }

    /**
    @brief
        Returns a new InputState of type 'type' and configures it first.
    @param type
        String name of the class (used by the factory)
    */
    InputState* InputManager::createInputState(const std::string& type, const std::string &name, int priority)
    {
        InputState* state = dynamic_cast<InputState*>(Factory::getIdentifier(type)->fabricate());
        if (_configureInputState(state, name, priority))
            return state;
        else
        {
            delete state;
            return 0;
        }
    }

    /**
    @brief
        Removes an input state internally.
    @param name
        Name of the handler.
    @return
        True if removal was successful, false if name was not found.
    @remarks
        You can't remove the internal states "empty", "calibrator" and "detector".
    */
    bool InputManager::destroyState(const std::string& name)
    {
        if (name == "empty" || name == "calibrator" || name == "detector")
        {
            COUT(2) << "InputManager: Removing the '" << name << "' state is not allowed!" << std::endl;
            return false;
        }
        std::map<std::string, InputState*>::iterator it = inputStatesByName_.find(name);
        if (it != inputStatesByName_.end())
        {
            _destroyState((*it).second);
            return true;
        }
        return false;
    }

    /**
    @brief
        Returns the pointer to the requested InputState.
    @param name
        Unique name of the state.
    @return
        Pointer to the instance, 0 if name was not found.
    */
    InputState* InputManager::getState(const std::string& name)
    {
        std::map<std::string, InputState*>::iterator it = inputStatesByName_.find(name);
        if (it != inputStatesByName_.end())
            return (*it).second;
        else
            return 0;
    }

    /**
    @brief
        Returns the current input state (there might be others active too!)
    @return
        The current highest prioritised active input state.
    */
    InputState* InputManager::getCurrentState()
    {
        return (*activeStates_.rbegin()).second;
    }

    /**
    @brief
        Activates a specific input state.
        It might not be really activated if the priority is too low!
    @param name
        Unique name of the state.
    @return
        False if name was not found, true otherwise.
    */
    bool InputManager::requestEnterState(const std::string& name)
    {
        // get pointer from the map with all stored handlers
        std::map<std::string, InputState*>::const_iterator it = inputStatesByName_.find(name);
        if (it != inputStatesByName_.end())
        {
            stateEnterRequests_.push_back((*it).second);
            return true;
        }
        return false;
    }

    /**
    @brief
        Deactivates a specific input state.
    @param name
        Unique name of the state.
    @return
        False if name was not found, true otherwise.
    */
    bool InputManager::requestLeaveState(const std::string& name)
    {
        // get pointer from the map with all stored handlers
        std::map<std::string, InputState*>::const_iterator it = inputStatesByName_.find(name);
        if (it != inputStatesByName_.end())
        {
            stateLeaveRequests_.push_back((*it).second);
            return true;
        }
        return false;
    }


    // ############################################################
    // #####                Console Commands                  #####
    // ##########                                        ##########
    // ############################################################

    /**
    @brief
        Method for easily storing a string with the command executor. It is used by the
        KeyDetector to get assign commands. The KeyDetector simply executes
        the command 'storeKeyStroke myName' for each button/axis.
    @remarks
        This is only a temporary hack until we thourouhgly support multiple KeyBinders.
    @param name
        The name of the button/axis.
    */
    void InputManager::storeKeyStroke(const std::string& name)
    {
        getInstance().requestLeaveState("detector");
        COUT(0) << "Binding string \"" << bindingCommmandString_s << "\" on key '" << name << "'" << std::endl;
        CommandExecutor::execute("config KeyBinder " + name + " " + bindingCommmandString_s, false);
    }

    /**
    @brief
        Assigns a command string to a key/button/axis. The name is determined via KeyDetector
        and InputManager::storeKeyStroke(.).
    @param command
        Command string that can be executed by the CommandExecutor
    */
    void InputManager::keyBind(const std::string& command)
    {
        bindingCommmandString_s = command;
        getInstance().requestEnterState("detector");
        COUT(0) << "Press any button/key or move a mouse/joystick axis" << std::endl;
    }

    /**
    @brief
        Starts joy stick calibration.
    */
    void InputManager::calibrate()
    {
        getInstance().bCalibrating_ = true;
        getInstance().requestEnterState("calibrator");
    }
}
