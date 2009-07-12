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
#include "core/Functor.h"

#include "InputBuffer.h"
#include "KeyDetector.h"
#include "InputHandler.h"
#include "InputState.h"
#include "JoyStickQuantityListener.h"
#include "JoyStick.h"
#include "Mouse.h"
#include "Keyboard.h"

namespace orxonox
{
    SetConsoleCommand(InputManager, calibrate, true);
    SetConsoleCommand(InputManager, reload, false);
    SetCommandLineSwitch(keyboard_no_grab).information("Whether not to exclusively grab the keyboard");

    InputHandler InputHandler::EMPTY;
    InputManager* InputManager::singletonRef_s = 0;

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
        , devices_(2)
        , windowHnd_(0)
        , internalState_(Uninitialised)
        , stateEmpty_(0)
        , keyDetector_(0)
        , calibratorCallbackBuffer_(0)
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

            // TODO: clean this up
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

            // TODO: Remove the two parameters
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
            stateEmpty_ = createInputState("empty", false, false, InputStatePriority::Empty);
            stateEmpty_->setHandler(&InputHandler::EMPTY);
            activeStates_[stateEmpty_->getPriority()] = stateEmpty_;

            // KeyDetector to evaluate a pressed key's name
            InputState* detector = createInputState("detector", false, false, InputStatePriority::Detector);
            FunctorMember<InputManager>* bufferFunctor = createFunctor(&InputManager::clearBuffers);
            bufferFunctor->setObject(this);
            detector->setLeaveFunctor(bufferFunctor);
            keyDetector_ = new KeyDetector();
            detector->setHandler(keyDetector_);

            // Joy stick calibration helper callback
            InputState* calibrator = createInputState("calibrator", false, false, InputStatePriority::Calibrator);
            calibrator->setHandler(&InputHandler::EMPTY);
            calibratorCallbackBuffer_ = new InputBuffer();
            calibratorCallbackBuffer_->registerListener(this, &InputManager::_stopCalibration, '\r', true);
            calibrator->setKeyHandler(calibratorCallbackBuffer_);

            internalState_ |= InternalsReady;

