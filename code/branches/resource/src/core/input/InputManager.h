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

#ifndef _InputManager_H__
#define _InputManager_H__

#include "InputPrereqs.h"

#include <map>
#include <set>
#include <string>
#include <vector>

#include "core/WindowEventListener.h"
#include "InputState.h"

namespace orxonox
{
    /**
    @brief
        Manages the input devices (mouse, keyboard, joy sticks) and the input states.

        Every input device has its own wrapper class which does the actualy input event
        distribution. The InputManager only creates reloads (on request) those devices.

        The other functionality concerns handling InputStates. They act as a layer
        between InputHandlers (like the KeyBinder or the GUIManager) and InputDevices.
        InputStates are memory managed by the IputManager. You cannot create or destroy
        them on your own. Therefore all states get destroyed with the InputManager d'tor.
    @note
        - The actual lists containing all the InputStates for a specific device are stored
          in the InputDevices themselves.
        - The devices_ vector always has at least two elements: Keyboard (first) and mouse.
          You best access them intenally with InputDeviceEnumerator::Keyboard/Mouse
          The first joy stick is accessed with InputDeviceEnumerator::FirstJoyStick.
        - Keyboard construction is mandatory , mouse and joy sticks are not.
          If the OIS::InputManager or the Keyboard fail, an exception is thrown.
    */
    class _CoreExport InputManager : public WindowEventListener
    {
    public:
        //! Represents internal states of the InputManager.
        enum State
        {
            Nothing       = 0x00,
            Bad           = 0x02,
            Ticking       = 0x04,
            Calibrating   = 0x08,
            ReloadRequest = 0x10,
        };

        /**
        @brief
            Loads the devices and initialises the KeyDetector and the Calibrator.
            
            If either the OIS input system and/or the keyboard could not be created,
            the constructor fails with an std::exception.
        */
        InputManager(size_t windowHnd);
        //! Destroys all devices AND all input states!
        ~InputManager();
        void setConfigValues();

        /**
        @brief
            Updates the devices (which distribute the input events) and the input states.

            Any InpuStates changes (destroy, enter, leave) and happens here. If a reload request
            was submitted while updating, the request wil be postponed until the next update call.
        */
        void update(const Clock& time);
        //! Clears all input device buffers. This usually only includes the pressed button list.
        void clearBuffers();
        //! Starts joy stick calibration.
        void calibrate();
        /**
        @brief
            Reloads all the input devices. Use this method to initialise new joy sticks.
        @note
            Only reloads immediately if the call stack doesn't include the update() method.
        */
        void reload();

        //-------------------------------
        // Input States
        //-------------------------------
        /**
        @brief
            Creates a new InputState that gets managed by the InputManager.
        @remarks
            The InputManager will take care of the state completely. That also
            means it gets deleted when the InputManager is destroyed!
        @param name
            Unique name of the InputState when referenced as string
        @param priority
            Priority matters when multiple states are active. You can specify any
            number, but 1 - 99 is preferred (99 means high priority).
        */
        InputState* createInputState(const std::string& name, bool bAlwaysGetsInput = false, bool bTransparent = false, InputStatePriority priority = InputStatePriority::Dynamic);
        /**
        @brief
            Returns a pointer to a InputState referenced by name.
        @return
            Returns NULL if state was not found.
        */
        InputState* getState(const std::string& name);
        /**
        @brief
            Activates a specific input state.
            It might not be actually activated if the priority is too low!
        @return
            False if name was not found, true otherwise.
        */
        bool enterState(const std::string& name);
        /**
        @brief
            Deactivates a specific input state.
        @return
            False if name was not found, true otherwise.
        */
        bool leaveState(const std::string& name);
        /**
        @brief
            Removes and destroys an input state.
        @return
            True if removal was successful, false if name was not found.
        @remarks
            - You can't remove the internal states "empty", "calibrator" and "detector".
            - The removal process is being postponed if InputManager::update() is currently running.
        */
        bool destroyState(const std::string& name);

        //-------------------------------
        // Various getters and setters
        //-------------------------------
        //! Sets the the name of the command used by the KeyDetector as callback.
        void setKeyDetectorCallback(const std::string& command);
        //! Returns the number of joy stick that have been created since the c'tor or last call to reload().
        unsigned int getJoyStickQuantity() const
            { return devices_.size() - InputDeviceEnumerator::FirstJoyStick; }
        //! Returns a pointer to the OIS InputManager. Only you if you know what you're doing!
        OIS::InputManager* getOISInputManager()
            { return this->oisInputManager_; }

        //! Returns a reference to the singleton instance
        static InputManager& getInstance() { assert(singletonRef_s); return *singletonRef_s; }

    private: // functions
        // don't mess with a Singleton
        InputManager(const InputManager&);

        // Intenal methods
        void loadDevices(size_t windowHnd);
        void loadMouse();
        void loadJoySticks();
        void destroyDevices();

        void stopCalibration();
        void reloadInternal();

        void destroyStateInternal(InputState* state);
        void updateActiveStates();

        // From WindowEventListener
        void windowFocusChanged();

    private: // variables
        State                               internalState_;        //!< Current internal state
        OIS::InputManager*                  oisInputManager_;      //!< OIS input manager
        std::vector<InputDevice*>           devices_;              //!< List of all input devices (keyboard, mouse, joy sticks)
        // TODO: Get this from the GraphicsManager during reload
        size_t                              windowHnd_;            //!< Render window handle (used to reload the InputManager)

        // some internally handled states and handlers
        InputState*                         emptyState_;           //!< Lowest priority states (makes handling easier)
        KeyDetector*                        keyDetector_;          //!< KeyDetector instance
        //! InputBuffer that reacts to the Enter key when calibrating the joy sticks
        InputBuffer*                        calibratorCallbackHandler_;

        std::map<std::string, InputState*>  statesByName_;         //!< Contains all the created input states by name
        std::map<int, InputState*>          activeStates_;         //!< Contains all active input states by priority (std::map is sorted!)
        std::vector<InputState*>            activeStatesTicked_;   //!< Like activeStates_, but only contains the ones that currently receive events

        std::set<InputState*>               stateEnterRequests_;   //!< Requests to enter a new state
        std::set<InputState*>               stateLeaveRequests_;   //!< Requests to leave a running state
        std::set<InputState*>               stateDestroyRequests_; //!< Requests to destroy a state

        static InputManager*                singletonRef_s;        //!< Pointer reference to the singleton
    };
}

#endif /* _InputManager_H__ */
