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

#include "Timer.h"

#include <set>

#include "util/Clock.h"
#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/CommandExecutor.h"
#include "core/Functor.h"

namespace orxonox
{
    SetConsoleCommandShortcutExtern(delay);
    SetConsoleCommandShortcutExtern(killdelays);

    static std::set<Timer*> delaytimerset;

    /**
        @brief Calls a console command after 'delay' seconds.
        @param delay The delay in seconds
        @param command The console command
    */
    void delay(float delay, const std::string& command)
    {
        Timer* delaytimer = new Timer();
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
    void executeDelayedCommand(Timer* timer, const std::string& command)
    {
        CommandExecutor::execute(command);
        timer->destroy();
        delaytimerset.erase(timer);
    }

    /**
        @brief Kills all delayed commands.
    */
    void killdelays()
    {
        for (std::set<Timer*>::iterator it = delaytimerset.begin(); it != delaytimerset.end(); ++it)
            (*it)->destroy();

        delaytimerset.clear();
    }

    /**
        @brief Constructor: Sets the default-values.
    */
    Timer::Timer()
    {
        this->init();
        RegisterObject(Timer);
    }

    /**
        @brief Constructor: Initializes the Timer with given values.
        @param interval The timer-interval in seconds
        @param bLoop If true, the function gets called every 'interval' seconds
        @param exeuctor A executor of the function to call
    */
    Timer::Timer(float interval, bool bLoop, Executor* executor, bool bKillAfterCall)
    {
        this->init();
        RegisterObject(Timer);

        this->setTimer(interval, bLoop, executor, bKillAfterCall);
    }

    /**
        @brief Deletes the executor.
    */
    Timer::~Timer()
    {
        this->deleteExecutor();
    }

    /**
        @brief Initializes the Timer
    */
    void Timer::init()
    {
        this->executor_ = 0;
        this->interval_ = 0;
        this->bLoop_ = false;
        this->bActive_ = false;
        this->bKillAfterCall_ = false;

        this->time_ = 0;
    }

    /**
        @brief Executes the executor.
    */
    void Timer::run()
    {
        bool temp = this->bKillAfterCall_; // to avoid errors with bKillAfterCall_=false and an exutors which destroy the timer

        (*this->executor_)();

        if (temp)
            this->destroy();
    }

    /**
        @brief Deletes the executor.
    */
    void Timer::deleteExecutor()
    {
      if (this->executor_)
          delete this->executor_;
    }

    /**
        @brief Updates the timer before the frames are rendered.
    */
    void Timer::tick(const Clock& time)
    {
        if (this->bActive_)
        {
            // If active: Decrease the timer by the duration of the last frame
            this->time_ -= static_cast<long long>(time.getDeltaTimeMicroseconds() * this->getTimeFactor());

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
