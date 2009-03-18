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

#ifndef _GSRoot_H__
#define _GSRoot_H__

#include "OrxonoxPrereqs.h"

#include <list>
#include "core/RootGameState.h"
#include "core/OrxonoxClass.h"

namespace orxonox
{
    class _OrxonoxExport GSRoot : public RootGameState, public OrxonoxClass
    {
        friend class ClassIdentifier<GSRoot>;

    public:
        struct statisticsTickInfo
        {
            uint64_t    tickTime;
            uint32_t    tickLength;
        };
    
    public:
        GSRoot();
        ~GSRoot();

        void exitGame()
        { requestState("root"); }

        // this has to be public because proteced triggers a bug in msvc
        // when taking the function address.
        void setTimeFactor(float factor);
        void pause();
        float getTimeFactor() { return this->timeFactor_; }

        float getAvgTickTime() { return this->avgTickTime_; }
        float getAvgFPS()      { return this->avgFPS_; }

        inline void addTickTime(uint32_t length)
            { assert(!this->statisticsTickTimes_.empty()); this->statisticsTickTimes_.back().tickLength += length;
              this->periodTickTime_+=length; }

    private:
        void enter();
        void leave();
        void ticked(const Clock& time);

        void setConfigValues();

        float                 timeFactor_;       //!< A factor that sets the gamespeed. 1 is normal.
        bool                  bPaused_;
        float                 timeFactorPauseBackup_;

        // variables for time statistics
        uint64_t              statisticsStartTime_;
        std::list<statisticsTickInfo>
                              statisticsTickTimes_;
        uint32_t              periodTickTime_;
        float                 avgFPS_;
        float                 avgTickTime_;

        // config values
        unsigned int          statisticsRefreshCycle_;
        unsigned int          statisticsAvgLength_;

        // console commands
        ConsoleCommand*       ccExit_;
        ConsoleCommand*       ccSelectGameState_;
        ConsoleCommand*       ccSetTimeFactor_;
        ConsoleCommand*       ccPause_;
    };

    class _OrxonoxExport TimeFactorListener : virtual public OrxonoxClass
    {
        friend class GSRoot;

        public:
            TimeFactorListener();
            virtual ~TimeFactorListener() {}

        protected:
            virtual void changedTimeFactor(float factor_new, float factor_old) {}
            inline float getTimeFactor() const
                { return TimeFactorListener::timefactor_s; }

        private:
            static float timefactor_s;
    };
}

#endif /* _GSRoot_H__ */
