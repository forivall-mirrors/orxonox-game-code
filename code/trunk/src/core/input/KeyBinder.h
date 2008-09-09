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

#include "core/OrxonoxClass.h"
#include "InputInterfaces.h"
#include "Button.h"
#include "HalfAxis.h"

namespace orxonox
{
    /**
    @brief
        Handles mouse, keyboard and joy stick input while in the actual game mode.
        Manages the key bindings.
    */
    class _CoreExport KeyBinder : public KeyHandler, public MouseHandler, public JoyStickHandler, public OrxonoxClass
    {
    public:
        KeyBinder ();
        virtual ~KeyBinder();

        void loadBindings();
        void clearBindings();
        void setConfigValues();
        void resetJoyStickAxes();

    protected: // functions
        void tickInput(float dt);
        //void tickInput(float dt, int device);
        void tickKey(float dt) { }
        void tickMouse(float dt);
        void tickJoyStick(float dt, unsigned int joyStick);
        void tickDevices(unsigned int begin, unsigned int end);

        virtual void readTrigger(Button& button);

        void keyPressed (const KeyEvent& evt);
        void keyReleased(const KeyEvent& evt);
        void keyHeld    (const KeyEvent& evt);

        void mouseButtonPressed (MouseButton::Enum id);
        void mouseButtonReleased(MouseButton::Enum id);
        void mouseButtonHeld    (MouseButton::Enum id);
        void mouseMoved         (IntVector2 abs, IntVector2 rel, IntVector2 clippingSize);
        void mouseScrolled      (int abs, int rel);

        void joyStickButtonPressed (unsigned int joyStickID, JoyStickButton::Enum id);
        void joyStickButtonReleased(unsigned int joyStickID, JoyStickButton::Enum id);
        void joyStickButtonHeld    (unsigned int joyStickID, JoyStickButton::Enum id);
        void joyStickAxisMoved     (unsigned int joyStickID, unsigned int axis, float value);

    protected: // variables
        //! denotes the number of different keys there are in OIS.
        static const unsigned int nKeys_s = 0xEE;
        //! Actual key bindings as bundle for Press, Hold and Release
        Button keys_ [nKeys_s];

        //! denotes the number of different mouse buttons there are in OIS.
        static const unsigned int nMouseButtons_s = 8 + 2*2; // 8 buttons and 2 scroll wheels
        //! Actual key bindings as bundle for Press, Hold and Release
        Button mouseButtons_ [nMouseButtons_s];

        //! denotes the number of different joy stick buttons there are in OIS.
        static const unsigned int nJoyStickButtons_s = 32 + 4 * 4; // 32 buttons and 4 POVs with 4 buttons
        //! Actual key bindings as bundle for Press, Hold and Release
        Button joyStickButtons_ [nJoyStickButtons_s];

        //! denotes the number of half axes (every axis twice) there can be.
        static const unsigned int nHalfAxes_s = 56;
        /**
        * Array with all the half axes for mouse and joy sticks.
        * Keep in mind that the positions are fixed and that the first entry is the
        * positive one and the second is negative.
        * Sequence is as follows:
        *  0 -  3: Mouse x and y
        *  4 -  7: empty
        *  8 - 23: joy stick slider axes 1 to 8
        * 24 - 55: joy stick axes 1 - 16
        */
        HalfAxis halfAxes_[nHalfAxes_s];

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


        //##### ConfigValues #####

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
        //! Whether or not to clip abslute mouse values to 1024
        bool bClipMouse_;
    };
}

#endif /* _KeyBinder_H__ */
