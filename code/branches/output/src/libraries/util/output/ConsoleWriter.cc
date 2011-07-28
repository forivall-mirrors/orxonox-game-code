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

#include "ConsoleWriter.h"

#include <iostream>

#include "OutputManager.h"

namespace orxonox
{
    ConsoleWriter::ConsoleWriter()
    {
        this->setLevelMax(level::user_info);
        this->bEnabled_ = true;
    }

    ConsoleWriter::~ConsoleWriter()
    {
    }

    /*static*/ ConsoleWriter& ConsoleWriter::getInstance()
    {
        static ConsoleWriter instance;
        return instance;
    }

    void ConsoleWriter::printLine(const std::string& line)
    {
        std::cout << line << std::endl;
    }

    void ConsoleWriter::enable()
    {
        if (!this->bEnabled_)
        {
            OutputManager::getInstance().registerListener(this);
            this->bEnabled_ = true;
        }
    }

    void ConsoleWriter::disable()
    {
        if (this->bEnabled_)
        {
            OutputManager::getInstance().unregisterListener(this);
            this->bEnabled_ = false;
        }
    }
}
