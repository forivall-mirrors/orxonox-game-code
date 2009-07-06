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
#include <boost/foreach.hpp>

#include "util/Convert.h"
#include "util/Exception.h"
#include "util/ScopeGuard.h"
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
#include "JoyStick.h"

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
    SetCommandLineSwitch(keyboard_no_grab).information("Whether not to exclusively grab the keyboard");

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
    InputManager::InputManager(size_t windowHnd, unsigned int windowWidth, unsigned int windowHeight)
        : inputSystem_(0)
        , keyboard_(0)
        , mouse_(0)
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

        initialise(windowHnd, windowWidth, windowHeight);
    }

    /**
    @brief
        Sets the configurable values.
    */
    void InputManager::setConfigValues()
    {
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
    void InputManager::initialise(size_t windowHnd, unsigned int windowWidth, unsigned int windowHeight)
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
#if defined(ORXONOX_PLATFORM_WINDOWS)
            //paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
            //paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
            //paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
            //paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
#elif defined(ORXONOX_PLATFORM_LINUX)
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

            try
            {
                inputSystem_ = OIS::InputManager::createInputSystem(paramList);
                // Exception-safety
                Loki::ScopeGuard guard = Loki::MakeGuard(OIS::InputManager::destroyInputSystem, inputSystem_);
                CCOUT(ORX_DEBUG) << "Created OIS input system" << std::endl;

                _initialiseKeyboard();

                // Nothing below should throw anymore, dismiss the guard
                guard.Dismiss();
            }
            catch (OIS::Exception& ex)
            {
                ThrowException(InitialisationFailed, "Could not initialise the input system: " << ex.what());
            }

            _initialiseMouse(windowWidth, windowHeight);

            _initialiseJoySticks();

            // clear all buffers
            clearBuffers();

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
            ThrowException(InitialisationFailed, "InputManager: No keyboard found, cannot proceed!");
        }
    }

    /**
    @brief
        Creates a mouse and sets the event handler.
    @return
        False if mouse stays uninitialised, true otherwise.
    */
    void InputManager::_initialiseMouse(unsigned int windowWidth, unsigned int windowHeight)
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

                // Set mouse region
                setWindowExtents(windowWidth, windowHeight);
            }
            else
            {
                CCOUT(ORX_WARNING) << "Warning: No mouse found! Proceeding without mouse support." << std::endl;
            }
        }
        catch (OIS::Exception ex)
        {
            CCOUT(ORX_WARNING) << "Warning: Failed to create an OIS mouse\n"
                << "OIS error message: \"" << ex.eText << "\"\n Proceeding without mouse support." << std::endl;
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
        if (!this->joySticks_.empty())
        {
            CCOUT(2) << "Warning: Joy sticks already initialised, skipping." << std::endl;
            return;
        }

        devicesNum_ = 2 + inputSystem_->getNumberOfDevices(OIS::OISJoyStick);
        // state management
        activeStatesTriggered_.resize(devicesNum_);

        for (int i = 0; i < inputSystem_->getNumberOfDevices(OIS::OISJoyStick); i++)
        {
            try
            {
                joySticks_.push_back(new JoyStick(activeStatesTriggered_[2 + i], i));
            }
            catch (std::exception ex)
            {
                CCOUT(2) << "Warning: Failed to create joy stick: " << ex.what() << std::endl;
            }
        }

        // inform all JoyStick Device Number Listeners
        for (ObjectList<JoyStickDeviceNumberListener>::iterator it = ObjectList<JoyStickDeviceNumberListener>::begin(); it; ++it)
            it->JoyStickDeviceNumberChanged(joySticks_.size());
    }

    void InputManager::_startCalibration()
    {
        BOOST_FOREACH(JoyStick* stick, joySticks_)
            stick->startCalibration();

        getInstance().internalState_ |= Calibrating;
        getInstance().requestEnterState("calibrator");
    }

    void InputManager::_completeCalibration()
    {
        BOOST_FOREACH(JoyStick* stick, joySticks_)
            stick->stopCalibration();

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

            try
            {
                OIS::InputManager::destroyInputSystem(inputSystem_);
            }
            catch (...)
            {
                CCOUT(1) << "OIS::InputManager destruction failed! Potential resource leak!" << std::endl;
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
        try
        {
            if (keyboard_)
                inputSystem_->destroyInputObject(keyboard_);
            keyboard_ = 0;
            CCOUT(4) << "Keyboard destroyed." << std::endl;
        }
        catch (...)
        {
            CCOUT(1) << "Keyboard destruction failed! Potential resource leak!" << std::endl;
        }
    }

    /**
    @brief
        Destroys the mouse and sets it to 0.
    */
    void InputManager::_destroyMouse()
    {
        assert(inputSystem_);
        try
        {
            if (mouse_)
                inputSystem_->destroyInputObject(mouse_);
            mouse_ = 0;
            CCOUT(4) << "Mouse destroyed." << std::endl;
        }
        catch (...)
        {
            CCOUT(1) << "Mouse destruction failed! Potential resource leak!" << std::endl;
        }
    }

    /**
    @brief
        Destroys all the joy sticks and resizes the lists to 0.
    */
    void InputManager::_destroyJoySticks()
    {
        assert(inputSystem_);
        while (!joySticks_.empty())
        {
            try
            {
                delete joySticks_.back();
            }
            catch (...)
            {
                CCOUT(1) << "Joy stick destruction failed! Potential resource leak!" << std::endl;
            }
            joySticks_.pop_back();
            devicesNum_ = 2;
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

    // ############################################################
    // #####                     Reloading                    #####
    // ##########                                        ##########
    // ############################################################

    /**
    @brief
        Public interface. Only reloads immediately if the call stack doesn't
        include the update() method.
    */
    void InputManager::reloadInputSystem()
    {
        if (internalState_ & Ticking)
        {
            // We cannot destroy OIS right now, because reload was probably
            // caused by a user clicking on a GUI item. The backtrace would then
            // include an OIS method. So it would be a very bad thing to destroy it..
            internalState_ |= ReloadRequest;
        }
        else if (internalState_ & OISReady)
            _reload();
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
    void InputManager::_reload()
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
            clearBuffers();

            initialise(windowHnd_, mouseWidth, mouseHeight);

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
            _reload();
            internalState_ &= ~ReloadRequest;
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
                    for(std::map<int, InputState*>::reverse_iterator rit = activeStates_.rbegin();
                        rit != activeStates_.rend(); ++rit)
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
        BOOST_FOREACH(JoyStick* stick, joySticks_)
            stick->capture();

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
            for (std::map<int, InputState*>::const_reverse_iterator rit = activeStates_.rbegin(); rit != activeStates_.rend(); ++rit)
            {
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
        keysDown_.clear();
        keyboardModifiers_ = 0;
        mouseButtonsDown_.clear();
        BOOST_FOREACH(JoyStick* stick, joySticks_)
            stick->clearBuffer();
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


    // ############################################################
    // #####                Friend functions                  #####
    // ##########                                        ##########
    // ############################################################

    /**
    @brief
        Checks whether there is already a joy stick with the given ID string.
    @return
        Returns true if ID is ok (unique), false otherwise.
    */
    bool InputManager::checkJoyStickID(const std::string& idString)
    {
        BOOST_FOREACH(JoyStick* stick, joySticks_)
        {
            if (stick->getIDString() == idString)
                return false;
        }
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
            state->JoyStickDeviceNumberChanged(numberOfJoySticks());
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
    void InputManager::reload()
    {
        getInstance().reloadInputSystem();
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
