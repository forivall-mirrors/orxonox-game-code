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

#include "util/Debug.h"
#include "core/Executor.h"
#include "core/ThreadPool.h"
#include "ClientInformation.h"
#include "packet/Acknowledgement.h"
#include "packet/Gamestate.h"
#include "synchronisable/NetworkCallbackManager.h"
#include "TrafficControl.h"

namespace orxonox
{
  GamestateManager::GamestateManager() :
  reference(0), id_(0)
  {
    trafficControl_ = new TrafficControl();
//     threadMutex_ = new boost::mutex();
//     threadPool_ = new ThreadPool();
  }

  GamestateManager::~GamestateManager()
  {
    if( this->reference )
        delete this->reference;std::map<unsigned int, packet::Gamestate*>::iterator it;
    for( it = gamestateQueue.begin(); it != gamestateQueue.end(); ++it )
      delete it->second;
    std::map<unsigned int, std::map<unsigned int, packet::Gamestate*> >::iterator it1;
    std::map<unsigned int, packet::Gamestate*>::iterator it2;
    for( it1 = gamestateMap_.begin(); it1 != gamestateMap_.end(); ++it1 )
    {
      for( it2 = it1->second.begin(); it2 != it1->second.end(); ++it2 )
        delete it2->second;
    }
    this->trafficControl_->destroy();
//     delete this->threadMutex_;
//     delete this->threadPool_;
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
    if ( reference != 0 )
      delete reference;
    reference = new packet::Gamestate();
    if(!reference->collectData(++id_, 0x1)){ //we have no data to send
      delete reference;
      reference=0;
    }
    return true;
  }

  void GamestateManager::sendGamestates()
  {
    ClientInformation *temp = ClientInformation::getBegin();
    std::queue<packet::Gamestate*> clientGamestates;
    while(temp != NULL){
      if( !(temp->getSynched()) ){
        COUT(5) << "Server: not sending gamestate" << std::endl;
        temp=temp->next();
        if(!temp)
          break;
        continue;
      }
      COUT(4) << "client id: " << temp->getID() << " RTT: " << temp->getRTT() << " loss: " << temp->getPacketLoss() << std::endl;
      COUT(5) << "Server: doing gamestate gamestate preparation" << std::endl;
      int cid = temp->getID(); //get client id

      unsigned int gID = temp->getGamestateID();
      if(!reference)
        return;

      packet::Gamestate *client=0;
      if(gID != GAMESTATEID_INITIAL){
        assert(gamestateMap_.find(cid)!=gamestateMap_.end());
        std::map<unsigned int, packet::Gamestate*>::iterator it = gamestateMap_[cid].find(gID);
        if(it!=gamestateMap_[cid].end())
        {
          client = it->second;
        }
      }

      clientGamestates.push(0);
      finishGamestate( cid, &clientGamestates.back(), client, reference );
      //FunctorMember<GamestateManager>* functor =
//       ExecutorMember<GamestateManager>* executor = createExecutor( createFunctor(&GamestateManager::finishGamestate, this) );
//       executor->setDefaultValues( cid, &clientGamestates.back(), client, reference );
//       (*static_cast<Executor*>(executor))();
//       this->threadPool_->passFunction( executor, true );
//       (*functor)( cid, &(clientGamestates.back()), client, reference );

      temp = temp->next();
    }

//     threadPool_->synchronise();

    while( !clientGamestates.empty() )
    {
      if(clientGamestates.front())
        clientGamestates.front()->send();
      clientGamestates.pop();
    }
  }


  void GamestateManager::finishGamestate( unsigned int clientID, packet::Gamestate** destgamestate, packet::Gamestate* base, packet::Gamestate* gamestate ) {
    //why are we searching the same client's gamestate id as we searched in
    //Server::sendGameState?
    // save the (undiffed) gamestate in the clients gamestate map
    //chose wheather the next gamestate is the first or not

    packet::Gamestate *gs = gamestate->doSelection(clientID, 20000);
//     packet::Gamestate *gs = new packet::Gamestate(*gamestate);
//     packet::Gamestate *gs = new packet::Gamestate();
//     gs->collectData( id_, 0x1 );
//     this->threadMutex_->lock();
    gamestateMap_[clientID][gamestate->getID()]=gs;
//     this->threadMutex_->unlock();

    if(base)
    {

//       COUT(3) << "diffing" << std::endl;
//       packet::Gamestate* gs1  = gs;
      packet::Gamestate *diffed = gs->diff(base);
      //packet::Gamestate *gs2 = diffed->undiff(gs);
//       assert(*gs == *gs2);
      gs = diffed;
//       packet::Gamestate* gs2 = gs->undiff(client);
//       gs = new packet::Gamestate(*gs);
//       assert(*gs1==*gs2);
    }
    else{
      gs = new packet::Gamestate(*gs);
    }


    bool b = gs->compressData();
    assert(b);
//     COUT(4) << "sending gamestate with id " << gs->getID();
//     if(gamestate->isDiffed())
//     COUT(4) << " and baseid " << gs->getBaseID() << endl;
//     else
//     COUT(4) << endl;
    gs->setClientID(clientID);
    *destgamestate = gs;
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

    assert(curid==GAMESTATEID_INITIAL || curid<gamestateID);
    COUT(5) << "acking gamestate " << gamestateID << " for clientid: " << clientID << " curid: " << curid << std::endl;
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
    return gs->spreadData(0x1);
  }

}
