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
#ifndef NETWORK_GAMESTATEMANAGER_H
#define NETWORK_GAMESTATEMANAGER_H

#include <vector>

#include "zlib.h"

#include "Synchronisable.h"
#include "orxonox/core/IdentifierIncludes.h"
#include "orxonox/core/Iterator.h"
#include "PacketTypes.h"

namespace network {



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
  GameStateManager();
  ~GameStateManager();
  void update();
  GameStateCompressed popGameState(int clientID);
private:
  GameState getSnapshot(int id);
  GameStateCompressed encode(GameState *a, GameState *b);
  GameState diff(GameState *a, GameState *b);
  GameStateCompressed compress_(GameState a);
  
  std::vector<GameState *> clientGameState;
  GameState reference;
  int id;
};

}

#endif
