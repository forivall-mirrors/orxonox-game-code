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

#include "core/Executor.h"
#include "core/CoreIncludes.h"
#include "core/ConsoleCommand.h"
#include "core/CommandExecutor.h"
#include "Timer.h"

namespace orxonox
{
    ConsoleCommandShortcutExtern(delay, AccessLevel::None);

    void delay(float delay, const std::string& command)
    {
        StaticTimer *delaytimer = new StaticTimer();
        ExecutorStatic* delayexecutor = createExecutor(createFunctor(&executeDelayedCommand));
        delayexecutor->setDefaultValues(delaytimer, command);
        delaytimer->setTimer(delay, false, delayexecutor);
    }

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

    TimerBase::~TimerBase()
    {
        delete this->executor_;
    }

    void TimerBase::run() const
    {
        (*this->executor_)();
    }
}
