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

#include "GSServer.h"

#include "util/Debug.h"
#include "core/CommandLineParser.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "network/Server.h"

namespace orxonox
{
    DeclareGameState(GSServer, "server", false, false);

    SetCommandLineArgument(port, 55556).shortcut("p").information("Network communication port to be used 0-65535 (default: 55556)");

    GSServer::GSServer(const GameStateInfo& info)
        : GameState(info)
        , server_(0)
    {
    }

    GSServer::~GSServer()
    {
    }

    void GSServer::activate()
    {
        GameMode::setHasServer(true);

        this->server_ = new Server(CommandLineParser::getValue("port"));
        COUT(0) << "Loading scene in server mode" << std::endl;

        server_->open();
    }

    void GSServer::deactivate()
    {
        this->server_->close();
        delete this->server_;

        GameMode::setHasServer(false);
    }

    void GSServer::update(const Clock& time)
    {
        server_->update(time);
    }
}
