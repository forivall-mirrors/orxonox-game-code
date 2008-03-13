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

#include "NetworkPrereqs.h"
#include "GameStateManager.h"

namespace network
{
  class GameStateClient
  {
  public:
    GameStateClient();
    ~GameStateClient();
    bool pushGameState(GameStateCompressed *compstate);
  private:
    bool loadSnapshot(GameState *state);
    GameState *diff(GameState a, GameState b);
    GameState *decompress(GameStateCompressed a);
    GameState *decode(GameState a, GameStateCompressed x);
    GameState *decode(GameStateCompressed x);
    void removeObject(orxonox::Iterator<Synchronisable> &it);

    GameState reference;
  };

}

#endif /* _GameStateClient_H__ */
