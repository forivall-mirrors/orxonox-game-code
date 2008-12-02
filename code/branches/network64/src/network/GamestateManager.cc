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
#include <cassert>

#include "core/CoreIncludes.h"
#include "core/BaseObject.h"
#include "ClientInformation.h"
#include "synchronisable/Synchronisable.h"
#include "synchronisable/NetworkCallbackManager.h"

namespace orxonox
{
  GamestateManager::GamestateManager() {
    id_=0;
  }

  GamestateManager::~GamestateManager() {
  }

  bool GamestateManager::update(){
//     cleanup();
    return getSnapshot();
  }

  bool GamestateManager::add(packet::Gamestate *gs, unsigned int clientID){
    assert(gs);
    std::map<unsigned int, packet::Gamestate*>::iterator it = gamestateQueue.find(clientID);
    if(it!=gamestateQueue.end()){
      // delete obsolete gamestate
      delete it->second;
    }
    gamestateQueue[clientID] = gs;
    return true;
  }

  bool GamestateManager::processGamestates(){
    std::map<unsigned int, packet::Gamestate*>::iterator it;
    // now push only the most recent gamestates we received (ignore obsolete ones)
    for(it = gamestateQueue.begin(); it!=gamestateQueue.end(); it++){
      bool b = processGamestate(it->second);
      assert(b);
      delete it->second;
    }
    // now clear the queue
    gamestateQueue.clear();
    //and call all queued callbacks
    NetworkCallbackManager::callCallbacks();
    return true;
  }


  bool GamestateManager::getSnapshot(){
    reference = new packet::Gamestate();
    if(!reference->collectData(++id_)){ //we have no data to send
      delete reference;
      reference=0;
    }
    return true;
  }

  /**
   * this function is used to keep the memory usage low
   * it tries to delete all the unused gamestates
   *
   *
   */
/*  void GamestateManager::cleanup(){
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
  }*/

  packet::Gamestate *GamestateManager::popGameState(unsigned int clientID) {
    //why are we searching the same client's gamestate id as we searched in
    //Server::sendGameState?
    packet::Gamestate *gs;
    unsigned int gID = ClientInformation::findClient(clientID)->getGamestateID();
    if(!reference)
      return 0;
    gs = reference->doSelection(clientID);
//     gs = new packet::Gamestate(*reference);
//     gs = new packet::Gamestate(*reference);
    // save the (undiffed) gamestate in the clients gamestate map
    gamestateMap_[clientID].insert(std::pair<int, packet::Gamestate*>(gs->getID(), gs));
    //chose wheather the next gamestate is the first or not
    packet::Gamestate *client=NULL;
    if(gID != GAMESTATEID_INITIAL){
      std::map<unsigned int, std::map<unsigned int, packet::Gamestate*> >::iterator clientMap = gamestateMap_.find(clientID);
      if(clientMap!=gamestateMap_.end()){
        std::map<unsigned int, packet::Gamestate*>::iterator it = clientMap->second.find(gID);
        if(it!=clientMap->second.end())
          client = it->second;
      }
    }
    if(client){
//       COUT(3) << "diffing" << std::endl;
      gs = gs->diff(client);
    }
    else{
//       COUT(3) << "not diffing" << std::endl;
      gs = new packet::Gamestate(*gs);
    }
    bool b = gs->compressData();
    assert(b);
    return gs;
  }


  bool GamestateManager::ack(unsigned int gamestateID, unsigned int clientID) {
    ClientInformation *temp = ClientInformation::findClient(clientID);
    assert(temp);
    unsigned int curid = temp->getGamestateID();

    if(gamestateID == 0){
      temp->setGamestateID(GAMESTATEID_INITIAL);
      return true;
    }

    assert(curid==(unsigned int)GAMESTATEID_INITIAL || curid<gamestateID);
    COUT(4) << "acking gamestate " << gamestateID << " for clientid: " << clientID << " curid: " << curid << std::endl;
    std::map<unsigned int, packet::Gamestate*>::iterator it, tempit;
    for(it = gamestateMap_[clientID].begin(); it!=gamestateMap_[clientID].end() && it->first<gamestateID; it++){
      delete it->second;
      tempit=it++;
      gamestateMap_[clientID].erase(tempit);
    }
    temp->setGamestateID(gamestateID);
    return true;
  }

  void GamestateManager::removeClient(ClientInformation* client){
    assert(client);
    std::map<unsigned int, std::map<unsigned int, packet::Gamestate*> >::iterator clientMap = gamestateMap_.find(client->getID());
    // first delete all remained gamestates
    std::map<unsigned int, packet::Gamestate*>::iterator it;
    for(it=clientMap->second.begin(); it!=clientMap->second.end(); it++)
      delete it->second;
    // now delete the clients gamestatemap
    gamestateMap_.erase(clientMap);
  }

  bool GamestateManager::processGamestate(packet::Gamestate *gs){
    if(gs->isCompressed())
    {
       bool b = gs->decompressData();
       assert(b);
    }
    assert(!gs->isDiffed());
    return gs->spreadData();
  }

}
