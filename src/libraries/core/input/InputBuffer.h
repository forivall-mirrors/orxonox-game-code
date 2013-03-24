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

#include "InputPrereqs.h"

#include <list>
#include <string>
#include "core/class/OrxonoxClass.h"
#include "InputHandler.h"

namespace orxonox
{
    class BaseInputBufferListenerTuple
    {
    public:
        BaseInputBufferListenerTuple(bool bListenToAllChanges, bool bOnlySingleInput,
            bool trueKeyFalseChar, char _char, KeyCode::ByEnum key)
            : bListenToAllChanges_(bListenToAllChanges), bOnlySingleInput_(bOnlySingleInput),
              trueKeyFalseChar_(trueKeyFalseChar), char_(_char), key_(key)
        { }
        virtual ~BaseInputBufferListenerTuple() { }
        virtual void callFunction() = 0;
        bool bListenToAllChanges_;
        bool bOnlySingleInput_;
        bool trueKeyFalseChar_;
        char char_;
        KeyCode::ByEnum key_;
    };

    template <class T>
    class InputBufferListenerTuple : public BaseInputBufferListenerTuple
    {
    public:
        InputBufferListenerTuple(T* listener, void (T::*function)(), bool bListenToAllChanges,
            bool bOnlySingleInput, bool trueKeyFalseChar, char _char, KeyCode::ByEnum key)
            : BaseInputBufferListenerTuple(bListenToAllChanges, bOnlySingleInput, trueKeyFalseChar, _char, key),
              listener_(listener), function_(function)
        { }
        virtual ~InputBufferListenerTuple() { }
        void callFunction()
        {
            (listener_->*function_)();
        }
        T* listener_;
        void (T::*function_)();
    };

    class _CoreExport InputBuffer : public InputHandler, public OrxonoxClass
    {
        public:
            InputBuffer();
            ~InputBuffer();
            InputBuffer(const std::string& allowedChars);

            void setConfigValues();

            unsigned int getMaxLength() const { return this->maxLength_; }
            void setMaxLength(unsigned int length);

            template <class T>
            void registerListener(T* listener, void (T::*function)(), bool bOnlySingleInput)
            {
                InputBufferListenerTuple<T>* newTuple = new InputBufferListenerTuple<T>(listener, (void (T::*)())function, true, bOnlySingleInput, false, '\0', KeyCode::Unassigned);
                this->listeners_.insert(this->listeners_.end(), newTuple);
            }
            template <class T>
            void registerListener(T* listener, void (T::*function)() const, bool bOnlySingleInput)
            {
                InputBufferListenerTuple<T>* newTuple = new InputBufferListenerTuple<T>(listener, (void (T::*)())function, true, bOnlySingleInput, false, '\0', KeyCode::Unassigned);
                this->listeners_.insert(this->listeners_.end(), newTuple);
            }
            template <class T>
            void registerListener(T* listener, void (T::*function)(), char _char, bool bOnlySingleInput)
            {
                InputBufferListenerTuple<T>* newTuple = new InputBufferListenerTuple<T>(listener, (void (T::*)())function, false, bOnlySingleInput, false, _char, KeyCode::Unassigned);
                this->listeners_.insert(this->listeners_.end(), newTuple);
            }
            template <class T>
            void registerListener(T* listener, void (T::*function)() const, char _char, bool bOnlySingleInput)
            {
                InputBufferListenerTuple<T>* newTuple = new InputBufferListenerTuple<T>(listener, (void (T::*)())function, false, bOnlySingleInput, false, _char, KeyCode::Unassigned);
                this->listeners_.insert(this->listeners_.end(), newTuple);
            }

            template <class T>
            void registerListener(T* listener, void (T::*function)(), KeyCode::ByEnum key)
            {
                InputBufferListenerTuple<T>* newTuple = new InputBufferListenerTuple<T>(listener, (void (T::*)())function, false, true, true, '\0', key);
                this->listeners_.insert(this->listeners_.end(), newTuple);
            }
            template <class T>
            void registerListener(T* listener, void (T::*function)() const, KeyCode::ByEnum key)
            {
                InputBufferListenerTuple<T>* newTuple = new InputBufferListenerTuple<T>(listener, (void (T::*)())function, false, true, true, '\0', key);
                this->listeners_.insert(this->listeners_.end(), newTuple);
            }

            template <class T>
            void unregisterListener(T* listener)
            {
                for (std::list<BaseInputBufferListenerTuple*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); )
                {
                    InputBufferListenerTuple<T>* refListener = static_cast<InputBufferListenerTuple<T>*>(*it);
                    if (refListener && refListener->listener_ == listener)
                        this->listeners_.erase(it++);
                    else
                        it++;
                }
            }

            void set(const std::string& input, bool update = true);
            void insert(const std::string& input, bool update = true);
            void insert(const char& input, bool update = true);
            void clear(bool update = true);
            void removeAtCursor(bool update = true);
            void removeBehindCursor(bool update = true);

            void updated();
            void updated(const char& update, bool bSingleInput);

            inline const std::string& get() const
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

            void buttonPressed(const KeyEvent& evt);

        private:
            bool charIsAllowed(const char& input);

            void buttonHeld   (const KeyEvent& evt);
            void processKey   (const KeyEvent& evt);

            void keyboardUpdated(float dt);

            std::string buffer_;
            std::list<BaseInputBufferListenerTuple*> listeners_;
            std::string allowedChars_;
            unsigned int maxLength_;
            unsigned int cursor_;

            KeyCode::ByEnum lastKey_;
            float timeSinceKeyPressed_;
            float timeSinceKeyRepeated_;
            int keysToRepeat_;

            float keyRepeatDeleay_;
            float keyRepeatTime_;
    };
}

#endif /* _InputBuffer_H__ */
