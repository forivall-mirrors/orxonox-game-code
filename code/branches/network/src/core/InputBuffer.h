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

#ifndef _InputBuffer_H__
#define _InputBuffer_H__

#include "CorePrereqs.h"

#include <string>
#include <list>

#include "InputInterfaces.h"
#include "Tickable.h"

namespace orxonox
{
  class _CoreExport InputBufferListener
  {};

  class _CoreExport InputBuffer : public KeyHandler, public OrxonoxClass
  {
    struct InputBufferListenerTuple
    {
      InputBufferListener* listener_;
      void (InputBufferListener::*function_)();
      bool bListenToAllChanges_;
      bool bOnlySingleInput_;
      char char_;
    };

  public:
    InputBuffer(const std::string allowedChars =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ‰ˆ¸ƒ÷‹0123456789 \\\"(){}[]<>.:,;_-+*/=!?|$&%^");

    void setConfigValues();

    template <class T>
    void registerListener(T* listener, void (T::*function)(), bool bOnlySingleInput)
    {
      struct InputBufferListenerTuple newListener = {listener, (void (InputBufferListener::*)())function, true, bOnlySingleInput, ' '};
      this->listeners_.insert(this->listeners_.end(), newListener);
    }
    template <class T>
    void registerListener(T* listener, void (T::*function)() const, bool bOnlySingleInput)
    {
      struct InputBufferListenerTuple newListener = {listener, (void (InputBufferListener::*)())function, true, bOnlySingleInput, ' '};
      this->listeners_.insert(this->listeners_.end(), newListener);
    }

    template <class T>
    void registerListener(T* listener, void (T::*function)(), char char_, bool bOnlySingleInput)
    {
      struct InputBufferListenerTuple newListener = {listener, (void (InputBufferListener::*)())function, false, bOnlySingleInput, char_};
      this->listeners_.insert(this->listeners_.end(), newListener);
    }
    template <class T>
    void registerListener(T* listener, void (T::*function)() const, char char_, bool bOnlySingleInput)
    {
      struct InputBufferListenerTuple newListener = {listener, (void (InputBufferListener::*)())function, false, bOnlySingleInput, char_};
      this->listeners_.insert(this->listeners_.end(), newListener);
    }

    void set(const std::string& input);
    void append(const std::string& input);
    void append(const char& input);
    void removeLast();
    void clear();

    void updated();
    void updated(const char& update, bool bSingleInput);

    inline std::string get() const
    { return this->buffer_; }

  private:  // functions
    bool charIsAllowed(const char& input);

    void keyPressed (const KeyEvent& evt);
    void keyReleased(const KeyEvent& evt) { }
    void keyHeld    (const KeyEvent& evt);
    void processKey (const KeyEvent &e);

    void tickInput(float dt, const HandlerState& state);

  private: // variables
    std::string buffer_;
    std::list<InputBufferListenerTuple> listeners_;
    std::string allowedChars_;

    KeyCode::Enum lastKey_;
    float timeSinceKeyPressed_;
    float timeSinceKeyRepeated_;
    int keysToRepeat_;

    // configured values
    float keyRepeatDeleay_;
    float keyRepeatTime_;
  };
}

#endif /* _InputBuffer_H__ */
