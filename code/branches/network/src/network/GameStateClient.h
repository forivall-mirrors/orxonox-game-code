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
#ifndef _GameStateClient_H__
#define _GameStateClient_H__

#include <queue>

#include "NetworkPrereqs.h"
#include "PacketTypes.h"

namespace network
{
  class GameStateClient
  {
  public:
    GameStateClient();
    ~GameStateClient();
    //#### ADDED FOR TESTING PURPOSE ####
    GameState* testDecompress( GameStateCompressed* gc );
    GameState* testUndiff( GameState* g_old, GameState* g_diffed );
    //#### END TESTING PURPOSE ####
    bool pushGameState(GameStateCompressed *compstate);
  private:
    bool loadSnapshot(GameState *state);
    GameState *undiff(GameState *a, GameState *b);
    GameState *decompress(GameStateCompressed *a);
    GameState *decode(GameState *a, GameStateCompressed *x);
    GameState *decode(GameStateCompressed *x);
    void removeObject(orxonox::Iterator<Synchronisable> &it);

    GameState     *reference;
    std::queue<GameState *> gameStateList;
  };

}

#endif /* _GameStateClient_H__ */