            CCOUT(4) << "Initialising InputStates complete." << std::endl;
        }

        _updateActiveStates();

        CCOUT(3) << "Initialising complete." << std::endl;
    }

    void InputManager::_initialiseKeyboard()
    {
        assert(devices_[InputDeviceEnumerator::Keyboard] == 0);
        if (inputSystem_->getNumberOfDevices(OIS::OISKeyboard) > 0)
            devices_[InputDeviceEnumerator::Keyboard] = new Keyboard(InputDeviceEnumerator::Keyboard);
        else
            ThrowException(InitialisationFailed, "InputManager: No keyboard found, cannot proceed!");
    }

    void InputManager::_initialiseMouse(unsigned int windowWidth, unsigned int windowHeight)
    {
        assert(devices_[InputDeviceEnumerator::Mouse] == 0);
        if (inputSystem_->getNumberOfDevices(OIS::OISMouse) > 0)
        {
            try
            {
                devices_[InputDeviceEnumerator::Mouse] = new Mouse(InputDeviceEnumerator::Mouse, windowWidth, windowHeight);
            }
            catch (const OIS::Exception& ex)
            {
                CCOUT(2) << "Warning: Failed to create Mouse:" << ex.eText << std::endl
                         << "Proceeding without mouse support." << std::endl;
            }
        }
        else
            CCOUT(ORX_WARNING) << "Warning: No mouse found! Proceeding without mouse support." << std::endl;
    }

    /**
    @brief
        Creates all joy sticks and sets the event handler.
    @return
        False joy stick stay uninitialised, true otherwise.
    */
    void InputManager::_initialiseJoySticks()
    {
        assert(devices_.size() == InputDeviceEnumerator::FirstJoyStick);

        for (int i = 0; i < inputSystem_->getNumberOfDevices(OIS::OISJoyStick); i++)
        {
            try
            {
                devices_.push_back(new JoyStick(InputDeviceEnumerator::FirstJoyStick + i));
            }
            catch (std::exception ex)
            {
                CCOUT(2) << "Warning: Failed to create joy stick: " << ex.what() << std::endl;
            }
        }

        // inform all JoyStick Device Number Listeners
        for (ObjectList<JoyStickQuantityListener>::iterator it = ObjectList<JoyStickQuantityListener>::begin(); it; ++it)
            it->JoyStickQuantityChanged(devices_.size() - InputDeviceEnumerator::FirstJoyStick);
    }

    void InputManager::_startCalibration()
    {
        BOOST_FOREACH(InputDevice* device, devices_)
            device->startCalibration();

        getInstance().internalState_ |= Calibrating;
        getInstance().requestEnterState("calibrator");
    }

    void InputManager::_stopCalibration()
    {
        BOOST_FOREACH(InputDevice* device, devices_)
            device->stopCalibration();

        // restore old input state
        requestLeaveState("calibrator");
        internalState_ &= ~Calibrating;
        // Clear buffers to prevent button hold events
        this->clearBuffers();
    }

    // ############################################################
    // #####                    Destruction                   #####
    // ##########                                        ##########
    // ############################################################

    /**
    @brief
        Destroys all the created input devices and states.
    */
    // TODO: export this to be used with reload()
    InputManager::~InputManager()
    {
        if (internalState_ != Uninitialised)
        {
            CCOUT(3) << "Destroying ..." << std::endl;

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
            BOOST_FOREACH(InputDevice*& device, devices_)
            {
                std::string className = device->getClassName();
                try
                {
                    if (device)
                        delete device;
                    device = 0;
                    CCOUT(4) << className << " destroyed." << std::endl;
                }
                catch (...)
                {
                    CCOUT(1) << className << " destruction failed! Potential resource leak!" << std::endl;
                }
            }
            devices_.resize(InputDeviceEnumerator::FirstJoyStick);

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
            int mouseWidth  = static_cast<Mouse*>(devices_[InputDeviceEnumerator::Mouse])->getClippingWidth();
            int mouseHeight = static_cast<Mouse*>(devices_[InputDeviceEnumerator::Mouse])->getClippingHeight();

            internalState_ &= ~OISReady;

            // destroy the devices
            // destroy the devices
            BOOST_FOREACH(InputDevice*& device, devices_)
            {
                try
                {
                    if (device)
                        delete device;
                    device = 0;
                    CCOUT(4) << device->getClassName() << " destroyed." << std::endl;
                }
                catch (...)
                {
                    CCOUT(1) << device->getClassName() << " destruction failed! Potential resource leak!" << std::endl;
                }
            }
            devices_.resize(InputDeviceEnumerator::FirstJoyStick);

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
                (*it)->left();
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
                (*it)->entered();
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

        // check whether a state has changed its EMPTY situation
        bool bUpdateRequired = false;
        for (std::map<int, InputState*>::iterator it = activeStates_.begin(); it != activeStates_.end(); ++it)
        {
            if (it->second->hasExpired())
            {
                it->second->resetExpiration();
                bUpdateRequired = true;
            }
        }
        if (bUpdateRequired)
            _updateActiveStates();

        // mark that we now start capturing and distributing input
        internalState_ |= Ticking;

        // Capture all the input. This calls the event handlers in InputManager.
        BOOST_FOREACH(InputDevice* device, devices_)
            device->update(time);

        if (!(internalState_ & Calibrating))
        {
            // update the states with a general tick afterwards
            for (unsigned int i = 0; i < activeStatesTicked_.size(); ++i)
                activeStatesTicked_[i]->update(time.getDeltaTime());
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
        // temporary resize
        for (unsigned int i = 0; i < devices_.size(); ++i)
        {
            std::vector<InputState*>& states = devices_[i]->getStateListRef();
            bool occupied = false;
            states.clear();
            for (std::map<int, InputState*>::reverse_iterator rit = activeStates_.rbegin(); rit != activeStates_.rend(); ++rit)
            {
                if (rit->second->isInputDeviceEnabled(i) && (!occupied || rit->second->bAlwaysGetsInput_))
                {
                    states.push_back(rit->second);
                    if (!rit->second->bTransparent_)
                        occupied = true;
                }
            }
        }

        // update tickables (every state will only appear once)
        // Using a std::set to avoid duplicates
        std::set<InputState*> tempSet;
        for (unsigned int i = 0; i < devices_.size(); ++i)
            for (unsigned int iState = 0; iState < devices_[i]->getStateListRef().size(); ++iState)
                tempSet.insert(devices_[i]->getStateListRef()[iState]);

        // copy the content of the std::set back to the actual vector
        activeStatesTicked_.clear();
        for (std::set<InputState*>::const_iterator it = tempSet.begin();it != tempSet.end(); ++it)
            activeStatesTicked_.push_back(*it);
    }

    /**
    @brief
        Clears all buffers that store what keys/buttons are being pressed at the moment.
    */
    void InputManager::clearBuffers()
    {
        BOOST_FOREACH(InputDevice* device, devices_)
            device->clearBuffers();
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
    bool InputManager::checkJoyStickID(const std::string& idString) const
    {
        for (unsigned int i = InputDeviceEnumerator::FirstJoyStick; i < devices_.size(); ++i)
            if (static_cast<JoyStick*>(devices_[i])->getIDString() == idString)
                return false;
        return true;
    }


    // ############################################################
    // #####         Other Public Interface Methods           #####
    // ##########                                        ##########
    // ############################################################

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
        Creates a new InputState by type, name and priority.
        
        You will have to use this method because the
        c'tors and d'tors are private.
    @remarks
        The InputManager will take care of the state completely. That also
        means it gets deleted when the InputManager is destroyed!
    @param name
        Name of the InputState when referenced as string
    @param priority
        Priority matters when multiple states are active. You can specify any
        number, but 1 - 99 is preferred (99 means high).
    */
    InputState* InputManager::createInputState(const std::string& name, bool bAlwaysGetsInput, bool bTransparent, InputStatePriority priority)
    {
        InputState* state = new InputState;
        if (_configureInputState(state, name, bAlwaysGetsInput, bTransparent, priority))
            return state;
        else
        {
            delete state;
            return 0;
        }
    }

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
            state->JoyStickQuantityChanged(devices_.size() - InputDeviceEnumerator::FirstJoyStick);
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
}
