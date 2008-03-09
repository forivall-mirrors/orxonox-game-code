/*
*   ORXONOX - the hottest 3D action shooter ever to exist
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

#include <utility>
#include <iostream>
#include <zlib.h>

#include "core/CoreIncludes.h"
#include "ClientInformation.h"
#include "Synchronisable.h"
#include "GameStateManager.h"

namespace network
{
  GameStateManager::GameStateManager(ClientInformation *head) {
    id=0;
    head_=head;
  }

  GameStateManager::~GameStateManager() {
  }

  void GameStateManager::update(){
    reference = getSnapshot(id);
    gameStateMap.insert(std::pair<int, GameState*>(id, reference));
    gameStateUsed[id]=0;
    ++id;
    return;
  }

  GameStateCompressed GameStateManager::popGameState(int clientID) {
    int gID = head_->findClient(clientID)->getGamestateID();
    std::cout << "popgamestate: sending gstate id: " << id << "diffed from: " << gID << std::endl;
    if(gID!=GAMESTATEID_INITIAL){
      GameState *client = gameStateMap[gID];
      GameState *server = reference;
      //head_->findClient(clientID)->setGamestateID(id);
      return encode(client, server);
    } else {
      GameState *server = reference;
      //head_->findClient(clientID)->setGamestateID(id);
      return encode(server);
      // return an undiffed gamestate and set appropriate flags
    }
  }

  /**
  * This function goes through the whole list of synchronisables and
  * saves all the synchronisables to a flat "list".
  * @return struct of type gamestate containing the size of the whole gamestate and a pointer linking to the flat list
  */
  GameState *GameStateManager::getSnapshot(int id)
  {
    //the size of the gamestate
    int totalsize=0;
    int memsize=1000;
    //the size of one specific synchronisable
    int tempsize=0;
    // get the start of the Synchronisable list
    orxonox::Iterator<Synchronisable> it;
    // struct for return value of Synchronisable::getData()
    syncData sync;

    GameState *retval=new GameState; //return value
    retval->id=id++;
    std::cout << "producing gamestate with id: " << retval->id << std::endl;
    // reserve a little memory and increase it later on
    //COUT(2) << "mallocing" << std::endl;
    retval->data = (unsigned char*)malloc(memsize);
    //COUT(2) << "malloced" << std::endl;

    // offset of memory functions
    int offset=0;
    // go through all Synchronisables
    for(it = orxonox::ObjectList<Synchronisable>::start(); it; ++it){
      //std::cout << "gamestatemanager: in for loop" << std::endl;
      //get size of the synchronisable
      tempsize=it->getSize();
      //std::cout << "size of temp gamestate: " << tempsize << std::endl;
      //COUT(2) << "size of synchronisable: " << tempsize << std::endl;
      // add place for data and 3 ints (length,classid,objectid)
      totalsize+=tempsize+3*sizeof(int);
      //std::cout << "totalsize: " << totalsize << std::endl;
      // allocate additional space
      if(totalsize+tempsize>memsize){
        if(tempsize<1000){
          retval->data = (unsigned char *)realloc((void *)retval->data, totalsize+1000);
          memsize+=1000;
        } else {
          retval->data = (unsigned char *)realloc((void *)retval->data, totalsize+1000);
          memsize+=tempsize+1000;
        }
      }

      // run Synchronisable::getData with offset and additional place for 3 ints in between (for ids and length)
      sync=it->getData((retval->data)+offset+3*sizeof(int));
      *(retval->data+offset)=sync.length;
      *(retval->data+offset+sizeof(int))=sync.objectID;
      *(retval->data+offset+2*sizeof(int))=sync.classID;
      // increase data pointer
      offset+=tempsize+3*sizeof(int);
    }
    retval->size=totalsize;
    return retval;
  }

  GameStateCompressed GameStateManager::encode(GameState *a, GameState *b) {
    //GameState r = diff(a,b);
    //r.diffed = true;
    GameState r = *b;
    r.diffed = false;
    return compress_(&r);
  }

  GameStateCompressed GameStateManager::encode(GameState *a) {
    a->diffed=false;
    return compress_(a);
  }

  GameState GameStateManager::diff(GameState *a, GameState *b) {
    unsigned char *ap = a->data, *bp = b->data;
    int of=0; // pointers offset
    int dest_length=0;
    if(a->size>=b->size)
      dest_length=a->size;
    else
      dest_length=b->size;
    unsigned char *dp = (unsigned char *)malloc(dest_length*sizeof(unsigned char));
    while(of<a->size && of<b->size){
      *(dp+of)=*(ap+of)^*(bp+of); // do the xor
      ++of;
    }
    if(a->size!=b->size){ // do we have to fill up ?
      unsigned char n=0;
      if(a->size<b->size){
        while(of<dest_length){
          *(dp+of)=n^*(bp+of);
          of++;
        }
      } else{
        while(of<dest_length){
          *(dp+of)=*(ap+of)^n;
          of++;
        }
      }
    }
    // should be finished now
    // FIXME: is it true or false now? (struct has changed, producing warnings)
    GameState r = {b->id, dest_length, true, dp};
    return r;
  }

  GameStateCompressed GameStateManager::compress_(GameState *a) {
    //COUT(2) << "compressing gamestate" << std::endl;
    int size = a->size;
    uLongf buffer = (uLongf)((a->size + 12)*1.01)+1;
    unsigned char* dest = (unsigned char*)malloc( buffer );
    int retval;
    //std::cout << "before ziped " << buffer << std::endl;
    retval = compress( dest, &buffer, a->data, (uLong)size );
    //std::cout << "after ziped " << buffer << std::endl;

    switch ( retval ) {
      case Z_OK: std::cout << "successfully compressed" << std::endl; break;
      case Z_MEM_ERROR: std::cout << "not enough memory available" << std::endl; break;
      case Z_BUF_ERROR: std::cout << "not enough memory available in the buffer" << std::endl; break;
      case Z_DATA_ERROR: std::cout << "decompress: data corrupted" << std::endl; break;
    }

    GameStateCompressed compressedGamestate;
    compressedGamestate.compsize = buffer;
    //std::cout << "compressedGamestate.compsize = buffer; " << buffer << std::endl;
    compressedGamestate.normsize = size;
    //std::cout << "compressedGamestate.normsize = size; " << size << std::endl;
    compressedGamestate.id = a->id;
    compressedGamestate.data = dest;
    compressedGamestate.diffed = a->diffed;

    return compressedGamestate;
  }

  void GameStateManager::ackGameState(int clientID, int gamestateID) {
    ClientInformation *temp = head_->findClient(clientID);
    int curid = temp->getID();
    // decrease usage of gamestate and save it
    deleteUnusedGameState(curid);
    //increase gamestateused
    ++gameStateUsed.find(gamestateID)->second;
    temp->setGamestateID(gamestateID);
    /*
    GameState *old = clientGameState[clientID];
    deleteUnusedGameState(old);
    clientGameState[clientID]=idGameState[gamestateID];*/
  }

  bool GameStateManager::deleteUnusedGameState(int gamestateID) {
    int used = --(gameStateUsed.find(gamestateID)->second);
    if(id-gamestateID>KEEP_GAMESTATES && used==0){
      // delete gamestate
      delete gameStateMap.find(gamestateID)->second;
      gameStateMap.erase(gamestateID);
      return true;
    }
    return false;
  }

}
