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
#ifndef _GameStateClient_H__
#define _GameStateClient_H__

#include <map>
// 
#include "NetworkPrereqs.h"
#include "core/CorePrereqs.h"
#include "PacketTypes.h"
#include "objects/SpaceShip.h"


#define GAMESTATEID_INITIAL -1

namespace network
{
  class GameStateClient
  {
  public:
    GameStateClient();
    ~GameStateClient();
    
    void addGameState(GameStateCompressed *gs);
    int processGameState();
    GameStateCompressed *popPartialGameState();
    void cleanup();
  private:
    bool pushGameState(GameStateCompressed *compstate);
    bool loadSnapshot(GameState *state);
    GameState *getPartialSnapshot();
    GameState *undiff(GameState *old, GameState *diff);
    GameStateCompressed *compress_(GameState *a);
    GameState *decompress(GameStateCompressed *a);
    GameState *decode(GameState *old, GameStateCompressed *diff);
    GameState *decode(GameStateCompressed *x);
    void removeObject(orxonox::Iterator<Synchronisable> &it);
    void printGameStateMap();
    bool saveShipCache();
    bool loadShipCache();

    int           last_diff_;
    int           last_gamestate_;
    std::map<int, GameState *> gameStateMap;
    GameStateCompressed *tempGameState_; // we save the received gamestates here during processQueue
    orxonox::SpaceShip *myShip_;
    syncData shipCache_;
    
    
    
    
  public:
    //#### ADDED FOR TESTING PURPOSE ####
    GameState* testDecompress( GameStateCompressed* gc );
    GameState* testUndiff( GameState* g_old, GameState* g_diffed );
    //#### END TESTING PURPOSE ####
  };

}

#endif /* _GameStateClient_H__ */
