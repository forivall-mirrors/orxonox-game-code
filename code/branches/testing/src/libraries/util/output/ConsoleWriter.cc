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
    @brief Implementation of the ConsoleWriter singleton.
*/

#include "ConsoleWriter.h"


#include "OutputManager.h"

namespace orxonox
{
    /**
        @brief Constructor, initializes the output level.

        In debug builds, it writes output up to level::internal_warning to the
        console, in release builds only up to level::user_info.

        After creation, the instance is enabled.
    */
    ConsoleWriter::ConsoleWriter(std::ostream& outputStream) : BaseWriter("Console"), outputStream_(outputStream)
    {
#ifdef ORXONOX_RELEASE
        this->setLevelMax(level::user_info);
#else
        this->setLevelMax(level::internal_warning);
#endif
        this->bEnabled_ = true;
    }

    /**
        @brief Destructor.
    */
    ConsoleWriter::~ConsoleWriter()
    {
    }

    /**
        @brief Inherited function from BaseWriter, writes output to the console using std::cout.
    */
    void ConsoleWriter::printLine(const std::string& line, OutputLevel)
    {
        this->outputStream_ << line << std::endl;
    }

    /**
        @brief Enables the instance by registering itself as listener at OutputManager.
    */
    void ConsoleWriter::enable()
    {
        if (!this->bEnabled_)
        {
            OutputManager::getInstance().registerListener(this);
            this->bEnabled_ = true;
        }
    }

    /**
        @brief Disables the instance by unregistering itself from OutputManager.
    */
    void ConsoleWriter::disable()
    {
        if (this->bEnabled_)
        {
            OutputManager::getInstance().unregisterListener(this);
            this->bEnabled_ = false;
        }
    }
}
