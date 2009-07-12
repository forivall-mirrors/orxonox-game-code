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
    Implementation of a little Input handler that distributes everything
    coming from OIS.
*/

#ifndef _InputManager_H__
#define _InputManager_H__

#include "InputPrereqs.h"

#include <map>
#include <set>
#include <string>
#include <vector>

#include "core/OrxonoxClass.h"
#include "InputState.h"

namespace orxonox
{
    /**
    @brief
        Captures and distributes mouse and keyboard input.
    */
    class _CoreExport InputManager : public OrxonoxClass
    {
    public:
        enum State
        {
            Nothing       = 0x00,
            Bad           = 0x02,
            Ticking       = 0x04,
            Calibrating   = 0x08,
            ReloadRequest = 0x10,
        };

        InputManager (size_t windowHnd, unsigned int windowWidth, unsigned int windowHeight);
        ~InputManager();
        void setConfigValues();

        void update(const Clock& time);
        void clearBuffers();
        void calibrate();
        void reload();

        //-------------------------------
        // Input States
        //-------------------------------
        InputState* createInputState(const std::string& name, bool bAlwaysGetsInput = false, bool bTransparent = false, InputStatePriority priority = InputStatePriority::Dynamic);
        InputState* getState(const std::string& name);
        bool enterState     (const std::string& name);
        bool leaveState     (const std::string& name);
        bool destroyState   (const std::string& name);

        //-------------------------------
        // Various getters and setters
        //-------------------------------
        void setKeyDetectorCallback(const std::string& command);
        bool checkJoyStickID(const std::string& idString) const;
        unsigned int getJoyStickQuantity() const
            { return devices_.size() - InputDeviceEnumerator::FirstJoyStick; }
        OIS::InputManager* getOISInputManager()
            { return this->oisInputManager_; }

        static InputManager& getInstance()    { assert(singletonRef_s); return *singletonRef_s; }

    private: // functions
        // don't mess with a Singleton
        InputManager(const InputManager&);

        // Intenal methods
        void loadDevices(size_t windowHnd, unsigned int windowWidth, unsigned int windowHeight);
        void loadMouse(unsigned int windowWidth, unsigned int windowHeight);
        void loadJoySticks();
        void destroyDevices();

        void stopCalibration();

        void destroyStateInternal(InputState* state);
        void updateActiveStates();
        bool configureInputState(InputState* state, const std::string& name, bool bAlwaysGetsInput, bool bTransparent, int priority);

        void reloadInternal();

    private: // variables
        State                               internalState_;        //!< Current internal state
        OIS::InputManager*                  oisInputManager_;      //!< OIS input manager
        std::vector<InputDevice*>           devices_;              //!< List of all input devices (keyboard, mouse, joy sticks)
        size_t                              windowHnd_;            //!< Render window handle

        // some internally handled states and handlers
        InputState*                         emptyState_;
        KeyDetector*                        keyDetector_;          //!< KeyDetector instance
        InputBuffer*                        calibratorCallbackHandler_;

        std::map<std::string, InputState*>  statesByName_;
        std::map<int, InputState*>          activeStates_;
        std::vector<InputState*>            activeStatesTicked_;

        std::set<InputState*>               stateEnterRequests_;   //!< Request to enter a new state
        std::set<InputState*>               stateLeaveRequests_;   //!< Request to leave a running state
        std::set<InputState*>               stateDestroyRequests_; //!< Request to destroy a state

        static InputManager*                singletonRef_s;
    };
}

#endif /* _InputManager_H__ */
