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
 *      Oliver Scheuss
 *   Co-authors:
 *      Dumeni Manatschal
 *
 */

//
// C++ Interface: GameStateClient
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _GamestateClient_H__
#define _GamestateClient_H__

#include <map>
//
#include "NetworkPrereqs.h"
#include "core/CorePrereqs.h"
#include "packet/Gamestate.h"
#include "objects/SpaceShip.h"
#include "GamestateHandler.h"

#define GAMESTATEID_INITIAL -1

namespace network
{
  class GamestateClient: public GamestateHandler
  {
  public:
    GamestateClient();
    ~GamestateClient();

    bool add(packet::Gamestate *gs, int clientID);
    bool ack(int gamestateID, int clientID);

    int processGamestates();
    packet::Gamestate *getGamestate();
    void cleanup();
  private:
    packet::Gamestate *processGamestate(packet::Gamestate *gs);
    void removeObject(orxonox::ObjectListIterator<Synchronisable> &it);
    void printGamestateMap();
    bool saveShipCache();
    bool loadShipCache();

    int           last_diff_;
    int           last_gamestate_;
    std::map<int, packet::Gamestate *> gamestateMap_;
    packet::Gamestate *tempGamestate_; // we save the received gamestates here during processQueue
    orxonox::SpaceShip *myShip_;
    unsigned char *shipCache_;

  };

}

#endif /* _GamestateClient_H__ */
