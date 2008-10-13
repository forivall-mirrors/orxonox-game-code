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
#include <vector>
#include <stack>
#include "util/Math.h"
#include "core/OrxonoxClass.h"
#include "InputInterfaces.h"

namespace orxonox
{
    /**
    @brief
        Helper class to realise a vector<int[4]>
    */
    class POVStates
    {
    public:
        int& operator[](unsigned int index) { return povStates[index]; }
        int povStates[4];
    };

    /**
    @brief
        Helper class to realise a vector< {int[4], int[4]} >
    */
    class SliderStates
    {
    public:
        IntVector2 sliderStates[4];
    };

    struct JoyStickCalibration
    {
        int zeroStates[24];
        float positiveCoeff[24];
        float negativeCoeff[24];
    };

    /**
    @brief
        Captures and distributes mouse and keyboard input.
    */
    class _CoreExport InputManager
        : public OrxonoxClass,
        public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
    {
        // --> setConfigValues is private
        friend class ClassIdentifier<InputManager>;
        // let Core class use tick(.)
        friend class Core;

    public:
        enum InputManagerState
        {
            Uninitialised    = 0x00,
            OISReady         = 0x01,
            InternalsReady   = 0x02,
            Ticking          = 0x04,
            Calibrating      = 0x08,
            ReloadRequest    = 0x10,
            JoyStickSupport  = 0x20 // used with ReloadRequest to store a bool
        };

        InputManager ();
        ~InputManager();

        void initialise(size_t windowHnd, int windowWidth, int windowHeight, bool joyStickSupport = true);

        void reloadInputSystem(bool joyStickSupport = true);

        void clearBuffers();

        unsigned int  numberOfKeyboards() { return keyboard_ ? 1 : 0; }
        unsigned int  numberOfMice()      { return mouse_    ? 1 : 0; }
        unsigned int  numberOfJoySticks() { return joySticksSize_; }

        void setWindowExtents(const int width, const int height);
        void setKeyDetectorCallback(const std::string& command);

        template <class T>
        T* createInputState(const std::string& name, int priority);

        InputState* getState       (const std::string& name);
        InputState* getCurrentState();
        ExtendedInputState* getMasterInputState() { return this->stateMaster_; }
        bool requestDestroyState   (const std::string& name);
        bool requestEnterState     (const std::string& name);
        bool requestLeaveState     (const std::string& name);

        void tick(float dt);

        static InputManager& getInstance()    { assert(singletonRef_s); return *singletonRef_s; }
        static InputManager* getInstancePtr() { return singletonRef_s; }

        // console commands
        static void calibrate();
        static void reload(bool joyStickSupport = true);

    public: // variables
        static EmptyHandler                 EMPTY_HANDLER;

    private: // functions
        // don't mess with a Singleton
        InputManager (const InputManager&);

        // Intenal methods
        void _initialiseKeyboard();
        void _initialiseMouse();
        void _initialiseJoySticks();
        void _configureNumberOfJoySticks();

        void _destroyKeyboard();
        void _destroyMouse();
        void _destroyJoySticks();
        void _destroyState(InputState* state);
        void _clearBuffers();

        void _reload(bool joyStickSupport);

        void _completeCalibration();

        void _fireAxis(unsigned int iJoyStick, int axis, int value);
        unsigned int _getJoystick(const OIS::JoyStickEvent& arg);

        void _updateActiveStates();
        bool _configureInputState(InputState* state, const std::string& name, int priority);

        // input events
        bool mousePressed  (const OIS::MouseEvent    &arg, OIS::MouseButtonID id);
        bool mouseReleased (const OIS::MouseEvent    &arg, OIS::MouseButtonID id);
        bool mouseMoved    (const OIS::MouseEvent    &arg);
        bool keyPressed    (const OIS::KeyEvent      &arg);
        bool keyReleased   (const OIS::KeyEvent      &arg);
        bool buttonPressed (const OIS::JoyStickEvent &arg, int button);
        bool buttonReleased(const OIS::JoyStickEvent &arg, int button);
        bool axisMoved     (const OIS::JoyStickEvent &arg, int axis);
        bool sliderMoved   (const OIS::JoyStickEvent &arg, int id);
        bool povMoved      (const OIS::JoyStickEvent &arg, int id);
        // don't remove that! Or else add OIS as dependency library to orxonox.
        bool vector3Moved  (const OIS::JoyStickEvent &arg, int id) { return true; }

        void setConfigValues();

    private: // variables
        OIS::InputManager*                  inputSystem_;          //!< OIS input manager
        OIS::Keyboard*                      keyboard_;             //!< OIS mouse
        OIS::Mouse*                         mouse_;                //!< OIS keyboard
        std::vector<OIS::JoyStick*>         joySticks_;            //!< OIS joy sticks
        unsigned int                        joySticksSize_;
        unsigned int                        devicesNum_;
        size_t                              windowHnd_;            //!< Render window handle
        InputManagerState                   internalState_;        //!< Current internal state

        // some internally handled states and handlers
        SimpleInputState*                   stateEmpty_;
        ExtendedInputState*                 stateMaster_;          //!< Always active master input state
        KeyDetector*                        keyDetector_;        //!< KeyDetector instance
        InputBuffer*                        calibratorCallbackBuffer_;

        std::map<std::string, InputState*>  inputStatesByName_;
        std::map<int, InputState*>          inputStatesByPriority_;

        std::set<InputState*>               stateEnterRequests_;   //!< Request to enter a new state
        std::set<InputState*>               stateLeaveRequests_;   //!< Request to leave a running state
        std::set<InputState*>               stateDestroyRequests_; //!< Request to destroy a state

        std::map<int, InputState*>          activeStates_;
        std::vector<InputState*>            activeStatesTop_;      //!< Current input states for joy stick events.
        std::vector<InputState*>            activeStatesTicked_;   //!< Current input states for joy stick events.

        // joystick calibration
        //std::vector<int> marginalsMaxConfig_;
        //std::vector<int> marginalsMinConfig_;
        int                                 marginalsMax_[24];
        int                                 marginalsMin_[24];
        bool                                bCalibrated_;
        bool                                bCalibrating_;

        unsigned int                        keyboardModifiers_;    //!< Bit mask representing keyboard modifiers.
        std::vector<POVStates>              povStates_;            //!< Keeps track of the joy stick POV states.
        std::vector<SliderStates>           sliderStates_;         //!< Keeps track of the possibly two slider axes.
        std::vector<JoyStickCalibration>    joySticksCalibration_; 

        std::vector<Key>                    keysDown_;
        std::vector<MouseButtonCode::ByEnum>      mouseButtonsDown_;
        std::vector<std::vector<JoyStickButtonCode::ByEnum> >  joyStickButtonsDown_;

        static std::string                  bindingCommmandString_s;
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
    T* InputManager::createInputState(const std::string& name, int priority)
    {
        T* state = new T;
        if (_configureInputState(state, name, priority))
            return state;
        else
        {
            delete state;
            return 0;
        }
    }
}

#endif /* _InputManager_H__ */
