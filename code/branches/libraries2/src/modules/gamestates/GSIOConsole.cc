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
 *      ...
 *
 */

#include "GSIOConsole.h"

#include <iostream>

#include "core/ConsoleCommand.h"
#include "core/Game.h"

namespace orxonox
{
    DeclareGameState(GSIOConsole, "ioConsole", false, false);

    GSIOConsole::GSIOConsole(const GameStateInfo& info)
        : GameState(info)
    {
    }

    GSIOConsole::~GSIOConsole()
    {
    }

    void GSIOConsole::activate()
    {
        {
            FunctorMember<GSIOConsole>* functor = createFunctor(&GSIOConsole::loadMenu);
            functor->setObject(this);
            this->ccLoadMenu_ = createConsoleCommand(functor, "loadMenu");
            CommandExecutor::addConsoleCommandShortcut(this->ccLoadMenu_);
        }
    }

    void GSIOConsole::deactivate()
    {
/*
        if (this->ccLoadMenu_)
        {
            delete this->ccLoadMenu_;
            this->ccLoadMenu_ = 0;
        }
*/
    }

    void GSIOConsole::update(const Clock& time)
    {
        std::cout << ">";
        std::string command;
        std::getline(std::cin, command);
        CommandExecutor::execute(command, true);
    }

    void GSIOConsole::loadMenu()
    {
        Game::getInstance().popState();
        Game::getInstance().requestStates("graphics, mainMenu");
    }
}
