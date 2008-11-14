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
    Implementation of the ExtendedInputState class.
*/

#include "ExtendedInputState.h"

#include <assert.h>
#include "util/Debug.h"

namespace orxonox
{
    using namespace InputDevice;

    void ExtendedInputState::numberOfJoySticksChanged(unsigned int n)
    {
        unsigned int oldSize = joyStickHandlers_.size();
        joyStickHandlers_.resize(n);

        if (oldSize > n)
        {
            // we have to add all the handlers in joyStickHandlersAll_ to the joyStickHandlers_[>n]
            for (unsigned int j = 0; j < joyStickHandlersAll_.size(); ++j)
            {
                for (unsigned int i = oldSize; i < n; ++i)
                {
                    joyStickHandlers_[i].push_back(joyStickHandlersAll_[j]);
                }
            }
        }
        update();
    }

    void ExtendedInputState::keyPressed(const KeyEvent& evt)
    {
        for (unsigned int i = 0; i < keyHandlers_.size(); i++)
            keyHandlers_[i]->keyPressed(evt);
    }

    void ExtendedInputState::keyReleased(const KeyEvent& evt)
    {
        for (unsigned int i = 0; i < keyHandlers_.size(); i++)
            keyHandlers_[i]->keyReleased(evt);
    }

    void ExtendedInputState::keyHeld(const KeyEvent& evt)
    {
        for (unsigned int i = 0; i < keyHandlers_.size(); i++)
            keyHandlers_[i]->keyHeld(evt);
    }


    void ExtendedInputState::mouseMoved(IntVector2 abs, IntVector2 rel, IntVector2 clippingSize)
    {
        for (unsigned int i = 0; i < mouseHandlers_.size(); i++)
            mouseHandlers_[i]->mouseMoved(abs, rel, clippingSize);
    }

    void ExtendedInputState::mouseScrolled(int abs, int rel)
    {
        for (unsigned int i = 0; i < mouseHandlers_.size(); i++)
            mouseHandlers_[i]->mouseScrolled(abs, rel);
    }

    void ExtendedInputState::mouseButtonPressed(MouseButtonCode::ByEnum id)
    {
        for (unsigned int i = 0; i < mouseHandlers_.size(); i++)
            mouseHandlers_[i]->mouseButtonPressed(id);
    }

    void ExtendedInputState::mouseButtonReleased(MouseButtonCode::ByEnum id)
    {
        for (unsigned int i = 0; i < mouseHandlers_.size(); i++)
            mouseHandlers_[i]->mouseButtonReleased(id);
    }

    void ExtendedInputState::mouseButtonHeld(MouseButtonCode::ByEnum id)
    {
        for (unsigned int i = 0; i < mouseHandlers_.size(); i++)
            mouseHandlers_[i]->mouseButtonHeld(id);
    }


    void ExtendedInputState::joyStickAxisMoved(unsigned int joyStickID, unsigned int axis, float value)
    {
        assert(joyStickID < joyStickHandlers_.size());
        for (unsigned int i = 0; i < joyStickHandlers_[joyStickID].size(); i++)
            joyStickHandlers_[joyStickID][i]->joyStickAxisMoved(joyStickID, axis, value);
    }

    void ExtendedInputState::joyStickButtonPressed(unsigned int joyStickID, JoyStickButtonCode::ByEnum id)
    {
        assert(joyStickID < joyStickHandlers_.size());
        for (unsigned int i = 0; i < joyStickHandlers_[joyStickID].size(); i++)
            joyStickHandlers_[joyStickID][i]->joyStickButtonPressed(joyStickID, id);
    }

    void ExtendedInputState::joyStickButtonReleased(unsigned int joyStickID, JoyStickButtonCode::ByEnum id)
    {
        assert(joyStickID < joyStickHandlers_.size());
        for (unsigned int i = 0; i < joyStickHandlers_[joyStickID].size(); i++)
            joyStickHandlers_[joyStickID][i]->joyStickButtonReleased(joyStickID, id);
    }

    void ExtendedInputState::joyStickButtonHeld(unsigned int joyStickID, JoyStickButtonCode::ByEnum id)
    {
        assert(joyStickID < joyStickHandlers_.size());
        for (unsigned int i = 0; i < joyStickHandlers_[joyStickID].size(); i++)
            joyStickHandlers_[joyStickID][i]->joyStickButtonHeld(joyStickID, id);
    }


