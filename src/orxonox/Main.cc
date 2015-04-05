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
    The main function of Orxonox (but not the entry point of the program!)
*/

#include "Main.h"

#include "core/commandline/CommandLineIncludes.h"
#include "core/Game.h"
#include "core/LuaState.h"

namespace orxonox
{
    SetCommandLineSwitch(console).information("Start in console mode (text IO only)");
    SetCommandLineSwitch(server).information("Start in server mode");
    SetCommandLineSwitch(client).information("Start in client mode");
    SetCommandLineSwitch(dedicated).information("Start in dedicated server mode");
    SetCommandLineSwitch(standalone).information("Start in standalone mode");
    SetCommandLineSwitch(dedicatedClient).information("Start in dedicated client mode");

    /* ADD masterserver command */
    SetCommandLineSwitch(masterserver).information("Start in masterserver mode");

    SetCommandLineArgument(generateDoc, "")
        .information("Generates a Doxygen file from things like SetConsoleCommand");

    /**
    @brief
        Starting point of orxonox (however not the entry point of the program!)
    */
    int main(const std::string& strCmdLine)
    {
        orxout(internal_status) << "entering orxonox::main()" << endl;
        orxout(internal_info) << "command line: " << strCmdLine << endl;

        orxout(internal_info) << "creating Game object:" << endl;
        Game* game = new Game(strCmdLine);
        orxout(user_status) << "Finished initialization" << endl;

        if (CommandLineParser::getValue("generateDoc").get<std::string>().empty())
        {
            orxout(internal_info) << "preparing game states" << endl;

            /* TODO make this clear */
            game->setStateHierarchy(
            "root"
            " graphics"
            "  mainMenu"
            "  standalone,server,client"
            "   level"
            " server,client,masterserver"
            "  level"
            );

            game->requestState("root");

            // Some development hacks (not really, but in the future, these calls won't make sense anymore)
            if (CommandLineParser::getValue("standalone").get<bool>())
                Game::getInstance().requestStates("graphics, standalone, level");
            else if (CommandLineParser::getValue("server").get<bool>())
                Game::getInstance().requestStates("graphics, server, level");
            else if (CommandLineParser::getValue("client").get<bool>())
                Game::getInstance().requestStates("graphics, client, level");
            else if (CommandLineParser::getValue("dedicated").get<bool>())
                Game::getInstance().requestStates("server, level");
            else if (CommandLineParser::getValue("dedicatedClient").get<bool>())
                Game::getInstance().requestStates("client, level");
            /* ADD masterserver command */
            else if (CommandLineParser::getValue("masterserver").get<bool>())
                Game::getInstance().requestStates("masterserver");
            else
            {
                if (!CommandLineParser::getValue("console").get<bool>())
                    Game::getInstance().requestStates("graphics, mainMenu");
            }

            orxout(internal_info) << "starting game" << endl;
            game->run();
        }

        delete game;

        return 0;
    }
}
