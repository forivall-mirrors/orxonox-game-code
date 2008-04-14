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
    {
    };

    class _CoreExport InputBuffer : public OIS::KeyListener
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
            InputBuffer();

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

            inline void activate()
                { this->setActivated(true); }
            inline void deactivate()
                { this->setActivated(false); }
            inline void setActivated(bool bActivated)
                { if (this->bActivated_ != bActivated) { this->bActivated_ = bActivated; this->activityChanged(); } else { this->bActivated_ = bActivated; } }
            inline bool isActivated() const
                { return this->bActivated_; }

            void activityChanged() const;

        private:
            bool charIsAllowed(const char& input);

            bool keyPressed(const OIS::KeyEvent &e);
            bool keyReleased(const OIS::KeyEvent &e);

            OIS::Keyboard* keyboard_;
            bool bActivated_;
            std::string buffer_;
            std::list<InputBufferListenerTuple> listeners_;
            std::string allowedChars_;
    };
}

#endif /* _InputBuffer_H__ */
