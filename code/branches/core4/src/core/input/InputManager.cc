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

#include <cassert>
#include <climits>
#include <ois/OISException.h>
#include <ois/OISInputManager.h>
#include <boost/foreach.hpp>

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
#include "JoyStick.h"
#include "JoyStickQuantityListener.h"
#include "Mouse.h"
#include "Keyboard.h"

namespace orxonox
{
    // TODO: Add console commands again as member commands
    //SetConsoleCommand(InputManager, calibrate, true);
    //SetConsoleCommand(InputManager, reload, false);
    SetCommandLineSwitch(keyboard_no_grab).information("Whether not to exclusively grab the keyboard");

    // Abuse of this source file for the InputHandler
    InputHandler InputHandler::EMPTY;

    InputManager* InputManager::singletonRef_s = 0;

    /**
    @brief
        Defines the |= operator for easier use.
    */
    inline InputManager::State operator|=(InputManager::State& lval, InputManager::State rval)
    {
        return (lval = (InputManager::State)(lval | rval));
    }

    /**
    @brief
        Defines the &= operator for easier use.
    */
    inline InputManager::State operator&=(InputManager::State& lval, int rval)
    {
        return (lval = (InputManager::State)(lval & rval));
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
        : internalState_(Bad)
        , oisInputManager_(0)
        , devices_(2)
        , windowHnd_(0)
        , emptyState_(0)
        , keyDetector_(0)
        , calibratorCallbackHandler_(0)
    {
        RegisterRootObject(InputManager);

        assert(singletonRef_s == 0);
        singletonRef_s = this;

        CCOUT(4) << "Constructing..." << std::endl;

        this->setConfigValues();

        this->loadDevices(windowHnd, windowWidth, windowHeight);

        // Lowest priority empty InputState
        emptyState_ = createInputState("empty", false, false, InputStatePriority::Empty);
        emptyState_->setHandler(&InputHandler::EMPTY);
        activeStates_[emptyState_->getPriority()] = emptyState_;

        // KeyDetector to evaluate a pressed key's name
        InputState* detector = createInputState("detector", false, false, InputStatePriority::Detector);
        // Create a callback to avoid buttonHeld events after the key has been detected
        FunctorMember<InputManager>* bufferFunctor = createFunctor(&InputManager::clearBuffers);
        bufferFunctor->setObject(this);
        detector->setLeaveFunctor(bufferFunctor);
        keyDetector_ = new KeyDetector();
        detector->setHandler(keyDetector_);

        // Joy stick calibration helper callback
        InputState* calibrator = createInputState("calibrator", false, false, InputStatePriority::Calibrator);
        calibrator->setHandler(&InputHandler::EMPTY);
        calibratorCallbackHandler_ = new InputBuffer();
        calibratorCallbackHandler_->registerListener(this, &InputManager::stopCalibration, '\r', true);
        calibrator->setKeyHandler(calibratorCallbackHandler_);

        this->updateActiveStates();

        {
            // calibrate console command
            FunctorMember<InputManager>* functor = createFunctor(&InputManager::calibrate);
            functor->setObject(this);
            this->getIdentifier()->addConsoleCommand(createConsoleCommand(functor, "calibrate"), true);
        }
        {
            // reload console command
            FunctorMember<InputManager>* functor = createFunctor(&InputManager::reload);
            functor->setObject(this);
            this->getIdentifier()->addConsoleCommand(createConsoleCommand(functor, "reload"), false);
        }

        internalState_ = Nothing;
        CCOUT(4) << "Construction complete." << std::endl;
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
    void InputManager::loadDevices(size_t windowHnd, unsigned int windowWidth, unsigned int windowHeight)
    {
        CCOUT(3) << "Loading input devices..." << std::endl;

        // When loading the devices they should not already be loaded
        assert(internalState_ & Bad);
        assert(devices_[InputDeviceEnumerator::Mouse] == 0);
        assert(devices_[InputDeviceEnumerator::Keyboard] == 0);
        assert(devices_.size() == InputDeviceEnumerator::FirstJoyStick);

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
            oisInputManager_ = OIS::InputManager::createInputSystem(paramList);
            // Exception-safety
            Loki::ScopeGuard guard = Loki::MakeGuard(OIS::InputManager::destroyInputSystem, oisInputManager_);
            CCOUT(ORX_DEBUG) << "Created OIS input manager." << std::endl;

            if (oisInputManager_->getNumberOfDevices(OIS::OISKeyboard) > 0)
                devices_[InputDeviceEnumerator::Keyboard] = new Keyboard(InputDeviceEnumerator::Keyboard);
            else
                ThrowException(InitialisationFailed, "InputManager: No keyboard found, cannot proceed!");

            // Successful initialisation
            guard.Dismiss();
        }
        catch (std::exception& ex)
        {
            oisInputManager_ = NULL;
            internalState_ |= Bad;
            ThrowException(InitialisationFailed, "Could not initialise the input system: " << ex.what());
        }

        // TODO: Remove the two parameters
        this->loadMouse(windowWidth, windowHeight);
        this->loadJoySticks();

        // Reorder states in case some joy sticks were added/removed
        this->updateActiveStates();

        CCOUT(3) << "Input devices loaded." << std::endl;
    }

    void InputManager::loadMouse(unsigned int windowWidth, unsigned int windowHeight)
    {
        if (oisInputManager_->getNumberOfDevices(OIS::OISMouse) > 0)
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
    void InputManager::loadJoySticks()
    {
        for (int i = 0; i < oisInputManager_->getNumberOfDevices(OIS::OISJoyStick); i++)
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
        CCOUT(4) << "Destroying..." << std::endl;

        // Destroy calibrator helper handler and state
        delete keyDetector_;
        this->destroyState("calibrator");
        // Destroy KeyDetector and state
        delete calibratorCallbackHandler_;
        this->destroyState("detector");
        // destroy the empty InputState
        this->destroyStateInternal(this->emptyState_);

        // destroy all user InputStates
        while (statesByName_.size() > 0)
            this->destroyStateInternal((*statesByName_.rbegin()).second);

        if (!(internalState_ & Bad))
            this->destroyDevices();

        CCOUT(4) << "Destruction complete." << std::endl;
        singletonRef_s = 0;
    }

    void InputManager::destroyDevices()
    {
        CCOUT(3) << "Destroying devices..." << std::endl;

        BOOST_FOREACH(InputDevice*& device, devices_)
        {
            if (device == NULL)
                continue;
            std::string className = device->getClassName();
            try
            {
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

        assert(oisInputManager_ != NULL);
        try
        {
            OIS::InputManager::destroyInputSystem(oisInputManager_);
        }
        catch (...)
        {
            CCOUT(1) << "OIS::InputManager destruction failed! Potential resource leak!" << std::endl;
        }
        oisInputManager_ = NULL;

        internalState_ |= Bad;
        CCOUT(3) << "Destroyed devices." << std::endl;
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
    void InputManager::reload()
    {
        if (internalState_ & Ticking)
        {
            // We cannot destroy OIS right now, because reload was probably
            // caused by a user clicking on a GUI item. The stack trace would then
            // include an OIS method. So it would be a very bad thing to destroy it..
            internalState_ |= ReloadRequest;
        }
        else if (internalState_ & Calibrating)
            CCOUT(2) << "Warning: Cannot reload input system. Joy sticks are currently being calibrated." << std::endl;
        else
            reloadInternal();
    }

    /**
    @brief
        Internal reload method. Destroys the OIS devices and loads them again.
    */
    void InputManager::reloadInternal()
    {
        CCOUT(3) << "Reloading ..." << std::endl;

        // Save mouse clipping size
        int clippingWidth = 800;
        int clippingHeight = 600;
        if (devices_[InputDeviceEnumerator::Mouse])
        {
            int clippingWidth  = static_cast<Mouse*>(devices_[InputDeviceEnumerator::Mouse])->getClippingWidth();
            int clippingHeight = static_cast<Mouse*>(devices_[InputDeviceEnumerator::Mouse])->getClippingHeight();
        }

        this->destroyDevices();
        this->loadDevices(windowHnd_, clippingWidth, clippingHeight);

        internalState_ &= ~Bad;
        internalState_ &= ~ReloadRequest;
        CCOUT(3) << "Reloading complete." << std::endl;
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
        if (internalState_ & Bad)
            ThrowException(General, "InputManager was not correctly reloaded.");

        else if (internalState_ & ReloadRequest)
            reloadInternal();

        // check for states to leave
        if (!stateLeaveRequests_.empty())
        {
            for (std::set<InputState*>::iterator it = stateLeaveRequests_.begin();
                it != stateLeaveRequests_.end(); ++it)
            {
                (*it)->left();
                // just to be sure that the state actually is registered
                assert(statesByName_.find((*it)->getName()) != statesByName_.end());

                activeStates_.erase((*it)->getPriority());
                if ((*it)->getPriority() < InputStatePriority::HighPriority)
                    (*it)->setPriority(0);
                updateActiveStates();
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
                assert(statesByName_.find((*it)->getName()) != statesByName_.end());

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
                updateActiveStates();
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
                destroyStateInternal((*it));
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
            updateActiveStates();

        // mark that we now start capturing and distributing input
        internalState_ |= Ticking;

        // Capture all the input and handle it
        BOOST_FOREACH(InputDevice* device, devices_)
            if (device != NULL)
                device->update(time);

        // Update the states
        for (unsigned int i = 0; i < activeStatesTicked_.size(); ++i)
            activeStatesTicked_[i]->update(time.getDeltaTime());

        internalState_ &= ~Ticking;
    }

    /**
    @brief
        Updates the currently active states (according to activeStates_) for each device.
        Also, a list of all active states (no duplicates!) is compiled for the general update().
    */
    void InputManager::updateActiveStates()
    {
        // temporary resize
        for (unsigned int i = 0; i < devices_.size(); ++i)
        {
            if (devices_[i] == NULL)
                continue;
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
            if (devices_[i] != NULL)
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
            if (device != NULL)
                device->clearBuffers();
    }

    /**
    @brief
        Starts joy stick calibration.
    */
    void InputManager::calibrate()
    {
        COUT(0) << "Move all joy stick axes fully in all directions." << std::endl
                << "When done, put the axex in the middle position and press enter." << std::endl;

        BOOST_FOREACH(InputDevice* device, devices_)
            if (device != NULL)
                device->startCalibration();

        internalState_ |= Calibrating;
        enterState("calibrator");
    }

    void InputManager::stopCalibration()
    {
        BOOST_FOREACH(InputDevice* device, devices_)
            if (device != NULL)
                device->stopCalibration();

        // restore old input state
        leaveState("calibrator");
        internalState_ &= ~Calibrating;
        // Clear buffers to prevent button hold events
        this->clearBuffers();
    }

    // ############################################################
    // #####                    Iput States                   #####
    // ##########                                        ##########
    // ############################################################

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
        if (configureInputState(state, name, bAlwaysGetsInput, bTransparent, priority))
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
    bool InputManager::configureInputState(InputState* state, const std::string& name, bool bAlwaysGetsInput, bool bTransparent, int priority)
    {
        if (name == "")
            return false;
        if (!state)
            return false;
        if (statesByName_.find(name) == statesByName_.end())
        {
            if (priority >= InputStatePriority::HighPriority || priority == InputStatePriority::Empty)
            {
                // Make sure we don't add two high priority states with the same priority
                for (std::map<std::string, InputState*>::const_iterator it = this->statesByName_.begin();
                    it != this->statesByName_.end(); ++it)
                {
                    if (it->second->getPriority() == priority)
                    {
                        COUT(2) << "Warning: Could not add an InputState with the same priority '"
                            << priority << "' != 0." << std::endl;
                        return false;
                    }
                }
            }
            statesByName_[name] = state;
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
        Returns the pointer to the requested InputState.
    @param name
        Unique name of the state.
    @return
        Pointer to the instance, 0 if name was not found.
    */
    InputState* InputManager::getState(const std::string& name)
    {
        std::map<std::string, InputState*>::iterator it = statesByName_.find(name);
        if (it != statesByName_.end())
            return it->second;
        else
            return 0;
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
    bool InputManager::enterState(const std::string& name)
    {
        // get pointer from the map with all stored handlers
        std::map<std::string, InputState*>::const_iterator it = statesByName_.find(name);
        if (it != statesByName_.end())
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
    bool InputManager::leaveState(const std::string& name)
    {
        if (name == "empty")
        {
            COUT(2) << "InputManager: Leaving the empty state is not allowed!" << std::endl;
            return false;
        }
        // get pointer from the map with all stored handlers
        std::map<std::string, InputState*>::const_iterator it = statesByName_.find(name);
        if (it != statesByName_.end())
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

    /**
    @brief
        Removes and destroys an input state.
    @param name
        Name of the handler.
    @return
        True if removal was successful, false if name was not found.
    @remarks
        You can't remove the internal states "empty", "calibrator" and "detector".
        The removal process is being postponed if InputManager::update() is currently running.
    */
    bool InputManager::destroyState(const std::string& name)
    {
        if (name == "empty")
        {
            COUT(2) << "InputManager: Removing the empty state is not allowed!" << std::endl;
            return false;
        }
        std::map<std::string, InputState*>::iterator it = statesByName_.find(name);
        if (it != statesByName_.end())
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
                destroyStateInternal(it->second);

            return true;
        }
        return false;
    }

    /**
    @brief
        Destroys an InputState internally
    */
    void InputManager::destroyStateInternal(InputState* state)
    {
        assert(state && !(this->internalState_ & Ticking));
        std::map<int, InputState*>::iterator it = this->activeStates_.find(state->getPriority());
        if (it != this->activeStates_.end())
        {
            this->activeStates_.erase(it);
            updateActiveStates();
        }
        statesByName_.erase(state->getName());
        delete state;
    }
}
