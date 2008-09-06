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
// C++ Implementation: GameStateManager
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "GamestateManager.h"

#include <utility>
#include <iostream>
#include <zlib.h>
#include <assert.h>

#include "core/CoreIncludes.h"
#include "core/BaseObject.h"
#include "ClientInformation.h"
#include "Synchronisable.h"

namespace network
{
  GamestateManager::GamestateManager() {
    id_=0;
  }

  GamestateManager::~GamestateManager() {
  }

  bool GamestateManager::update(){
    cleanup();
    getSnapshot();
    return true;
  }
  
  bool GamestateManager::add(packet::Gamestate *gs, int clientID){
    assert(gs);
    std::map<int, packet::Gamestate*>::iterator it = gamestateQueue.find(clientID);
    if(it!=gamestateQueue.end()){
      // delete obsolete gamestate
      delete it->second;
    }
    gamestateQueue[clientID] = gs;
    return true;
  }
  
  bool GamestateManager::processGamestates(){
    std::map<int, packet::Gamestate*>::iterator it;
    // now push only the most recent gamestates we received (ignore obsolete ones)
    for(it = gamestateQueue.begin(); it!=gamestateQueue.end(); it++){
      assert(processGamestate(it->second));
      delete it->second;
    }
    // now clear the queue
    gamestateQueue.clear();
    return true;
  }
  
  
  bool GamestateManager::getSnapshot(){
    reference = new packet::Gamestate();
    reference->collectData(++id_);
    COUT(4) << "inserting gamestate: " << reference << std::endl;
    gamestateMap.insert(std::pair<int, packet::Gamestate*>(id_, reference));
    gamestateUsed[id_]=0;
    return true;
  }
  
  /**
   * this function is used to keep the memory usage low
   * it tries to delete all the unused gamestates
   * 
   * 
   */
  void GamestateManager::cleanup(){
    std::map<int,int>::iterator it = gamestateUsed.begin();
    while(it!=gamestateUsed.end()){
      if((id_-(*it).first)<KEEP_GAMESTATES)
        break;
      if( (*it).second <= 0 ){
        COUT(5) << "GameStateManager: deleting gamestate with id: " << (*it).first << ", uses: " << (*it).second << std::endl;
        std::map<int, packet::Gamestate *>::iterator tempit = gamestateMap.find((*it).first);
        if( tempit != gamestateMap.end() ){
          packet::Gamestate *temp = tempit->second;
          if(temp){
            delete gamestateMap[(*it).first];
            gamestateMap.erase((*it).first);
          }
        }
        gamestateUsed.erase(it++);
        continue;
      }
      it++;
    }
  }

  packet::Gamestate *GamestateManager::popGameState(int clientID) {
    //why are we searching the same client's gamestate id as we searched in
    //Server::sendGameState?
    int gID = ClientInformation::findClient(clientID)->getGamestateID();
    COUT(4) << "G.St.Man: popgamestate: sending gstate_id: " << id_ << " diffed from: " << gID << std::endl;
//     COUT(3) << "gamestatemap: " << &gameStateMap << std::endl;
    //chose wheather the next gamestate is the first or not
    if(gID != GAMESTATEID_INITIAL){
      // TODO something with the gamestatemap is wrong
      packet::Gamestate *client=NULL;
      std::map<int, packet::Gamestate*>::iterator it = gamestateMap.find(gID);
      if(it!=gamestateMap.end())
        client = it->second;
      packet::Gamestate *gs;
      if(client)
        gs = reference->diff(client);
      else
        gs = new packet::Gamestate(*reference);
      gs->compressData();
      return gs;
    } else {
      COUT(4) << "we got a GAMESTATEID_INITIAL for clientID: " << clientID << std::endl;
//       ackGameState(clientID, reference->id);
      return new packet::Gamestate(*reference);
      // return an undiffed gamestate and set appropriate flags
    }
  }
  
  
  bool GamestateManager::ack(int gamestateID, int clientID) {
    ClientInformation *temp = ClientInformation::findClient(clientID);
    if(temp==0)
      return false;
    int curid = temp->getGamestateID();
    
    if(gamestateID == GAMESTATEID_INITIAL){
      temp->setGamestateID(GAMESTATEID_INITIAL);
      if(curid!=GAMESTATEID_INITIAL){
        assert(gamestateUsed.find(curid)!=gamestateUsed.end());
        --(gamestateUsed.find(curid)->second);
      }
      return false;
    }
    if(curid > gamestateID)
      // the network packets got messed up 
      return true;
    COUT(4) << "acking gamestate " << gamestateID << " for clientid: " << clientID << " curid: " << curid << std::endl;
    // decrease usage of gamestate and save it
//     deleteUnusedGameState(curid);
    //increase gamestateused
    std::map<int, int>::iterator it = gamestateUsed.find(curid);
    if(curid!=GAMESTATEID_INITIAL){
      if(it!=gamestateUsed.end())
        --(it->second);
    }
    it = gamestateUsed.find(gamestateID);
    if(it!=gamestateUsed.end()){
      ++(it->second);
      temp->setGamestateID(gamestateID);
    }
    return true;
  }

  void GamestateManager::removeClient(ClientInformation* client){
    if(!client)
      return;
    if(client->getGamestateID()>=0)
      gamestateUsed[client->getGamestateID()]--;
    ClientInformation::removeClient(client->getID());
  }
  
  bool GamestateManager::processGamestate(packet::Gamestate *gs){
    assert(gs->decompressData());
    assert(!gs->isDiffed());
    return gs->spreadData();
  }

}
