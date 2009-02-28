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
    @file
    @brief Implementation of the OutputBuffer.
*/

#include "OutputBuffer.h"

namespace orxonox
{
    const int OUTPUTBUFFER_MAX_LINE_LENGTH = 16384; //! The maximal number of lines that can be stored within the OutputBuffer.

    /**
        @brief Adds a new listener to the list.
        @param listener The new listener
    */
    void OutputBuffer::registerListener(OutputBufferListener* listener)
    {
        this->listeners_.insert(this->listeners_.end(), listener);
    }

    /**
        @brief Removes a listener from the list.
        @param listener The listener
    */
    void OutputBuffer::unregisterListener(OutputBufferListener* listener)
    {
        for (std::list<OutputBufferListener*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); )
        {
            if ((*it) == listener)
                this->listeners_.erase(it++);
            else
                ++it;
        }
    }

    /**
        @brief Puts a stream manipulator to the stream.
        @param manipulator The manipulator
    */
    OutputBuffer& OutputBuffer::operator<<(std::ostream& (*manipulator)(std::ostream&))
    {
        this->stream_ << manipulator;
        this->callListeners();
        return *this;
    }

    /**
        @brief Puts a stream manipulator to the stream.
        @param manipulator The manipulator
    */
    OutputBuffer& OutputBuffer::operator<<(std::ios& (*manipulator)(std::ios&))
    {
        this->stream_ << manipulator;
        this->callListeners();
        return *this;
    }

    /**
        @brief Puts a stream manipulator to the stream.
        @param manipulator The manipulator
    */
    OutputBuffer& OutputBuffer::operator<<(std::ios_base& (*manipulator)(std::ios_base&))
    {
        this->stream_ << manipulator;
        this->callListeners();
        return *this;
    }

    /**
        @brief Removes the first line from the stream and assigns it to a given string object.
        @param output The string object to assign the first line
        @return True if there was at least one line in the stream and this line was successfully assigned

        It's important to know the returned line will be removed from the stream. If there are more than one
        listener, they have to cooperate to avoid conflicts.
    */
    bool OutputBuffer::getLine(std::string* output)
    {
        char line[OUTPUTBUFFER_MAX_LINE_LENGTH];

        this->stream_.getline(line, OUTPUTBUFFER_MAX_LINE_LENGTH);
        (*output) = std::string(line);

        bool eof = this->stream_.eof();
        bool fail = this->stream_.fail();

        if (eof)
            this->stream_.flush();

        if (eof || fail)
            this->stream_.clear();

        return (!eof && !fail);
    }

    /**
        @brief Calls the outputChanged() function of all registered listeners.
    */
    void OutputBuffer::callListeners()
    {
        for (std::list<OutputBufferListener*>::iterator it = this->listeners_.begin(); it != this->listeners_.end(); ++it)
            (*it)->outputChanged();
    }
}
