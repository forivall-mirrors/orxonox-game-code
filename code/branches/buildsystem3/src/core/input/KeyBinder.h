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
    Different definitions of input processing.
*/

#ifndef _KeyBinder_H__
#define _KeyBinder_H__

#include "core/CorePrereqs.h"

#include <vector>
#include <cassert>

#include "InputInterfaces.h"
#include "Button.h"
#include "HalfAxis.h"
#include "InputCommands.h"
#include "JoyStickDeviceNumberListener.h"

namespace orxonox
{
    /**
    @brief
        Handles mouse, keyboard and joy stick input while in the actual game mode.
        Manages the key bindings.
    */
    class _CoreExport KeyBinder : public KeyHandler, public MouseHandler, public JoyStickHandler,
                                  public JoyStickDeviceNumberListener
    {
    public:
        KeyBinder ();
        virtual ~KeyBinder();

        void loadBindings(const std::string& filename, const std::string& defaultFilename);
        void clearBindings();
        bool setBinding(const std::string& binding, const std::string& name, bool bTemporary = false);
        void setConfigValues();
        void resetJoyStickAxes();

    protected: // functions
        void tickInput(float dt);
        void tickKey(float dt) { }
        void tickMouse(float dt);
        void tickJoyStick(float dt, unsigned int joyStick);
        // internal
        void tickHalfAxis(HalfAxis& halfAxis);

        void buttonThresholdChanged();
        // from JoyStickDeviceNumberListener interface
        virtual void JoyStickDeviceNumberChanged(unsigned int value);
        void initialiseJoyStickBindings();
        void compilePointerLists();

        void keyPressed (const KeyEvent& evt);
        void keyReleased(const KeyEvent& evt);
        void keyHeld    (const KeyEvent& evt);

        void mouseButtonPressed (MouseButtonCode::ByEnum id);
        void mouseButtonReleased(MouseButtonCode::ByEnum id);
        void mouseButtonHeld    (MouseButtonCode::ByEnum id);
        void mouseMoved         (IntVector2 abs, IntVector2 rel, IntVector2 clippingSize);
        void mouseScrolled      (int abs, int rel);

        void joyStickButtonPressed (unsigned int joyStickID, JoyStickButtonCode::ByEnum id);
        void joyStickButtonReleased(unsigned int joyStickID, JoyStickButtonCode::ByEnum id);
        void joyStickButtonHeld    (unsigned int joyStickID, JoyStickButtonCode::ByEnum id);
        void joyStickAxisMoved     (unsigned int joyStickID, unsigned int axis, float value);

    protected: // variables
        //! Currently active joy sticks
        unsigned int numberOfJoySticks_;

        //! Actual key bindings for keys on the keyboard
        Button keys_            [KeyCode::numberOfKeys];
        //! Number of mouse buttons in KeyBinder (+4)
        static const unsigned int numberOfMouseButtons_ = MouseButtonCode::numberOfButtons + 4;
        //! Actual key bindings for mouse buttons including the wheel(s)
        Button mouseButtons_    [numberOfMouseButtons_];
        //! Actual key bindings for mouse axes
        HalfAxis mouseAxes_     [MouseAxisCode::numberOfAxes * 2];

        //! Helper class to use something like std:vector<Button[64]>
        struct JoyStickButtonVector
        {
            Button& operator[](unsigned int index) { return buttons[index]; }
            Button buttons[JoyStickButtonCode::numberOfButtons];
        };
        //! Actual key bindings for joy stick buttons
        std::vector<JoyStickButtonVector> joyStickButtons_;
        //! Helper class to use something like std:vector<HalfAxis[48]>
        struct JoyStickAxisVector
        {
            HalfAxis& operator[](unsigned int index) { return halfAxes[index]; }
            HalfAxis halfAxes[JoyStickAxisCode::numberOfAxes * 2];
        };
        //! Actual key bindings for joy stick axes (and sliders)
        std::vector<JoyStickAxisVector> joyStickAxes_;

        //! Pointer map with all Buttons, including half axes
        std::map<std::string, Button*> allButtons_;
        //! Pointer list with all half axes
        std::vector<HalfAxis*> allHalfAxes_;

        /**
        @brief
            Commands that have additional parameters (axes) are executed at the end of
            the tick() so that all values can be buffered for single execution.
        */
        std::vector<BufferedParamCommand*> paramCommandBuffer_;

        //! Keeps track of the absolute mouse value (incl. scroll wheel)
        int mousePosition_[2];
        //! Used to derive mouse input if requested
        int mouseRelative_[2];
        float deriveTime_;

        //! Config file used. ConfigFileType::NoType in case of KeyDetector. Also indicates whether we've already loaded.
        ConfigFileType configFile_;

    private:
        //##### ConfigValues #####
        //! Whether to filter small value analog input
        bool bFilterAnalogNoise_;
        //! Threshold for analog triggers until which the state is 0.
        float analogThreshold_;
        //! Threshold for analog triggers until which the button is not pressed.
        float buttonThreshold_;
        //! Derive mouse input for absolute values?
        bool bDeriveMouseInput_;
        //! Accuracy of the mouse input deriver. The higher the more precise, but laggier.
        float derivePeriod_;
        //! mouse sensitivity
        float mouseSensitivity_;
        //! mouse sensitivity if mouse input is derived
        float mouseSensitivityDerived_;
        //! Equals one step of the mousewheel
        int mouseWheelStepSize_;

        //##### Constant config variables #####
        // Use some value at about 1000. This can be configured with mouseSensitivity_ anyway.
        static const int mouseClippingSize_ = 1024;
    };

