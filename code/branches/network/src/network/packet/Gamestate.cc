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
 *      Oliver Scheuss, (C) 2008
 *   Co-authors:
 *      ...
 *
 */

#include "Gamestate.h"
#include "network/ClientInformation.h"
#include "network/GamestateHandler.h"

#include <zlib.h>
#include <assert.h>



namespace network {

namespace packet {
  

#define GAMESTATE_START(data) (data + sizeof(GamestateHeader))
#define GAMESTATE_HEADER(data) ((GamestateHeader *)data)
#define HEADER GAMESTATE_HEADER(data_)
  
Gamestate::Gamestate()
{
}

Gamestate::Gamestate(unsigned char *data, int clientID):
    Packet(data, clientID)
{
}


Gamestate::~Gamestate()
{
}

bool Gamestate::collectData(int id, int mode)
{
  int tempsize=0, currentsize=0;
  assert(data_==0);
  int size = calcGamestateSize(id, mode);
  
  COUT(4) << "G.ST.Man: producing gamestate with id: " << id << std::endl;
  if(size==0)
    return false;
  data_ = new unsigned char[size + sizeof(GamestateHeader)];
  if(!data_){
    COUT(2) << "GameStateManager: could not allocate memory" << std::endl;
    return false;
  }

  //start collect data synchronisable by synchronisable
  unsigned char *mem=data_;
  mem+=sizeof(GamestateHeader);
  orxonox::Iterator<Synchronisable> it;
  for(it = orxonox::ObjectList<Synchronisable>::start(); it; ++it){
    tempsize=it->getSize2(id, mode);
    
    if(currentsize+tempsize > size){
      // start allocate additional memory
      COUT(3) << "G.St.Man: need additional memory" << std::endl;
      orxonox::Iterator<Synchronisable> temp = it;
      int addsize=tempsize;
      while(++temp)
        addsize+=temp->getSize2(id, mode);
      data_ = (unsigned char *)realloc(data_, sizeof(GamestateHeader) + currentsize + addsize);
      if(!data_)
        return false;
      size = currentsize+addsize;
    }// stop allocate additional memory

    if(!it->getData(mem, id, mode))
      return false; // mem pointer gets automatically increased because of call by reference
    // increase size counter by size of current synchronisable
    currentsize+=tempsize;
  }
  
  
  //start write gamestate header
  HEADER->packetType = ENUM::Gamestate;
  assert( *(ENUM::Type *)(data_) == ENUM::Gamestate); 
  HEADER->normsize = currentsize;
  HEADER->id = id;
  HEADER->diffed = false;
  HEADER->complete = true;
  HEADER->compressed = false;
  //stop write gamestate header
  
  COUT(5) << "G.ST.Man: Gamestate size: " << currentsize << std::endl;
  COUT(5) << "G.ST.Man: 'estimated' (and corrected) Gamestate size: " << size << std::endl;
  return true;
}

bool Gamestate::spreadData(int mode)
{
  assert(data_);
  assert(!HEADER->compressed);
  assert(!HEADER->diffed);
  unsigned int size, objectID, classID;
  unsigned char *mem=data_+sizeof(GamestateHeader);
    // get the start of the Synchronisable list
  orxonox::Iterator<Synchronisable> it=orxonox::ObjectList<Synchronisable>::start();
  
  while(mem < data_+sizeof(GamestateHeader)+HEADER->normsize){
      // extract synchronisable header
    size = *(unsigned int *)mem;
    objectID = *(unsigned int*)(mem+sizeof(unsigned int));
    classID = *(unsigned int*)(mem+2*sizeof(unsigned int));

    if(!it || it->objectID!=objectID || it->classID!=classID){
        // bad luck ;)
        // delete the synchronisable (obviously seems to be deleted on the server)
      while(it && it->objectID!=objectID)
        removeObject(it);

      if(!it){
        //fabricate the new synchronisable
        if(!Synchronisable::fabricate(mem, mode))
          return false;
        it=orxonox::ObjectList<Synchronisable>::end();
      }
    } else 
    {
        // we have our object
      if(! it->updateData(mem, mode))
      {
        COUT(1) << "We couldn't update objectID: " \
            << objectID << "; classID: " << classID << std::endl;
      }
    }
    ++it;
  }

  return true;
}

int Gamestate::getID(){
  return HEADER->id;
}

unsigned int Gamestate::getSize() const
{
  assert(data_);
  if(HEADER->compressed)
    return HEADER->compsize+sizeof(GamestateHeader);
  else
  {
    return HEADER->normsize+sizeof(GamestateHeader);
  }
}

bool Gamestate::operator==(packet::Gamestate gs){
  unsigned char *d1 = data_+sizeof(GamestateHeader);
  unsigned char *d2 = gs.data_+sizeof(GamestateHeader);
  assert(!isCompressed());
  assert(!gs.isCompressed());
  while(d1<data_+HEADER->normsize)
  {
    if(*d1!=*d2)
      return false;
    d1++;
    d2++;
  }
  return true;
}

bool Gamestate::process()
{
  return GamestateHandler::addGamestate(this, getClientID());
}

bool Gamestate::compressData()
{
  assert(HEADER);
  assert(!HEADER->compressed);
  uLongf buffer = (uLongf)(((HEADER->normsize + 12)*1.01)+1);
  if(buffer==0)
    return false;
  
  unsigned char *ndata = new unsigned char[buffer+sizeof(GamestateHeader)];
  unsigned char *dest = GAMESTATE_START(ndata);
  //unsigned char *dest = new unsigned char[buffer];
  unsigned char *source = GAMESTATE_START(data_);
  int retval;
  retval = compress( dest, &buffer, source, (uLong)(HEADER->normsize) );
  switch ( retval ) {
    case Z_OK: COUT(5) << "G.St.Man: compress: successfully compressed" << std::endl; break;
    case Z_MEM_ERROR: COUT(1) << "G.St.Man: compress: not enough memory available in gamestate.compress" << std::endl; return false;
    case Z_BUF_ERROR: COUT(2) << "G.St.Man: compress: not enough memory available in the buffer in gamestate.compress" << std::endl; return false;
    case Z_DATA_ERROR: COUT(2) << "G.St.Man: compress: data corrupted in gamestate.compress" << std::endl; return false;
  }
#ifndef NDEBUG
  //decompress and compare the start and the decompressed data
  unsigned char *rdata = new unsigned char[HEADER->normsize+sizeof(GamestateHeader)];
  unsigned char *d2 = GAMESTATE_START(rdata);
  uLongf length2 = HEADER->normsize;
  uncompress(d2, &length2, dest, buffer);
  for(unsigned int i=0; i<HEADER->normsize; i++){
    assert(*(source+i)==*(d2+i));
  }
  delete[] rdata;
#endif

  //copy and modify header
#ifndef NDEBUG
  HEADER->crc32 = calcCRC(data_+sizeof(GamestateHeader), HEADER->normsize);
#endif
  *GAMESTATE_HEADER(ndata) = *HEADER;
  //delete old data
  delete[] data_;
  //save new data
  data_ = ndata;
  HEADER->compsize = buffer;
  HEADER->compressed = true;
  assert(HEADER->compressed);
  COUT(3) << "gamestate compress normsize: " << HEADER->normsize << " compsize: " << HEADER->compsize << std::endl;
  return true;
}
bool Gamestate::decompressData()
{
  assert(HEADER);
  assert(HEADER->compressed);
  COUT(3) << "GameStateClient: uncompressing gamestate. id: " << HEADER->id << ", baseid: " << HEADER->base_id << ", normsize: " << HEADER->normsize << ", compsize: " << HEADER->compsize << std::endl;
  unsigned int normsize = HEADER->normsize;
  unsigned int compsize = HEADER->compsize;
  unsigned int bufsize;
  assert(compsize<=normsize);
  bufsize = normsize;
  assert(bufsize!=0);
  unsigned char *ndata = new unsigned char[bufsize + sizeof(GamestateHeader)];
  unsigned char *dest = ndata + sizeof(GamestateHeader);
  unsigned char *source = data_ + sizeof(GamestateHeader);
  int retval;
  uLongf length=bufsize;
  retval = uncompress( dest, &length, source, (uLong)compsize );
  switch ( retval ) {
    case Z_OK: COUT(5) << "successfully decompressed" << std::endl; break;
    case Z_MEM_ERROR: COUT(1) << "not enough memory available" << std::endl; return false;
    case Z_BUF_ERROR: COUT(2) << "not enough memory available in the buffer" << std::endl; return false;
    case Z_DATA_ERROR: COUT(2) << "data corrupted (zlib)" << std::endl; return false;
  }
#ifndef NDEBUG
  assert(HEADER->crc32==calcCRC(ndata+sizeof(GamestateHeader), HEADER->normsize));
#endif
  
  //copy over the header
  *GAMESTATE_HEADER(ndata) = *HEADER;
  //delete old (compressed data)
  delete[] data_;
  //set new pointers
  data_ = ndata;
  HEADER->compressed = false;
  assert(HEADER->normsize==normsize);
  assert(HEADER->compsize==compsize);
  return true;
}

Gamestate *Gamestate::diff(Gamestate *base)
{
  assert(HEADER);
  assert(!HEADER->compressed);
  assert(!HEADER->diffed);
  //unsigned char *basep = base->getGs()/*, *gs = getGs()*/;
  unsigned char *basep = GAMESTATE_START(base->data_), *gs = GAMESTATE_START(this->data_);
  unsigned int of=0; // pointers offset
  unsigned int dest_length=0;
  dest_length=HEADER->normsize;
  if(dest_length==0)
    return NULL;
  unsigned char *ndata = new unsigned char[dest_length*sizeof(unsigned char)+sizeof(GamestateHeader)];
  unsigned char *dest = ndata + sizeof(GamestateHeader);
  while(of < GAMESTATE_HEADER(base->data_)->normsize && of < HEADER->normsize){
    *(dest+of)=*(basep+of)^*(gs+of); // do the xor
    ++of;
  }
  if(GAMESTATE_HEADER(base->data_)->normsize!=HEADER->normsize){
    unsigned char n=0;
    if(GAMESTATE_HEADER(base->data_)->normsize < HEADER->normsize){
      while(of<dest_length){
        *(dest+of)=n^*(gs+of);
        of++;
      }
    }
  }

  *GAMESTATE_HEADER(ndata) = *HEADER;
  GAMESTATE_HEADER(ndata)->diffed = true;
  GAMESTATE_HEADER(ndata)->base_id = base->getID();
  Gamestate *g = new Gamestate(ndata, getClientID());
  g->flags_=flags_;
  g->packetDirection_ = packetDirection_;
  return g;
}

Gamestate *Gamestate::undiff(Gamestate *base)
{
  assert(this && base);assert(HEADER);
  assert(HEADER->diffed);
  assert(!HEADER->compressed && !GAMESTATE_HEADER(base->data_)->compressed);
  //unsigned char *basep = base->getGs()/*, *gs = getGs()*/;
  unsigned char *basep = GAMESTATE_START(base->data_);
  unsigned char *gs = GAMESTATE_START(this->data_);
  unsigned int of=0; // pointers offset
  unsigned int dest_length=0;
  dest_length=HEADER->normsize;
  if(dest_length==0)
    return NULL;
  unsigned char *ndata = new unsigned char[dest_length*sizeof(unsigned char)+sizeof(GamestateHeader)];
  unsigned char *dest = ndata + sizeof(GamestateHeader);
  while(of < GAMESTATE_HEADER(base->data_)->normsize && of < HEADER->normsize){
    *(dest+of)=*(basep+of)^*(gs+of); // do the xor
    ++of;
  }
  if(GAMESTATE_HEADER(base->data_)->normsize!=HEADER->normsize){
    unsigned char n=0;
    if(GAMESTATE_HEADER(base->data_)->normsize < HEADER->normsize){
      while(of < dest_length){
        *(dest+of)=n^*(gs+of);
        of++;
      }
    }
  }
  *GAMESTATE_HEADER(ndata) = *HEADER;
  GAMESTATE_HEADER(ndata)->diffed = false;
  Gamestate *g = new Gamestate(ndata, getClientID());
  g->flags_=flags_;
  g->packetDirection_ = packetDirection_;
  assert(!g->isDiffed());
  assert(!g->isCompressed());
  return g;
}


unsigned int Gamestate::calcGamestateSize(unsigned int id, int mode)
{
  int size=0;
    // get the start of the Synchronisable list
  orxonox::Iterator<Synchronisable> it;
    // get total size of gamestate
  for(it = orxonox::ObjectList<Synchronisable>::start(); it; ++it)
    size+=it->getSize2(id, mode); // size of the actual data of the synchronisable
//  size+=sizeof(GamestateHeader);
  return size;
}

/**
 * This function removes a Synchronisable out of the universe
 * @param it iterator of the list pointing to the object
 * @return iterator pointing to the next object in the list
 */
  void Gamestate::removeObject(orxonox::Iterator<Synchronisable> &it) {
    orxonox::Iterator<Synchronisable> temp=it;
    ++it;
    delete  *temp;
  }

  bool Gamestate::isDiffed(){
    return HEADER->diffed;
  }
  
  bool Gamestate::isCompressed(){
    return HEADER->compressed;
  }
  
  int Gamestate::getBaseID(){
    return HEADER->base_id;
  }
}

}
