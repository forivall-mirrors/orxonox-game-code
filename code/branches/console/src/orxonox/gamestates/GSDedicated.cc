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

#include "GSDedicated.h"

#include "util/Debug.h"
#include "core/CommandLine.h"
#include "core/Game.h"
#include "core/GameMode.h"
#include "network/Server.h"

namespace orxonox
{
    DeclareGameState(GSDedicated, "dedicated", false, false);
    
    GSDedicated::GSDedicated(const GameStateInfo& info)
        : GameState(info)
        , server_(0)
    {
    }

    GSDedicated::~GSDedicated()
    {
    }

    void GSDedicated::activate()
    {
        GameMode::setHasServer(true);

        this->server_ = new Server(CommandLine::getValue("port"));
        COUT(0) << "Loading scene in server mode" << std::endl;

        server_->open();
    }

    void GSDedicated::deactivate()
    {
        this->server_->close();
        delete this->server_;

        GameMode::setHasServer(false);
    }

    void GSDedicated::update(const Clock& time)
    {
        server_->update(time);
    }
}
