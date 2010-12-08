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
#include "core/Game.h"
#include "core/GameMode.h"

namespace orxonox
{
  DeclareGameState(GSMasterServer, "masterserver", false, false);

  GSMasterServer::GSMasterServer(const GameStateInfo& info)
    : GameState(info)
  {

  }

  GSMasterServer::~GSMasterServer()
  {
  }

  void GSMasterServer::activate()
  {
    /* TODO make this work for masterserver as well */
    //GameMode::setIsServer(true);

    this->mserver = new MasterServer();
    COUT(0) << "Loading masterserver mode" << std::endl;

    this->mserver->run();
  }

  void GSMasterServer::deactivate()
  { /* nothing so far */ }

  void GSMasterServer::update(const Clock& time)
  { 
    this->mserver->run();
  }
}
