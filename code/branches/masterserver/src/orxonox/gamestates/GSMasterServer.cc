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
 *      Sandro 'smerkli' Merkli 
 *   Co-authors:
 *      Reto Grieder (original file for GSServer.cc)
 *      Fabian 'x3n' Landau (original file for GSServer.cc)
 *
 */

#include "GSMasterServer.h"

#include "util/Debug.h"
#include "core/CommandLineParser.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "network/Server.h"

namespace orxonox
{
    DeclareGameState(GSMasterServer, "masterserver", false, false);

    SetCommandLineArgument(port, 55566).shortcut("p").information("Network communication port to be used 0-65535 (default: 55566)");

    GSMasterServer::GSMasterServer(const GameStateInfo& info)
        : GameState(info)
        , server_(0)
    {
    }

    GSMasterServer::~GSMasterServer()
    {
    }

    void GSMasterServer::activate()
    {
        GameMode::setIsServer(true);

        this->server_ = new Server(CommandLineParser::getValue("port"));
        COUT(0) << "Loading scene in server mode" << std::endl;

        server_->open();
    }

    void GSMasterServer::deactivate()
    {
        this->server_->close();
        delete this->server_;

        GameMode::setIsServer(false);
    }

    void GSMasterServer::update(const Clock& time)
    {
        server_->update(time);
    }
}
