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

#include "OrxonoxPrereqs.h"

#include "core/CommandLineParser.h"
#include "core/Game.h"
#include "core/LuaState.h"
#include "Main.h"

namespace orxonox
{
    /**
    @brief
        Starting point of orxonox (however not the entry point of the program!)
    */
    int main(const std::string& strCmdLine)
    {
        Game* game = new Game(strCmdLine);

        game->setStateHierarchy(
        "root"
        );

        game->requestState("root");

        game->run();
        delete game;

        return 0;
    }
}
