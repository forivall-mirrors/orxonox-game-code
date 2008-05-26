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

#include "GameStateClient.h"

#include <zlib.h>

#include "core/CoreIncludes.h"
#include "core/BaseObject.h"
#include "Synchronisable.h"


namespace network
{
  struct GameStateItem{
    GameState *state;
    int id;
  };
  
  GameStateClient::GameStateClient() {
    COUT(5) << "this: " << this << std::endl;
    last_diff_=0;
    last_gamestate_=GAMESTATEID_INITIAL-1;
    tempGameState_=NULL;
    myShip_=NULL;
  }

  GameStateClient::~GameStateClient() {
  }

  bool GameStateClient::pushGameState(GameStateCompressed *compstate) {
    cleanup();
    printGameStateMap();
    GameState *gs, *reference;
    /*if(compstate->id<last_gamestate_){
      // network packets got messed up
      COUT(3) << "received an obsolete gamestate" << std::endl;
      return false;
    }*/
    if(compstate->diffed && compstate->base_id!=GAMESTATEID_INITIAL){
      std::map<int, GameState*>::iterator it = gameStateMap.find(compstate->base_id);
      if(it!=gameStateMap.end())
        reference = (it)->second;
      else
        reference = NULL;
      if(!reference){
        COUT(4) << "pushGameState: no reference found to diff" << std::endl;
        delete[] compstate->data;
        delete compstate;
        return false;
      }
      gs = decode(reference, compstate);
    }
    else
      gs = decode(compstate);
    if(gs){
      if (loadSnapshot(gs)){
        gameStateMap.insert(std::pair<int, GameState*>(gs->id, gs));
        COUT(4) << "adding decoded gs with id: " << gs->id << " diffed from: " << gs->base_id << std::endl;
        last_diff_=gs->base_id;
        //last_gamestate_=gs->id;
        return true;
      }else{
        COUT(4) << "could not decode gs with id: " << gs->id << " diffed from: " << gs->base_id << std::endl;
        delete[] gs->data;
        delete gs;
        return false;
      }
    }
    COUT(4) << "could not use gamestate sent by server" << std::endl;
    return false;
  }
  
  GameStateCompressed *GameStateClient::popPartialGameState(){
    GameState *gs = getPartialSnapshot();
    GameStateCompressed *cgs = compress_(gs);
    delete[] gs->data;
    delete gs;
    return cgs;
  }
  
  void GameStateClient::addGameState(GameStateCompressed *gs){
    if(tempGameState_!=NULL){
      //delete the obsolete gamestate
      if(tempGameState_->id>gs->id)
        return;
      delete[] tempGameState_->data;
      delete tempGameState_;
    }
    tempGameState_=gs;
  }
  int GameStateClient::processGameState(){
    if(tempGameState_==NULL)
      return 0;
    int id=tempGameState_->id;
    bool b = saveShipCache();
    if(pushGameState(tempGameState_)){
      if(b)
        loadShipCache();
      return id;
    }
    else
      return GAMESTATEID_INITIAL;
  }
  

  /**
  * This function removes a Synchronisable out of the universe
  * @param it iterator of the list pointing to the object
  * @return iterator pointing to the next object in the list
  */
  void GameStateClient::removeObject(orxonox::Iterator<Synchronisable> &it) {
    orxonox::Iterator<Synchronisable> temp=it;
    ++it;
    delete  *temp;
  }

