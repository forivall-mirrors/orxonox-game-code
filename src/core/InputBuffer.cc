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
 *      Reto Grieder
 *
 */

#include "InputBuffer.h"

#include <iostream>

#include "util/Clipboard.h"
#include "CoreIncludes.h"
#include "ConfigValueIncludes.h"

namespace orxonox
{
    InputBuffer::InputBuffer()
    {
        RegisterObject(InputBuffer);

        this->buffer_ = "";
        this->cursor_ = 0;
        this->allowedChars_ = "abcdefghijklmnopqrstuvwxyz \
                               ABCDEFGHIJKLMNOPQRSTUVWXYZ \
                               ‰ÎÔˆ¸ƒÀœ÷‹·‚‡ÈÍËÌÓÏÛÙÚ˙˚˘ \
                               0123456789 \
                               \\\"(){}[]<>.:,;_-+*/=!?|$&%^~#";

        this->lastKey_ = KeyCode::Unassigned;
        this->timeSinceKeyPressed_ = 0.0f;
        this->timeSinceKeyRepeated_ = 0.0f;
        this->keysToRepeat_ = 0;

        setConfigValues();
    }

    InputBuffer::InputBuffer(const std::string allowedChars)
    {
        RegisterObject(InputBuffer);

        this->allowedChars_ = allowedChars;
        this->buffer_ = "";
        this->cursor_ = 0;

        this->lastKey_ = KeyCode::Unassigned;
        this->timeSinceKeyPressed_ = 0.0f;
        this->timeSinceKeyRepeated_ = 0.0f;
        this->keysToRepeat_ = 0;

        setConfigValues();
    }

    void InputBuffer::setConfigValues()
    {
        SetConfigValue(keyRepeatDeleay_, 0.4).description("Key repeat deleay of the input buffer");
        SetConfigValue(keyRepeatTime_, 0.022).description("Key repeat time of the input buffer");

        if (keyRepeatDeleay_ < 0.0)
        {
            ResetConfigValue(keyRepeatDeleay_);
        }
        if (keyRepeatTime_ < 0.0)
        {
            ResetConfigValue(keyRepeatTime_);
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
        for (std::list<BaseInputBufferListenerTuple*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if ((*it)->bListenToAllChanges_)
                (*it)->callFunction();
        }
    }

    void InputBuffer::updated(const char& update, bool bSingleInput)
    {
        for (std::list<BaseInputBufferListenerTuple*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if ((!(*it)->trueKeyFalseChar_) && ((*it)->bListenToAllChanges_ || ((*it)->char_ == update)) && (!(*it)->bOnlySingleInput_ || bSingleInput))
                (*it)->callFunction();
        }
    }

    bool InputBuffer::charIsAllowed(const char& input)
    {
        if (this->allowedChars_ == "")
            return true;
        else
            return (this->allowedChars_.find(input) != std::string::npos);
    }


    void InputBuffer::processKey(const KeyEvent &evt)
    {
        if (evt.isModifierDown(KeyboardModifier::Alt) && evt.key == KeyCode::Tab)
            return;

        for (std::list<BaseInputBufferListenerTuple*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if ((*it)->trueKeyFalseChar_ && ((*it)->key_ == evt.key))
                (*it)->callFunction();
        }

        if (evt.isModifierDown(KeyboardModifier::Ctrl))
        {
            if (evt.key == KeyCode::V)
                this->insert(fromClipboard());
            else if (evt.key == KeyCode::C)
                toClipboard(this->buffer_);
            else if (evt.key == KeyCode::X)
            {
                toClipboard(this->buffer_);
                this->clear();
            }
        }
        else if (evt.isModifierDown(KeyboardModifier::Shift))
        {
            if (evt.key == KeyCode::Insert)
                this->insert(fromClipboard());
            else if (evt.key == KeyCode::Delete)
            {
                toClipboard(this->buffer_);
                this->clear();
            }
        }

        this->insert((char)evt.text);
    }

    /**
        @brief This tick() function is called by the InputManager if the InputBuffer is active.
        @param dt Delta time
    */
    void InputBuffer::tickInput(float dt, const HandlerState& state)
    {
        timeSinceKeyPressed_ += dt;
        if (keysToRepeat_ < 10 && timeSinceKeyPressed_ > keyRepeatDeleay_)
        {
            // initial time out has gone by, start repeating keys
            while (timeSinceKeyPressed_ - timeSinceKeyRepeated_ > keyRepeatTime_)
            {
                timeSinceKeyRepeated_ += keyRepeatTime_;
                keysToRepeat_++;
            }
        }
    }

    void InputBuffer::keyPressed(const KeyEvent &evt)
    {
        lastKey_ = evt.key;
        timeSinceKeyPressed_ = 0.0;
        timeSinceKeyRepeated_ = keyRepeatDeleay_;
        keysToRepeat_ = 0;

        processKey(evt);
    }

    void InputBuffer::keyHeld(const KeyEvent& evt)
    {
        if (evt.key == lastKey_)
        {
            while (keysToRepeat_)
            {
                processKey(evt);
                keysToRepeat_--;
            }
        }
    }
}
