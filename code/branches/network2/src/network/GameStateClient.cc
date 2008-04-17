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
 *      ...
 *   Co-authors:
 *      ...
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
  }

  GameStateClient::~GameStateClient() {
  }

  bool GameStateClient::pushGameState(GameStateCompressed *compstate) {
    GameState *gs;
    if(compstate->diffed){
      while(compstate->base_id > gameStateList.front()->id){
        // clean up old gamestates
        free(gameStateList.front()->data);
        // TODO: critical section
        delete gameStateList.front();
        gameStateList.pop();
      }
      if(compstate->base_id!=gameStateList.front()->id){
        COUT(4) << "pushGameState: no reference found to diff" << std::endl;
        return false;
      }
      gs = decode(gameStateList.front(), compstate);
    }
    else
      gs = decode(compstate);
    if(gs)
      return loadSnapshot(gs);
    COUT(4) << "could not use gamestate sent by server" << std::endl;
    return false;
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
      sync.length = (int)*data;
      data+=sizeof(int);
      sync.objectID = (int)*data;
      data+=sizeof(int);
      sync.classID = (int)*data;
      data+=sizeof(int);
      sync.data = data;
      data+=sync.length;

      if(it->objectID!=sync.objectID){
        // bad luck ;)
        // delete the synchronisable (obviously seems to be deleted on the server)
        while(it && it->objectID!=sync.objectID)
          removeObject(it);


        if(!it){
          COUT(4) << "loadSnapshot:\tclassid: " << sync.classID << ", name: " << ID((unsigned int) sync.classID)->getName() << std::endl;
          Synchronisable *no = dynamic_cast<Synchronisable *>(ID((unsigned int) sync.classID)->fabricate());
          no->objectID=sync.objectID;
          no->classID=sync.classID;
          it=orxonox::ObjectList<Synchronisable>::end();
          // update data and create object/entity...
          if( !no->updateData(sync) )
            COUT(1) << "We couldn't update the object: " << sync.objectID << std::endl;
          if( !no->create() )
            COUT(1) << "We couldn't manifest (create() ) the object: " << sync.objectID << std::endl;
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

  GameState *GameStateClient::undiff(GameState *a, GameState *b) {
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
    GameState *r = new GameState;
    r->id = b->id;
    r->size = dest_length;
    r->diffed = false;
    r->data = dp;
    return r;
  }

  //##### ADDED FOR TESTING PURPOSE #####
  GameState* GameStateClient::testDecompress( GameStateCompressed* gc ) {
    return decompress( gc );
  }
  
  GameState* GameStateClient::testUndiff( GameState* g_old, GameState* g_diffed ) {
    return undiff( g_old, g_diffed );
  }
  //##### ADDED FOR TESTING PURPOSE #####

  GameState *GameStateClient::decompress(GameStateCompressed *a) {
    //COUT(4) << "GameStateClient: uncompressing gamestate. id: " << a->id << ", baseid: " << a->base_id << ", normsize: " << a->normsize << ", compsize: " << a->compsize << std::endl;
    int normsize = a->normsize;
    int compsize = a->compsize;
    int bufsize;
    if(normsize < compsize)
      bufsize = compsize;
    else
      bufsize = normsize;
    unsigned char* dest = (unsigned char*)malloc( bufsize );
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
    gamestate->diffed = a->diffed;

    delete a->data; //delete compressed data
    delete a; //we do not need the old (struct) gamestate anymore

    return gamestate;
  }

  GameState *GameStateClient::decode(GameState *a, GameStateCompressed *x) {
    GameState *t = decode(x);
    gameStateList.push(t);
    //return undiff(a, t);
    return t;
  }

  GameState *GameStateClient::decode(GameStateCompressed *x) {
    //GameState *t = decompress(x);
    GameState *t = new GameState;
    t->base_id = x->base_id;
    t->id = x->id;
    t->diffed = x->diffed;
    t->data = x->data;
    t->size = x->normsize;
    gameStateList.push(t);
    return t;
  }

}
