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

#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/CommandExecutor.h"
#include "core/ConsoleCommand.h"
#include "util/Debug.h"

#include "InputBuffer.h"
#include "KeyBinder.h"
#include "KeyDetector.h"
#include "CalibratorCallback.h"
#include "InputState.h"
#include "SimpleInputState.h"
#include "ExtendedInputState.h"
#include "JoyStickDeviceNumberListener.h"

namespace orxonox
{
    SetConsoleCommand(InputManager, calibrate, true);
    SetConsoleCommand(InputManager, reload, false);

    std::string InputManager::bindingCommmandString_s = "";
    EmptyHandler InputManager::EMPTY_HANDLER;
    InputManager* InputManager::singletonRef_s = 0;

    using namespace InputDevice;

    /**
    @brief
        Defines the |= operator for easier use.
    */
    inline InputManager::InputManagerState operator|=(InputManager::InputManagerState& lval,
                                                      InputManager::InputManagerState rval)
    {
        return (lval = (InputManager::InputManagerState)(lval | rval));
    }

    /**
    @brief
        Defines the &= operator for easier use.
    */
    inline InputManager::InputManagerState operator&=(InputManager::InputManagerState& lval, int rval)
    {
        return (lval = (InputManager::InputManagerState)(lval & rval));
    }

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
        , windowHnd_(0)
        , internalState_(Uninitialised)
        , stateEmpty_(0)
        , stateMaster_(0)
        , keyDetector_(0)
        , calibratorCallbackBuffer_(0)
        , bCalibrating_(false)
        , keyboardModifiers_(0)
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
    @param joyStickSupport
        Whether or not to load the joy sticks as well
    */
    void InputManager::initialise(size_t windowHnd, int windowWidth, int windowHeight, bool joyStickSupport)
    {
        CCOUT(3) << "Initialising Input System..." << std::endl;

        if (!(internalState_ & OISReady))
        {
            CCOUT(4) << "Initialising OIS components..." << std::endl;

            // store handle internally so we can reload OIS
            windowHnd_ = windowHnd;

            OIS::ParamList paramList;
            std::ostringstream windowHndStr;

            // Fill parameter list
            windowHndStr << (unsigned int)windowHnd_;
            paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
            //paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
            //paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
#if defined OIS_LINUX_PLATFORM
            paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

            inputSystem_ = OIS::InputManager::createInputSystem(paramList);
            CCOUT(ORX_DEBUG) << "Created OIS input system" << std::endl;

            _initialiseKeyboard();

            _initialiseMouse();

            if (joyStickSupport)
                _initialiseJoySticks();
            // Do this anyway to also inform everyone if a joystick was detached.
            _configureNumberOfJoySticks();

            // Set mouse/joystick region
            if (mouse_)
                setWindowExtents(windowWidth, windowHeight);

            // clear all buffers
            _clearBuffers();

            // load joy stick calibration
            setConfigValues();

            internalState_ |= OISReady;

            CCOUT(ORX_DEBUG) << "Initialising OIS components done." << std::endl;
        }
        else
        {
            CCOUT(2) << "Warning: OIS compoments already initialised, skipping" << std::endl;
        }

        if (!(internalState_ & InternalsReady))
        {
            CCOUT(4) << "Initialising InputStates components..." << std::endl;

            // Lowest priority empty InputState
            stateEmpty_ = createInputState<SimpleInputState>("empty", -1);
            stateEmpty_->setHandler(&EMPTY_HANDLER);
            activeStates_[stateEmpty_->getPriority()] = stateEmpty_;

            // Always active master InputState
            stateMaster_ = new ExtendedInputState();
            stateMaster_->setName("master");
            stateMaster_->setNumOfJoySticks(joySticksSize_);

            // KeyDetector to evaluate a pressed key's name
            SimpleInputState* detector = createInputState<SimpleInputState>("detector", 101);
            keyDetector_ = new KeyDetector();
            detector->setHandler(keyDetector_);

            // Joy stick calibration helper callback
            SimpleInputState* calibrator = createInputState<SimpleInputState>("calibrator", 100);
            calibrator->setHandler(&EMPTY_HANDLER);
            calibratorCallbackBuffer_ = new InputBuffer();
            calibratorCallbackBuffer_->registerListener(this, &InputManager::_completeCalibration, '\r', true);
            calibrator->setKeyHandler(calibratorCallbackBuffer_);

            internalState_ |= InternalsReady;

            CCOUT(4) << "Initialising InputStates complete." << std::endl;
        }

        _updateActiveStates();

        CCOUT(3) << "Initialising complete." << std::endl;
    }

