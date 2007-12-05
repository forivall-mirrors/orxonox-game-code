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

#include "Synchronisable.h"
#include "orxonox/core/IdentifierIncludes.h"
#include "orxonox/core/Iterator.h"

namespace network {

/**
 * This struct defines a gamestate:
 * size: total size of the data in *data
 * data: pointer to the data allocated in the memory
 */
  struct GameState{
  int id;
  int size;
  unsigned char *data;
};

/**
 * this struct defines a gamestate:
 * compsize is the size of the compressed data
 * normsize is the size of the uncompressed data
 * data are the gamestates
 */
  struct GameStateCompressed{
	int id; 
	int compsize;
	int normsize;
	unsigned char *data;
  };

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
  GameStateCompressed getSnapshot(int id);
  bool loadSnapshot(GameStateCompressed state);
  GameStateCompressed encode(GameState a, GameState b);
  GameState decode(GameState a, GameStateCompressed x);
private:
  void removeObject(orxonox::Iterator<Synchronisable> &it);
  GameState diff(GameState a, GameState b);
  GameStateCompressed compress(GameState a);
  GameState decompress(GameStateCompressed a);
};

}

#endif
