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
    @brief Implementation of the Timer class.
*/

#include "Timer.h"

#include <set>

#include <boost/bimap.hpp>

#include "util/Clock.h"
#include "core/CoreIncludes.h"
#include "core/command/ConsoleCommand.h"
#include "core/command/CommandExecutor.h"
#include "core/command/Executor.h"
#include "core/command/Functor.h"
#include "tools/interfaces/TimeFactorListener.h"

namespace orxonox
{
    SetConsoleCommand("delay", &delay).argumentCompleter(1, autocompletion::command());
    SetConsoleCommand("delayreal", &delayreal).argumentCompleter(1, autocompletion::command());
    SetConsoleCommand("killdelay", &killdelay);
    SetConsoleCommand("killdelays", &killdelays);

    static boost::bimap<unsigned int, Timer*> delaytimers;
    static unsigned int delayHandleCounter = 0;

    /**
        @brief Console-command: Calls another console command after @a delay seconds (game time).
        @param delay The delay in seconds
        @param command The console command
        @return The handle of the delayed command, can be used as argument for killdelay()
    */
    unsigned int delay(float delay, const std::string& command)
    {
        return addDelayedCommand(new Timer(), delay, command);
    }

    /**
        @brief Console-command: Calls another console command after @a delay seconds (real time)
        @param delay The delay in seconds
        @param command The console command
        @return The handle of the delayed command, can be used as argument for killdelay()
    */
    unsigned int delayreal(float delay, const std::string& command)
    {
        return addDelayedCommand(new RealTimer(), delay, command);
    }

    /**
        @brief Helper function, used by delay() and delayreal() to add a delayed command.
        @param timer The timer which will execute the command
        @param delay The delay in seconds
        @param command The console command
        @return The handle of the delayed command, can be used as argument for killdelay()
    */
    unsigned int addDelayedCommand(Timer* timer, float delay, const std::string& command)
    {
        delaytimers.insert(boost::bimap<unsigned int, Timer*>::value_type(++delayHandleCounter, timer));

        const ExecutorStaticPtr& delayexecutor = createExecutor(createFunctor(&executeDelayedCommand));
        delayexecutor->setDefaultValues(timer, command);
        timer->setTimer(delay, false, delayexecutor);

        return delayHandleCounter;
    }

    /**
        @brief Helper function for delay(), executes the command and destroys the timer.
        @param timer The timer which called this function.
        @param command The command to execute
    */
    void executeDelayedCommand(Timer* timer, const std::string& command)
    {
        CommandExecutor::execute(command);
        timer->destroy();
        delaytimers.right.erase(timer);
    }

    /**
        @brief Console-command: Kills all scheduled commands that were delayed using delay().
    */
    void killdelays()
    {
        for (boost::bimap<unsigned int, Timer*>::left_map::iterator it = delaytimers.left.begin(); it != delaytimers.left.end(); ++it)
            it->second->destroy();

        delaytimers.clear();
    }

    /**
        @brief Console-command: Kills a delayed command with given handle.
    */
    void killdelay(unsigned int handle)
    {
        boost::bimap<unsigned int, Timer*>::left_map::iterator it = delaytimers.left.find(handle);
        if (it != delaytimers.left.end())
        {
            it->second->destroy();
            delaytimers.left.erase(it);
        }
    }

    RegisterClassNoArgs(Timer);

    /**
        @brief Constructor: Sets the default-values.
    */
    Timer::Timer()
    {
        this->init();
        RegisterRootObject(Timer);
    }

    /**
        @brief Constructor: Initializes and starts the timer, which will call an executor after some time.
        @param interval         The timer-interval in seconds
        @param bLoop            If true, the executor gets called every @a interval seconds
        @param executor         The executor that will be called
        @param bKillAfterCall   If true, the timer will be deleted after the executor was called
    */
    Timer::Timer(float interval, bool bLoop, const ExecutorPtr& executor, bool bKillAfterCall)
    {
        this->init();
        RegisterRootObject(Timer);

        this->setTimer(interval, bLoop, executor, bKillAfterCall);
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
        @brief Returns the current time factor of the game.
    */
    float Timer::getTimeFactor()
    {
        return TimeFactorListener::getTimeFactor();
    }

    /**
        @brief Initializes and starts the timer, which will call an executor after some time.
        @param interval         The timer-interval in seconds
        @param bLoop            If true, the executor gets called every @a interval seconds
        @param executor         The executor that will be called
        @param bKillAfterCall   If true, the timer will be deleted after the executor was called
    */
    void Timer::setTimer(float interval, bool bLoop, const ExecutorPtr& executor, bool bKillAfterCall)
    {
        this->setInterval(interval);
        this->bLoop_ = bLoop;
        this->executor_ = executor;
        this->bActive_ = true;

        this->time_ = this->interval_;
        this->bKillAfterCall_ = bKillAfterCall;

        executor->getFunctor()->setSafeMode(true);
    }

    /**
        @brief Calls the executor and destroys the timer if requested.
    */
    void Timer::run()
    {
        bool temp = this->bKillAfterCall_; // to avoid errors with bKillAfterCall_=false and an exutors which destroy the timer

        (*this->executor_)();

        if (temp)
            this->destroy();
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

    ///////////////
    // RealTimer //
    ///////////////
    /// @copydoc Timer::Timer
    RealTimer::RealTimer()
    {
        RegisterObject(RealTimer);
    }

    /// @copydoc Timer::Timer(float, bool, const ExecutorPtr&, bool)
    RealTimer::RealTimer(float interval, bool bLoop, const ExecutorPtr& executor, bool bKillAfterCall) : Timer(interval, bLoop, executor, bKillAfterCall)
    {
        RegisterObject(RealTimer);
    }

    /// Returns always 1 because RealTimer doesn't depend on the game time.
    float RealTimer::getTimeFactor()
    {
        return 1;
    }
}
