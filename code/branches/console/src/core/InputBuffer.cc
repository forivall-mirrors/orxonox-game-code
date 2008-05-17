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
        this->allowedChars_ = "abcdefghijklmnopqrstuvwxyz \
                               ABCDEFGHIJKLMNOPQRSTUVWXYZ \
                               äëïöüÄËÏÖÜáâàéêèíîìóôòúûù \
                               0123456789 \
                               \\\"(){}[]<>.:,;_-+*/=!?|$&%^~#";
        this->keyboard_ = InputManager::getSingleton().getKeyboard();
        this->buffer_ = "";
        this->cursor_ = 0;

        InputManager::getSingleton().feedInputBuffer(this);
    }

    InputBuffer::InputBuffer(const std::string allowedChars)
    {
        this->allowedChars_ = allowedChars;
        this->keyboard_ = InputManager::getSingleton().getKeyboard();
        this->buffer_ = "";
        this->cursor_ = 0;

        InputManager::getSingleton().feedInputBuffer(this);
    }

    void InputBuffer::unregisterListener(InputBufferListener* listener)
    {
        for (std::list<InputBufferListenerTuple>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); )
        {
            if ((*it).listener_ == listener)
                this->listeners_.erase(it++);
            else
                ++it;
        }
    }

    void InputBuffer::set(const std::string& input, bool update)
    {
        this->clear(false);
        this->insert(input, update);
    }

    void InputBuffer::insert(const std::string& input, bool update)
    {
        for (unsigned int i = 0; i < input.size(); ++i)
        {
            this->insert(input[i], false);

            if (update)
                this->updated(input[i], false);
        }

        if (update)
            this->updated();
    }

    void InputBuffer::insert(const char& input, bool update)
    {
        if (this->charIsAllowed(input))
        {
            this->buffer_.insert(this->cursor_, 1, input);
            ++this->cursor_;
        }

        if (update)
            this->updated(input, true);
    }

    void InputBuffer::clear(bool update)
    {
        this->buffer_ = "";
        this->cursor_ = 0;

        if (update)
            this->updated();
    }

    void InputBuffer::removeBehindCursor(bool update)
    {
        if (this->cursor_ > 0)
        {
            --this->cursor_;
            this->buffer_.erase(this->cursor_, 1);

            if (update)
                this->updated();
        }
    }

    void InputBuffer::removeAtCursor(bool update)
    {
        if (this->cursor_ < this->buffer_.size())
        {
            this->buffer_.erase(this->cursor_, 1);

            if (update)
                this->updated();
        }
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
            if ((!(*it).trueKeyFalseChar_) && ((*it).bListenToAllChanges_ || ((*it).char_ == update)) && (!(*it).bOnlySingleInput_ || bSingleInput))
                (*(*it).listener_.*(*it).function_)();
        }
    }

    bool InputBuffer::charIsAllowed(const char& input)
    {
        if (this->allowedChars_ == "")
            return true;
        else
            return (this->allowedChars_.find(input) != std::string::npos);
    }

    bool InputBuffer::keyPressed(const OIS::KeyEvent &e)
    {
        for (std::list<InputBufferListenerTuple>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if ((*it).trueKeyFalseChar_ && ((*it).key_ == e.key))
                (*(*it).listener_.*(*it).function_)();
        }

        if (this->keyboard_->isModifierDown(OIS::Keyboard::Ctrl))
        {
            if (e.key == OIS::KC_V)
            {
                this->insert(fromClipboard());
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
                this->insert(fromClipboard());
                return true;
            }
            else if (e.key == OIS::KC_DELETE)
            {
                toClipboard(this->buffer_);
                this->clear();
                return true;
            }
        }

        this->insert((char)e.text);
        return true;
    }

    bool InputBuffer::keyReleased(const OIS::KeyEvent &e)
    {
        return true;
    }
}
