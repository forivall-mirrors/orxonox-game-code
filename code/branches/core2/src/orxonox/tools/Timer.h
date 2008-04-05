/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
    @file Timer.h
    @brief Definition and Implementation of the Timer class.

    The Timer is a callback-object, calling a given function after a given time-interval.

    Usage:
    header.h:
        class ClassName
        {
            public:
                ClassName();
                void functionName();
                Timer<ClassName> myTimer;
        };

    source.cc:
        include "core/Executor.h"

        ClassName::ClassName()
        {
            myTimer.setTimer(interval_in_seconds, bLoop, this, createExecutor(createFunctor(&ClassName::functionName)));
        }

        void ClassName::functionName()
        {
            whateveryouwant();
            something(else);
        }
*/

#ifndef _Timer_H__
#define _Timer_H__

#include <OgreFrameListener.h>
#include "OrxonoxPrereqs.h"
#include "core/CorePrereqs.h"

namespace orxonox
{
    class StaticTimer;
    void delay(float delay, const std::string& command);
    void executeDelayedCommand(StaticTimer* timer, const std::string& command);

    //! TimerBase is the parent of the Timer class.
    class _OrxonoxExport TimerBase : public OrxonoxClass
    {
        friend class TimerFrameListener;

        public:
            ~TimerBase();

            void run() const;

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
            /** @brief Gives the Timer some extra time. @param time The amount of extra time in seconds */
            inline void addTime(float time)
                { this->time_ += time; }
            /** @brief Decreases the remaining time of the Timer. @param time The amount of time to remove */
            inline void removeTime(float time)
                { this->time_ -= time; }
            /** @brief Sets the interval of the Timer. @param interval The interval */
            inline void setInterval(float interval)
                { this->interval_ = interval; }
            /** @brief Sets bLoop to a given value. @param bLoop True = loop */
            inline void setLoop(bool bLoop)
                { this->bLoop_ = bLoop; }

        protected:
            TimerBase();

            Executor* executor_; //!< The executor of the function that should be called when the time expires

            float interval_;     //!< The time-interval in seconds
            bool bLoop_;         //!< If true, the function gets called every 'interval' seconds
            bool bActive_;       //!< If true, the Timer ticks and calls the function if the time's up

            float time_;         //!< Internal variable, counting the time till the next function-call
    };

    //! The Timer is a callback-object, calling a given function after a given time-interval.
    template <class T = BaseObject>
    class Timer : public TimerBase
    {
        public:
            Timer() {}

            /**
                @brief Constructor: Initializes the Timer with given values.
                @param interval The timer-interval in seconds
                @param bLoop If true, the function gets called every 'interval' seconds
                @param object The object owning the timer and the function
                @param exeuctor A executor of the function to call
            */
            Timer(float interval, bool bLoop, T* object, ExecutorMember<T>* exeuctor)
            {
                this->setTimer(interval, bLoop, object, exeuctor);
            }

            /**
                @brief Initializes the Timer with given values.
                @param interval The timer-interval in seconds
                @param bLoop If true, the function gets called every 'interval' seconds
                @param object The object owning the timer and the function
                @param exeuctor A executor of the function to call
            */
            void setTimer(float interval, bool bLoop, T* object, ExecutorMember<T>* executor)
            {
                this->interval_ = interval;
                this->bLoop_ = bLoop;
                executor->setObject(object);
                this->executor_ = (Executor*)executor;
                this->bActive_ = true;

                this->time_ = interval;
            }
    };

    //! The StaticTimer is a callback-object, calling a static function after a given time-interval.
    class StaticTimer : public TimerBase
    {
        public:
            StaticTimer() {}

            /**
                @brief Constructor: Initializes the Timer with given values.
                @param interval The timer-interval in seconds
                @param bLoop If true, the function gets called every 'interval' seconds
                @param exeuctor A executor of the function to call
            */
            StaticTimer(float interval, bool bLoop, ExecutorStatic* executor)
            {
                this->setTimer(interval, bLoop, executor);
            }

            /**
                @brief Initializes the Timer with given values.
                @param interval The timer-interval in seconds
                @param bLoop If true, the function gets called every 'interval' seconds
                @param object The object owning the timer and the function
                @param executor A executor of the function to call
            */
            void setTimer(float interval, bool bLoop, ExecutorStatic* executor)
            {
                this->interval_ = interval;
                this->bLoop_ = bLoop;
                this->executor_ = (Executor*)executor;
                this->bActive_ = true;

                this->time_ = interval;
            }
    };

    //! The TimerFrameListener manages all Timers in the game.
    class TimerFrameListener : public Ogre::FrameListener
    {
        private:
            /** @brief Gets called before a frame gets rendered. */
            bool frameStarted(const Ogre::FrameEvent &evt)
            {
                // Iterate through all Timers
                for (Iterator<TimerBase> it = ObjectList<TimerBase>::start(); it; )
                {
                    if (it->isActive())
                    {
                        // If active: Decrease the timer by the duration of the last frame
                        it->time_ -= evt.timeSinceLastFrame;

                        if (it->time_ <= 0)
                        {
                            // It's time to call the function
                            if (it->bLoop_)
                            {
                                it->time_ += it->interval_; // Q: Why '+=' and not '='? A: Think about it. It's more accurate like that. Seriously.
                                while (it->time_ <= 0)
                                {
                                    // The interval was shorter than one tick, so execute the function more than once
                                    it->run();
                                    it->time_ += it->interval_;
                                }
                            }
                            else
                                it->stopTimer(); // Stop the timer if we don't want to loop

                            (it++)->run();
                        }
                        else
                            ++it;
                    }
                    else
                        ++it;
                }

                return FrameListener::frameStarted(evt);
            }
    };
}

#endif /* _Timer_H__ */
