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
#include "packet/Acknowledgement.h"

namespace orxonox
{
  GamestateManager::GamestateManager() :
  reference(0), id_(0)
  {
    trafficControl_ = new TrafficControl();
  }

  GamestateManager::~GamestateManager()
  {
    delete trafficControl_;
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
    if ( reference != 0 )
      delete reference;
    reference = new packet::Gamestate();
    if(!reference->collectData(++id_)){ //we have no data to send
      delete reference;
      reference=0;
    }
    return true;
  }


  packet::Gamestate *GamestateManager::popGameState(unsigned int clientID) {
    //why are we searching the same client's gamestate id as we searched in
    //Server::sendGameState?
    packet::Gamestate *gs;
    unsigned int gID = ClientInformation::findClient(clientID)->getGamestateID();
    if(!reference)
      return 0;
    gs = reference->doSelection(clientID, 10000);
//     gs = new packet::Gamestate(*reference);
    // save the (undiffed) gamestate in the clients gamestate map
    gamestateMap_[clientID][gs->getID()]=gs;
    //chose wheather the next gamestate is the first or not
    packet::Gamestate *client=0;
    if(gID != GAMESTATEID_INITIAL){
      assert(gamestateMap_.find(clientID)!=gamestateMap_.end());
      std::map<unsigned int, packet::Gamestate*>::iterator it = gamestateMap_[clientID].find(gID);
      if(it!=gamestateMap_[clientID].end())
      {
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

    if(gamestateID == ACKID_NACK){
      temp->setGamestateID(GAMESTATEID_INITIAL);
      // now delete all saved gamestates for this client
      std::map<unsigned int, packet::Gamestate*>::iterator it;
      for(it = gamestateMap_[clientID].begin(); it!=gamestateMap_[clientID].end(); ){
        delete it->second;
        gamestateMap_[clientID].erase(it++);
      }
      return true;
    }

    assert(curid==(unsigned int)GAMESTATEID_INITIAL || curid<gamestateID);
    COUT(4) << "acking gamestate " << gamestateID << " for clientid: " << clientID << " curid: " << curid << std::endl;
    std::map<unsigned int, packet::Gamestate*>::iterator it;
    for(it = gamestateMap_[clientID].begin(); it!=gamestateMap_[clientID].end() && it->first<gamestateID; ){
      delete it->second;
      gamestateMap_[clientID].erase(it++);
    }
    temp->setGamestateID(gamestateID);
    TrafficControl::processAck(clientID, gamestateID);
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
