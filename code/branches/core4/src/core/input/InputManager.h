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

#include "util/Math.h"
#include "util/OrxEnum.h"
#include "core/OrxonoxClass.h"

namespace orxonox
{
    struct InputStatePriority : OrxEnum<InputStatePriority>
    {
        OrxEnumConstructors(InputStatePriority);

        static const int Empty        = -1;
        static const int Dynamic      = 0;

        static const int HighPriority = 1000;
        static const int Console      = HighPriority + 0;
        static const int Calibrator   = HighPriority + 1;
        static const int Detector     = HighPriority + 2;
    };

    /**
    @brief
        Captures and distributes mouse and keyboard input.
    */
    class _CoreExport InputManager : public OrxonoxClass
    {
        // --> setConfigValues is private
        friend class ClassIdentifier<InputManager>;

    public:
        enum InputManagerState
        {
            Uninitialised    = 0x00,
            OISReady         = 0x01,
            InternalsReady   = 0x02,
            Ticking          = 0x04,
            Calibrating      = 0x08,
            ReloadRequest    = 0x10,
        };

        InputManager (size_t windowHnd, unsigned int windowWidth, unsigned int windowHeight);
        ~InputManager();

        void initialise(size_t windowHnd, unsigned int windowWidth, unsigned int windowHeight);

        void reloadInputSystem();

        void clearBuffers();

        void setWindowExtents(const int width, const int height);
        void setKeyDetectorCallback(const std::string& command);

        InputState* createInputState(const std::string& name, bool bAlwaysGetsInput = false, bool bTransparent = false, InputStatePriority priority = InputStatePriority::Dynamic);

        InputState* getState       (const std::string& name);
        InputState* getCurrentState();
        bool requestDestroyState   (const std::string& name);
        bool requestEnterState     (const std::string& name);
        bool requestLeaveState     (const std::string& name);

        OIS::InputManager* getInputSystem() { return this->inputSystem_; }
        bool checkJoyStickID(const std::string& idString) const;
        unsigned int getJoyStickQuantity() const
            { return devices_.size() - InputDeviceEnumerator::FirstJoyStick; }

#ifdef ORXONOX_PLATFORM_LINUX
        // HACK!
        static void grabMouse();
        static void ungrabMouse();
#endif

        void update(const Clock& time);

        static InputManager& getInstance()    { assert(singletonRef_s); return *singletonRef_s; }
        static InputManager* getInstancePtr() { return singletonRef_s; }

        // console commands
        static void calibrate();
        static void reload();

    private: // functions
        // don't mess with a Singleton
        InputManager(const InputManager&);

        // Intenal methods
        void _initialiseKeyboard();
        void _initialiseMouse(unsigned int windowWidth, unsigned int windowHeight);
        void _initialiseJoySticks();

        void _startCalibration();
        void _stopCalibration();

        void _destroyState(InputState* state);

        void _reload();

        void _updateActiveStates();
        bool _configureInputState(InputState* state, const std::string& name, bool bAlwaysGetsInput, bool bTransparent, int priority);

        void setConfigValues();

    private: // variables
        OIS::InputManager*                  inputSystem_;          //!< OIS input manager
        std::vector<InputDevice*>           devices_;              //!< List of all input devices (keyboard, mouse, joy sticks)
        size_t                              windowHnd_;            //!< Render window handle
        InputManagerState                   internalState_;        //!< Current internal state

        // some internally handled states and handlers
        InputState*                         stateEmpty_;
        KeyDetector*                        keyDetector_;          //!< KeyDetector instance
        InputBuffer*                        calibratorCallbackBuffer_;

        std::map<std::string, InputState*>  inputStatesByName_;

        std::set<InputState*>               stateEnterRequests_;   //!< Request to enter a new state
        std::set<InputState*>               stateLeaveRequests_;   //!< Request to leave a running state
        std::set<InputState*>               stateDestroyRequests_; //!< Request to destroy a state

        std::map<int, InputState*>          activeStates_;
        std::vector<InputState*>            activeStatesTicked_;

        static InputManager*                singletonRef_s;
    };
}

#endif /* _InputManager_H__ */
