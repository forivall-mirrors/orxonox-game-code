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

#ifndef _SimpleInputState_H__
#define _SimpleInputState_H__

#include "core/CorePrereqs.h"

#include <vector>
#include "InputInterfaces.h"
#include "InputState.h"

namespace orxonox
{
    class _CoreExport SimpleInputState : public InputState
    {
        friend class InputManager;

    public:
        void setKeyHandler        (KeyHandler* handler) { keyHandler_ = handler; update(); }
        void setMouseHandler      (MouseHandler* handler) { mouseHandler_ = handler; update(); }
        bool setJoyStickHandler   (JoyStickHandler* handler, unsigned int joyStickID);
        bool setJoyStickHandler   (JoyStickHandler* handler);
        bool setHandler(InputTickable* handler);
        void removeAndDestroyAllHandlers();

    private:
        SimpleInputState();
        ~SimpleInputState() { }

        void tickInput(float dt);
        void tickInput(float dt, unsigned int device);

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

        void update();
        void numberOfJoySticksChanged(unsigned int n);

        KeyHandler*                   keyHandler_;
        MouseHandler*                 mouseHandler_;
        std::vector<JoyStickHandler*> joyStickHandler_;
        JoyStickHandler*              joyStickHandlerAll_;
        std::vector<InputTickable*>   allHandlers_;
    };
}

#endif /* _SimpleInputState_H__ */
