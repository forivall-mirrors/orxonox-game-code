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
 *      Fabian 'x3n' Landau
 *
 */

#include "OrxonoxStableHeaders.h"
#include "GSStandalone.h"

#include "core/input/InputManager.h"
#include "core/ConsoleCommand.h"

namespace orxonox
{
    GSStandalone::GSStandalone()
        : GSLevel("standalone")
    {
    }

    GSStandalone::~GSStandalone()
    {
    }

    void GSStandalone::enter()
    {
        GSLevel::enter();

        this->loadLevel();

        // add console commands
        FunctorMember<GSLevel>* functor = createFunctor(&GSLevel::setTimeFactor);
        functor->setObject(this);
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(functor, "setTimeFactor"));

        // level is loaded: we can start capturing the input
        InputManager::getInstance().requestEnterState("game");
    }

    void GSStandalone::leave()
    {
        InputManager::getInstance().requestLeaveState("game");

        // TODO: Remove and destroy console command

        this->unloadLevel();

        GSLevel::leave();
    }

    void GSStandalone::ticked(const Clock& time)
    {
        GSLevel::ticked(time);
        this->tickChild(time);
    }
}
