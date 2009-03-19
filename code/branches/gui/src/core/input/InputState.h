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

#ifndef _InputState_H__
#define _InputState_H__

#include "core/CorePrereqs.h"

#include <string>
#include <vector>
#include "core/Executor.h"
#include "InputInterfaces.h"

namespace orxonox
{
    class _CoreExport InputState
    {
        friend class InputManager;

    public:
        const std::string& getName() const { return name_; }
        int getPriority()            const { return priority_; }

        bool isInputDeviceEnabled(unsigned int device)
        {
            if (device < bInputDeviceEnabled_.size())
                return bInputDeviceEnabled_[device];
            else
                return false;
        }

        bool handlersChanged() { return this->bHandlersChanged_; }
        void resetHandlersChanged() { bHandlersChanged_ = false; }

        virtual void onEnter() { if (executorOnEnter_) (*executorOnEnter_)(); }
        virtual void onLeave() { if (executorOnLeave_) (*executorOnLeave_)(); }

        virtual void registerOnEnter(Executor* executor)      { executorOnEnter_ = executor; }
        virtual void unRegisterOnEnter()                      { executorOnEnter_ = 0; }
        virtual void registerOnLeave(Executor* executor)      { executorOnLeave_ = executor; }
        virtual void unRegisterOnLeave()                      { executorOnLeave_ = 0; }

        virtual void updateInput(float dt, unsigned int device) = 0;
        virtual void updateInput(float dt) = 0;

        virtual void keyPressed (const KeyEvent& evt) = 0;
        virtual void keyReleased(const KeyEvent& evt) = 0;
        virtual void keyHeld    (const KeyEvent& evt) = 0;

        virtual void mouseButtonPressed (MouseButtonCode::ByEnum id) = 0;
        virtual void mouseButtonReleased(MouseButtonCode::ByEnum id) = 0;
        virtual void mouseButtonHeld    (MouseButtonCode::ByEnum id) = 0;
        virtual void mouseMoved         (IntVector2 abs, IntVector2 rel, IntVector2 clippingSize) = 0;
        virtual void mouseScrolled      (int abs, int rel) = 0;

        virtual void joyStickButtonPressed (unsigned int joyStickID, JoyStickButtonCode::ByEnum id) = 0;
        virtual void joyStickButtonReleased(unsigned int joyStickID, JoyStickButtonCode::ByEnum id) = 0;
        virtual void joyStickButtonHeld    (unsigned int joyStickID, JoyStickButtonCode::ByEnum id) = 0;
        virtual void joyStickAxisMoved     (unsigned int joyStickID, unsigned int axis, float value) = 0;

    protected:
        InputState() : bHandlersChanged_(false), priority_(0), executorOnEnter_(0), executorOnLeave_(0) { }
        virtual ~InputState() { }

        virtual void numberOfJoySticksChanged(unsigned int n) = 0;
        void setInputDeviceEnabled(unsigned int device, bool bEnabled)
        {
            if (device < bInputDeviceEnabled_.size())
                bInputDeviceEnabled_[device] = bEnabled;
        }

        bool bHandlersChanged_;

    private:
        void setNumOfJoySticks(unsigned int n)
        {
            bInputDeviceEnabled_.resize(n + 2);
            numberOfJoySticksChanged(n);
        }
        void setName(const std::string& name)  { name_ = name; }
        void setPriority(int priority)         { priority_ = priority; }

        std::string                                 name_;
        int                                         priority_;
        std::vector<bool>                           bInputDeviceEnabled_;

        Executor*                                   executorOnEnter_;
        Executor*                                   executorOnLeave_;
    };
}

#endif /* _InputState_H__ */
