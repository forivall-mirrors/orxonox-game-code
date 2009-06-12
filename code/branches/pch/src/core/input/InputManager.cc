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
#include <ois/OISException.h>
#include <ois/OISInputManager.h>

#include "util/Exception.h"
#include "util/Debug.h"
#include "core/Clock.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/CommandLine.h"

#include "InputBuffer.h"
#include "KeyDetector.h"
#include "InputState.h"
#include "SimpleInputState.h"
#include "ExtendedInputState.h"
#include "JoyStickDeviceNumberListener.h"

// HACK (include this as last, X11 seems to define some macros...)
#ifdef ORXONOX_PLATFORM_LINUX
#  include <ois/linux/LinuxMouse.h>
#endif

namespace orxonox
{
    SetConsoleCommand(InputManager, calibrate, true);
    SetConsoleCommand(InputManager, reload, false);
#ifdef ORXONOX_PLATFORM_LINUX
    SetConsoleCommand(InputManager, grabMouse, true);
    SetConsoleCommand(InputManager, ungrabMouse, true);
#endif
    SetCommandLineSwitch(keyboard_no_grab);

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
        , keyDetector_(0)
        , calibratorCallbackBuffer_(0)
        , keyboardModifiers_(0)
    {
        RegisterRootObject(InputManager);

        assert(singletonRef_s == 0);
        singletonRef_s = this;

        setConfigValues();
    }

    /**
    @brief
        Sets the configurable values.
    */
    void InputManager::setConfigValues()
    {
        SetConfigValue(calibrationFilename_, "joystick_calibration.ini")
            .description("Ini filename for the the joy stick calibration data.")
            .callback(this, &InputManager::_calibrationFileCallback);
    }

    /**
    @brief
        Callback for the joy stick calibration config file. @see setConfigValues.
    */
    void InputManager::_calibrationFileCallback()
    {
        ConfigFileManager::getInstance().setFilename(ConfigFileType::JoyStickCalibration, calibrationFilename_);
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
#if defined ORXONOX_PLATFORM_LINUX
            paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
            paramList.insert(std::make_pair(std::string("x11_mouse_grab"), "true"));
            paramList.insert(std::make_pair(std::string("x11_mouse_hide"), "true"));
            bool kbNoGrab;
            CommandLine::getValue("keyboard_no_grab", &kbNoGrab);
            if (kbNoGrab)
                paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
            else
                paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("true")));
