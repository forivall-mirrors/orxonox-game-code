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

#include "core/CommandExecutor.h"
#include "core/ConsoleCommand.h"
#include "core/Game.h"
#include "core/GUIManager.h"
// HACK:
#include "overlays/Map.h"

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
        // add console command to toggle GUI
        CommandExecutor::addConsoleCommandShortcut(createConsoleCommand(createFunctor(&GSGraphics::toggleGUI, this), "toggleGUI"));
    }

    /**
    @brief
        This function is called when the game state is left
    */
    void GSGraphics::deactivate()
    {
        // HACK: (destroys a resource smart pointer)
        Map::hackDestroyMap();
    }

    /**
    @brief
        Toggles the visibility of the current GUI

        This function just executes a Lua function in the main script of the GUI by accessing the GUIManager.
        For more details on this function check out the Lua code.
    */
    void GSGraphics::toggleGUI()
    {
        GUIManager::getInstance().executeCode("toggleGUI()");
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
