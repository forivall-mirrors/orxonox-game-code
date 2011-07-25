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

/*!
    @file
    @brief Definition and Implementation of the Timer class.

    The Timer is a callback-object, calling a given function after a given time-interval.

    Usage:
    header.h:
        class ClassName
        {
            public:
                ClassName();
                void functionName();
                Timer myTimer;
        };

    source.cc:
        include "core/Executor.h"

        ClassName::ClassName()
        {
            myTimer.setTimer(interval_in_seconds, bLoop, createExecutor(createFunctor(&ClassName::functionName, this)));
        }

        void ClassName::functionName()
        {
            whateveryouwant();
            something(else);
        }
*/

#ifndef _Timer_H__
#define _Timer_H__

#include "tools/ToolsPrereqs.h"

#include "core/Executor.h"
#include "core/OrxonoxClass.h"
#include "tools/interfaces/TimeFactorListener.h"

namespace orxonox
{
    void delay(float delay, const std::string& command);
    void killdelays();
    void executeDelayedCommand(Timer* timer, const std::string& command);

    //! The Timer is a callback-object, calling a given function after a given time-interval.
    class _ToolsExport Timer : public TimeFactorListener
    {
        public:
            Timer();
            ~Timer();

            Timer(float interval, bool bLoop, Executor* executor, bool bKillAfterCall = false);

            /**
                @brief Initializes the Timer with given values.
                @param interval The timer-interval in seconds
                @param bLoop If true, the function gets called every 'interval' seconds
                @param object The object owning the timer and the function
                @param executor A executor of the function to call
            */
            void setTimer(float interval, bool bLoop, Executor* executor, bool bKillAfterCall = false)
            {
                this->deleteExecutor();

                this->setInterval(interval);
                this->bLoop_ = bLoop;
                this->executor_ = executor;
                this->bActive_ = true;

                this->time_ = this->interval_;
                this->bKillAfterCall_ = bKillAfterCall;
            }

            void run();
            void deleteExecutor();

            /** @brief Starts the Timer: Function-call after 'interval' seconds. */
            inline void startTimer()
                { this->bActive_ = true; this->time_ = this->interval_; }
            /** @brief Stops the Timer. */
            inline void stopTimer()
                { this->bActive_ = false; this->time_ = this->interval_; }
            /** @brief Pauses the Timer - it will continue with the actual state if you unpause it. */
            inline void pauseTimer()
                { this->bActive_ = false; }
            /** @brief Unpauses the Timer - continues with the given state. */
            inline void unpauseTimer()
                { this->bActive_ = true; }
            /** @brief Returns true if the Timer is active (= not stoped, not paused). @return True = Time is active */
            inline bool isActive() const
                { return this->bActive_; }
            /** @brief Returns the remaining time until the Timer calls the function. @return The remaining time */
            inline float getRemainingTime() const
                { return static_cast<float>(this->time_ / 1000000.0f); }
            /** @brief Gives the Timer some extra time. @param time The amount of extra time in seconds */
            inline void addTime(float time)
                { if (time > 0.0f) this->time_ += static_cast<long long>(time * 1000000.0f); }
            /** @brief Decreases the remaining time of the Timer. @param time The amount of time to remove */
            inline void removeTime(float time)
                { if (time > 0.0f) this->time_ -= static_cast<long long>(time * 1000000.0f); }
            /** @brief Sets the interval of the Timer. @param interval The interval */
            inline void setInterval(float interval)
                { this->interval_ = static_cast<long long>(interval * 1000000.0f); }
            /** @brief Sets bLoop to a given value. @param bLoop True = loop */
            inline void setLoop(bool bLoop)
                { this->bLoop_ = bLoop; }

            void tick(const Clock& time);

        private:
            void init();

            Executor* executor_;  //!< The executor of the function that should be called when the time expires

            long long interval_;  //!< The time-interval in micro seconds
            bool bLoop_;          //!< If true, the function gets called every 'interval' seconds
            bool bActive_;        //!< If true, the Timer ticks and calls the function if the time's up
            bool bKillAfterCall_; //!< If true the timer gets deleted after it called the function

            long long time_;      //!< Internal variable, counting the time till the next function-call
    };
}

#endif /* _Timer_H__ */
