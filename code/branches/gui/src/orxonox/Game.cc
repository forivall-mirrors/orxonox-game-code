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

/**
@file
@brief
    Implementation of the Game class.
*/

#include "OrxonoxStableHeaders.h"
#include "Game.h"

#include <exception>
#include <cassert>

#include "util/Debug.h"
#include "util/Exception.h"
#include "core/CommandLine.h"
#include "core/ConsoleCommand.h"
#include "core/Core.h"
#include "core/Identifier.h"

#include "gamestates/GSRoot.h"
#include "gamestates/GSGraphics.h"
#include "gamestates/GSStandalone.h"
#include "gamestates/GSServer.h"
#include "gamestates/GSClient.h"
#include "gamestates/GSDedicated.h"
#include "gamestates/GSGUI.h"
#include "gamestates/GSIOConsole.h"

/*
@brief
    Main method. Game starts here (except for static initialisations).
*/
int main(int argc, char** argv)
{
    orxonox::Game orxonox(argc, argv);
    orxonox.run();
    return 0;
}

namespace orxonox
{
    void stop_game()
    {
        Game::getInstance().stop();
    }

    SetCommandLineArgument(state, "gui").shortcut("s");
    SetConsoleCommandShortcutExternAlias(stop_game, "exit");

    Game* Game::singletonRef_s = 0;

    /**
    @brief
        Non-initialising constructor.
    */
    Game::Game(int argc, char** argv)
    {
        assert(singletonRef_s == 0);
        singletonRef_s = this;

        this->abort_ = false;

        this->core_ = new orxonox::Core();
        this->gameClock_ = this->core_->initialise(argc, argv);
    }

    /**
    @brief
    */
    Game::~Game()
    {
        // Destroy pretty much everyhting left
        delete this->core_;

        // Clean up class hierarchy stuff (identifiers, xmlport, configvalue, consolecommand)
        // Needs to be done after 'delete core' because of ~OrxonoxClass
        orxonox::Identifier::destroyAllIdentifiers();

        assert(singletonRef_s);
        singletonRef_s = 0;
    }

    /**
    @brief
        Main loop of the orxonox game.
    @note
        We use the Ogre::Timer to measure time since it uses the most precise
        method an any platform (however the windows timer lacks time when under
        heavy kernel load!).
    */
    void Game::run()
    {
        // create the gamestates
        GSRoot root;
        GSGraphics graphics;
        GSStandalone standalone;
        GSServer server;
        GSClient client;
        GSDedicated dedicated;
        GSGUI gui;
        GSIOConsole ioConsole;

        // make the hierarchy
        root.addChild(&graphics);
        graphics.addChild(&standalone);
        graphics.addChild(&server);
        graphics.addChild(&client);
        graphics.addChild(&gui);
        root.addChild(&ioConsole);
        root.addChild(&dedicated);

        root.activate();

        // get initial state from command line
        root.gotoState(CommandLine::getValue("state"));

        while (!this->abort_)
        {
            this->gameClock_->capture();

            root.tick(*this->gameClock_);

            if (root.stateRequest_ != "")
                root.gotoState(root.stateRequest_);
        }

        root.gotoState("root");
        root.deactivate();
    }

    void Game::stop()
    {
        this->abort_ = true;
    }
}
