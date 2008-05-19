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
  InputBuffer::InputBuffer(const std::string allowedChars) :
    buffer_(""),
    allowedChars_(allowedChars),
    lastKey_(KeyCode::Unassigned),
    timeSinceKeyPressed_(0.0f),
    timeSinceKeyRepeated_(0.0f),
    keysToRepeat_(0)
  {
    RegisterObject(InputBuffer);
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
  /*
  void InputBuffer::registerListener(InputBufferListener* listener, void (InputBufferListener::*function)(), bool bOnlySingleInput)
  {
    struct InputBufferListenerTuple newListener = {listener, function, true, bOnlySingleInput, ' '};
    listeners_.insert(listeners_.end(), newListener);
  }

  void InputBuffer::registerListener(InputBufferListener* listener, void (InputBufferListener::*function)(), char char_, bool bOnlySingleInput)
  {
    struct InputBufferListenerTuple newListener = {listener, function, false, bOnlySingleInput, char_};
    listeners_.insert(listeners_.end(), newListener);
  }
  */
  void InputBuffer::set(const std::string& input)
  {
    buffer_ = "";
    append(input);
  }

  void InputBuffer::append(const std::string& input)
  {
    for (unsigned int i = 0; i < input.size(); i++)
    {
      if (charIsAllowed(input[i]))
        buffer_ += input[i];

      updated(input[i], false);
    }
    updated();
  }

  void InputBuffer::append(const char& input)
  {
    if (charIsAllowed(input))
      buffer_ += input;

    updated(input, true);
  }

  void InputBuffer::clear()
  {
    buffer_ = "";
    updated();
  }

  void InputBuffer::removeLast()
  {
    buffer_ = buffer_.substr(0, buffer_.size() - 1);
    updated();
  }

  void InputBuffer::updated()
  {
    for (std::list<InputBufferListenerTuple>::iterator it = listeners_.begin(); it != listeners_.end(); ++it)
    {
      if ((*it).bListenToAllChanges_)
        (*(*it).listener_.*(*it).function_)();
    }
  }

  void InputBuffer::updated(const char& update, bool bSingleInput)
  {
    for (std::list<InputBufferListenerTuple>::iterator it = listeners_.begin(); it != listeners_.end(); ++it)
    {
      if (((*it).bListenToAllChanges_ || ((*it).char_ == update)) && (!(*it).bOnlySingleInput_ || bSingleInput))
        (*(*it).listener_.*(*it).function_)();
    }
  }

  bool InputBuffer::charIsAllowed(const char& input)
  {
    if (allowedChars_ == "")
      return true;
    else
      return (allowedChars_.find(input) != std::string::npos);
  }


  void InputBuffer::processKey(const KeyEvent &evt)
  {
    if (evt.isModifierDown(KeyboardModifier::Ctrl))
    {
      if (evt.key == KeyCode::V)
        append(fromClipboard());
      else if (evt.key == KeyCode::C)
        toClipboard(buffer_);
      else if (evt.key == KeyCode::X)
      {
        toClipboard(buffer_);
        clear();
      }
    }
    else if (evt.isModifierDown(KeyboardModifier::Shift))
    {
      if (evt.key == KeyCode::Insert)
      {
        append(fromClipboard());
      }
      else if (evt.key == KeyCode::Delete)
      {
        toClipboard(buffer_);
        clear();
      }
    }
    //std::string input = keyboard_->getAsString(e.key);
    /*if (input.size() >= 1)
    append(input[0]);*/

    append((char)evt.text);
  }

  /**
  * This tick() function is called by the InputManager if the InputBuffer is active.
  * @param dt Delta time
  */
  void InputBuffer::tick(float dt)
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

  bool InputBuffer::keyPressed(const KeyEvent &evt)
  {
    lastKey_ = evt.key;
    timeSinceKeyPressed_ = 0.0;
    timeSinceKeyRepeated_ = keyRepeatDeleay_;
    keysToRepeat_ = 0;

    processKey(evt);
    return true;
  }

  bool InputBuffer::keyHeld(const KeyEvent& evt)
  {
    if (evt.key == lastKey_)
    {
      while (keysToRepeat_)
      {
        processKey(evt);
        keysToRepeat_--;
      }
    }
    return true;
  }

}
