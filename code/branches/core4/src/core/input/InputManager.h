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

#include "core/CorePrereqs.h"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <ois/OISKeyboard.h>
#include <ois/OISMouse.h>
#include <ois/OISJoyStick.h>

#include "util/Math.h"
#include "util/OrxEnum.h"
#include "core/OrxonoxClass.h"
#include "InputInterfaces.h"

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
    class _CoreExport InputManager
        : public OrxonoxClass,
        public OIS::KeyListener, public OIS::MouseListener
    {
        // --> setConfigValues is private
        friend class ClassIdentifier<InputManager>;
        friend class JoyStick;

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

        unsigned int  numberOfKeyboards() { return keyboard_ ? 1 : 0; }
        unsigned int  numberOfMice()      { return mouse_    ? 1 : 0; }
        unsigned int  numberOfJoySticks() { return joySticks_.size(); }

        void setWindowExtents(const int width, const int height);
        void setKeyDetectorCallback(const std::string& command);

        template <class T>
        T* createInputState(const std::string& name, bool bAlwaysGetsInput = false, bool bTransparent = false, InputStatePriority priority = InputStatePriority::Dynamic);

        InputState* getState       (const std::string& name);
        InputState* getCurrentState();
        bool requestDestroyState   (const std::string& name);
        bool requestEnterState     (const std::string& name);
        bool requestLeaveState     (const std::string& name);

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

    public: // variables
        static EmptyHandler                 EMPTY_HANDLER;

    private: // functions for friends
        OIS::InputManager* getInputSystem() { return this->inputSystem_; }
        bool checkJoyStickID(const std::string&);

    private: // functions
        // don't mess with a Singleton
        InputManager (const InputManager&);

        // Intenal methods
        void _initialiseKeyboard();
        void _initialiseMouse(unsigned int windowWidth, unsigned int windowHeight);
        void _initialiseJoySticks();
        void _configureJoySticks();

        void _loadCalibration();
        void _startCalibration();
        void _completeCalibration();
        void _evaluateCalibration();

        void _destroyKeyboard();
        void _destroyMouse();
        void _destroyJoySticks();
        void _destroyState(InputState* state);

        void _reload();

        void _fireAxis(unsigned int iJoyStick, int axis, int value);
        unsigned int _getJoystick(const OIS::JoyStickEvent& arg);

        void _updateActiveStates();
        bool _configureInputState(InputState* state, const std::string& name, bool bAlwaysGetsInput, bool bTransparent, int priority);

        // input events
        bool mousePressed  (const OIS::MouseEvent    &arg, OIS::MouseButtonID id);
        bool mouseReleased (const OIS::MouseEvent    &arg, OIS::MouseButtonID id);
        bool mouseMoved    (const OIS::MouseEvent    &arg);
        bool keyPressed    (const OIS::KeyEvent      &arg);
        bool keyReleased   (const OIS::KeyEvent      &arg);

        void setConfigValues();
        void _calibrationFileCallback();

    private: // variables
        OIS::InputManager*                  inputSystem_;          //!< OIS input manager
        OIS::Keyboard*                      keyboard_;             //!< OIS mouse
        OIS::Mouse*                         mouse_;                //!< OIS keyboard
        std::vector<JoyStick*>              joySticks_;            //!< Orxonox joy sticks
        unsigned int                        devicesNum_;
        size_t                              windowHnd_;            //!< Render window handle
        InputManagerState                   internalState_;        //!< Current internal state

        // some internally handled states and handlers
        SimpleInputState*                   stateEmpty_;
        KeyDetector*                        keyDetector_;          //!< KeyDetector instance
        InputBuffer*                        calibratorCallbackBuffer_;

        std::map<std::string, InputState*>  inputStatesByName_;

        std::set<InputState*>               stateEnterRequests_;   //!< Request to enter a new state
        std::set<InputState*>               stateLeaveRequests_;   //!< Request to leave a running state
        std::set<InputState*>               stateDestroyRequests_; //!< Request to destroy a state

        std::map<int, InputState*>          activeStates_;
        std::vector<std::vector<InputState*> > activeStatesTriggered_;
        std::vector<InputState*>            activeStatesTicked_;

        unsigned int                        keyboardModifiers_;    //!< Bit mask representing keyboard modifiers.

        std::vector<Key>                    keysDown_;
        std::vector<MouseButtonCode::ByEnum> mouseButtonsDown_;

        static InputManager*                singletonRef_s;
    };

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
    template <class T>
    T* InputManager::createInputState(const std::string& name, bool bAlwaysGetsInput, bool bTransparent, InputStatePriority priority)
    {
        T* state = new T;
        if (_configureInputState(state, name, bAlwaysGetsInput, bTransparent, priority))
            return state;
        else
        {
            delete state;
            return 0;
        }
    }
}

#endif /* _InputManager_H__ */