#endif

            inputSystem_ = OIS::InputManager::createInputSystem(paramList);
            CCOUT(ORX_DEBUG) << "Created OIS input system" << std::endl;

            _initialiseKeyboard();

            _initialiseMouse();

            if (joyStickSupport)
                _initialiseJoySticks();
            // Do this anyway to also inform everything when a joystick was detached.
            _configureJoySticks();

            // Set mouse/joystick region
            if (mouse_)
                setWindowExtents(windowWidth, windowHeight);

            // clear all buffers
            _clearBuffers();

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
            stateEmpty_ = createInputState<SimpleInputState>("empty", false, false, InputStatePriority::Empty);
            stateEmpty_->setHandler(&EMPTY_HANDLER);
            activeStates_[stateEmpty_->getPriority()] = stateEmpty_;

            // KeyDetector to evaluate a pressed key's name
            SimpleInputState* detector = createInputState<SimpleInputState>("detector", false, false, InputStatePriority::Detector);
            keyDetector_ = new KeyDetector();
            detector->setHandler(keyDetector_);

            // Joy stick calibration helper callback
            SimpleInputState* calibrator = createInputState<SimpleInputState>("calibrator", false, false, InputStatePriority::Calibrator);
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
        Helper function that loads the config value vector of one coefficient
    */
    void loadCalibration(std::vector<int>& list, const std::string& sectionName, const std::string& valueName, size_t size, int defaultValue)
    {
        list.resize(size);
        unsigned int configValueVectorSize = ConfigFileManager::getInstance().getVectorSize(ConfigFileType::JoyStickCalibration, sectionName, valueName);
        if (configValueVectorSize > size)
            configValueVectorSize = size;

        for (unsigned int i = 0; i < configValueVectorSize; ++i)
        {
            list[i] = multi_cast<int>(ConfigFileManager::getInstance().getValue(
                ConfigFileType::JoyStickCalibration, sectionName, valueName, i, multi_cast<std::string>(defaultValue), false));
        }

        // fill the rest with default values
        for (unsigned int i = configValueVectorSize; i < size; ++i)
        {
            list[i] = defaultValue;
        }
    }

    /**
    @brief
        Sets the size of all the different lists that are dependent on the number
        of joy stick devices created and loads the joy stick calibration.
    @remarks
        No matter whether there are a mouse and/or keyboard, they will always
        occupy 2 places in the device number dependent lists.
    */
    void InputManager::_configureJoySticks()
    {
        joySticksSize_ = joySticks_.size();
        devicesNum_    = 2 + joySticksSize_;
        joyStickIDs_         .resize(joySticksSize_);
        joyStickButtonsDown_ .resize(joySticksSize_);
        povStates_           .resize(joySticksSize_);
        sliderStates_        .resize(joySticksSize_);
        joyStickMinValues_   .resize(joySticksSize_);
        joyStickMaxValues_   .resize(joySticksSize_);
        joyStickMiddleValues_.resize(joySticksSize_);
        joyStickCalibrations_.resize(joySticksSize_);

        for (unsigned int iJoyStick = 0; iJoyStick < joySticksSize_; iJoyStick++)
        {
            // Generate some sort of execution unique id per joy stick
            std::string id = "JoyStick_";
            id += multi_cast<std::string>(joySticks_[iJoyStick]->getNumberOfComponents(OIS::OIS_Button))  + "_";
            id += multi_cast<std::string>(joySticks_[iJoyStick]->getNumberOfComponents(OIS::OIS_Axis))    + "_";
            id += multi_cast<std::string>(joySticks_[iJoyStick]->getNumberOfComponents(OIS::OIS_Slider))  + "_";
            id += multi_cast<std::string>(joySticks_[iJoyStick]->getNumberOfComponents(OIS::OIS_POV))     + "_";
            id += multi_cast<std::string>(joySticks_[iJoyStick]->getNumberOfComponents(OIS::OIS_Vector3)) + "_";
            id += joySticks_[iJoyStick]->vendor();
            for (unsigned int i = 0; i < iJoyStick; ++i)
            {
                if (id == joyStickIDs_[i])
                {
                    // Two joysticks are probably equal --> add the index as well
                    id += "_" + multi_cast<std::string>(iJoyStick);
                }
            }
            joyStickIDs_[iJoyStick] = id;

            size_t axes = sliderAxes + (size_t)this->joySticks_[iJoyStick]->getNumberOfComponents(OIS::OIS_Axis);
            loadCalibration(joyStickMinValues_[iJoyStick], id, "MinValue", axes, -32768);
            loadCalibration(joyStickMaxValues_[iJoyStick], id, "MaxValue", axes,  32768);
            loadCalibration(joyStickMiddleValues_[iJoyStick], id, "MiddleValue", axes,      0);
        }

        _evaluateCalibration();

        // state management
        activeStatesTriggered_.resize(devicesNum_);

        // inform all states
        for (std::map<std::string, InputState*>::const_iterator it = inputStatesByName_.begin();
            it != inputStatesByName_.end(); ++it)
        {
            it->second->setNumOfJoySticks(joySticksSize_);
        }

        // inform all JoyStick Device Number Listeners
        for (ObjectList<JoyStickDeviceNumberListener>::iterator it = ObjectList<JoyStickDeviceNumberListener>::begin(); it; ++it)
            it->JoyStickDeviceNumberChanged(joySticksSize_);

    }

    void InputManager::_evaluateCalibration()
    {
        for (unsigned int iJoyStick = 0; iJoyStick < this->joySticksSize_; ++iJoyStick)
        {
            for (unsigned int i = 0; i < this->joyStickMinValues_[iJoyStick].size(); i++)
            {
                this->joyStickCalibrations_[iJoyStick].middleValue[i] = this->joyStickMiddleValues_[iJoyStick][i];
                this->joyStickCalibrations_[iJoyStick].negativeCoeff[i] = - 1.0f / (this->joyStickMinValues_[iJoyStick][i] - this->joyStickMiddleValues_[iJoyStick][i]);
                this->joyStickCalibrations_[iJoyStick].positiveCoeff[i] =   1.0f / (this->joyStickMaxValues_[iJoyStick][i] - this->joyStickMiddleValues_[iJoyStick][i]);
            }
        }
    }

    void InputManager::_startCalibration()
    {
        for (unsigned int iJoyStick = 0; iJoyStick < this->joySticksSize_; ++iJoyStick)
        {
            // Set initial values
            for (unsigned int i = 0; i < this->joyStickMinValues_[iJoyStick].size(); ++i)
                this->joyStickMinValues_[iJoyStick][i] = INT_MAX;
            for (unsigned int i = 0; i < this->joyStickMaxValues_[iJoyStick].size(); ++i)
                this->joyStickMaxValues_[iJoyStick][i] = INT_MIN;
            for (unsigned int i = 0; i < this->joyStickMiddleValues_[iJoyStick].size(); ++i)
                this->joyStickMiddleValues_[iJoyStick][i] = 0;
        }

        getInstance().internalState_ |= Calibrating;
        getInstance().requestEnterState("calibrator");
    }

    void InputManager::_completeCalibration()
    {
        for (unsigned int iJoyStick = 0; iJoyStick < this->joySticksSize_; ++iJoyStick)
        {
            // Get the middle positions now
            unsigned int iAxis = 0;
            for (unsigned int i = 0; i < sliderAxes/2; ++i)
            {
                this->joyStickMiddleValues_[iJoyStick][iAxis++] = this->joySticks_[iJoyStick]->getJoyStickState().mSliders[i].abX;
                this->joyStickMiddleValues_[iJoyStick][iAxis++] = this->joySticks_[iJoyStick]->getJoyStickState().mSliders[i].abY;
            }
            // Note: joyStickMiddleValues_[iJoyStick] was already correctly resized in _configureJoySticks()
            assert(joySticks_[iJoyStick]->getJoyStickState().mAxes.size() == joyStickMiddleValues_[iJoyStick].size() - sliderAxes);
            for (unsigned int i = 0; i < joyStickMiddleValues_[iJoyStick].size() - sliderAxes; ++i)
            {
                this->joyStickMiddleValues_[iJoyStick][iAxis++] = this->joySticks_[iJoyStick]->getJoyStickState().mAxes[i].abs;
            }

            for (unsigned int i = 0; i < joyStickMinValues_[iJoyStick].size(); ++i)
            {
                // Minimum values
                if (joyStickMinValues_[iJoyStick][i] == INT_MAX)
                    joyStickMinValues_[iJoyStick][i] = -32768;
                ConfigFileManager::getInstance().setValue(ConfigFileType::JoyStickCalibration,
                    this->joyStickIDs_[iJoyStick], "MinValue", i, multi_cast<std::string>(joyStickMinValues_[iJoyStick][i]), false);

                // Maximum values
                if (joyStickMaxValues_[iJoyStick][i] == INT_MIN)
                    joyStickMaxValues_[iJoyStick][i] = 32767;
                ConfigFileManager::getInstance().setValue(ConfigFileType::JoyStickCalibration,
                    this->joyStickIDs_[iJoyStick], "MaxValue", i, multi_cast<std::string>(joyStickMaxValues_[iJoyStick][i]), false);

                // Middle values
                ConfigFileManager::getInstance().setValue(ConfigFileType::JoyStickCalibration,
                    this->joyStickIDs_[iJoyStick], "MiddleValue", i, multi_cast<std::string>(joyStickMiddleValues_[iJoyStick][i]), false);
            }
        }

        _evaluateCalibration();

        // restore old input state
        requestLeaveState("calibrator");
        internalState_ &= ~Calibrating;
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

                // destroy all user InputStates
                while (inputStatesByName_.size() > 0)
                    _destroyState((*inputStatesByName_.rbegin()).second);

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
        include the update() method.
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
        Updates the states and the InputState situation.
    @param time
        Clock holding the current time.
    */
    void InputManager::update(const Clock& time)
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
        if (!stateLeaveRequests_.empty())
        {
            for (std::set<InputState*>::iterator it = stateLeaveRequests_.begin();
                it != stateLeaveRequests_.end(); ++it)
            {
                (*it)->onLeave();
                // just to be sure that the state actually is registered
                assert(inputStatesByName_.find((*it)->getName()) != inputStatesByName_.end());

                activeStates_.erase((*it)->getPriority());
                if ((*it)->getPriority() < InputStatePriority::HighPriority)
                    (*it)->setPriority(0);
                _updateActiveStates();
            }
            stateLeaveRequests_.clear();
        }

        // check for states to enter
        if (!stateEnterRequests_.empty())
        {
            for (std::set<InputState*>::const_iterator it = stateEnterRequests_.begin();
                it != stateEnterRequests_.end(); ++it)
            {
                // just to be sure that the state actually is registered
                assert(inputStatesByName_.find((*it)->getName()) != inputStatesByName_.end());

                if ((*it)->getPriority() == 0)
                {
                    // Get smallest possible priority between 1 and maxStateStackSize_s
#if defined( __MINGW32__ ) // Avoid the strange mingw-stl bug with const_reverse_iterator
                    for(std::map<int, InputState*>::reverse_iterator rit = activeStates_.rbegin();
                        rit != activeStates_.rend(); ++rit)
#else
                    for(std::map<int, InputState*>::const_reverse_iterator rit = activeStates_.rbegin();
                        rit != activeStates_.rend(); ++rit)
#endif
                    {
                        if (rit->first < InputStatePriority::HighPriority)
                        {
                            (*it)->setPriority(rit->first + 1);
                            break;
                        }
                    }
                    // In case no normal handler was on the stack
                    if ((*it)->getPriority() == 0)
                        (*it)->setPriority(1);
                }
                activeStates_[(*it)->getPriority()] = (*it);
                _updateActiveStates();
                (*it)->onEnter();
            }
            stateEnterRequests_.clear();
        }

        // check for states to destroy
        if (!stateDestroyRequests_.empty())
        {
            for (std::set<InputState*>::iterator it = stateDestroyRequests_.begin();
                it != stateDestroyRequests_.end(); ++it)
            {
                _destroyState((*it));
            }
            stateDestroyRequests_.clear();
        }

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

        // mark that we now start capturing and distributing input
        internalState_ |= Ticking;

        // Capture all the input. This calls the event handlers in InputManager.
        if (keyboard_)
            keyboard_->capture();
        if (mouse_)
            mouse_->capture();
        for (unsigned  int i = 0; i < joySticksSize_; i++)
            joySticks_[i]->capture();

        if (!(internalState_ & Calibrating))
        {
            // call all the handlers for the held key events
            for (unsigned int iKey = 0; iKey < keysDown_.size(); iKey++)
            {
                KeyEvent kEvt(keysDown_[iKey], keyboardModifiers_);

                for (unsigned int iState = 0; iState < activeStatesTriggered_[Keyboard].size(); ++iState)
                    activeStatesTriggered_[Keyboard][iState]->keyHeld(kEvt);
            }

            // call all the handlers for the held mouse button events
            for (unsigned int iButton = 0; iButton < mouseButtonsDown_.size(); iButton++)
            {
                for (unsigned int iState = 0; iState < activeStatesTriggered_[Mouse].size(); ++iState)
                    activeStatesTriggered_[Mouse][iState]->mouseButtonHeld(mouseButtonsDown_[iButton]);
            }

            // call all the handlers for the held joy stick button events
            for (unsigned int iJoyStick  = 0; iJoyStick < joySticksSize_; iJoyStick++)
                for (unsigned int iButton   = 0; iButton   < joyStickButtonsDown_[iJoyStick].size(); iButton++)
                {
                    for (unsigned int iState = 0; iState < activeStatesTriggered_[JoyStick0 + iJoyStick].size(); ++iState)
                        activeStatesTriggered_[JoyStick0 + iJoyStick][iState]->joyStickButtonHeld(iJoyStick, joyStickButtonsDown_[iJoyStick][iButton]);
                }

            // update the handlers for each active handler
            for (unsigned int i = 0; i < devicesNum_; ++i)
            {
                for (unsigned int iState = 0; iState < activeStatesTriggered_[i].size(); ++iState)
                    activeStatesTriggered_[i][iState]->updateInput(time.getDeltaTime(), i);
            }

            // update the handler with a general tick afterwards
            for (unsigned int i = 0; i < activeStatesTicked_.size(); ++i)
                activeStatesTicked_[i]->updateInput(time.getDeltaTime());
        }

        internalState_ &= ~Ticking;
    }

    /**
    @brief
        Updates the currently active states (according to activeStates_) for each device.
        Also, a list of all active states (no duplicates!) is compiled for the general update().
    */
    void InputManager::_updateActiveStates()
    {
        for (unsigned int i = 0; i < devicesNum_; ++i)
        {
            bool occupied = false;
            activeStatesTriggered_[i].clear();
#if defined( __MINGW32__ ) // Avoid the strange mingw-stl bug with const_reverse_iterator
            for (std::map<int, InputState*>::reverse_iterator rit = activeStates_.rbegin(); rit != activeStates_.rend(); ++rit)
            {
#else
            for (std::map<int, InputState*>::const_reverse_iterator rit = activeStates_.rbegin(); rit != activeStates_.rend(); ++rit)
            {
#endif
                if (rit->second->isInputDeviceEnabled(i) && (!occupied || rit->second->bAlwaysGetsInput_))
                {
                    activeStatesTriggered_[i].push_back(rit->second);
                    if (!rit->second->bTransparent_)
                        occupied = true;
                }
            }
        }

        // update tickables (every state will only appear once)
        // Using a std::set to avoid duplicates
        std::set<InputState*> tempSet;
        for (unsigned int i = 0; i < devicesNum_; ++i)
            for (unsigned int iState = 0; iState < activeStatesTriggered_[i].size(); ++iState)
                tempSet.insert(activeStatesTriggered_[i][iState]);

        // copy the content of the std::set back to the actual vector
        activeStatesTicked_.clear();
        for (std::set<InputState*>::const_iterator it = tempSet.begin();it != tempSet.end(); ++it)
            activeStatesTicked_.push_back(*it);

        this->mouseButtonsDown_.clear();
    }

    /**
    @brief
        Clears all buffers that store what keys/buttons are being pressed at the moment.
    */
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
        for (unsigned int iState = 0; iState < activeStatesTriggered_[Keyboard].size(); ++iState)
            activeStatesTriggered_[Keyboard][iState]->keyPressed(kEvt);

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
        for (unsigned int iState = 0; iState < activeStatesTriggered_[Keyboard].size(); ++iState)
            activeStatesTriggered_[Keyboard][iState]->keyReleased(kEvt);

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
            for (unsigned int iState = 0; iState < activeStatesTriggered_[Mouse].size(); ++iState)
                activeStatesTriggered_[Mouse][iState]->mouseMoved(abs, rel, clippingSize);
        }

        // check for mouse scrolled event
        if (e.state.Z.rel != 0)
        {
            for (unsigned int iState = 0; iState < activeStatesTriggered_[Mouse].size(); ++iState)
                activeStatesTriggered_[Mouse][iState]->mouseScrolled(e.state.Z.abs, e.state.Z.rel);
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

        for (unsigned int iState = 0; iState < activeStatesTriggered_[Mouse].size(); ++iState)
            activeStatesTriggered_[Mouse][iState]->mouseButtonPressed((MouseButtonCode::ByEnum)id);

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

        for (unsigned int iState = 0; iState < activeStatesTriggered_[Mouse].size(); ++iState)
            activeStatesTriggered_[Mouse][iState]->mouseButtonReleased((MouseButtonCode::ByEnum)id);

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

        for (unsigned int iState = 0; iState < activeStatesTriggered_[2 + iJoyStick].size(); ++iState)
            activeStatesTriggered_[2 + iJoyStick][iState]->joyStickButtonPressed(iJoyStick, (JoyStickButtonCode::ByEnum)button);

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

        for (unsigned int iState = 0; iState < activeStatesTriggered_[2 + iJoyStick].size(); ++iState)
            activeStatesTriggered_[2 + iJoyStick][iState]->joyStickButtonReleased(iJoyStick, (JoyStickButtonCode::ByEnum)button);

        return true;
    }

    /**
    @brief
        Calls the states for a particular axis with our enumeration.
        Used by OIS sliders and OIS axes.
    */
    void InputManager::_fireAxis(unsigned int iJoyStick, int axis, int value)
    {
        if (internalState_ & Calibrating)
        {
            if (value < joyStickMinValues_[iJoyStick][axis])
                joyStickMinValues_[iJoyStick][axis] = value;
            if (value > joyStickMaxValues_[iJoyStick][axis])
                joyStickMaxValues_[iJoyStick][axis] = value;
        }
        else
        {
            float fValue = value - joyStickCalibrations_[iJoyStick].middleValue[axis];
            if (fValue > 0.0f)
                fValue *= joyStickCalibrations_[iJoyStick].positiveCoeff[axis];
            else
                fValue *= joyStickCalibrations_[iJoyStick].negativeCoeff[axis];

            for (unsigned int iState = 0; iState < activeStatesTriggered_[2 + iJoyStick].size(); ++iState)
                activeStatesTriggered_[2 + iJoyStick][iState]->joyStickAxisMoved(iJoyStick, axis, fValue);
        }
    }

    bool InputManager::axisMoved(const OIS::JoyStickEvent &arg, int axis)
    {
        unsigned int iJoyStick = _getJoystick(arg);

        // keep in mind that the first 8 axes are reserved for the sliders
        _fireAxis(iJoyStick, axis + sliderAxes, arg.state.mAxes[axis].abs);

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
        Determines which InputState gets the input. Higher is better.
        Use 0 to handle it implicitely by the order of activation.
        Otherwise numbers larger than maxStateStackSize_s have to be used!
    @return
        True if added, false if name or priority already existed.
    */
    bool InputManager::_configureInputState(InputState* state, const std::string& name, bool bAlwaysGetsInput, bool bTransparent, int priority)
    {
        if (name == "")
            return false;
        if (!state)
            return false;
        if (inputStatesByName_.find(name) == inputStatesByName_.end())
        {
            if (priority >= InputStatePriority::HighPriority || priority == InputStatePriority::Empty)
            {
                // Make sure we don't add two high priority states with the same priority
                for (std::map<std::string, InputState*>::const_iterator it = this->inputStatesByName_.begin();
                    it != this->inputStatesByName_.end(); ++it)
                {
                    if (it->second->getPriority() == priority)
                    {
                        COUT(2) << "Warning: Could not add an InputState with the same priority '"
                            << priority << "' != 0." << std::endl;
                        return false;
                    }
                }
            }
            inputStatesByName_[name] = state;
            state->setNumOfJoySticks(numberOfJoySticks());
            state->setName(name);
            state->bAlwaysGetsInput_ = bAlwaysGetsInput;
            state->bTransparent_ = bTransparent;
            if (priority >= InputStatePriority::HighPriority || priority == InputStatePriority::Empty)
                state->setPriority(priority);
            return true;
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
        The removal process is being postponed if InputManager::update() is currently running.
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
                    // prevents a state being added multiple times
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
        if (name == "empty")
        {
            COUT(2) << "InputManager: Leaving the empty state is not allowed!" << std::endl;
            return false;
        }
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
        COUT(0) << "Move all joy stick axes fully in all directions." << std::endl
                << "When done, put the axex in the middle position and press enter." << std::endl;

        getInstance()._startCalibration();
    }

    /**
    @brief
        Reloads the input system
    */
    void InputManager::reload(bool joyStickSupport)
    {
        getInstance().reloadInputSystem(joyStickSupport);
    }


    // ############################################################
    // #####                   ugly hacks                     #####
    // ##########                                        ##########
    // ############################################################

#ifdef ORXONOX_PLATFORM_LINUX
    void InputManager::grabMouse()
    {
        OIS::LinuxMouse* linuxMouse = dynamic_cast<OIS::LinuxMouse*>(singletonRef_s->mouse_);
        assert(linuxMouse);
        linuxMouse->grab(true);
    }

    void InputManager::ungrabMouse()
    {
        OIS::LinuxMouse* linuxMouse = dynamic_cast<OIS::LinuxMouse*>(singletonRef_s->mouse_);
        assert(linuxMouse);
        linuxMouse->grab(false);
    }
#endif
}
