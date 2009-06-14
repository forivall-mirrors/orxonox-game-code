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

#include "OrxonoxConfig.h"

#include "util/Debug.h"
#include "core/Identifier.h"
#include "core/Game.h"

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
    {
        orxonox::Game orxonox(argc, argv);

        orxonox.setStateHierarchy(
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

        orxonox.run();

        // destroy the GameStates created pre-mainly
        orxonox::Game::destroyStates();
    } // orxonox gets destroyed right here!

    // Clean up class hierarchy stuff (identifiers, xmlport, configvalue, consolecommand)
    // Needs to be done after Game destructor because of ~OrxonoxClass
    orxonox::Identifier::destroyAllIdentifiers();

    return 0;
}
