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
*/

#ifndef _ExtendedInputState_H__
#define _ExtendedInputState_H__

#include "core/CorePrereqs.h"

#include <vector>
#include "InputInterfaces.h"
#include "InputState.h"

namespace orxonox
{

    class _CoreExport ExtendedInputState : public InputState
    {
        friend class InputManager;

    public:
        bool addKeyHandler        (KeyHandler* handler);
        bool removeKeyHandler     (KeyHandler* handler);

        bool addMouseHandler      (MouseHandler* handler);
        bool removeMouseHandler   (MouseHandler* handler);

        bool addJoyStickHandler   (JoyStickHandler* handler, unsigned int joyStickID);
        bool removeJoyStickHandler(JoyStickHandler* handler, unsigned int joyStickID);
        bool addJoyStickHandler   (JoyStickHandler* handler);
        bool removeJoyStickHandler(JoyStickHandler* handler);

        bool addHandler(InputHandler* handler);
        bool removeHandler(InputHandler* handler);

    private:
        ExtendedInputState() { }
        ~ExtendedInputState() { }

        void updateInput(float dt);
        void updateInput(float dt, unsigned int device);

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
        void joyStickAxisMoved     (unsigned int joyStickID, unsigned int axis, float value);        void updateTickables();

        void numberOfJoySticksChanged(unsigned int n);
        void update();

        void onEnter();
        void onLeave();

        std::vector<KeyHandler*>                    keyHandlers_;
        std::vector<MouseHandler*>                  mouseHandlers_;
        std::vector<std::vector<JoyStickHandler*> > joyStickHandlers_;
        std::vector<JoyStickHandler*>               joyStickHandlersAll_;

        std::vector<InputHandler*> allHandlers_;
    };
}

#endif /* _ExtendedInputState_H__ */