  /**
  * This function loads a Snapshort of the gamestate into the universe
  * @param state a GameState struct containing the size of the gamestate and a pointer linking to a flat list (returned by getSnapshot)
  */
  bool GameStateClient::loadSnapshot(GameState *state) {
    unsigned char *data=state->data;
    COUT(4) << "loadSnapshot: loading gs: " << state->id << std::endl;
    // get the start of the Synchronisable list
    orxonox::Iterator<Synchronisable> it=orxonox::ObjectList<Synchronisable>::start();
    syncData sync;
    // loop as long as we have some data ;)
    while(data < state->data+state->size){
      // prepare the syncData struct
      sync.length = *(int *)data;
      data+=sizeof(int);
      sync.objectID = *(int*)data;
      data+=sizeof(int);
      sync.classID = *(int*)data;
      data+=sizeof(int);
      sync.data = data;
      data+=sync.length;

      if(!it || it->objectID!=sync.objectID){
        // bad luck ;)
        // delete the synchronisable (obviously seems to be deleted on the server)
        while(it && it->objectID!=sync.objectID)
          removeObject(it);


        if(!it){
          //COUT(4) << "loadSnapshot:\tclassid: " << sync.classID << ", name: " << ID((unsigned int) sync.classID)->getName() << std::endl;
          ///sigsegv may happen here again for some reason
          ///sigsegv is receved after the COUT(4) above
          orxonox::Identifier* id = ID((unsigned int)sync.classID);
          if(!id){
            COUT(3) << "We could not identify a new object; classid: " << sync.classID << " objectID: " << sync.objectID << " size: " << sync.length << std::endl;
            return false; // most probably the gamestate is corrupted
          }
          Synchronisable *no = dynamic_cast<Synchronisable *>(id->fabricate());
          COUT(4) << "loadsnapshot: classid: " << sync.classID << " objectID: " << sync.objectID << " length: " << sync.length << std::endl;
          if(!no){
            COUT(2) << "coudl not frabricate classid: " << sync.classID << " objectID: " << sync.objectID << " identifier: " << id << std::endl;
            break;
          }
          no->objectID=sync.objectID;
          no->classID=sync.classID;
          // update data and create object/entity...
          if( !no->updateData(sync) ){
            COUT(1) << "We couldn't update the object: " << sync.objectID << std::endl;
            return false;
          }
          if( !no->create() )
            COUT(1) << "We couldn't manifest (create() ) the object: " << sync.objectID << std::endl;
          it=orxonox::ObjectList<Synchronisable>::end();
        }
      } else {
        // we have our object
        if(! it->updateData(sync))
          COUT(1) << "We couldn't update objectID: " \
          << sync.objectID << "; classID: " << sync.classID << std::endl;
      }
      ++it;
    }

    return true;
  }

