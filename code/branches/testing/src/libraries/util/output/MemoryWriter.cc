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

/**
    @file
    @brief Implementation of the MemoryWriter singleton.
*/

#include "MemoryWriter.h"
#include "OutputManager.h"

namespace orxonox
{
    /**
        @brief Constructor, initializes the level mask with all levels activated.
    */
    MemoryWriter::MemoryWriter()
    {
        this->setLevelMask(level::all);
    }

    /**
        @brief Destructor.
    */
    MemoryWriter::~MemoryWriter()
    {
    }

    /**
        @brief Implementation of the output() function inherited from OutputListener, stores the received output in memory.
    */
    void MemoryWriter::output(OutputLevel level, const OutputContextContainer& context, const std::vector<std::string>& lines)
    {
        this->messages_.push_back(Message(level, context, lines));
    }

    /**
        @brief Iterates over all stored output messages and sends them to the OutputListener.
    */
    void MemoryWriter::resendOutput(OutputListener* listener) const
    {
        for (size_t i = 0; i < this->messages_.size(); ++i)
        {
            const Message& message = this->messages_[i];
            listener->unfilteredOutput(message.level, *message.context, message.lines);
        }
    }

    /**
        @brief Unregisters the instance from OutputManager, hence it will not receive any further output.
    */
    void MemoryWriter::disable()
    {
        OutputManager::getInstance().unregisterListener(this);
        this->output(level::debug_output, context::undefined(), std::vector<std::string>(1, "MemoryWriter disabled, further messages may be lost"));
    }
}
