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
 *      ...
 *
 */

/**
@file
@brief
    Declaration of Game Singleton.
 */

#ifndef _Game_H__
#define _Game_H__

#include "OrxonoxPrereqs.h"
#include <cassert>
#include <list>
#include "core/OrxonoxClass.h"

namespace orxonox
{
    /**
    @brief
        Main class responsible for running the game.
    */
    class _OrxonoxExport Game : public OrxonoxClass
    {
    public:
        Game(int argc, char** argv);
        ~Game();
        void setConfigValues();

        void run();
        void stop();

        float getAvgTickTime() { return this->avgTickTime_; }
        float getAvgFPS()      { return this->avgFPS_; }

        void addTickTime(uint32_t length);

        static Game& getInstance() { assert(singletonRef_s); return *singletonRef_s; }

    private:
        struct statisticsTickInfo
        {
            uint64_t    tickTime;
            uint32_t    tickLength;
        };

        Game(Game&); // don't mess with singletons

        Core* core_;
        Clock* gameClock_;

        bool abort_;

        // variables for time statistics
        uint64_t              statisticsStartTime_;
        std::list<statisticsTickInfo>
                              statisticsTickTimes_;
        uint32_t              periodTime_;
        uint32_t              periodTickTime_;
        float                 avgFPS_;
        float                 avgTickTime_;

        // config values
        unsigned int          statisticsRefreshCycle_;
        unsigned int          statisticsAvgLength_;

        static Game* singletonRef_s;        //!< Pointer to the Singleton
    };
}

#endif /* _Game_H__ */
