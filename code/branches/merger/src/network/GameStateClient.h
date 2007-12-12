//
// C++ Interface: GameStateClient
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NETWORK_GAMESTATECLIENT_H
#define NETWORK_GAMESTATECLIENT_H

#include "zlib.h"
#include "Synchronisable.h"
#include "orxonox/core/IdentifierIncludes.h"
#include "GameStateManager.h"

namespace network {

/**
	@author 
*/
class GameStateClient{
public:
  GameStateClient();
  ~GameStateClient();
  bool pushGameState(GameStateCompressed compstate);
private:
  bool loadSnapshot(GameState state);
  GameState diff(GameState a, GameState b);
  GameState decompress(GameStateCompressed a);
  GameState decode(GameState a, GameStateCompressed x);
  GameState decode(GameStateCompressed x);
  void removeObject(orxonox::Iterator<Synchronisable> &it);
  
  GameState reference;
};

}

#endif
