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
    Implementation of the InputManager and a static variable from the InputHandler.
*/

#include "InputManager.h"

#include <cassert>
#include <climits>
#include <ois/OISException.h>
#include <ois/OISInputManager.h>
#include <boost/foreach.hpp>
#include <loki/ScopeGuard.h>

#include "util/Clock.h"
#include "util/Convert.h"
#include "util/Exception.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "core/CommandLineParser.h"
#include "core/GraphicsManager.h"
#include "core/command/ConsoleCommand.h"
#include "core/command/Functor.h"

#include "InputBuffer.h"
#include "JoyStick.h"
#include "JoyStickQuantityListener.h"
#include "Mouse.h"
#include "Keyboard.h"

namespace orxonox
{
    SetCommandLineSwitch(keyboard_no_grab).information("Whether not to exclusively grab the keyboard");

    static const std::string __CC_InputManager_name = "InputManager";
    static const std::string __CC_calibrate_name = "calibrate";
    static const std::string __CC_reload_name = "reload";

    SetConsoleCommand(__CC_InputManager_name, __CC_calibrate_name, &InputManager::calibrate).addShortcut();
    SetConsoleCommand(__CC_InputManager_name, __CC_reload_name,    &InputManager::reload   );

    // Abuse of this source file for the InputHandler
    InputHandler InputHandler::EMPTY;

    InputManager* InputManager::singletonPtr_s = 0;

    //! Defines the |= operator for easier use.
    inline InputManager::State operator|=(InputManager::State& lval, InputManager::State rval)
    {
        return (lval = (InputManager::State)(lval | rval));
    }

    //! Defines the &= operator for easier use.
    inline InputManager::State operator&=(InputManager::State& lval, int rval)
    {
        return (lval = (InputManager::State)(lval & rval));
    }

    // ############################################################
    // #####                  Initialisation                  #####
    // ##########                                        ##########
    // ############################################################
    InputManager::InputManager()
        : internalState_(Bad)
        , oisInputManager_(0)
        , devices_(2)
        , exclusiveMouse_(TriBool::False)
        , emptyState_(0)
        , calibratorCallbackHandler_(0)
    {
        RegisterRootObject(InputManager);

        CCOUT(4) << "Constructing..." << std::endl;

        // Allocate space for the function call buffer
        this->callBuffer_.reserve(16);

        this->setConfigValues();

        if (GraphicsManager::getInstance().isFullScreen())
            exclusiveMouse_ = TriBool::True;
        this->loadDevices();

        // Lowest priority empty InputState
        emptyState_ = createInputState("empty", false, false, InputStatePriority::Empty);
        emptyState_->setHandler(&InputHandler::EMPTY);
        activeStates_[emptyState_->getPriority()] = emptyState_;

        // Joy stick calibration helper callback
        InputState* calibrator = createInputState("calibrator", false, false, InputStatePriority::Calibrator);
        calibrator->setHandler(&InputHandler::EMPTY);
        calibratorCallbackHandler_ = new InputBuffer();
        calibratorCallbackHandler_->registerListener(this, &InputManager::stopCalibration, '\r', true);
        calibrator->setKeyHandler(calibratorCallbackHandler_);

        this->updateActiveStates();

        ModifyConsoleCommand(__CC_InputManager_name, __CC_calibrate_name).setObject(this);
        ModifyConsoleCommand(__CC_InputManager_name, __CC_reload_name).setObject(this);

        CCOUT(4) << "Construction complete." << std::endl;
        internalState_ = Nothing;
    }

    void InputManager::setConfigValues()
    {
    }

