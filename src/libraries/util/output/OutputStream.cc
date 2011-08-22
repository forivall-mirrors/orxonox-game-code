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
    @brief Implementation of the non-generic functions of the OutputStream class.
*/

#include "OutputStream.h"

#include "OutputManager.h"

namespace orxonox
{
    /**
        @brief Default constructor, initializes level and context with default values.
    */
    OutputStream::OutputStream()
    {
        this->setOutputAttributes(level::debug_output, context::undefined());
    }

    /**
        @brief Constructor, initializes level and context with the provided values.
    */
    OutputStream::OutputStream(OutputLevel level, const OutputContextContainer& context)
    {
        this->setOutputAttributes(level, context);
    }

    /**
        @brief Destructor, sends remaining output to OutputManager (if any).
    */
    OutputStream::~OutputStream()
    {
        if (this->str() != "")
            *this << " [missing endl]" << std::endl;
    }

    /**
        @brief Sends the buffered message to OutputManager together with the stored level and context.
        Additionally empties the buffer.
    */
    void OutputStream::sendMessage()
    {
        OutputManager::getInstanceAndCreateListeners().pushMessage(this->level_, *this->context_, this->str());
        this->str("");
    }

    /**
        @brief Defines level and context of the following output.
        Also sets the bAcceptsOutput_ flag according to the masks defined in OutputManager.
    */
    void OutputStream::setOutputAttributes(OutputLevel level, const OutputContextContainer& context)
    {
        this->level_ = level;
        this->context_ = &context;

        this->bAcceptsOutput_ = OutputManager::getInstanceAndCreateListeners().acceptsOutput(level, context);
    }
}