  GameState *GameStateClient::getPartialSnapshot(){
    //std::cout << "begin getSnapshot" << std::endl;
    //the size of the gamestate
    int totalsize=0;
    int memsize=0;
    //the size of one specific synchronisable
    int tempsize=0;
    // get the start of the Synchronisable list
    orxonox::Iterator<Synchronisable> it;
    // struct for return value of Synchronisable::getData()
    syncData sync;

    GameState *retval=new GameState; //return value
//     retval->id=reference->id;
    if(gameStateMap.size()!=0)
      retval->id=(--gameStateMap.end())->second->id;
    retval->diffed=false;
    retval->complete=false;
    COUT(4) << "G.ST.Client: producing partial gamestate with id: " << retval->id << std::endl;
    // offset of memory functions
    int offset=0, size=0;
    // get total size of gamestate
    for(it = orxonox::ObjectList<Synchronisable>::start(); it; ++it){
      if(!it->getBacksync())
        continue;
      size+=it->getSize(); // size of the actual data of the synchronisable
      size+=3*sizeof(int); // size of datasize, classID and objectID
      COUT(4) << "getpartialsnapshot: size: " << size << std::endl;
    }
    //retval->data = (unsigned char*)malloc(size);
    retval->data = new unsigned char[size];
    if(!retval->data){
      COUT(2) << "GameStateClient: could not allocate memory" << std::endl;
      return NULL;
    }
    memsize=size;
    // go through all Synchronisables
    for(it = orxonox::ObjectList<Synchronisable>::start(); it; ++it){
      if(!it->getBacksync())
        continue;
      //get size of the synchronisable
      tempsize=it->getSize();
      // add place for data and 3 ints (length,classid,objectid)
      totalsize+=tempsize+3*sizeof(int);
      // allocate+tempsize additional space
      if(totalsize > size){
        COUT(3) << "G.St.Cl: need additional memory" << std::endl;
      }

      // run Synchronisable::getData with offset and additional place for 3 ints in between (for ids and length)
      sync=it->getData((retval->data)+offset+3*sizeof(int));
      memcpy(retval->data+offset, (void *)&(sync.length), sizeof(int));
      memcpy(retval->data+offset+sizeof(int), (void *)&(sync.objectID), sizeof(int));
      memcpy(retval->data+offset+2*sizeof(int), (void *)&(sync.classID), sizeof(int));
      // increase data pointer
      offset+=tempsize+3*sizeof(int);
    }
    retval->size=totalsize;
    COUT(5) << "G.ST.Cl: Gamestate size: " << totalsize << std::endl;
    COUT(5) << "G.ST.Cl: 'estimated' Gamestate size: " << size << std::endl;
    return retval;
  }
  
  
  GameState *GameStateClient::undiff(GameState *old, GameState *diff) {
    if(!old || !diff)
      return NULL;
    unsigned char *ap = old->data, *bp = diff->data;
    int of=0; // pointers offset
    int dest_length=0;
    /*if(old->size>=diff->size)
      dest_length=old->size;
    else*/
      dest_length=diff->size;
//     unsigned char *dp = (unsigned char *)malloc(dest_length*sizeof(unsigned char));
    unsigned char *dp = new unsigned char[dest_length*sizeof(unsigned char)];
    while(of<old->size && of<diff->size){
      *(dp+of)=*(ap+of)^*(bp+of); // do the xor
      ++of;
    }
    if(old->size!=diff->size){ // do we have to fill up ?
      unsigned char n=0;
      if(old->size<diff->size){
        while(of<dest_length){
          *(dp+of)=n^*(bp+of);
          of++;
        }
      } /*else{
        while(of<dest_length){
          *(dp+of)=*(ap+of)^n;
          of++;
        }
      }*/
    }
    // should be finished now
    // FIXME: is it true or false now? (struct has changed, producing warnings)
    GameState *r = new GameState;
    r->id = diff->id;
    r->size = dest_length;
    r->base_id = old->id;
    r->diffed = false;
    r->data = dp;
    r->complete = true;
    return r;
  }