    /**
    @brief
        Creates a keyboard and sets the event handler.
    @return
        False if keyboard stays uninitialised, true otherwise.
    */
    void InputManager::_initialiseKeyboard()
    {
        if (keyboard_ != 0)
        {
            CCOUT(2) << "Warning: Keyboard already initialised, skipping." << std::endl;
            return;
        }
        if (inputSystem_->getNumberOfDevices(OIS::OISKeyboard) > 0)
        {
            keyboard_ = (OIS::Keyboard*)inputSystem_->createInputObject(OIS::OISKeyboard, true);
            // register our listener in OIS.
            keyboard_->setEventCallback(this);
            // note: OIS will not detect keys that have already been down when the keyboard was created.
            CCOUT(ORX_DEBUG) << "Created OIS keyboard" << std::endl;
        }
        else
        {
            ThrowException(InitialisationFailed, "No keyboard found!");
        }
    }

    /**
    @brief
        Creates a mouse and sets the event handler.
    @return
        False if mouse stays uninitialised, true otherwise.
    */
    void InputManager::_initialiseMouse()
    {
        if (mouse_ != 0)
        {
            CCOUT(2) << "Warning: Mouse already initialised, skipping." << std::endl;
            return;
        }
        try
        {
            if (inputSystem_->getNumberOfDevices(OIS::OISMouse) > 0)
            {
                mouse_ = static_cast<OIS::Mouse*>(inputSystem_->createInputObject(OIS::OISMouse, true));
                // register our listener in OIS.
                mouse_->setEventCallback(this);
                CCOUT(ORX_DEBUG) << "Created OIS mouse" << std::endl;
            }
            else
            {
                CCOUT(ORX_WARNING) << "Warning: No mouse found!" << std::endl;
            }
        }
        catch (OIS::Exception ex)
        {
            CCOUT(ORX_WARNING) << "Warning: Failed to create an OIS mouse\n"
                << "OIS error message: \"" << ex.eText << "\"" << std::endl;
            mouse_ = 0;
        }
    }

