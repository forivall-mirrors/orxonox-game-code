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
        orxout() << "Hello World" << endl;
    }
    @endcode

    The code in this example prints "Hello World" to the console, 3 seconds after creating
    an instance of MyClass.
*/

#ifndef _Timer_H__
#define _Timer_H__

#include "tools/ToolsPrereqs.h"

#include "core/class/OrxonoxClass.h"
#include "core/command/ExecutorPtr.h"

namespace orxonox
{
    unsigned int delay(float delay, const std::string& command);
    unsigned int delayreal(float delay, const std::string& command);

    unsigned int addDelayedCommand(Timer* timer, float delay, const std::string& command);
    void executeDelayedCommand(Timer* timer, const std::string& command);

    void killdelay(unsigned int handle);
    void killdelays();

    /**
        @brief Timer is a helper class that executes a function after a given amount of seconds in game-time.

        @see See @ref TimerExample "Timer.h" for an example.

        The time interval of Timer depends on the game time, hence it stops if the game is paused or runs
        slower/faster if the game-speed is modified. See RealTimer for a timer class which doesn't depend
        on the game time.
    */
    class _ToolsExport Timer : virtual public OrxonoxClass
    {
        public:
            Timer();

            Timer(float interval, bool bLoop, const ExecutorPtr& executor, bool bKillAfterCall = false);

            void setTimer(float interval, bool bLoop, const ExecutorPtr& executor, bool bKillAfterCall = false);

            void run();

            /// Re-starts the timer: The executor will be called after @a interval seconds.
            inline void startTimer()
                { this->bActive_ = true; this->time_ = this->interval_; }
            /// Stops the timer.
            inline void stopTimer()
                { this->bActive_ = false; this->time_ = this->interval_; }
            /// Pauses the timer - it will continue with the actual state if you call unpauseTimer().
            inline void pauseTimer()
                { this->bActive_ = false; }
            /// Unpauses the timer - continues with the given state.
            inline void unpauseTimer()
                { this->bActive_ = true; }
            /// Returns true if the timer is active (neither stopped nor paused).
            inline bool isActive() const
                { return this->bActive_; }
            /// Returns the remaining time until the timer calls the executor.
            inline float getRemainingTime() const
                { return static_cast<float>(this->time_ / 1000000.0f); }
            /// Increases the remaining time of the timer by the given amount of time (in seconds).
            inline void addTime(float time)
                { if (time > 0.0f) this->time_ += static_cast<long long>(time * 1000000.0f); }
            /// Decreases the remaining time of the timer by the given amount of time (in seconds)
            inline void removeTime(float time)
                { if (time > 0.0f) this->time_ -= static_cast<long long>(time * 1000000.0f); }
            /// Changes the calling interval.
            inline void setInterval(float interval)
                { this->interval_ = static_cast<long long>(interval * 1000000.0f); }
            /// Defines if the timer call the executor every @a interval seconds or only once.
            inline void setLoop(bool bLoop)
                { this->bLoop_ = bLoop; }

            void tick(const Clock& time);

        protected:
            virtual float getTimeFactor();

        private:
            void init();

            ExecutorPtr executor_;  //!< The executor of the function that will be called when the time expires

            long long interval_;    //!< The time-interval in micro seconds
            bool bLoop_;            //!< If true, the executor gets called every @a interval seconds
            bool bActive_;          //!< If true, the timer ticks and calls the executor if the time's up
            bool bKillAfterCall_;   //!< If true the timer gets deleted after it expired and called the executor

            long long time_;        //!< Internal variable, counting the time untill the next executor-call
    };

    /**
        @brief RealTimer is a helper class that executes a function after a given amount of seconds in real-time.

        The time interval of RealTimer doesn't depend on the game time, it will also call the function
        if the game is paused. See Timer for a timer class that depends on the game time.
    */
    class _ToolsExport RealTimer : public Timer
    {
        public:
            RealTimer();
            RealTimer(float interval, bool bLoop, const ExecutorPtr& executor, bool bKillAfterCall = false);

        protected:
            virtual float getTimeFactor();
    };
}

#endif /* _Timer_H__ */
