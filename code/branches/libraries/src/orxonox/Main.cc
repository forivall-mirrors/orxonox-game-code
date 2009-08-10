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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */
 
/**
@file
@brief
    Entry point of the program.
*/

#include "OrxonoxPrereqs.h"
#include "SpecialConfig.h"

#ifdef ORXONOX_USE_WINMAIN
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
#include <windows.h>
#endif

#include "util/Debug.h"
#include "util/Exception.h"
#include "core/CommandLine.h"
#include "core/Game.h"

SetCommandLineSwitch(console).information("Start in console mode (text IO only)");
// Shortcuts for easy direct loading
SetCommandLineSwitch(server).information("Start in server mode");
SetCommandLineSwitch(client).information("Start in client mode");
SetCommandLineSwitch(dedicated).information("Start in dedicated server mode");
SetCommandLineSwitch(standalone).information("Start in standalone mode");

/*
@brief
    Main method. Game starts here (except for static initialisations).
*/
#ifdef ORXONOX_USE_WINMAIN
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char** argv)
#endif
{
    using namespace orxonox;

    Game* game = 0;
    try
    {
#ifndef ORXONOX_USE_WINMAIN
        std::string strCmdLine;
        for (int i = 1; i < argc; ++i)
            strCmdLine += argv[i] + std::string(" ");
#endif
        game = new Game(strCmdLine);

        game->setStateHierarchy(
        "root"
        " graphics"
        "  mainMenu"
        "  standalone"
        "   level"
        "  server"
        "   level"
        "  client"
        "   level"
        " dedicated"
        "  level"
        " ioConsole"
        );

        game->requestState("root");

        // Some development hacks (not really, but in the future, this calls won't make sense anymore)
        if (CommandLine::getValue("standalone").getBool())
            Game::getInstance().requestStates("graphics, standalone, level");
        else if (CommandLine::getValue("server").getBool())
            Game::getInstance().requestStates("graphics, server, level");
        else if (CommandLine::getValue("client").getBool())
            Game::getInstance().requestStates("graphics, client, level");
        else if (CommandLine::getValue("dedicated").getBool())
            Game::getInstance().requestStates("dedicated, level");
        else if (CommandLine::getValue("console").getBool())
            Game::getInstance().requestStates("ioConsole");
        else
            Game::getInstance().requestStates("graphics, mainMenu");
    }
    catch (const std::exception& ex)
    {
        COUT(0) << "Orxonox failed to initialise: " << ex.what() << std::endl;
        COUT(0) << "Terminating program." << std::endl;
        return 1;
    }
    catch (...)
    {
        COUT(0) << "Orxonox failed to initialise: " << std::endl;
        COUT(0) << "Terminating program." << std::endl;
        return 1;
    }

    game->run();
    delete game;

    return 0;
}