    /**
    @brief
        Creates all joy sticks and sets the event handler.
    @return
        False joy stick stay uninitialised, true otherwise.
    */
    void InputManager::_initialiseJoySticks()
    {
        if (joySticksSize_ > 0)
        {
            CCOUT(2) << "Warning: Joy sticks already initialised, skipping." << std::endl;
            return;
        }
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
            //CCOUT(ORX_WARNING) << "Warning: Joy stick support requested, but no joy stick was found" << std::endl;
        }
    }

    /**
    @brief
        Sets the size of all the different lists that are dependent on the number
        of joy stick devices created.
    @remarks
        No matter whether there are a mouse and/or keyboard, they will always
        occupy 2 places in the device number dependent lists.
    */
    void InputManager::_configureNumberOfJoySticks()
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
        {
            it->second->setNumOfJoySticks(joySticksSize_);
        }
        // inform master state
        if (stateMaster_)
            this->stateMaster_->setNumOfJoySticks(joySticksSize_);

        // inform all JoyStick Device Number Listeners
        for (ObjectList<JoyStickDeviceNumberListener>::iterator it = ObjectList<JoyStickDeviceNumberListener>::begin(); it; ++it)
            it->JoyStickDeviceNumberChanged(joySticksSize_);

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
            std::vector<double> coeffPos;
            std::vector<double> coeffNeg;
            std::vector<int> zero;
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
                cont = new ConfigValueContainer(ConfigFileType::Settings, getIdentifier(), getIdentifier()->getName(), "CoeffPos", coeffPos);
                getIdentifier()->addConfigValueContainer("CoeffPos", cont);
            }
            cont->getValue(&coeffPos, this);

            cont = getIdentifier()->getConfigValueContainer("CoeffNeg");
            if (!cont)
            {
                cont = new ConfigValueContainer(ConfigFileType::Settings, getIdentifier(), getIdentifier()->getName(), "CoeffNeg", coeffNeg);
                getIdentifier()->addConfigValueContainer("CoeffNeg", cont);
            }
            cont->getValue(&coeffNeg, this);

            cont = getIdentifier()->getConfigValueContainer("Zero");
            if (!cont)
            {
                cont = new ConfigValueContainer(ConfigFileType::Settings, getIdentifier(), getIdentifier()->getName(), "Zero", zero);
                getIdentifier()->addConfigValueContainer("Zero", cont);
            }
            cont->getValue(&zero, this);

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
        if (internalState_ != Uninitialised)
        {
            try
            {
                CCOUT(3) << "Destroying ..." << std::endl;

                // kick all active states 'nicely'
                for (std::map<int, InputState*>::reverse_iterator rit = activeStates_.rbegin();
                    rit != activeStates_.rend(); ++rit)
                {
                    (*rit).second->onLeave();
                }

                // Destroy calibrator helper handler and state
                delete keyDetector_;
                requestDestroyState("calibrator");
                // Destroy KeyDetector and state
                delete calibratorCallbackBuffer_;
                requestDestroyState("detector");
                // destroy the empty InputState
                _destroyState(this->stateEmpty_);
                // destroy the master input state. This might trigger a memory leak
                // because the user has forgotten to destroy the KeyBinder or any Handler!
                delete stateMaster_;

                // destroy all user InputStates
                while (inputStatesByPriority_.size() > 0)
                    _destroyState((*inputStatesByPriority_.rbegin()).second);

                // destroy the devices
                _destroyKeyboard();
                _destroyMouse();
                _destroyJoySticks();

                OIS::InputManager::destroyInputSystem(inputSystem_);

                CCOUT(3) << "Destroying done." << std::endl;
            }
            catch (OIS::Exception& ex)
            {
                CCOUT(1) << "An exception has occured while destroying:\n" << ex.what()
                         << "This could lead to a possible memory/resource leak!" << std::endl;
            }
        }
	singletonRef_s = 0;
    }

    /**
    @brief
        Destroys the keyboard and sets it to 0.
    */
    void InputManager::_destroyKeyboard()
    {
        assert(inputSystem_);
        if (keyboard_)
            inputSystem_->destroyInputObject(keyboard_);
        keyboard_ = 0;
        CCOUT(4) << "Keyboard destroyed." << std::endl;
    }

    /**
    @brief
        Destroys the mouse and sets it to 0.
    */
    void InputManager::_destroyMouse()
    {
        assert(inputSystem_);
        if (mouse_)
            inputSystem_->destroyInputObject(mouse_);
        mouse_ = 0;
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
            assert(inputSystem_);
            for (unsigned int i = 0; i < joySticksSize_; i++)
                if (joySticks_[i] != 0)
                    inputSystem_->destroyInputObject(joySticks_[i]);

            joySticks_.clear();
            // don't use _configureNumberOfJoySticks(), might mess with registered handler if
            // downgrading from 2 to 1 joystick
            //_configureNumberOfJoySticks();
            joySticksSize_ = 0;
        }
        CCOUT(4) << "Joy sticks destroyed." << std::endl;
    }

    /**
    @brief
        Removes and destroys an InputState.
    @return
        True if state was removed immediately, false if postponed.
    */
    void InputManager::_destroyState(InputState* state)
    {
        assert(state && !(this->internalState_ & Ticking));
        std::map<int, InputState*>::iterator it = this->activeStates_.find(state->getPriority());
        if (it != this->activeStates_.end())
        {
            this->activeStates_.erase(it);
            _updateActiveStates();
        }
        inputStatesByPriority_.erase(state->getPriority());
        inputStatesByName_.erase(state->getName());
        delete state;
    }

    void InputManager::_clearBuffers()
    {
        keysDown_.clear();
        keyboardModifiers_ = 0;
        mouseButtonsDown_.clear();
        for (unsigned int i = 0; i < joySticksSize_; ++i)
        {
            joyStickButtonsDown_[i].clear();
            for (int j = 0; j < 4; ++j)
            {
                sliderStates_[i].sliderStates[j].x = 0;
                sliderStates_[i].sliderStates[j].y = 0;
                povStates_[i][j] = 0;
            }
        }
    }


    // ############################################################
    // #####                     Reloading                    #####
    // ##########                                        ##########
    // ############################################################

    /**
    @brief
        Public interface. Only reloads immediately if the call stack doesn't
        include the tick() method.
    @param joyStickSupport
        Whether or not to initialise joy sticks as well.
    */
    void InputManager::reloadInputSystem(bool joyStickSupport)
    {
        if (internalState_ & Ticking)
        {
            // We cannot destroy OIS right now, because reload was probably
            // caused by a user clicking on a GUI item. The backtrace would then
            // include an OIS method. So it would be a very bad thing to destroy it..
            internalState_ |= ReloadRequest;
            // Misuse of internalState_: We can easily store the joyStickSupport bool.
            // use Uninitialised as 0 value in order to make use of the overloaded |= operator
            internalState_ |= joyStickSupport ? JoyStickSupport : Uninitialised;
        }
        else if (internalState_ & OISReady)
        {
            _reload(joyStickSupport);
        }
        else
        {
            CCOUT(2) << "Warning: Cannot reload OIS. May not yet be initialised or"
                     << "joy sticks are currently calibrating." << std::endl;
        }
    }

    /**
    @brief
        Internal reload method. Destroys the OIS devices and loads them again.
    */
    void InputManager::_reload(bool joyStickSupport)
    {
        try
        {
            CCOUT(3) << "Reloading ..." << std::endl;

            // Save mouse clipping size
            int mouseWidth  = mouse_->getMouseState().width;
            int mouseHeight = mouse_->getMouseState().height;

            internalState_ &= ~OISReady;

            // destroy the devices
            _destroyKeyboard();
            _destroyMouse();
            _destroyJoySticks();

            OIS::InputManager::destroyInputSystem(inputSystem_);
            inputSystem_ = 0;

            // clear all buffers containing input information
            _clearBuffers();

            initialise(windowHnd_, mouseWidth, mouseHeight, joyStickSupport);

            CCOUT(3) << "Reloading done." << std::endl;
        }
        catch (OIS::Exception& ex)
        {
            CCOUT(1) << "An exception has occured while reloading:\n" << ex.what() << std::endl;
        }
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
        if (internalState_ == Uninitialised)
            return;
        else if (internalState_ & ReloadRequest)
        {
            _reload(internalState_ & JoyStickSupport);
            internalState_ &= ~ReloadRequest;
            internalState_ &= ~JoyStickSupport;
        }

        // check for states to leave
        for (std::set<InputState*>::reverse_iterator rit = stateLeaveRequests_.rbegin();
            rit != stateLeaveRequests_.rend(); ++rit)
        {
            (*rit)->onLeave();
            // just to be sure that the state actually is registered
            assert(inputStatesByName_.find((*rit)->getName()) != inputStatesByName_.end());

            activeStates_.erase((*rit)->getPriority());
            _updateActiveStates();
        }
        stateLeaveRequests_.clear();

        // check for states to enter
        for (std::set<InputState*>::reverse_iterator rit = stateEnterRequests_.rbegin();
            rit != stateEnterRequests_.rend(); ++rit)
        {
            // just to be sure that the state actually is registered
            assert(inputStatesByName_.find((*rit)->getName()) != inputStatesByName_.end());

            activeStates_[(*rit)->getPriority()] = (*rit);
            _updateActiveStates();
            (*rit)->onEnter();
        }
        stateEnterRequests_.clear();

        // check for states to destroy
        for (std::set<InputState*>::reverse_iterator rit = stateDestroyRequests_.rbegin();
            rit != stateDestroyRequests_.rend(); ++rit)
        {
            _destroyState((*rit));
        }
        stateDestroyRequests_.clear();

        // check whether a state has changed its EMPTY_HANDLER situation
        bool bUpdateRequired = false;
        for (std::map<int, InputState*>::iterator it = activeStates_.begin(); it != activeStates_.end(); ++it)
        {
            if (it->second->handlersChanged())
            {
                it->second->resetHandlersChanged();
                bUpdateRequired = true;
            }
        }
        if (bUpdateRequired)
            _updateActiveStates();

        // mark that we capture and distribute input
        internalState_ |= Ticking;

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
            {
                KeyEvent kEvt(keysDown_[iKey], keyboardModifiers_);
                activeStatesTop_[Keyboard]->keyHeld(kEvt);
                stateMaster_->keyHeld(kEvt);
            }

            // call all the handlers for the held mouse button events
            for (unsigned int iButton = 0; iButton < mouseButtonsDown_.size(); iButton++)
            {
                activeStatesTop_[Mouse]->mouseButtonHeld(mouseButtonsDown_[iButton]);
                stateMaster_->mouseButtonHeld(mouseButtonsDown_[iButton]);
            }

            // call all the handlers for the held joy stick button events
            for (unsigned int iJoyStick  = 0; iJoyStick < joySticksSize_; iJoyStick++)
                for (unsigned int iButton   = 0; iButton   < joyStickButtonsDown_[iJoyStick].size(); iButton++)
                {
                    activeStatesTop_[JoyStick0 + iJoyStick]
                        ->joyStickButtonHeld(iJoyStick, joyStickButtonsDown_[iJoyStick][iButton]);
                    stateMaster_->joyStickButtonHeld(iJoyStick, joyStickButtonsDown_[iJoyStick][iButton]);
                }

            // tick the handlers for each active handler
            for (unsigned int i = 0; i < devicesNum_; ++i)
            {
                activeStatesTop_[i]->tickInput(dt, i);
                if (stateMaster_->isInputDeviceEnabled(i))
                    stateMaster_->tickInput(dt, i);
            }

            // tick the handler with a general tick afterwards
            for (unsigned int i = 0; i < activeStatesTicked_.size(); ++i)
                activeStatesTicked_[i]->tickInput(dt);
            stateMaster_->tickInput(dt);
        }

        internalState_ &= ~Ticking;
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
                if (it->second->isInputDeviceEnabled(i))
                    activeStatesTop_[i] = it->second;

        // update tickables (every state will only appear once)
        // Using a std::set to avoid duplicates
        std::set<InputState*> tempSet;
        for (unsigned int i = 0; i < devicesNum_; ++i)
            tempSet.insert(activeStatesTop_[i]);

        // copy the content of the set back to the actual vector
        activeStatesTicked_.clear();
        for (std::set<InputState*>::const_iterator it = tempSet.begin();it != tempSet.end(); ++it)
            activeStatesTicked_.push_back(*it);

        this->mouseButtonsDown_.clear();
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

    void InputManager::clearBuffers()
    {
        this->keysDown_.clear();
        this->mouseButtonsDown_.clear();
        for (unsigned int i = 0; i < this->joySticksSize_; ++i)
            this->joyStickButtonsDown_[i].clear();
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
        while (iKey < keysDown_.size() && keysDown_[iKey].key != (KeyCode::ByEnum)e.key)
            iKey++;
        if (iKey == keysDown_.size())
            keysDown_.push_back(Key(e));
        else
        {
            // This happens when XAutoRepeat is set under linux. The KeyPressed event gets then sent
            // continuously.
            return true;
        }

        // update modifiers
        if(e.key == OIS::KC_RMENU    || e.key == OIS::KC_LMENU)
            keyboardModifiers_ |= KeyboardModifier::Alt;   // alt key
        if(e.key == OIS::KC_RCONTROL || e.key == OIS::KC_LCONTROL)
            keyboardModifiers_ |= KeyboardModifier::Ctrl;  // ctrl key
        if(e.key == OIS::KC_RSHIFT   || e.key == OIS::KC_LSHIFT)
            keyboardModifiers_ |= KeyboardModifier::Shift; // shift key

        KeyEvent kEvt(e, keyboardModifiers_);
        activeStatesTop_[Keyboard]->keyPressed(kEvt);
        stateMaster_->keyPressed(kEvt);

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
            if (keysDown_[iKey].key == (KeyCode::ByEnum)e.key)
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

        KeyEvent kEvt(e, keyboardModifiers_);
        activeStatesTop_[Keyboard]->keyReleased(kEvt);
        stateMaster_->keyReleased(kEvt);

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
            IntVector2 abs(e.state.X.abs, e.state.Y.abs);
            IntVector2 rel(e.state.X.rel, e.state.Y.rel);
            IntVector2 clippingSize(e.state.width, e.state.height);
            activeStatesTop_[Mouse]->mouseMoved(abs, rel, clippingSize);
            stateMaster_->mouseMoved(abs, rel, clippingSize);
        }

        // check for mouse scrolled event
        if (e.state.Z.rel != 0)
        {
            activeStatesTop_[Mouse]->mouseScrolled(e.state.Z.abs, e.state.Z.rel);
            stateMaster_->mouseScrolled(e.state.Z.abs, e.state.Z.rel);
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
        while (iButton < mouseButtonsDown_.size() && mouseButtonsDown_[iButton] != (MouseButtonCode::ByEnum)id)
            iButton++;
        if (iButton == mouseButtonsDown_.size())
            mouseButtonsDown_.push_back((MouseButtonCode::ByEnum)id);

        activeStatesTop_[Mouse]->mouseButtonPressed((MouseButtonCode::ByEnum)id);
        stateMaster_->mouseButtonPressed((MouseButtonCode::ByEnum)id);

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
            if (mouseButtonsDown_[iButton] == (MouseButtonCode::ByEnum)id)
            {
                mouseButtonsDown_.erase(mouseButtonsDown_.begin() + iButton);
                break;
            }
        }

        activeStatesTop_[Mouse]->mouseButtonReleased((MouseButtonCode::ByEnum)id);
        stateMaster_->mouseButtonReleased((MouseButtonCode::ByEnum)id);

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
        std::vector<JoyStickButtonCode::ByEnum>& buttonsDown = joyStickButtonsDown_[iJoyStick];
        unsigned int iButton = 0;
        while (iButton < buttonsDown.size() && buttonsDown[iButton] != button)
            iButton++;
        if (iButton == buttonsDown.size())
            buttonsDown.push_back((JoyStickButtonCode::ByEnum)button);

        activeStatesTop_[2 + iJoyStick]->joyStickButtonPressed(iJoyStick, (JoyStickButtonCode::ByEnum)button);
        stateMaster_->joyStickButtonPressed(iJoyStick, (JoyStickButtonCode::ByEnum)button);

        return true;
    }

    bool InputManager::buttonReleased(const OIS::JoyStickEvent &arg, int button)
    {
        unsigned int iJoyStick = _getJoystick(arg);

        // remove the button from the joyStickButtonsDown_ list
        std::vector<JoyStickButtonCode::ByEnum>& buttonsDown = joyStickButtonsDown_[iJoyStick];
        for (unsigned int iButton = 0; iButton < buttonsDown.size(); iButton++)
        {
            if (buttonsDown[iButton] == button)
            {
                buttonsDown.erase(buttonsDown.begin() + iButton);
                break;
            }
        }

        activeStatesTop_[2 + iJoyStick]->joyStickButtonReleased(iJoyStick, (JoyStickButtonCode::ByEnum)button);
        stateMaster_->joyStickButtonReleased(iJoyStick, (JoyStickButtonCode::ByEnum)button);

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
            stateMaster_->joyStickAxisMoved(iJoyStick, axis, fValue);
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

    /**
    @brief
        Sets the the name of the command used by the KeyDetector as callback.
    @param command
        Command name as string
    */
    void InputManager::setKeyDetectorCallback(const std::string& command)
    {
        this->keyDetector_->setCallbackCommand(command);
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
        Removes and destroys an input state internally.
    @param name
        Name of the handler.
    @return
        True if removal was successful, false if name was not found.
    @remarks
        You can't remove the internal states "empty", "calibrator" and "detector".
        The removal process is being postponed if InputManager::tick() is currently running.
    */
    bool InputManager::requestDestroyState(const std::string& name)
    {
        if (name == "empty")
        {
            COUT(2) << "InputManager: Removing the empty state is not allowed!" << std::endl;
            return false;
        }
        std::map<std::string, InputState*>::iterator it = inputStatesByName_.find(name);
        if (it != inputStatesByName_.end())
        {
            if (activeStates_.find(it->second->getPriority()) != activeStates_.end())
            {
                // The state is still active. We have to postpone
                stateLeaveRequests_.insert(it->second);
                stateDestroyRequests_.insert(it->second);
            }
            else if (this->internalState_ & Ticking)
            {
                // cannot remove state while ticking
                stateDestroyRequests_.insert(it->second);
            }
            else
                _destroyState(it->second);

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
            return it->second;
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
            // exists
            if (activeStates_.find(it->second->getPriority()) == activeStates_.end())
            {
                // not active
                if (stateDestroyRequests_.find(it->second) == stateDestroyRequests_.end())
                {
                    // not scheduled for destruction
                    // set prevents a state being added multiple times
                    stateEnterRequests_.insert(it->second);
                    return true;
                }
            }
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
            // exists
            if (activeStates_.find(it->second->getPriority()) != activeStates_.end())
            {
                // active
                stateLeaveRequests_.insert(it->second);
                return true;
            }
        }
        return false;
    }


    // ############################################################
    // #####                Console Commands                  #####
    // ##########                                        ##########
    // ############################################################

    /**
    @brief
        Starts joy stick calibration.
    */
    void InputManager::calibrate()
    {
        getInstance().bCalibrating_ = true;
        getInstance().requestEnterState("calibrator");
    }

    /**
    @brief
        Reloads the input system
    */
    void InputManager::reload(bool joyStickSupport)
    {
        getInstance().reloadInputSystem(joyStickSupport);
    }
}
