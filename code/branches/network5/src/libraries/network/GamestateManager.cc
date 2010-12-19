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

#include <cassert>
#include <queue>
// #include <boost/thread/mutex.hpp>

#include "packet/Acknowledgement.h"
#include "packet/Gamestate.h"
#include "synchronisable/NetworkCallbackManager.h"

#include "core/ThreadPool.h"
#include "core/command/Executor.h"
#include "core/GameMode.h"
#include "util/Debug.h"
#include "util/Clock.h"
// #include "TrafficControl.h"

namespace orxonox
{
  GamestateManager::GamestateManager() :
  currentGamestate_(0), id_(0)
  {
//     trafficControl_ = new TrafficControl();
//     threadMutex_ = new boost::mutex();
//     threadPool_ = new ThreadPool();
  }

  GamestateManager::~GamestateManager()
  {
    if( this->currentGamestate_ )
        delete this->currentGamestate_;std::map<unsigned int, packet::Gamestate*>::iterator it;
    for( it = gamestateQueue.begin(); it != gamestateQueue.end(); ++it )
      delete it->second;
    std::map<uint32_t, peerInfo>::iterator peerIt;
    std::map<uint32_t, packet::Gamestate*>::iterator gamestateIt;
    for( peerIt = peerMap_.begin(); peerIt != peerMap_.end(); ++peerIt )
    {
      for( gamestateIt = peerIt->second.gamestates.begin(); gamestateIt != peerIt->second.gamestates.end(); ++gamestateIt )
        delete gamestateIt->second;
    }
//     this->trafficControl_->destroy();
//     delete this->threadMutex_;
//     delete this->threadPool_;
  }

  bool GamestateManager::update(){
//     cleanup();
    return getSnapshot();
  }

  bool GamestateManager::addGamestate(packet::Gamestate *gs, unsigned int clientID)
  {
    assert(gs);
    std::map<unsigned int, packet::Gamestate*>::iterator it = gamestateQueue.find(clientID);
    if(it!=gamestateQueue.end()){
      // delete obsolete gamestate
      delete it->second;
    }
    gamestateQueue[clientID] = gs;
    return true;
  }

  bool GamestateManager::processGamestates()
  {
    if( this->gamestateQueue.empty() )
        return true;
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
    if ( currentGamestate_ != 0 )
      delete currentGamestate_;
    currentGamestate_ = new packet::Gamestate();
    uint8_t gsMode;
    if( GameMode::isMaster() )
      gsMode = packet::GAMESTATE_MODE_SERVER;
    else
      gsMode = packet::GAMESTATE_MODE_CLIENT;
    uint32_t newID;
    if( GameMode::isMaster() )
      newID = ++id_;
    else
      newID = peerMap_[NETWORK_PEER_ID_SERVER].lastProcessedGamestateID;
    
    if(!currentGamestate_->collectData(newID, gsMode)){ //we have no data to send
      delete currentGamestate_;
      currentGamestate_=0;
    }
    return true;
  }

  std::vector<packet::Gamestate*> GamestateManager::getGamestates()
  {
    if(!currentGamestate_)
      return std::vector<packet::Gamestate*>();
    std::vector<packet::Gamestate*> peerGamestates;
    
    std::map<uint32_t, peerInfo>::iterator peerIt;
    for( peerIt=peerMap_.begin(); peerIt!=peerMap_.end(); ++peerIt )
    {
      if( !peerIt->second.isSynched )
      {
        COUT(5) << "Server: not sending gamestate" << std::endl;
        continue;
      }
      COUT(4) << "client id: " << peerIt->first << std::endl;
      COUT(5) << "Server: doing gamestate gamestate preparation" << std::endl;
      int peerID = peerIt->first; //get client id

      unsigned int lastAckedGamestateID = peerIt->second.lastAckedGamestateID;

      packet::Gamestate* baseGamestate=0;
      if(lastAckedGamestateID != GAMESTATEID_INITIAL)
      {
        assert(peerMap_.find(peerID)!=peerMap_.end());
        std::map<uint32_t, packet::Gamestate*>::iterator it = peerMap_[peerID].gamestates.find(lastAckedGamestateID);
        assert(it!=peerMap_[peerID].gamestates.end());
        baseGamestate = it->second;
      }

      peerGamestates.push_back(0);  // insert an empty gamestate* to change
      finishGamestate( peerID, peerGamestates.back(), baseGamestate, currentGamestate_ );
      //FunctorMember<GamestateManager>* functor =
//       ExecutorMember<GamestateManager>* executor = createExecutor( createFunctor(&GamestateManager::finishGamestate, this) );
//       executor->setDefaultValues( cid, &clientGamestates.back(), client, currentGamestate_ );
//       (*static_cast<Executor*>(executor))();
//       this->threadPool_->passFunction( executor, true );
//       (*functor)( cid, &(clientGamestates.back()), client, currentGamestate_ );
    }

//     threadPool_->synchronise();

    return peerGamestates;
  }


