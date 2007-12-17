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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#include "GameStateClient.h"

namespace network {

GameStateClient::GameStateClient()
{
}


GameStateClient::~GameStateClient()
{
}

bool GameStateClient::pushGameState(GameStateCompressed compstate){
  if(compstate.diffed)
    return loadSnapshot(decode(reference, compstate));
  else
    return loadSnapshot(decode(compstate));
}


/**
 * This function removes a Synchronisable out of the universe
 * @param it iterator of the list pointing to the object
 * @return iterator pointing to the next object in the list
 */
void GameStateClient::removeObject(orxonox::Iterator<Synchronisable> &it){
  orxonox::Iterator<Synchronisable> temp=it;
  ++it;
  delete  *temp;
}

/**
 * This function loads a Snapshort of the gamestate into the universe
 * @param state a GameState struct containing the size of the gamestate and a pointer linking to a flat list (returned by getSnapshot)
 */
bool GameStateClient::loadSnapshot(GameState state)
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
      if(it==0){
        orxonox::BaseObject *no = ID(sync.classID)->fabricate();
        ((Synchronisable *)no)->objectID=sync.objectID;
        ((Synchronisable *)no)->classID=sync.classID;
        it=orxonox::ObjectList<Synchronisable>::end();
        // update data and create object/entity...
        if( !(((Synchronisable *)no)->updateData(sync)) && !(((Synchronisable *)no)->create()) )
          COUT(0) << "We couldn't create/update the object: " << sync.objectID << std::endl;
        ++it;
      }
    } else {
      // we have our object
      if(! it->updateData(sync))
        std::cout << "We couldn't update objectID: " \
            << sync.objectID << "; classID: " << sync.classID << std::endl;
    }
    ++it;
  }

  return true;
}

GameState GameStateClient::diff(GameState a, GameState b){
  unsigned char *ap = a.data, *bp = b.data;
  int of=0; // pointers offset
  int dest_length=0;
  if(a.size>=b.size)
    dest_length=a.size;
  else
    dest_length=b.size;
  unsigned char *dp = (unsigned char *)malloc(dest_length*sizeof(unsigned char));
  while(of<a.size && of<b.size){
    *(dp+of)=*(ap+of)^*(bp+of); // do the xor
    ++of;
  }
  if(a.size!=b.size){ // do we have to fill up ?
    unsigned char n=0;
    if(a.size<b.size){
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
  GameState r = {b.id, dest_length, dp};
  return r;
}


GameState GameStateClient::decompress(GameStateCompressed a){
  int normsize = a.normsize;
  int compsize = a.compsize;
  unsigned char* dest = (unsigned char*)malloc( normsize );
  int retval;
  uLongf length=normsize;
  retval = uncompress( dest, &length, a.data, (uLong)compsize );

  switch ( retval ) {
    case Z_OK: std::cout << "successfully compressed" << std::endl; break;
    case Z_MEM_ERROR: std::cout << "not enough memory available" << std::endl; break;
    case Z_BUF_ERROR: std::cout << "not enough memory available in the buffer" << std::endl; break;
    case Z_DATA_ERROR: std::cout << "data corrupted" << std::endl; break;
  }

  GameState gamestate;
  gamestate.id = a.id;
  gamestate.size = normsize;
  gamestate.data = dest;
  gamestate.diffed = a.diffed;

  return gamestate;
}


GameState GameStateClient::decode(GameState a, GameStateCompressed x){
  GameState t = decompress(x);
  return diff(a, t);
}

GameState GameStateClient::decode(GameStateCompressed x){
  GameState t = decompress(x);
  return t;
}



}
