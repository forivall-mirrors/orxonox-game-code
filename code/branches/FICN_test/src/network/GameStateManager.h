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

#include <map>

#include "zlib.h"

#include "ClientInformation.h"
#include "Synchronisable.h"
#include "orxonox/core/CoreIncludes.h"
#include "orxonox/core/Iterator.h"
#include "PacketTypes.h"

namespace network {

#define KEEP_GAMESTATES 20

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
  GameStateCompressed popGameState(int clientID);
  void ackGameState(int clientID, int gamestateID);
  int id;
private:
  GameState *getSnapshot(int id);
  GameStateCompressed encode(GameState *a, GameState *b);
  GameStateCompressed encode(GameState *a);
  GameState diff(GameState *a, GameState *b);
  GameStateCompressed compress_(GameState *a);
  bool deleteUnusedGameState(int gamestateID);
  
  std::map<int, GameState*> gameStateMap; //map gsID to gamestate*
  std::map<int, int> gameStateUsed; // save the number of clients, that use the specific gamestate
  GameState *reference;
  ClientInformation *head_;
};

}

#endif /* _GameStateManager_H__ */
