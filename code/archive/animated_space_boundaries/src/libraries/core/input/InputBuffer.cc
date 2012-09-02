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

#include "util/Clipboard.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"

namespace orxonox
{
    InputBuffer::InputBuffer()
    {
        RegisterRootObject(InputBuffer);

        this->cursor_ = 0;
        this->maxLength_ = 1024;
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

    InputBuffer::InputBuffer(const std::string& allowedChars)
    {
        RegisterRootObject(InputBuffer);

        this->maxLength_ = 1024;
        this->allowedChars_ = allowedChars;
        this->cursor_ = 0;

        this->lastKey_ = KeyCode::Unassigned;
        this->timeSinceKeyPressed_ = 0.0f;
        this->timeSinceKeyRepeated_ = 0.0f;
        this->keysToRepeat_ = 0;

        setConfigValues();
    }

    InputBuffer::~InputBuffer()
    {
        for (std::list<BaseInputBufferListenerTuple*>::const_iterator it = this->listeners_.begin();
            it != this->listeners_.end(); ++it)
            delete *it;
    }

    void InputBuffer::setConfigValues()
    {
        SetConfigValue(keyRepeatDeleay_, 0.4).description("Key repeat delay of the input buffer");
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

    void InputBuffer::setMaxLength(unsigned int length)
    {
        this->maxLength_ = length;
        if (this->buffer_.size() > length)
            this->buffer_.resize(length);
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
            if (this->buffer_.size() >= this->maxLength_)
                return;
            this->buffer_.insert(this->cursor_, 1, input);
            ++this->cursor_;
        }

        if (update)
            this->updated(input, true);
    }

    void InputBuffer::clear(bool update)
    {
        this->buffer_.clear();
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
        if (this->allowedChars_.empty())
            return true;
        else
            return (this->allowedChars_.find(input) != std::string::npos);
    }


    void InputBuffer::processKey(const KeyEvent& evt)
    {
        // Prevent disaster when switching applications
        if (evt.isModifierDown(KeyboardModifier::Alt) && evt.getKeyCode() == KeyCode::Tab)
            return;

        for (std::list<BaseInputBufferListenerTuple*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
        {
            if ((*it)->trueKeyFalseChar_ && ((*it)->key_ == evt.getKeyCode()))
                (*it)->callFunction();
        }

        if (evt.isModifierDown(KeyboardModifier::Ctrl))
        {
            if (evt.getKeyCode() == KeyCode::V)
                this->insert(fromClipboard());
            else if (evt.getKeyCode() == KeyCode::C)
                toClipboard(this->buffer_);
            else if (evt.getKeyCode() == KeyCode::X)
            {
                toClipboard(this->buffer_);
                this->clear();
            }
        }
        else if (evt.isModifierDown(KeyboardModifier::Shift))
        {
            if (evt.getKeyCode() == KeyCode::Insert)
                this->insert(fromClipboard());
            else if (evt.getKeyCode() == KeyCode::Delete)
            {
                toClipboard(this->buffer_);
                this->clear();
            }
        }

        this->insert(static_cast<char>(evt.getText()));
    }

    /**
        @brief This update() function is called by the InputState if the InputBuffer is active.
        @param dt Delta time
    */
    void InputBuffer::keyboardUpdated(float dt)
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

    void InputBuffer::buttonPressed(const KeyEvent& evt)
    {
        lastKey_ = evt.getKeyCode();
        timeSinceKeyPressed_ = 0.0;
        timeSinceKeyRepeated_ = keyRepeatDeleay_;
        keysToRepeat_ = 0;

        processKey(evt);
    }

    void InputBuffer::buttonHeld(const KeyEvent& evt)
    {
        if (evt.getKeyCode() == lastKey_)
        {
            while (keysToRepeat_)
            {
                processKey(evt);
                keysToRepeat_--;
            }
        }
    }
}