    /**
    @brief
        Creates the OIS::InputMananger, the keyboard, the mouse and
        the joys ticks. If either of the first two fail, this method throws an exception.
    */
    void InputManager::loadDevices()
    {
        CCOUT(4) << "Loading input devices..." << std::endl;

        // When loading the devices they should not already be loaded
        assert(internalState_ & Bad);
        assert(devices_[InputDeviceEnumerator::Mouse] == 0);
        assert(devices_[InputDeviceEnumerator::Keyboard] == 0);
        assert(devices_.size() == InputDeviceEnumerator::FirstJoyStick);

        typedef std::pair<std::string, std::string> StringPair;

        // Fill parameter list
        OIS::ParamList paramList;
        size_t windowHnd = GraphicsManager::getInstance().getRenderWindowHandle();
        paramList.insert(StringPair("WINDOW", multi_cast<std::string>(windowHnd)));
#if defined(ORXONOX_PLATFORM_WINDOWS)
        paramList.insert(StringPair("w32_keyboard", "DISCL_NONEXCLUSIVE"));
        paramList.insert(StringPair("w32_keyboard", "DISCL_FOREGROUND"));
        paramList.insert(StringPair("w32_mouse", "DISCL_FOREGROUND"));
        if (exclusiveMouse_ == TriBool::True || GraphicsManager::getInstance().isFullScreen())
        {
            // Disable Windows key plus special keys (like play, stop, next, etc.)
            paramList.insert(StringPair("w32_keyboard", "DISCL_NOWINKEY"));
            paramList.insert(StringPair("w32_mouse", "DISCL_EXCLUSIVE"));
        }
        else
            paramList.insert(StringPair("w32_mouse", "DISCL_NONEXCLUSIVE"));
#elif defined(ORXONOX_PLATFORM_LINUX)
        // Enabling this is probably a bad idea, but whenever orxonox crashes, the setting stays on
        // Trouble might be that the Pressed event occurs a bit too often...
        paramList.insert(StringPair("XAutoRepeatOn", "true"));

        if (exclusiveMouse_ == TriBool::True || GraphicsManager::getInstance().isFullScreen())
        {
            if (CommandLineParser::getValue("keyboard_no_grab").getBool())
                paramList.insert(StringPair("x11_keyboard_grab", "false"));
            else
                paramList.insert(StringPair("x11_keyboard_grab", "true"));
            paramList.insert(StringPair("x11_mouse_grab",  "true"));
            paramList.insert(StringPair("x11_mouse_hide", "true"));
        }
        else
        {
            paramList.insert(StringPair("x11_keyboard_grab", "false"));
            paramList.insert(StringPair("x11_mouse_grab",  "false"));
            paramList.insert(StringPair("x11_mouse_hide", "false"));
        }
#endif

        try
        {
            oisInputManager_ = OIS::InputManager::createInputSystem(paramList);
            // Exception-safety
            Loki::ScopeGuard guard = Loki::MakeGuard(OIS::InputManager::destroyInputSystem, oisInputManager_);
            CCOUT(4) << "Created OIS input manager." << std::endl;

            if (oisInputManager_->getNumberOfDevices(OIS::OISKeyboard) > 0)
                devices_[InputDeviceEnumerator::Keyboard] = new Keyboard(InputDeviceEnumerator::Keyboard, oisInputManager_);
            else
                ThrowException(InitialisationFailed, "InputManager: No keyboard found, cannot proceed!");

            // Successful initialisation
            guard.Dismiss();
        }
        catch (const std::exception& ex)
        {
            oisInputManager_ = NULL;
            internalState_ |= Bad;
            ThrowException(InitialisationFailed, "Could not initialise the input system: " << ex.what());
        }

        this->loadMouse();
        this->loadJoySticks();

        // Reorder states in case some joy sticks were added/removed
        this->updateActiveStates();

        CCOUT(4) << "Input devices loaded." << std::endl;
    }

    //! Creates a new orxonox::Mouse
    void InputManager::loadMouse()
    {
        if (oisInputManager_->getNumberOfDevices(OIS::OISMouse) > 0)
        {
            try
            {
                devices_[InputDeviceEnumerator::Mouse] = new Mouse(InputDeviceEnumerator::Mouse, oisInputManager_);
            }
            catch (const std::exception& ex)
            {
                CCOUT(2) << "Warning: Failed to create Mouse:" << ex.what() << std::endl
                         << "Proceeding without mouse support." << std::endl;
            }
        }
        else
            CCOUT(2) << "Warning: No mouse found! Proceeding without mouse support." << std::endl;
    }

    //! Creates as many joy sticks as are available.
    void InputManager::loadJoySticks()
    {
        for (int i = 0; i < oisInputManager_->getNumberOfDevices(OIS::OISJoyStick); i++)
        {
            try
            {
                devices_.push_back(new JoyStick(InputDeviceEnumerator::FirstJoyStick + i, oisInputManager_));
            }
            catch (const std::exception& ex)
            {
                CCOUT(2) << "Warning: Failed to create joy stick: " << ex.what() << std::endl;
            }
        }

        // inform all JoyStick Device Number Listeners
        std::vector<JoyStick*> joyStickList;
        for (unsigned int i = InputDeviceEnumerator::FirstJoyStick; i < devices_.size(); ++i)
            joyStickList.push_back(static_cast<JoyStick*>(devices_[i]));
        JoyStickQuantityListener::changeJoyStickQuantity(joyStickList);
    }

    // ############################################################
    // #####                    Destruction                   #####
    // ##########                                        ##########
    // ############################################################

