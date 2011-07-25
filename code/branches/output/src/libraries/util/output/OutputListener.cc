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

#include "OutputListener.h"

#include "OutputManager.h"

namespace orxonox
{
namespace test
{
    OutputListener::OutputListener()
    {
        this->levelMask_ = level::none;
        this->contextMask_ = context::all;

        OutputManager::getInstance().registerListener(this);
    }

    OutputListener::~OutputListener()
    {
        OutputManager::getInstance().unregisterListener(this);
    }

    void OutputListener::setLevelMax(OutputLevel max)
    {
        this->setLevelRange(level::debug_output, max);
    }

    void OutputListener::setLevelRange(OutputLevel min, OutputLevel max)
    {
        OutputLevel mask = 0;

        for (OutputLevel level = min; level <= max; level = level << 1)
            mask |= level;

        this->setLevelMask(mask);
    }

    void OutputListener::setLevelMask(OutputLevel mask)
    {
        this->levelMask_ = mask;

        OutputManager::getInstance().updateCombinedLevelMask();
    }

    void OutputListener::setContextMask(OutputContext mask)
    {
        this->contextMask_ = mask;

        OutputManager::getInstance().updateCombinedContextMask();
    }
}
}
