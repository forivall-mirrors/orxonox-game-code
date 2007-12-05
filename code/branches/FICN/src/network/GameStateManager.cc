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
#include "GameStateManager.h"

namespace network {

GameStateManager::GameStateManager()
{
  id=0;
}

GameStateManager::~GameStateManager()
{
}

void GameStateManager::update(){
  reference = getSnapshot(id);
  idGameState[id]=reference;
  ++id;
  return;
}

GameStateCompressed GameStateManager::popGameState(int clientID){
  GameState *client = clientGameState[clientID];  
  GameState *server = reference;
  return encode(client, server);
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
  //the size of one specific synchronisable
  int tempsize=0;
  // get the start of the Synchronisable list
  orxonox::Iterator<Synchronisable> it;
  // struct for return value of Synchronisable::getData()
  syncData sync;
  
  GameState *retval=new GameState; //return value
  retval->id=id;
  // reserve a little memory and increase it later on
  retval->data = (unsigned char*)malloc(1);
  
  // offset of memory functions
  int offset=0;
  // go through all Synchronisables
  for(it = orxonox::ObjectList<Synchronisable>::start(); it != 0; ++it){
    //get size of the synchronisable
    tempsize=it->getSize();
    // add place for data and 3 ints (length,classid,objectid)
    totalsize+=tempsize+3*sizeof(int);
    // allocate additional space
    retval->data = (unsigned char *)realloc((void *)retval->data, totalsize);
    
    // run Synchronisable::getData with offset and additional place for 3 ints in between (for ids and length)
    sync=it->getData(retval->data+offset+3*sizeof(int));
    *(retval->data+offset)=sync.length;
    *(retval->data+offset+sizeof(int))=sync.objectID;
    *(retval->data+offset+2*sizeof(int))=sync.classID;
    // increase data pointer
    offset+=tempsize+3*sizeof(int);
  }
  retval->size=totalsize;
  return retval;
}



GameStateCompressed GameStateManager::encode(GameState *a, GameState *b){
  GameState r = diff(a,b);
  return compress_(r);
}


GameState GameStateManager::diff(GameState *a, GameState *b){
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
  GameState r = {b->id, dest_length, dp};
  return r;
}

GameStateCompressed GameStateManager::compress_(GameState a) {
  int size = a.size;
  uLongf buffer = (uLongf)((a.size + 12)*1.01)+1;
  unsigned char* dest = (unsigned char*)malloc( buffer );
  int retval;
  retval = compress( dest, &buffer, a.data, (uLong)size );  
  
  switch ( retval ) {
  case Z_OK: std::cout << "successfully compressed" << std::endl; break;
  case Z_MEM_ERROR: std::cout << "not enough memory available" << std::endl; break;
  case Z_BUF_ERROR: std::cout << "not enough memory available in the buffer" << std::endl; break;
  case Z_DATA_ERROR: std::cout << "data corrupted" << std::endl; break;
  }
  
  GameStateCompressed compressedGamestate;
  compressedGamestate.compsize = buffer;
  compressedGamestate.normsize = size;
  compressedGamestate.id = GAMESTATE;
  compressedGamestate.data = dest;
  
  return compressedGamestate;
}

void GameStateManager::ackGameState(int clientID, int gamestateID){
  GameState *old = clientGameState[clientID];
  deleteUnusedGameState(old);
  clientGameState[clientID]=idGameState[gamestateID];
}

bool GameStateManager::deleteUnusedGameState(GameState *state){
  for(unsigned int i=0; i<clientGameState.size(); i++){
    if(clientGameState[i]==state)
      return false;
  }
  delete state;
  return true;
}

}


