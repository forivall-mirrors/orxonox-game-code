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

#include <set>

#include "Timer.h"

#include "core/Executor.h"
#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/CommandExecutor.h"
#include "core/Clock.h"

namespace orxonox
{
    SetConsoleCommandShortcutExtern(delay);
    SetConsoleCommandShortcutExtern(killdelays);

    static std::set<StaticTimer*> delaytimerset;

    /**
        @brief Calls a console command after 'delay' seconds.
        @param delay The delay in seconds
        @param command The console command
    */
    void delay(float delay, const std::string& command)
    {
        StaticTimer *delaytimer = new StaticTimer();
        delaytimerset.insert(delaytimer);

        ExecutorStatic* delayexecutor = createExecutor(createFunctor(&executeDelayedCommand));
        delayexecutor->setDefaultValues(delaytimer, command);
        delaytimer->setTimer(delay, false, delayexecutor);
    }

    /**
        @brief Executes the command.
        @param timer The timer to destroy after the command-execution
        @param command The command to execute
    */
    void executeDelayedCommand(StaticTimer* timer, const std::string& command)
    {
        CommandExecutor::execute(command);
        delete timer;
        delaytimerset.erase(timer);
    }

    /**
        @brief Kills all delayed commands.
    */
    void killdelays()
    {
        for (std::set<StaticTimer*>::iterator it = delaytimerset.begin(); it != delaytimerset.end(); ++it)
            delete (*it);

        delaytimerset.clear();
    }

    /**
        @brief Constructor: Sets the default-values.
    */
    TimerBase::TimerBase()
    {
        this->executor_ = 0;
        this->interval_ = 0;
        this->bLoop_ = false;
        this->bActive_ = false;
        this->bKillAfterCall_ = false;

        this->time_ = 0;

        RegisterObject(TimerBase);
    }

    /**
        @brief Deletes the executor.
    */
    TimerBase::~TimerBase()
    {
        this->deleteExecutor();
    }

    /**
        @brief Executes the executor.
    */
    void TimerBase::run() const
    {
        bool temp = this->bKillAfterCall_; // to avoid errors with bKillAfterCall_=false and an exutors which destroy the timer

        (*this->executor_)();

        if (temp)
            delete this;
    }

    /**
        @brief Deletes the executor.
    */
    void TimerBase::deleteExecutor()
    {
      if (this->executor_)
          delete this->executor_;
    }

    /**
        @brief Updates the timer before the frames are rendered.
    */
    void TimerBase::tick(const Clock& time)
    {
        if (this->bActive_)
        {
            // If active: Decrease the timer by the duration of the last frame
            this->time_ -= (long long)(time.getDeltaTimeMicroseconds() * this->getTimeFactor());

            if (this->time_ <= 0)
            {
                // It's time to call the function
                if (this->bLoop_ && !this->bKillAfterCall_)
                {
                    this->time_ += this->interval_; // Q: Why '+=' and not '='? A: Think about it. It's more accurate like that. Seriously.
                    while (this->time_ <= 0)
                    {
                        // The interval was shorter than one tick, so execute the function more than once
                        this->run();
                        this->time_ += this->interval_;
                    }
                }
                else
                    this->stopTimer(); // Stop the timer if we don't want to loop

                this->run();
            }
        }
    }
}
