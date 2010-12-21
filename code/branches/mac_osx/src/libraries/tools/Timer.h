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
    @defgroup Timer Timer
    @ingroup Tools
*/

/**
    @file
    @ingroup Timer
    @brief Declaration of the Timer class, used to call functions after a given time-interval.

    @anchor TimerExample

    Timer is a helper class that executes a function after a given amount of time.

    Usage: <br>
    header.h:
    @code
    class MyClass
    {
        public:
            MyClass();
            void functionName();

        private:
            Timer myTimer;
    };
    @endcode

    source.cc:
    @code
    #include "core/command/Executor.h"

    MyClass::MyClass()
    {
        myTimer.setTimer(3, false, createExecutor(createFunctor(&ClassName::myFunction, this)));
    }

    void MyClass::myFunction()
    {
        COUT(0) << "Hello World" << std::endl;
    }
    @endcode

    The code in this example prints "Hello World" to the console, 3 seconds after creating
    an instance of MyClass.
*/

#ifndef _Timer_H__
#define _Timer_H__

#include "tools/ToolsPrereqs.h"

#include "core/OrxonoxClass.h"
#include "core/command/Executor.h"
#include "tools/interfaces/TimeFactorListener.h"

namespace orxonox
{
    void delay(float delay, const std::string& command);
    void killdelays();
    void executeDelayedCommand(Timer* timer, const std::string& command);

    /**
        @brief Timer is a helper class that executes a function after a given amount of time.

        @see See @ref TimerExample "Timer.h" for an example.
    */
    class _ToolsExport Timer : public TimeFactorListener
    {
        public:
            Timer();

            Timer(float interval, bool bLoop, const ExecutorPtr& executor, bool bKillAfterCall = false);

            /**
                @brief Initializes and starts the timer, which will call an executor after some time.
                @param interval         The timer-interval in seconds
                @param bLoop            If true, the executor gets called every @a interval seconds
                @param executor         The executor that will be called
                @param bKillAfterCall   If true, the timer will be deleted after the executor was called
            */
            void setTimer(float interval, bool bLoop, const ExecutorPtr& executor, bool bKillAfterCall = false)
            {
                this->setInterval(interval);
                this->bLoop_ = bLoop;
                this->executor_ = executor;
                this->bActive_ = true;

                this->time_ = this->interval_;
                this->bKillAfterCall_ = bKillAfterCall;
            }

            void run();

            /// Re-starts the Timer: The executor will be called after @a interval seconds.
            inline void startTimer()
                { this->bActive_ = true; this->time_ = this->interval_; }
            /// Stops the Timer.
            inline void stopTimer()
                { this->bActive_ = false; this->time_ = this->interval_; }
            /// Pauses the Timer - it will continue with the actual state if you call unpauseTimer().
            inline void pauseTimer()
                { this->bActive_ = false; }
            /// Unpauses the Timer - continues with the given state.
            inline void unpauseTimer()
                { this->bActive_ = true; }
            /// Returns true if the Timer is active (neither stoped nor paused).
            inline bool isActive() const
                { return this->bActive_; }
            /// Returns the remaining time until the Timer calls the executor.
            inline float getRemainingTime() const
                { return static_cast<float>(this->time_ / 1000000.0f); }
            /// Increases the remaining time of the Timer by the given amount of time (in seconds).
            inline void addTime(float time)
                { if (time > 0.0f) this->time_ += static_cast<long long>(time * 1000000.0f); }
            /// Decreases the remaining time of the Timer by the given amount of time (in seconds)
            inline void removeTime(float time)
                { if (time > 0.0f) this->time_ -= static_cast<long long>(time * 1000000.0f); }
            /// Changes the calling interval.
            inline void setInterval(float interval)
                { this->interval_ = static_cast<long long>(interval * 1000000.0f); }
            /// Defines if the timer call the executor every @a interval seconds or only once.
            inline void setLoop(bool bLoop)
                { this->bLoop_ = bLoop; }

            void tick(const Clock& time);

        private:
            void init();

            ExecutorPtr executor_;  //!< The executor of the function that will be called when the time expires

            long long interval_;    //!< The time-interval in micro seconds
            bool bLoop_;            //!< If true, the executor gets called every @a interval seconds
            bool bActive_;          //!< If true, the Timer ticks and calls the executor if the time's up
            bool bKillAfterCall_;   //!< If true the timer gets deleted after it expired and called the executor

            long long time_;        //!< Internal variable, counting the time untill the next executor-call
    };
}

#endif /* _Timer_H__ */
