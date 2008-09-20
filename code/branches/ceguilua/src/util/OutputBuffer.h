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

/**
    @file OutputBuffer.h
    @brief Declaration of the OutputBuffer class.

    The OutputBuffer acts almost like std::ostream. You can put text and other values to the
    OutputBuffer by using the << operator. The OutputBuffer stores the text and calls registerd
    listeners if new text gets assigned.
    The listeners are then able to retrieve the text line by line.

    It's important to know that getLine actually removes the line from the OutputBuffer, so it's
    better to only have one "active" listener.
*/

#ifndef _OutputBuffer_H__
#define _OutputBuffer_H__

#include <list>
#include <sstream>
#include <iostream>

#include "UtilPrereqs.h"

namespace orxonox
{
    /**
        @brief A pure virtual baseclass for classes that want to register as listener to an OutputBuffer.

        This class is pure virtual, so an inheriting class has to implement the function on it's own.
        The function get's called, if an instance of the inheriting class registers as a listener at
        an OutputBuffer and this buffer changes.
    */
    class _UtilExport OutputBufferListener
    {
        friend class OutputBuffer;

        public:
            virtual ~OutputBufferListener() {}

        private:
            virtual void outputChanged() = 0;
    };

    /**
        @brief The OutputBuffer acts almost like std::ostream and stores the assigned text.

        If text gets assigned by using the << operator or another function, the OutputBuffer
        calls it's listeners, allowing them to retrieve the text line by line.

        It's important to know that getLine actually removes the line from the OutputBuffer, so it's
        better to only have one "active" listener.
    */
    class _UtilExport OutputBuffer
    {
        public:
            OutputBuffer() {}
            ~OutputBuffer() {}

            /**
                @brief Puts some object/value to the OutputBuffer. The text gets assigned and the OutputBuffer calls it's listeners.
                @param object The object/value to assign
            */
            template <class T>
            inline OutputBuffer& operator<<(T object)
            {
                this->stream_ << object;
                this->callListeners();
                return *this;
            }

            /**
                @brief Reads the stored text of the other OutputBuffer and calls the listeners.
                @param object The other OutputBuffer
            */
            template <const OutputBuffer&>
            inline OutputBuffer& operator<<(const OutputBuffer& object)
            {
                this->stream_ << object.stream_.rdbuf();
                this->callListeners();
                return *this;
            }

            OutputBuffer& operator<<(std::ostream& (*manipulator)(std::ostream&));
            OutputBuffer& operator<<(std::ios& (*manipulator)(std::ios&));
            OutputBuffer& operator<<(std::ios_base& (*manipulator)(std::ios_base&));

            /**
                @brief Does the same like operator<<: Assigns the object to the stream and calls the listeners.
                @param object The object/value
            */
            template <class T>
            inline void add(T object)
            {
                this->stream_ << object;
                this->callListeners();
            }

            /**
                @brief Assigns an object/value and adds std::endl.
                @param object The object/value
            */
            template <class T>
            inline void addLine(T object)
            {
                this->stream_ << object << std::endl;
                this->callListeners();
            }

            /**
                @brief Puts std::endl to the stream and calls the listeners.
            */
            inline void newline()
            {
                this->stream_ << std::endl;
                this->callListeners();
            }

            /**
                @brief Flushes the stored text (~empties the OutputBuffer).
            */
            inline void flush()
            {
                this->stream_.flush();
            }

            bool getLine(std::string* output);

            void registerListener(OutputBufferListener* listener);
            void unregisterListener(OutputBufferListener* listener);

            /**
                @brief Returns the internal stringstream object.
            */
            inline std::stringstream& getStream()
            {
                return this->stream_;
            }

        private:
            void callListeners();

            std::stringstream stream_;                   //! The stringstream that stores the assigned text
            std::list<OutputBufferListener*> listeners_; //! A list of all listeners
    };
}

#endif /* _OutputBuffer_H__ */
