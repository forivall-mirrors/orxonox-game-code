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
#include <cassert>
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
        bool setHandler(InputHandler* handler);

    private:
        SimpleInputState();
        ~SimpleInputState() { }

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
        void joyStickAxisMoved     (unsigned int joyStickID, unsigned int axis, float value);

        void update();
        void numberOfJoySticksChanged(unsigned int n);

        void onEnter();
        void onLeave();

        KeyHandler*                   keyHandler_;
        MouseHandler*                 mouseHandler_;
        std::vector<JoyStickHandler*> joyStickHandler_;
        JoyStickHandler*              joyStickHandlerAll_;
        std::vector<InputHandler*>    allHandlers_;
    };

    inline void SimpleInputState::updateInput(float dt)
    {
        for (unsigned int i = 0; i < allHandlers_.size(); ++i)
        {
            allHandlers_[i]->updateInput(dt);
        }
    }

    inline void SimpleInputState::updateInput(float dt, unsigned int device)
    {
        switch (device)
        {
        case InputDevice::Keyboard:
            if (keyHandler_)
                keyHandler_->updateKey(dt);
            break;

        case InputDevice::Mouse:
            if (mouseHandler_)
                mouseHandler_->updateMouse(dt);
            break;

        default: // joy sticks
            if (joyStickHandler_[device - 2])
                joyStickHandler_[device - 2]->updateJoyStick(dt, device - 2);
            break;
        }
    }

    inline void SimpleInputState::keyPressed(const KeyEvent& evt)
    {
        if (keyHandler_)
            keyHandler_->keyPressed(evt);
    }

    inline void SimpleInputState::keyReleased(const KeyEvent& evt)
    {
        if (keyHandler_)
            keyHandler_->keyReleased(evt);
    }

    inline void SimpleInputState::keyHeld(const KeyEvent& evt)
    {
        if (keyHandler_)
            keyHandler_->keyHeld(evt);
    }

    inline void SimpleInputState::mouseMoved(IntVector2 abs, IntVector2 rel, IntVector2 clippingSize)
    {
        if (mouseHandler_)
            mouseHandler_->mouseMoved(abs, rel, clippingSize);
    }

    inline void SimpleInputState::mouseScrolled(int abs, int rel)
    {
        if (mouseHandler_)
            mouseHandler_->mouseScrolled(abs, rel);
    }

    inline void SimpleInputState::mouseButtonPressed(MouseButtonCode::ByEnum id)
    {
        if (mouseHandler_)
            mouseHandler_->mouseButtonPressed(id);
    }

    inline void SimpleInputState::mouseButtonReleased(MouseButtonCode::ByEnum id)
    {
        if (mouseHandler_)
            mouseHandler_->mouseButtonReleased(id);
    }

    inline void SimpleInputState::mouseButtonHeld(MouseButtonCode::ByEnum id)
    {
        if (mouseHandler_)
            mouseHandler_->mouseButtonHeld(id);
    }

    inline void SimpleInputState::joyStickAxisMoved(unsigned int joyStickID, unsigned int axis, float value)
    {
        assert(joyStickID < joyStickHandler_.size());
        if (joyStickHandler_[joyStickID])
            joyStickHandler_[joyStickID]->joyStickAxisMoved(joyStickID, axis, value);
    }

    inline void SimpleInputState::joyStickButtonPressed(unsigned int joyStickID, JoyStickButtonCode::ByEnum id)
    {
        assert(joyStickID < joyStickHandler_.size());
        if (joyStickHandler_[joyStickID])
            joyStickHandler_[joyStickID]->joyStickButtonPressed(joyStickID, id);
    }

    inline void SimpleInputState::joyStickButtonReleased(unsigned int joyStickID, JoyStickButtonCode::ByEnum id)
    {
        assert(joyStickID < joyStickHandler_.size());
        if (joyStickHandler_[joyStickID])
            joyStickHandler_[joyStickID]->joyStickButtonReleased(joyStickID, id);
    }

    inline void SimpleInputState::joyStickButtonHeld(unsigned int joyStickID, JoyStickButtonCode::ByEnum id)
    {
        assert(joyStickID < joyStickHandler_.size());
        if (joyStickHandler_[joyStickID])
            joyStickHandler_[joyStickID]->joyStickButtonHeld(joyStickID, id);
    }
}

#endif /* _SimpleInputState_H__ */
