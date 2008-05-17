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

#ifndef _InputBuffer_H__
#define _InputBuffer_H__

#include "CorePrereqs.h"

#include <string>
#include <list>

#ifdef WIN32
#include <OIS/OISKeyboard.h>
#else
#include <OISKeyboard.h>
#endif

namespace orxonox
{
    class _CoreExport InputBufferListener
    {};

    class _CoreExport InputBuffer : public OIS::KeyListener
    {
        struct InputBufferListenerTuple
        {
            InputBufferListener* listener_;
            void (InputBufferListener::*function_)();
            bool bListenToAllChanges_;
            bool bOnlySingleInput_;
            bool trueKeyFalseChar_;
            char char_;
            OIS::KeyCode key_;
        };

        public:
            InputBuffer();
            InputBuffer(const std::string allowedChars);

            template <class T>
            void registerListener(T* listener, void (T::*function)(), bool bOnlySingleInput)
            {
                struct InputBufferListenerTuple newListener = {listener, (void (InputBufferListener::*)())function, true, bOnlySingleInput, false, '\0', OIS::KC_UNASSIGNED};
                this->listeners_.insert(this->listeners_.end(), newListener);
            }
            template <class T>
            void registerListener(T* listener, void (T::*function)() const, bool bOnlySingleInput)
            {
                struct InputBufferListenerTuple newListener = {listener, (void (InputBufferListener::*)())function, true, bOnlySingleInput, false, '\0', OIS::KC_UNASSIGNED};
                this->listeners_.insert(this->listeners_.end(), newListener);
            }

            template <class T>
            void registerListener(T* listener, void (T::*function)(), char _char, bool bOnlySingleInput)
            {
                struct InputBufferListenerTuple newListener = {listener, (void (InputBufferListener::*)())function, false, bOnlySingleInput, false, _char, OIS::KC_UNASSIGNED};
                this->listeners_.insert(this->listeners_.end(), newListener);
            }
            template <class T>
            void registerListener(T* listener, void (T::*function)() const, char _char, bool bOnlySingleInput)
            {
                struct InputBufferListenerTuple newListener = {listener, (void (InputBufferListener::*)())function, false, bOnlySingleInput, false, _char, OIS::KC_UNASSIGNED};
                this->listeners_.insert(this->listeners_.end(), newListener);
            }

            template <class T>
            void registerListener(T* listener, void (T::*function)(), OIS::KeyCode key)
            {
                struct InputBufferListenerTuple newListener = {listener, (void (InputBufferListener::*)())function, false, true, true, '\0', key};
                this->listeners_.insert(this->listeners_.end(), newListener);
            }
            template <class T>
            void registerListener(T* listener, void (T::*function)() const, OIS::KeyCode key)
            {
                struct InputBufferListenerTuple newListener = {listener, (void (InputBufferListener::*)())function, false, true, true, '\0', key};
                this->listeners_.insert(this->listeners_.end(), newListener);
            }

            void unregisterListener(InputBufferListener* listener);

            void set(const std::string& input, bool update = true);
            void insert(const std::string& input, bool update = true);
            void insert(const char& input, bool update = true);
            void clear(bool update = true);
            void removeAtCursor(bool update = true);
            void removeBehindCursor(bool update = true);

            void updated();
            void updated(const char& update, bool bSingleInput);

            inline std::string get() const
                { return this->buffer_; }
            inline unsigned int getSize() const
                { return this->buffer_.size(); }

            inline unsigned int getCursorPosition() const
                { return this->cursor_; }
            inline void setCursorPosition(unsigned int cursor)
                { if (cursor <= this->buffer_.size()) { this->cursor_ = cursor; } }
            inline void setCursorToEnd()
                { this->cursor_ = this->buffer_.size(); }
            inline void setCursorToBegin()
                { this->cursor_ = 0; }
            inline void increaseCursor()
                { if (this->cursor_ < this->buffer_.size()) { ++this->cursor_; } }
            inline void decreaseCursor()
                { if (this->cursor_ > 0) { --this->cursor_; } }

        private:
            bool charIsAllowed(const char& input);

            bool keyPressed(const OIS::KeyEvent &e);
            bool keyReleased(const OIS::KeyEvent &e);

            OIS::Keyboard* keyboard_;
            std::string buffer_;
            std::list<InputBufferListenerTuple> listeners_;
            std::string allowedChars_;
            unsigned int cursor_;
    };
}

#endif /* _InputBuffer_H__ */
