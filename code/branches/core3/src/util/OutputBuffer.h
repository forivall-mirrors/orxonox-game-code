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

#ifndef _OutputBuffer_H__
#define _OutputBuffer_H__

#include <list>
#include <sstream>
#include <iostream>

#include "UtilPrereqs.h"

namespace orxonox
{
    class _UtilExport OutputBufferListener
    {
        friend class OutputBuffer;

        public:
            virtual ~OutputBufferListener() {}

        private:
            virtual void outputChanged() = 0;
    };

    class _UtilExport OutputBuffer
    {
        public:
            OutputBuffer() {}
            ~OutputBuffer() {}

            template <class T>
            inline OutputBuffer& operator<<(T object)
            {
                this->stream_ << object;
                this->callListeners();
                return *this;
            }

            template <const OutputBuffer&>
            inline OutputBuffer& operator<<(const OutputBuffer& object)
            {
                this->stream_ << object.stream_;
                this->callListeners();
                return *this;
            }

            OutputBuffer& operator<<(std::ostream& (*manipulator)(std::ostream&));
            OutputBuffer& operator<<(std::ios& (*manipulator)(std::ios&));
            OutputBuffer& operator<<(std::ios_base& (*manipulator)(std::ios_base&));

            template <class T>
            inline void add(T object)
            {
                this->stream_ << object;
                this->callListeners();
            }

            template <class T>
            inline void addLine(T object)
            {
                this->stream_ << object << std::endl;
                this->callListeners();
            }

            inline void newline()
            {
                this->stream_ << std::endl;
                this->callListeners();
            }

            inline void flush()
            {
                this->stream_.flush();
            }

            bool getLine(std::string* output);

            void registerListener(OutputBufferListener* listener);
            void unregisterListener(OutputBufferListener* listener);

            inline std::stringstream& getStream()
                { return this->stream_; }

        private:
            void callListeners();

            std::stringstream stream_;
            std::list<OutputBufferListener*> listeners_;
    };
}

#endif /* _OutputBuffer_H__ */
