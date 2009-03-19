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

#include "OrxonoxStableHeaders.h"
#include "GSRoot.h"

#include "util/Exception.h"
#include "util/Debug.h"
#include "core/Core.h"
#include "core/ConfigValueIncludes.h"
#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "tools/Timer.h"
#include "objects/Tickable.h"

namespace orxonox
{
    GSRoot::GSRoot()
        : RootGameState("root")
        , timeFactor_(1.0f)
        , bPaused_(false)
        , timeFactorPauseBackup_(1.0f)
    {
        RegisterRootObject(GSRoot);
        setConfigValues();

        this->ccSetTimeFactor_ = 0;
        this->ccPause_ = 0;
    }

    GSRoot::~GSRoot()
    {
    }

    void GSRoot::setConfigValues()
    {
        SetConfigValue(statisticsRefreshCycle_, 250000)
            .description("Sets the time in microseconds interval at which average fps, etc. get updated.");
        SetConfigValue(statisticsAvgLength_, 1000000)
            .description("Sets the time in microseconds interval at which average fps, etc. gets calculated.");
    }

    void GSRoot::enter()
    {
        // reset game speed to normal
        timeFactor_ = 1.0f;

        // reset frame counter
        this->statisticsStartTime_ = 0;
        this->statisticsTickTimes_.clear();
        this->periodTickTime_ = 0;
        this->avgFPS_ = 0.0f;
        this->avgTickTime_ = 0.0f;

        {
            // add console commands
            FunctorMember<GSRoot>* functor = createFunctor(&GSRoot::exitGame);
            functor->setObject(this);
            this->ccExit_ = createConsoleCommand(functor, "exit");
            CommandExecutor::addConsoleCommandShortcut(this->ccExit_);
        }

        {
            // add console commands
            FunctorMember01<GameStateBase, const std::string&>* functor = createFunctor(&GameStateBase::requestState);
            functor->setObject(this);
            this->ccSelectGameState_ = createConsoleCommand(functor, "selectGameState");
            CommandExecutor::addConsoleCommandShortcut(this->ccSelectGameState_);
        }

        {
            // time factor console command
            FunctorMember<GSRoot>* functor = createFunctor(&GSRoot::setTimeFactor);
            functor->setObject(this);
            this->ccSetTimeFactor_ = createConsoleCommand(functor, "setTimeFactor");
            CommandExecutor::addConsoleCommandShortcut(this->ccSetTimeFactor_).accessLevel(AccessLevel::Offline).defaultValue(0, 1.0);
        }

        {
            // time factor console command
            FunctorMember<GSRoot>* functor = createFunctor(&GSRoot::pause);
            functor->setObject(this);
            this->ccPause_ = createConsoleCommand(functor, "pause");
            CommandExecutor::addConsoleCommandShortcut(this->ccPause_).accessLevel(AccessLevel::Offline);
        }
    }

    void GSRoot::leave()
    {
        // destroy console commands
        delete this->ccExit_;
        delete this->ccSelectGameState_;

        if (this->ccSetTimeFactor_)
        {
            delete this->ccSetTimeFactor_;
            this->ccSetTimeFactor_ = 0;
        }

        if (this->ccPause_)
        {
            delete this->ccPause_;
            this->ccPause_ = 0;
        }
    }

    void GSRoot::ticked(const Clock& time)
    {
        uint64_t timeBeforeTick = time.getRealMicroseconds();

        Core::getInstance().update(time);

        for (ObjectList<TimerBase>::iterator it = ObjectList<TimerBase>::begin(); it; ++it)
            it->tick(time);

        /*** HACK *** HACK ***/
        // Call the Tickable objects
        float leveldt = time.getDeltaTime();
        if (leveldt > 1.0f)
        {
            // just loaded
            leveldt = 0.0f;
        }
        for (ObjectList<Tickable>::iterator it = ObjectList<Tickable>::begin(); it; ++it)
            it->tick(leveldt * this->timeFactor_);
        /*** HACK *** HACK ***/

        uint64_t timeAfterTick = time.getRealMicroseconds();

        // STATISTICS
        assert(timeAfterTick - timeBeforeTick >= 0 );
        statisticsTickInfo tickInfo = {timeAfterTick, timeAfterTick - timeBeforeTick};
        statisticsTickTimes_.push_back(tickInfo);
        assert(statisticsTickTimes_.back().tickLength==tickInfo.tickLength);
        this->periodTickTime_ += tickInfo.tickLength;

        // Ticks GSGraphics or GSDedicated
        this->tickChild(time);

        if (timeAfterTick > statisticsStartTime_ + statisticsRefreshCycle_)
        {
            std::list<statisticsTickInfo>::iterator it = this->statisticsTickTimes_.begin();
            assert(it != this->statisticsTickTimes_.end());
            int64_t lastTime = timeAfterTick - statisticsAvgLength_;
            if ((int64_t)it->tickTime < lastTime)
            {
                do
                {
                    assert(this->periodTickTime_ > it->tickLength);
                    this->periodTickTime_ -= it->tickLength;
                    ++it;
                    assert(it != this->statisticsTickTimes_.end());
                } while ((int64_t)it->tickTime < lastTime);
                this->statisticsTickTimes_.erase(this->statisticsTickTimes_.begin(), it);
            }

            uint32_t framesPerPeriod = this->statisticsTickTimes_.size();
            this->avgFPS_ = (float)framesPerPeriod / (timeAfterTick - this->statisticsTickTimes_.front().tickTime) * 1000000.0;
            this->avgTickTime_ = (float)this->periodTickTime_ / framesPerPeriod / 1000.0;

            statisticsStartTime_ = timeAfterTick;
        }

    }

    /**
    @brief
        Changes the speed of Orxonox
    */
    void GSRoot::setTimeFactor(float factor)
    {
        if (Core::isMaster())
        {
            if (!this->bPaused_)
            {
                TimeFactorListener::timefactor_s = factor;

                for (ObjectList<TimeFactorListener>::iterator it = ObjectList<TimeFactorListener>::begin(); it != ObjectList<TimeFactorListener>::end(); ++it)
                    it->changedTimeFactor(factor, this->timeFactor_);

                this->timeFactor_ = factor;
            }
            else
                this->timeFactorPauseBackup_ = factor;
        }
    }

    void GSRoot::pause()
    {
        if (Core::isMaster())
        {
            if (!this->bPaused_)
            {
                this->timeFactorPauseBackup_ = this->timeFactor_;
                this->setTimeFactor(0.0f);
                this->bPaused_ = true;
            }
            else
            {
                this->bPaused_ = false;
                this->setTimeFactor(this->timeFactorPauseBackup_);
            }
        }
    }

    ////////////////////////
    // TimeFactorListener //
    ////////////////////////
    float TimeFactorListener::timefactor_s = 1.0f;

    TimeFactorListener::TimeFactorListener()
    {
        RegisterRootObject(TimeFactorListener);
    }
}
