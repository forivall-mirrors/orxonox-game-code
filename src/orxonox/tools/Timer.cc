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

#include "OrxonoxStableHeaders.h"
#include "core/Executor.h"
#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/CommandExecutor.h"
#include "Timer.h"

namespace orxonox
{
    ConsoleCommandShortcutExtern(delay, AccessLevel::None);

    /**
        @brief Calls a console command after 'delay' seconds.
        @param delay The delay in seconds
        @param command The console command
    */
    void delay(float delay, const std::string& command)
    {
        StaticTimer *delaytimer = new StaticTimer();
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
    }

    /**
        @brief Constructor: Sets the default-values.
    */
    TimerBase::TimerBase()
    {
        RegisterRootObject(TimerBase);

        this->executor_ = 0;
        this->interval_ = 0;
        this->bLoop_ = false;
        this->bActive_ = false;

        this->time_ = 0;
    }

    /**
        @brief Deletes the executor.
    */
    TimerBase::~TimerBase()
    {
        delete this->executor_;
    }

    /**
        @brief Executes the executor.
    */
    void TimerBase::run() const
    {
        (*this->executor_)();
    }

    /**
        @brief Updates the timer before the frames are rendered.
    */
    void TimerBase::tick(float dt)
    {
        if (this->bActive_)
        {
            // If active: Decrease the timer by the duration of the last frame
            this->time_ -= dt;

            if (this->time_ <= 0)
            {
                // It's time to call the function
                if (this->bLoop_)
                    // Q: Why '+=' and not '='? A: Think about it. It's more accurate like that. Seriously.
                    this->time_ += this->interval_;
                else
                    this->stopTimer(); // Stop the timer if we don't want to loop

                this->run();
            }
        }
    }

}