    InputManager::~InputManager()
    {
        CCOUT(3) << "Destroying..." << std::endl;

        // Leave all active InputStates (except "empty")
        while (this->activeStates_.size() > 1)
            this->leaveState(this->activeStates_.rbegin()->second->getName());
        this->activeStates_.clear();

        // Destroy calibrator helper handler and state
        this->destroyState("calibrator");
        // Destroy KeyDetector and state
        calibratorCallbackHandler_->destroy();
        // Destroy the empty InputState
        this->destroyStateInternal(this->emptyState_);

        // Destroy all user InputStates
        while (statesByName_.size() > 0)
            this->destroyStateInternal(statesByName_.rbegin()->second);

        if (!(internalState_ & Bad))
            this->destroyDevices();

        // Reset console commands
        ModifyConsoleCommand(__CC_InputManager_name, __CC_calibrate_name).setObject(0);
        ModifyConsoleCommand(__CC_InputManager_name, __CC_reload_name).setObject(0);

        CCOUT(3) << "Destruction complete." << std::endl;
    }

    /**
    @brief
        Destoys all input devices (joy sticks, mouse, keyboard and OIS::InputManager)
    @throw
        Method does not throw
    */
    void InputManager::destroyDevices()
    {
        CCOUT(4) << "Destroying devices..." << std::endl;

        BOOST_FOREACH(InputDevice*& device, devices_)
        {
            if (device == NULL)
                continue;
            const std::string& className = device->getClassName();
            delete device;
            device = 0;
            CCOUT(4) << className << " destroyed." << std::endl;
        }
        devices_.resize(InputDeviceEnumerator::FirstJoyStick);

        assert(oisInputManager_ != NULL);
        try
        {
            OIS::InputManager::destroyInputSystem(oisInputManager_);
        }
        catch (const OIS::Exception& ex)
        {
            COUT(1) << "OIS::InputManager destruction failed" << ex.eText << std::endl
                    << "    Potential resource leak!" << std::endl;
        }
        oisInputManager_ = NULL;

        internalState_ |= Bad;
        CCOUT(4) << "Destroyed devices." << std::endl;
    }

    // ############################################################
    // #####                     Reloading                    #####
    // ##########                                        ##########
    // ############################################################

    void InputManager::reload()
    {
        if (internalState_ & Calibrating)
            CCOUT(2) << "Warning: Cannot reload input system. Joy sticks are currently being calibrated." << std::endl;
        else
            reloadInternal();
    }

    //! Internal reload method. Destroys the OIS devices and loads them again.
    void InputManager::reloadInternal()
    {
        CCOUT(4) << "Reloading ..." << std::endl;

        this->destroyDevices();
        this->loadDevices();

        internalState_ &= ~Bad;
        CCOUT(4) << "Reloading complete." << std::endl;
    }

    // ############################################################
    // #####                  Runtime Methods                 #####
    // ##########                                        ##########
    // ############################################################

    void InputManager::preUpdate(const Clock& time)
    {
        if (internalState_ & Bad)
            ThrowException(General, "InputManager was not correctly reloaded.");

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

        // Capture all the input and collect the function calls
        // No event gets triggered here yet!
        BOOST_FOREACH(InputDevice* device, devices_)
            if (device != NULL)
                device->update(time);

        // Collect function calls for the update
        for (unsigned int i = 0; i < activeStatesTicked_.size(); ++i)
            activeStatesTicked_[i]->update(time.getDeltaTime());

        // Execute all cached function calls in order
        // Why so complicated? The problem is that an InputHandler could trigger
        // a reload that would destroy the OIS devices or it could even leave and
        // then destroy its own InputState. That would of course lead to access
        // violations.
        // If we delay the calls, then OIS and and the InputStates are not anymore
        // in the call stack and can therefore be edited.
        for (size_t i = 0; i < this->callBuffer_.size(); ++i)
            this->callBuffer_[i]();

        this->callBuffer_.clear();
    }

    /**
    @brief
        Updates the currently active states (according to activeStates_) for each device.
        Also, a list of all active states (no duplicates!) is compiled for the general preUpdate().
    */
    void InputManager::updateActiveStates()
    {
        // Calculate the stack of input states
        // and assign it to the corresponding device
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

        // See that we only update each InputState once for each device
        // Using an std::set to avoid duplicates
        std::set<InputState*> tempSet;
        for (unsigned int i = 0; i < devices_.size(); ++i)
            if (devices_[i] != NULL)
                for (unsigned int iState = 0; iState < devices_[i]->getStateListRef().size(); ++iState)
                    tempSet.insert(devices_[i]->getStateListRef()[iState]);

        // Copy the content of the std::set back to the actual vector
        activeStatesTicked_.clear();
        for (std::set<InputState*>::const_iterator it = tempSet.begin();it != tempSet.end(); ++it)
            activeStatesTicked_.push_back(*it);

        // Check whether we have to change the mouse mode
        TriBool::Value requestedMode = TriBool::Dontcare;
        std::vector<InputState*>& mouseStates = devices_[InputDeviceEnumerator::Mouse]->getStateListRef();
        if (mouseStates.empty())
            requestedMode = TriBool::False;
        else
            requestedMode = mouseStates.front()->getMouseExclusive();
        if (requestedMode != TriBool::Dontcare && exclusiveMouse_ != requestedMode)
        {
            exclusiveMouse_ = requestedMode;
            if (!GraphicsManager::getInstance().isFullScreen())
                this->reloadInternal();
        }
    }

