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
 *      Oliver Scheuss, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

//
// C++ Interface: GamestateManager
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _GamestateManager_H__
#define _GamestateManager_H__

#include "NetworkPrereqs.h"
#include "GamestateHandler.h"
#include <map>

#include "packet/Gamestate.h"

namespace network
{

#define KEEP_GAMESTATES 10

  /**
  * This Class implements a manager for gamestates:
  * - creating snapshots of gamestates
  * - writing gamestates to universe
  * - diffing gamestates
  * EN/DECODATION:
  * a: last Gamestate a client has received
  * b: new Gamestate
  * x: diffed and compressed gamestate
  * x=(a^b)
  * b=(a^x)
  * diff(a,diff(a,x))=x (hope this is correct)
  * @author Oliver Scheuss
  */
  class GamestateManager: public GamestateHandler{
  public:
    GamestateManager();
    ~GamestateManager();
    
    bool add(packet::Gamestate *gs, int clientID);
    bool processGamestates();
    bool update();
    packet::Gamestate *popGameState(int clientID);

    bool getSnapshot();
    
    bool ack(int gamestateID, int clientID);
    void removeClient(ClientInformation *client);
    private:
    void cleanup(); // "garbage handler"

    std::map<int, packet::Gamestate*> gamestateMap; //map gsID to gamestate*
    std::map<int, int> gamestateUsed; // save the number of clients, that use the specific gamestate
    std::map<int, packet::Gamestate*> gamestateQueue;
    packet::Gamestate *reference;
    int id_;
  };

}

#endif /* _GameStateManager_H__ */
