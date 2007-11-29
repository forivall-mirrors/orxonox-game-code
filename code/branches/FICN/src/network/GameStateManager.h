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
 * This Class implements a manager for gamestates:
 * - creating snapshots of gamestates
 * - writing gamestates to universe
 * - diffing gamestates ?
 * @author Oliver Scheuss
*/
class GameStateManager{
public:
  GameStateManager();
  ~GameStateManager();
  GameState getSnapshot(int id);
  bool loadSnapshot(GameState state);
private:
  void removeObject(orxonox::Iterator<Synchronisable> &it);

};

}

#endif