    void InputManager::clearBuffers()
    {
        BOOST_FOREACH(InputDevice* device, devices_)
            if (device != NULL)
                device->clearBuffers();
    }

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

    //! Tells all devices to stop the calibration and evaluate it. Buffers are being cleared as well!
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

        COUT(0) << "Calibration has been stored." << std::endl;
    }

    //! Gets called by WindowEventListener upon focus change --> clear buffers
    void InputManager::windowFocusChanged(bool bFocus)
    {
        this->clearBuffers();
    }

    std::pair<int, int> InputManager::getMousePosition() const
    {
        Mouse* mouse = static_cast<Mouse*>(devices_[InputDeviceEnumerator::Mouse]);
        if (mouse != NULL)
        {
            const OIS::MouseState state = mouse->getOISDevice()->getMouseState();
            return std::make_pair(state.X.abs, state.Y.abs);
        }
        else
            return std::make_pair(0, 0);
    }

    // ############################################################
    // #####                    Input States                  #####
    // ##########                                        ##########
    // ############################################################

    InputState* InputManager::createInputState(const std::string& name, bool bAlwaysGetsInput, bool bTransparent, InputStatePriority priority)
    {
        if (name.empty())
            return 0;
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
                            << static_cast<int>(priority) << "' != 0." << std::endl;
                        return 0;
                    }
                }
            }
            InputState* state = new InputState(name, bAlwaysGetsInput, bTransparent, priority);
            statesByName_[name] = state;

            return state;
        }
        else
        {
            COUT(2) << "Warning: Could not add an InputState with the same name '" << name << "'." << std::endl;
            return 0;
        }
    }

    InputState* InputManager::getState(const std::string& name)
    {
        std::map<std::string, InputState*>::iterator it = statesByName_.find(name);
        if (it != statesByName_.end())
            return it->second;
        else
            return 0;
    }

    bool InputManager::enterState(const std::string& name)
    {
        // get pointer from the map with all stored handlers
        std::map<std::string, InputState*>::const_iterator it = statesByName_.find(name);
        if (it != statesByName_.end() && activeStates_.find(it->second->getPriority()) == activeStates_.end())
        {
            // exists and not active
            if (it->second->getPriority() == 0)
            {
                // Get smallest possible priority between 1 and maxStateStackSize_s
                for (std::map<int, InputState*>::reverse_iterator rit = activeStates_.rbegin();
                    rit != activeStates_.rend(); ++rit)
                {
                    if (rit->first < InputStatePriority::HighPriority)
                    {
                        it->second->setPriority(rit->first + 1);
                        break;
                    }
                }
                // In case no normal handler was on the stack
                if (it->second->getPriority() == 0)
                    it->second->setPriority(1);
            }
            activeStates_[it->second->getPriority()] = it->second;
            updateActiveStates();
            it->second->entered();

            return true;
        }
        return false;
    }

    bool InputManager::leaveState(const std::string& name)
    {
        if (name == "empty")
        {
            COUT(2) << "InputManager: Leaving the empty state is not allowed!" << std::endl;
            return false;
        }
        // get pointer from the map with all stored handlers
        std::map<std::string, InputState*>::const_iterator it = statesByName_.find(name);
        if (it != statesByName_.end() && activeStates_.find(it->second->getPriority()) != activeStates_.end())
        {
            // exists and active

            it->second->left();

            activeStates_.erase(it->second->getPriority());
            if (it->second->getPriority() < InputStatePriority::HighPriority)
                it->second->setPriority(0);
            updateActiveStates();

            return true;
        }
        return false;
    }

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
            this->leaveState(name);
            destroyStateInternal(it->second);

            return true;
        }
        return false;
    }

    //! Destroys an InputState internally.
    void InputManager::destroyStateInternal(InputState* state)
    {
        assert(state && this->activeStates_.find(state->getPriority()) == this->activeStates_.end());
        statesByName_.erase(state->getName());
        state->destroy();
    }

    bool InputManager::setMouseExclusive(const std::string& name, TriBool::Value value)
    {
        if (name == "empty")
        {
            COUT(2) << "InputManager: Changing the empty state is not allowed!" << std::endl;
            return false;
        }
        std::map<std::string, InputState*>::iterator it = statesByName_.find(name);
        if (it != statesByName_.end())
        {
            it->second->setMouseExclusive(value);
            return true;
        }
        return false;
    }
}
