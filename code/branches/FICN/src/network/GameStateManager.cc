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
}

GameStateManager::~GameStateManager()
{
}

/**
 * This function goes through the whole list of synchronisables and 
 * saves all the synchronisables to a flat "list".
 * @return struct of type gamestate containing the size of the whole gamestate and a pointer linking to the flat list
 */
GameState GameStateManager::getSnapshot(int id)
{
  //the size of the gamestate
  int totalsize=0;
  //the size of one specific synchronisable
  int tempsize=0;
  // get the start of the Synchronisable list
  orxonox::Iterator<Synchronisable> it;
  // struct for return value of Synchronisable::getData()
  syncData sync;
  
  GameState retval; //return value
  retval.id=id;
  // reserve a little memory and increase it later on
  retval.data = (unsigned char*)malloc(1);
  
  // offset of memory functions
  int offset=0;
  // go through all Synchronisables
  for(it = orxonox::ObjectList<Synchronisable>::start(); it != 0; ++it){
    //get size of the synchronisable
    tempsize=it->getSize();
    // add place for data and 3 ints (length,classid,objectid)
    totalsize+=tempsize+3*sizeof(int);
    // allocate additional space
    retval.data = (unsigned char *)realloc((void *)retval.data, totalsize);
    
    // run Synchronisable::getData with offset and additional place for 3 ints in between (for ids and length)
    sync=it->getData(retval.data+offset+3*sizeof(int));
    *(retval.data+offset)=sync.length;
    *(retval.data+offset+sizeof(int))=sync.objectID;
    *(retval.data+offset+2*sizeof(int))=sync.classID;
    // increase data pointer
    offset+=tempsize+3*sizeof(int);
  }
  retval.size=totalsize;
  return retval;
}

/**
 * This function loads a Snapshort of the gamestate into the universe
 * @param state a GameState struct containing the size of the gamestate and a pointer linking to a flat list (returned by getSnapshot)
 */
bool GameStateManager::loadSnapshot(GameState state)
{
  unsigned char *data=state.data;
  // get the start of the Synchronisable list
  orxonox::Iterator<Synchronisable> it=orxonox::ObjectList<Synchronisable>::start();
  syncData sync;
  // loop as long as we have some data ;)
  while(data < state.data+state.size){
    // prepare the syncData struct
    sync.length = *(int *)data;
    data+=sizeof(int);
    sync.objectID = *(int *)data;
    data+=sizeof(int);
    sync.classID = *(int *)data;
    data+=sizeof(int);
    sync.data = data;
    data+=sync.length;
    
    if(it->objectID!=sync.objectID){
      // bad luck ;)
      // delete the synchronisable (obviously seems to be deleted on the server)
      while(it != 0 && it->objectID!=sync.objectID){
        removeObject(it);
      }
      if(it==0){  // add the new object
        // =================== factory command to add object
        // can we be sure the object really was added?
        it=orxonox::ObjectList<Synchronisable>::end();
        it->objectID=sync.objectID;
        it->classID=sync.classID;
      }
    } else {
      // we have our object
      if(! it->updateData(sync))
        std::cout << "We couldn't update objectID: " \
          << sync.objectID << "; classID: " << sync.classID << std::endl;
    }
    
  }
  
  return true;
}

/**
 * This function removes a Synchronisable out of the universe
 * @param it iterator of the list pointing to the object
 * @return iterator pointing to the next object in the list
 */
// orxonox::Iterator<Synchronisable> removeObject(orxonox::Iterator<Synchronisable> it){
void GameStateManager::removeObject(orxonox::Iterator<Synchronisable> &it){
  orxonox::Iterator<Synchronisable> temp=it;
  ++it;
  delete  *temp;
//   return it;
}


}