    /**
    @brief
        Adds a key handler.
    @param handler
        Pointer to the handler object.
    @return
        True if added, false if handler already existed.
    */
    bool ExtendedInputState::addKeyHandler(KeyHandler* handler)
    {
        if (!handler)
            return false;

        // see whether the handler already is in the list
        for (std::vector<KeyHandler*>::iterator it = keyHandlers_.begin(); it != keyHandlers_.end(); it++)
        {
            if ((*it) == handler)
            {
                return false;
            }
        }
        keyHandlers_.push_back(handler);
        update();
        return true;
    }

    /**
    @brief
        Removes a Key handler from the state.
    @param handler
        Pointer to the handler.
    @return
        True if removal was successful, false if handler was not found.
    */
    bool ExtendedInputState::removeKeyHandler(KeyHandler* handler)
    {
        if (!handler)
            return false;

        for (std::vector<KeyHandler*>::iterator it = keyHandlers_.begin(); it != keyHandlers_.end(); it++)
        {
            if ((*it) == handler)
            {
                keyHandlers_.erase(it);
                update();
                return true;
            }
        }
        return false;
    }


    /**
    @brief
        Adds a mouse handler.
    @param handler
        Pointer to the handler object.
    @return
        True if added, false if handler already existed.
    */
    bool ExtendedInputState::addMouseHandler(MouseHandler* handler)
    {
        if (!handler)
            return false;

        // see whether the handler already is in the list
        for (std::vector<MouseHandler*>::iterator it = mouseHandlers_.begin(); it != mouseHandlers_.end(); it++)
        {
            if ((*it) == handler)
            {
                return false;
            }
        }
        mouseHandlers_.push_back(handler);
        update();
        return true;
    }

    /**
    @brief
        Removes a mouse handler from the state.
    @param handler
        Pointer to the handler.
    @return
        True if removal was successful, false if handler was not found.
    */
    bool ExtendedInputState::removeMouseHandler(MouseHandler* handler)
    {
        if (!handler)
            return false;

        for (std::vector<MouseHandler*>::iterator it = mouseHandlers_.begin(); it != mouseHandlers_.end(); it++)
        {
            if ((*it) == handler)
            {
                mouseHandlers_.erase(it);
                update();
                return true;
            }
        }
        return false;
    }


    /**
    @brief
        Adds a joy stick handler.
    @param handler
        Pointer to the handler object.
    @param joyStickID
        ID of the joy stick
    @return
        True if added, false if handler already existed.
    */
    bool ExtendedInputState::addJoyStickHandler(JoyStickHandler* handler, unsigned int joyStickID)
    {
        if (!handler || joyStickID >= joyStickHandlers_.size())
            return false;

        // see whether the handler already is in the list
        for (std::vector<JoyStickHandler*>::iterator it = joyStickHandlers_[joyStickID].begin();
            it != joyStickHandlers_[joyStickID].end(); it++)
        {
            if ((*it) == handler)
            {
                return false;
            }
        }
        joyStickHandlers_[joyStickID].push_back(handler);
        update();
        return true;
    }

    /**
    @brief
        Removes a joy stick handler from the state.
    @param handler
        Pointer to the handler.
    @param joyStickID
        ID of the joy stick
    @return
        True if removal was successful, false if handler was not found.
    */
    bool ExtendedInputState::removeJoyStickHandler(JoyStickHandler* handler, unsigned int joyStickID)
    {
        if (!handler || joyStickID >= joyStickHandlers_.size())
            return false;

        // remove it from the list of all-joystick handlers if present
        for (std::vector<JoyStickHandler*>::iterator it = joyStickHandlersAll_.begin();
            it != joyStickHandlersAll_.end(); it++)
        {
            if ((*it) == handler)
            {
                joyStickHandlersAll_.erase(it);
            }
        }

        // remove handler from the list of seperate lists of handlers
        for (std::vector<JoyStickHandler*>::iterator it = joyStickHandlers_[joyStickID].begin();
            it != joyStickHandlers_[joyStickID].end(); it++)
        {
            if ((*it) == handler)
            {
                joyStickHandlers_[joyStickID].erase(it);
                update();
                return true;
            }
        }
        return false;
    }

    /**
    @brief
        Adds a joy stick handler.
    @param handler
        Pointer to the handler object.
    @return
        True if added, false if handler already existed.
    */
    bool ExtendedInputState::addJoyStickHandler(JoyStickHandler* handler)
    {
        if (!handler)
            return false;

        // see whether the handler already is in the big list
        for (std::vector<JoyStickHandler*>::iterator it = joyStickHandlersAll_.begin();
            it != joyStickHandlersAll_.end(); it++)
        {
            if ((*it) == handler)
            {
                return false;
            }
        }
        joyStickHandlersAll_.push_back(handler);
        update();

        // now, we have to add it to all the separate lists
        for (unsigned int iJoyStick = 0; iJoyStick < joyStickHandlers_.size(); ++iJoyStick)
            addJoyStickHandler(handler, iJoyStick);
        return true;
    }