  void GamestateManager::finishGamestate( unsigned int peerID, packet::Gamestate*& destgamestate, packet::Gamestate* base, packet::Gamestate* gamestate ) {
    //why are we searching the same client's gamestate id as we searched in
    //Server::sendGameState?
    // save the (undiffed) gamestate in the clients gamestate map
    //chose wheather the next gamestate is the first or not

//     packet::Gamestate *gs = gamestate->doSelection(clientID, 20000);
//       packet::Gamestate* gs = new packet::Gamestate(*gamestate);
//     packet::Gamestate* gs = gamestate;
    packet::Gamestate *gs = new packet::Gamestate(*gamestate); //TODO: is this neccessary ?
//     packet::Gamestate *gs = new packet::Gamestate();
//     gs->collectData( id_, 0x1 );
//     this->threadMutex_->lock();
    peerMap_[peerID].gamestates[gamestate->getID()]=gs;
//     this->threadMutex_->unlock();
    Clock clock;
    clock.capture();

    if(base)
    {
      packet::Gamestate *diffed1 = gs->diffVariables(base);
      if( diffed1->getDataSize() == 0 )
      {
        delete diffed1;
        destgamestate = 0;
        return;
      }
      gs = diffed1;
    }
    else
    {
      gs = new packet::Gamestate(*gs);
    }


//     bool b = gs->compressData();
//     assert(b);
    clock.capture();
    COUT(4) << "diff and compress time: " << clock.getDeltaTime() << endl;
//     COUT(5) << "sending gamestate with id " << gs->getID();
//     if(gamestate->isDiffed())
//       COUT(5) << " and baseid " << gs->getBaseID() << endl;
//     else
//       COUT(5) << endl;
    gs->setPeerID(peerID);
    destgamestate = gs;
  }


  bool GamestateManager::ackGamestate(unsigned int gamestateID, unsigned int peerID) {
//     ClientInformation *temp = ClientInformation::findClient(peerID);
//     assert(temp);
    std::map<uint32_t, peerInfo>::iterator it = this->peerMap_.find(peerID);
    assert(it!=this->peerMap_.end());
    unsigned int curid = it->second.lastAckedGamestateID;

    if(gamestateID == ACKID_NACK){
      it->second.lastAckedGamestateID = GAMESTATEID_INITIAL;
//       temp->setGamestateID(GAMESTATEID_INITIAL);
      // now delete all saved gamestates for this client
      std::map<uint32_t, packet::Gamestate*>::iterator it2;
      for(it2 = it->second.gamestates.begin(); it2!=it->second.gamestates.end(); ++it2 ){
        delete it2->second;
      }
      it->second.gamestates.clear();
      return true;
    }

    assert(curid==GAMESTATEID_INITIAL || curid<gamestateID);
    COUT(5) << "acking gamestate " << gamestateID << " for peerID: " << peerID << " curid: " << curid << std::endl;
    std::map<uint32_t, packet::Gamestate*>::iterator it2;
    for( it2=it->second.gamestates.begin(); it2!=it->second.gamestates.end(); )
    {
      if( it2->second->getID() < gamestateID )
      {
        delete it2->second;
        it->second.gamestates.erase(it2++);
      }
      else
        ++it2;
    }
    
//     std::map<unsigned int, packet::Gamestate*>::iterator it;
//     for(it = gamestateMap_[peerID].begin(); it!=gamestateMap_[peerID].end() && it->first<gamestateID; ){
//       delete it->second;
//       gamestateMap_[peerID].erase(it++);
//     }
    it->second.lastAckedGamestateID = gamestateID;
//     temp->setGamestateID(gamestateID);
//     TrafficControl::processAck(peerID, gamestateID);
    return true;
  }
  
  uint32_t GamestateManager::getLastProcessedGamestateID(unsigned int peerID)
  {
    assert( this->peerMap_.find(peerID)!=this->peerMap_.end() );
    if( this->peerMap_.find(peerID) != this->peerMap_.end() )
      return this->peerMap_[peerID].lastProcessedGamestateID;
    else
      return GAMESTATEID_INITIAL;
  }
  
  
  void GamestateManager::addPeer(uint32_t peerID)
  {
    assert(peerMap_.find(peerID)==peerMap_.end());
    peerMap_[peerID].peerID = peerID;
    peerMap_[peerID].lastProcessedGamestateID = GAMESTATEID_INITIAL;
    peerMap_[peerID].lastAckedGamestateID = GAMESTATEID_INITIAL;
    if( GameMode::isMaster() )
      peerMap_[peerID].isSynched = false;
    else
      peerMap_[peerID].isSynched = true;
  }

  void GamestateManager::removePeer(uint32_t peerID)
  {
    assert(peerMap_.find(peerID)!=peerMap_.end());
    std::map<uint32_t, packet::Gamestate*>::iterator peerIt;
    for( peerIt = peerMap_[peerID].gamestates.begin(); peerIt!=peerMap_[peerID].gamestates.end(); ++peerIt )
    {
      delete peerIt->second;
    }
    peerMap_.erase(peerMap_.find(peerID));
  }


//   void GamestateManager::removeClient(ClientInformation* client){
//     assert(client);
//     std::map<unsigned int, std::map<unsigned int, packet::Gamestate*> >::iterator clientMap = gamestateMap_.find(client->getID());
//     // first delete all remained gamestates
//     std::map<unsigned int, packet::Gamestate*>::iterator it;
//     for(it=clientMap->second.begin(); it!=clientMap->second.end(); it++)
//       delete it->second;
//     // now delete the clients gamestatemap
//     gamestateMap_.erase(clientMap);
//   }

  bool GamestateManager::processGamestate(packet::Gamestate *gs)
  {
    if(gs->isCompressed())
    {
       bool b = gs->decompressData();
       assert(b);
    }
    assert(!gs->isDiffed());
    uint8_t gsMode;
    if( GameMode::isMaster() )
      gsMode = packet::GAMESTATE_MODE_SERVER;
    else
      gsMode = packet::GAMESTATE_MODE_CLIENT;
    if( gs->spreadData(gsMode) )
    {
      this->peerMap_[gs->getPeerID()].lastProcessedGamestateID = gs->getID();
      return true;
    }
    else
      return false;
  }

}
