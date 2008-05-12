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
// C++ Interface: GameStateManager
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _GameStateManager_H__
#define _GameStateManager_H__

#include "NetworkPrereqs.h"

#include <map>

#include "PacketTypes.h"

namespace network
{

#define KEEP_GAMESTATES 10

  /**
  * This Class implements a manager for gamestates:
  * - creating snapshots of gamestates
  * - writing gamestates to universe
  * - diffing gamestates ?
  *
  * EN/DECODATION:
  * a: last Gamestate a client has received
  * b: new Gamestate
  * x: diffed and compressed gamestate
  * x=(a^b)
  * b=(a^x)
  * diff(a,diff(a,x))=x (hope this is correct)
  * @author Oliver Scheuss
  */
  class GameStateManager{
  public:
    GameStateManager(ClientInformation *head);
    ~GameStateManager();
    
    void update();
    GameStateCompressed *popGameState(int clientID);
    bool pushGameState(GameStateCompressed *gs, int clientID);
    void ackGameState(int clientID, int gamestateID);
    void removeClient(ClientInformation *client);
  private:
    void cleanup(); // "garbage handler"
    GameState *getSnapshot();
    bool loadPartialSnapshot(GameState *state, int clientID);
    GameStateCompressed *encode(GameState *a, GameState *b);
    GameStateCompressed *encode(GameState *a);
    GameState *diff(GameState *a, GameState *b);
    GameStateCompressed *compress_(GameState *a);
    GameState *decompress(GameStateCompressed *a);
    bool printGameStates();
    bool checkAccess(int clientID, int objectID);

    std::map<int, GameState*> gameStateMap; //map gsID to gamestate*
    std::map<int, int> gameStateUsed; // save the number of clients, that use the specific gamestate
    GameState *reference;
    ClientInformation *head_;
    int id_;
    
    
    
    
  public:
    //#### ADDED FOR TESTING PURPOSE ####
    GameStateCompressed* testCompress( GameState* g );
    GameState* testDiff( GameState* a, GameState* b );
  //#### END TESTING PURPOSE ####
  };

}

#endif /* _GameStateManager_H__ */
