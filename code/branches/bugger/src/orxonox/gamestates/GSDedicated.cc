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
 *      Reto Grieder
 *   Co-authors:
 *      Fabian 'x3n' Landau
 *
 */

#include "OrxonoxStableHeaders.h"
#include "GSDedicated.h"

#include "core/CommandLine.h"
#include "core/Core.h"
#include "core/Iterator.h"
#include "core/CoreIncludes.h"
#include "core/ConfigValueIncludes.h"
#include "util/Sleep.h"
#include "network/Server.h"
#include "objects/Tickable.h"
#include "util/Sleep.h"

const unsigned int  DEFAULT_DEDICATED_SERVER_TICKRATE = 25;
const float         MIN_WIN32_SLEEP_TIME = 0.01; // scheduler limited

namespace orxonox
{
    GSDedicated::GSDedicated()
        : GameState<GSRoot>("dedicated")
        , server_(0)
    {
        RegisterObject(GSDedicated);

        this->setConfigValues();
    }

    GSDedicated::~GSDedicated()
    {
    }

    void GSDedicated::enter()
    {
        Core::setHasServer(true);

        this->server_ = new Server(CommandLine::getValue("port"));
        COUT(0) << "Loading scene in dedicated server mode" << std::endl;

        GSLevel::enter(0);

        server_->open();
    }

    void GSDedicated::leave()
    {
        GSLevel::leave();

        this->server_->close();
        delete this->server_;

        Core::setHasServer(false);
    }

    void GSDedicated::ticked(const Clock& time)
    {
        static int timeSinceLastTick = 0; // in microseconds
        const int tickPeriod = (int)(1000000.0f / this->tickrate_); // in microseconds

        GSLevel::ticked(time);

        timeSinceLastTick += time.getDeltaTimeMicroseconds();
        if ( timeSinceLastTick >= tickPeriod )
        {
            server_->tick( timeSinceLastTick );
            //timeSinceLastTick -= static_cast<unsigned int>( timeSinceLastTick / tickPeriod ) * tickPeriod;
            timeSinceLastTick = 0;
        }
        else
        {
            unsigned int sleepTime;

#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32
            if ( tickPeriod-timeSinceLastTick < MIN_WIN32_SLEEP_TIME )
                sleepTime = (unsigned int)(MIN_WIN32_SLEEP_TIME * 1000000);
            else
                sleepTime = tickPeriod - timeSinceLastTick;
            msleep( sleepTime / 1000 );

#else /* unix */
            sleepTime = tickPeriod - timeSinceLastTick;
            usleep( sleepTime );
#endif

        }

        this->tickChild(time);
    }

    void GSDedicated::setConfigValues()
    {
        SetConfigValue ( tickrate_, DEFAULT_DEDICATED_SERVER_TICKRATE );
    }
}
