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
 *      Benjamin Knecht
 *
 */

/**
@file
@brief
    Implementation of Graphics GameState class.
 */

#include "GSGraphics.h"

#include "core/Game.h"
#include "core/GUIManager.h"
#include "core/command/ConsoleCommand.h"
#include "core/command/CommandExecutor.h"

namespace orxonox
{
    DeclareGameState(GSGraphics, "graphics", false, true);

    GSGraphics::GSGraphics(const GameStateInfo& info)
        : GameState(info)
    {
    }

    GSGraphics::~GSGraphics()
    {
    }

    /**
    @brief
        This function is called when we enter this game state.

        There is only one thing to do here:
        \li create console command to toggle GUI
    */
    void GSGraphics::activate()
    {

    }

    /**
    @brief
        This function is called when the game state is left
    */
    void GSGraphics::deactivate()
    {

    }

    void GSGraphics::update(const Clock& time)
    {
        if (this->getActivity().topState)
        {
            // This state can not 'survive' on its own.
            // Load a user interface therefore
            Game::getInstance().requestState("mainMenu");
        }
    }
}