    /**
    @brief
        Removes a joy stick handler from the state.
    @param handler
        Pointer to the handler.
    @return
        True if removal was successful, false if handler was not found.
    */
    bool ExtendedInputState::removeJoyStickHandler(JoyStickHandler* handler)
    {
        if (!handler)
            return false;

        bool success = false;
        // remove from all lists in a loop (also removes it from the big list)
        for (unsigned int iJoyStick = 0; iJoyStick < joyStickHandlers_.size(); ++iJoyStick)
            success |= removeJoyStickHandler(handler, iJoyStick);

        return success;
    }

    /**
    @brief
        Adds a handler of any kind. dynamic_cast determines to which list it is added.
    @param handler
        Pointer to the handler object.
    @return
        True if added, false if handler already existed.
    */
    bool ExtendedInputState::addHandler(InputHandler* handler)
    {
        bool success = false;

        success |= addKeyHandler(dynamic_cast<KeyHandler*>(handler));
        success |= addMouseHandler(dynamic_cast<MouseHandler*>(handler));
        success |= addJoyStickHandler(dynamic_cast<JoyStickHandler*>(handler));

        return success;
    }

    /**
    @brief
        Removes a handler from all lists.
    @param handler
        Pointer to the handler.
    @return
        True if removal was successful, false if handler was not found.
    */
    bool ExtendedInputState::removeHandler(InputHandler* handler)
    {
        bool success = false;

        success |= removeKeyHandler(dynamic_cast<KeyHandler*>(handler));
        success |= removeMouseHandler(dynamic_cast<MouseHandler*>(handler));
        success |= removeJoyStickHandler(dynamic_cast<JoyStickHandler*>(handler));

        return success;
    }

    void ExtendedInputState::tickInput(float dt)
    {
        for (unsigned int i = 0; i < allHandlers_.size(); ++i)
        {
            allHandlers_[i]->tickInput(dt);
        }
    }

    void ExtendedInputState::tickInput(float dt, unsigned int device)
    {
        switch (device)
        {
        case Keyboard:
            for (unsigned int i = 0; i < keyHandlers_.size(); ++i)
                keyHandlers_[i]->tickKey(dt);
            break;

        case Mouse:
            for (unsigned int i = 0; i < mouseHandlers_.size(); ++i)
                mouseHandlers_[i]->tickMouse(dt);
            break;

        default: // joy sticks
            for (unsigned int i = 0; i < joyStickHandlers_[device - 2].size(); ++i)
                joyStickHandlers_[device - 2][i]->tickJoyStick(dt, device - 2);
            break;
        }
    }

    void ExtendedInputState::update()
    {
        // we can use a set to have a list of unique pointers (an object can implement all 3 handlers)
        std::set<InputHandler*> tempSet;
        for (unsigned int iHandler = 0; iHandler < keyHandlers_.size(); iHandler++)
            tempSet.insert(keyHandlers_[iHandler]);
        for (unsigned int iHandler = 0; iHandler < mouseHandlers_.size(); iHandler++)
            tempSet.insert(mouseHandlers_[iHandler]);
        for (unsigned int iJoyStick = 0; iJoyStick < joyStickHandlers_.size(); iJoyStick++)
            for (unsigned int iHandler = 0; iHandler < joyStickHandlers_[iJoyStick].size(); iHandler++)
                tempSet.insert(joyStickHandlers_[iJoyStick][iHandler]);

        // copy the content of the map back to the actual vector
        allHandlers_.clear();
        for (std::set<InputHandler*>::const_iterator itHandler = tempSet.begin();
            itHandler != tempSet.end(); itHandler++)
            allHandlers_.push_back(*itHandler);

        // update the deviceEnabled options
        setInputDeviceEnabled(Keyboard, (keyHandlers_.size() != 0));
        setInputDeviceEnabled(Mouse, (mouseHandlers_.size() != 0));
        for (unsigned int i = 0; i < joyStickHandlers_.size(); ++i)
            setInputDeviceEnabled(2 + i, (joyStickHandlers_[i].size() != 0));

        this->bHandlersChanged_ = true;
    }
}