  GameStateCompressed *GameStateClient::compress_(GameState *a) {
    if(!a)
      return NULL;
    int size = a->size;

    uLongf buffer = (uLongf)((a->size + 12)*1.01)+1;
    unsigned char *dest = new unsigned char[buffer];
    int retval;
    retval = compress( dest, &buffer, a->data, (uLong)size );

    switch ( retval ) {
      case Z_OK: COUT(5) << "G.St.Cl: compress: successfully compressed" << std::endl; break;
      case Z_MEM_ERROR: COUT(1) << "G.St.Cl: compress: not enough memory available in gamestate.compress" << std::endl; 
      return NULL;
      case Z_BUF_ERROR: COUT(2) << "G.St.Cl: compress: not enough memory available in the buffer in gamestate.compress" << std::endl;
      return NULL;
      case Z_DATA_ERROR: COUT(2) << "G.St.Cl: compress: data corrupted in gamestate.compress" << std::endl;
      return NULL;
    }

    GameStateCompressed *compressedGamestate = new GameStateCompressed;
    compressedGamestate->compsize = buffer;
    compressedGamestate->normsize = size;
    compressedGamestate->id = a->id;
    compressedGamestate->data = dest;
    compressedGamestate->diffed = a->diffed;
    compressedGamestate->complete = a->complete;
    compressedGamestate->base_id = a->base_id;
    return compressedGamestate;
  }
  
  
  GameState *GameStateClient::decompress(GameStateCompressed *a) {
    //COUT(4) << "GameStateClient: uncompressing gamestate. id: " << a->id << ", baseid: " << a->base_id << ", normsize: " << a->normsize << ", compsize: " << a->compsize << std::endl;
    int normsize = a->normsize;
    int compsize = a->compsize;
    int bufsize;
    if(normsize < compsize)
      bufsize = compsize;
    else
      bufsize = normsize;
//     unsigned char* dest = (unsigned char*)malloc( bufsize );
    unsigned char *dest = new unsigned char[bufsize];
    int retval;
    uLongf length=normsize;
    //std::cout << "gamestateclient" << std::endl;
    //std::cout << "normsize " << a.normsize << " compsize " << a.compsize << " " << bufsize << std::endl;
    retval = uncompress( dest, &length, a->data, (uLong)compsize );
    //std::cout << "length " << length << std::endl;
    switch ( retval ) {
      case Z_OK: COUT(4) << "successfully decompressed" << std::endl; break;
      case Z_MEM_ERROR: COUT(1) << "not enough memory available" << std::endl; return NULL;
      case Z_BUF_ERROR: COUT(2) << "not enough memory available in the buffer" << std::endl; return NULL;
      case Z_DATA_ERROR: COUT(2) << "data corrupted (zlib)" << std::endl; return NULL;
    }

    GameState *gamestate = new GameState;
    gamestate->id = a->id;
    gamestate->size = normsize;
    gamestate->data = dest;
    gamestate->base_id = a->base_id;
    gamestate->diffed = a->diffed;
    gamestate->complete = a->complete;


    return gamestate;
  }

  GameState *GameStateClient::decode(GameState *old, GameStateCompressed *diff) {
    COUT(4) << "using diffed gamestate" << std::endl;
    GameState *t = decode(diff);
    if(!t)
      return NULL;
    GameState *r = undiff(old, t);
    delete[] t->data;
    delete t;
    return r;
  }

  GameState *GameStateClient::decode(GameStateCompressed *x) {
    GameState *t = decompress(x);
    delete[] x->data;
    delete x;
    return t;
  }
  
  void GameStateClient::cleanup(){
    std::map<int, GameState*>::iterator temp, it = gameStateMap.begin();
    while(it!=gameStateMap.end()){
      if(it->first>=last_diff_)
        break;
      // otherwise delete that stuff
      delete[] (*it).second->data;
      delete (*it).second;
      temp=it++;
      gameStateMap.erase(temp);
    }
    tempGameState_=NULL;
  }

  void GameStateClient::printGameStateMap(){
    std::map<int, GameState*>::iterator it;
    COUT(4) << "gamestates: ";
    for(it=gameStateMap.begin(); it!=gameStateMap.end(); it++){
      COUT(4) << it->first << ":" << it->second << "|";
    }
    COUT(4) << std::endl;
    
  }
  
  bool GameStateClient::saveShipCache(){
    if(myShip_==NULL)
      myShip_ = orxonox::SpaceShip::getLocalShip();
    if(myShip_){
      //      unsigned char *data = new unsigned char[myShip_->getSize()];
      int size=myShip_->getSize(0x3);
      if(size==0)
        return false;
      unsigned char *data = new unsigned char [size];
      shipCache_ = myShip_->getData(data, 0x1);
      return true;
    }else
      return false;
  }
  
  bool GameStateClient::loadShipCache(){
    if(myShip_){
      myShip_->updateData(shipCache_, 0x2);
      if(shipCache_.data){
        delete[] shipCache_.data;
      }
      return true;
    }else
      return false;
  }
  
    //##### ADDED FOR TESTING PURPOSE #####
  GameState* GameStateClient::testDecompress( GameStateCompressed* gc ) {
    return decompress( gc );
  }
  
  GameState* GameStateClient::testUndiff( GameState* g_old, GameState* g_diffed ) {
    return undiff( g_old, g_diffed );
  }
  //##### ADDED FOR TESTING PURPOSE #####
  
  
}

