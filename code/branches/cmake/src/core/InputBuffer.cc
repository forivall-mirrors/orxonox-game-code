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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "InputBuffer.h"

#include <iostream>

#include "util/Clipboard.h"
#include "InputManager.h"

namespace orxonox
{
    InputBuffer::InputBuffer()
    {
        //this->bActivated_ = false;
        this->allowedChars_ = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ‰ˆ¸ƒ÷‹0123456789 \\\"().:,;_-+*/=!?<>[|]";
        this->keyboard_ = InputManager::getSingleton().getKeyboard();
        this->buffer_ = "";

        //this->keyboard_->setEventCallback(this);
    }
/*
    void InputBuffer::registerListener(InputBufferListener* listener, void (InputBufferListener::*function)(), bool bOnlySingleInput)
    {
        struct InputBufferListenerTuple newListener = {listener, function, true, bOnlySingleInput, ' '};
        this->listeners_.insert(this->listeners_.end(), newListener);
    }

    void InputBuffer::registerListener(InputBufferListener* listener, void (InputBufferListener::*function)(), char char_, bool bOnlySingleInput)
    {
        struct InputBufferListenerTuple newListener = {listener, function, false, bOnlySingleInput, char_};
        this->listeners_.insert(this->listeners_.end(), newListener);
    }
*/
    void InputBuffer::set(const std::string& input)
    {
        this->buffer_ = "";
        this->append(input);
    }

    void InputBuffer::append(const std::string& input)
    {
        for (unsigned int i = 0; i < input.size(); i++)
        {
            if (this->charIsAllowed(input[i]))
                this->buffer_ += input[i];

            this->updated(input[i], false);
        }
        this->updated();
    }

    void InputBuffer::append(const char& input)
    {
        if (this->charIsAllowed(input))
            this->buffer_ += input;

        this->updated(input, true);
    }

    void InputBuffer::clear()
    {
        this->buffer_ = "";
        this->updated();
    }

    void InputBuffer::removeLast()
    {
        this->buffer_ = this->buffer_.substr(0, this->buffer_.size() - 1);
        this->updated();
    }

    void InputBuffer::updated()
    {
        for (std::list<InputBufferListenerTuple>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if ((*it).bListenToAllChanges_)
                (*(*it).listener_.*(*it).function_)();
        }
    }

    void InputBuffer::updated(const char& update, bool bSingleInput)
    {
        for (std::list<InputBufferListenerTuple>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if (((*it).bListenToAllChanges_ || ((*it).char_ == update)) && (!(*it).bOnlySingleInput_ || bSingleInput))
                (*(*it).listener_.*(*it).function_)();
        }
    }

    /*void InputBuffer::activityChanged() const
    {
    }*/

    bool InputBuffer::charIsAllowed(const char& input)
    {
        if (this->allowedChars_ == "")
            return true;
        else
            return (this->allowedChars_.find(input) != std::string::npos);
    }

    bool InputBuffer::keyPressed(const OIS::KeyEvent &e)
    {
        /*if (e.key == OIS::KC_NUMPADENTER)
        {
            this->setActivated(!this->isActivated());
            this->clear();
            return true;
        }*/

        if (this->keyboard_->isModifierDown(OIS::Keyboard::Ctrl))
        {
            if (e.key == OIS::KC_V)
            {
                this->append(fromClipboard());
                return true;
            }
            else if (e.key == OIS::KC_C)
            {
                toClipboard(this->buffer_);
                return true;
            }
            else if (e.key == OIS::KC_X)
            {
                toClipboard(this->buffer_);
                this->clear();
                return true;
            }
        }
        else if (this->keyboard_->isModifierDown(OIS::Keyboard::Shift))
        {
            if (e.key == OIS::KC_INSERT)
            {
                this->append(fromClipboard());
                return true;
            }
            else if (e.key == OIS::KC_DELETE)
            {
                toClipboard(this->buffer_);
                this->clear();
                return true;
            }
        }

        //std::cout << this->keyboard_->getAsString(e.key) << " / " << (char)e.text << std::endl;

        std::string input = this->keyboard_->getAsString(e.key);
        /*if (input.size() >= 1)
            this->append(input[0]);*/

        this->append((char)e.text);
        return true;
    }

    bool InputBuffer::keyReleased(const OIS::KeyEvent &e)
    {
        return true;
    }
}