    inline void KeyBinder::keyPressed (const KeyEvent& evt)
    { assert(!keys_[evt.key].name_.empty()); keys_[evt.key].execute(KeybindMode::OnPress); }

    inline void KeyBinder::keyReleased(const KeyEvent& evt)
    { assert(!keys_[evt.key].name_.empty()); keys_[evt.key].execute(KeybindMode::OnRelease); }

    inline void KeyBinder::keyHeld    (const KeyEvent& evt)
    { assert(!keys_[evt.key].name_.empty()); keys_[evt.key].execute(KeybindMode::OnHold); }


    inline void KeyBinder::mouseButtonPressed (MouseButtonCode::ByEnum id)
    { mouseButtons_[id].execute(KeybindMode::OnPress); }

    inline void KeyBinder::mouseButtonReleased(MouseButtonCode::ByEnum id)
    { mouseButtons_[id].execute(KeybindMode::OnRelease); }

    inline void KeyBinder::mouseButtonHeld    (MouseButtonCode::ByEnum id)
    { mouseButtons_[id].execute(KeybindMode::OnHold); }


    inline void KeyBinder::joyStickButtonPressed (unsigned int joyStickID, JoyStickButtonCode::ByEnum id)
    { joyStickButtons_[joyStickID][id].execute(KeybindMode::OnPress); }

    inline void KeyBinder::joyStickButtonReleased(unsigned int joyStickID, JoyStickButtonCode::ByEnum id)
    { joyStickButtons_[joyStickID][id].execute(KeybindMode::OnRelease); }

    inline void KeyBinder::joyStickButtonHeld    (unsigned int joyStickID, JoyStickButtonCode::ByEnum id)
    { joyStickButtons_[joyStickID][id].execute(KeybindMode::OnHold); }

    inline void KeyBinder::tickInput(float dt)
    {
        // execute all buffered bindings (additional parameter)
        for (unsigned int i = 0; i < paramCommandBuffer_.size(); i++)
        {
            paramCommandBuffer_[i]->rel_ *= dt;
            paramCommandBuffer_[i]->execute();
        }

        // always reset the relative movement of the mouse
        for (unsigned int i = 0; i < MouseAxisCode::numberOfAxes * 2; i++)
            mouseAxes_[i].relVal_ = 0.0f;
    }
}

#endif /* _KeyBinder_H__ */
