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
#include "../GamestateHandler.h"
#include "../synchronisable/Synchronisable.h"
#include "../TrafficControl.h"
#include "core/Core.h"
#include "core/CoreIncludes.h"
#include "core/Iterator.h"

#include <zlib.h>
#include <cassert>



namespace orxonox {

namespace packet {


#define PACKET_FLAG_GAMESTATE  ENET_PACKET_FLAG_RELIABLE

Gamestate::Gamestate()
{
  flags_ = flags_ | PACKET_FLAG_GAMESTATE;
}

Gamestate::Gamestate(uint8_t *data, unsigned int clientID):
    Packet(data, clientID)
{
  flags_ = flags_ | PACKET_FLAG_GAMESTATE;
}

Gamestate::Gamestate(uint8_t *data)
{
  flags_ = flags_ | PACKET_FLAG_GAMESTATE;
  data_=data;
}


Gamestate::~Gamestate()
{
}

bool Gamestate::collectData(int id, uint8_t mode)
{
  uint32_t tempsize=0, currentsize=0;
  assert(data_==0);
  uint32_t size = calcGamestateSize(id, mode);

  COUT(4) << "G.ST.Man: producing gamestate with id: " << id << std::endl;
  if(size==0)
    return false;
  data_ = new unsigned char[size + sizeof(GamestateHeader)];
  if(!data_){
    COUT(2) << "GameStateManager: could not allocate memory" << std::endl;
    return false;
  }

  //start collect data synchronisable by synchronisable
  uint8_t *mem=data_;
  mem+=sizeof(GamestateHeader);
  ObjectList<Synchronisable>::iterator it;
  for(it = ObjectList<Synchronisable>::begin(); it; ++it){
    
#ifndef NDEBUG
    tempsize=it->getSize(id, mode);
    if(currentsize+tempsize > size){
      assert(0); // if we don't use multithreading this part shouldn't be neccessary
      // start allocate additional memory
      COUT(3) << "G.St.Man: need additional memory" << std::endl;
      ObjectList<Synchronisable>::iterator temp = it;
      uint32_t addsize=tempsize;
      while(++temp)
        addsize+=temp->getSize(id, mode);
      data_ = (uint8_t *)realloc(data_, sizeof(GamestateHeader) + currentsize + addsize);
      if(!data_)
        return false;
      size = currentsize+addsize;
    }// stop allocate additional memory
#endif

    //if(it->doSelection(id))
    if ( it->doSync( id, mode ) )
      dataMap_.push_back( obj(it->getObjectID(), it->getCreatorID(), tempsize, mem-data_) );
//     dataMap_[mem-data_]=(*it);  // save the mem location of the synchronisable data
    if(!it->getData(mem, id, mode))
      return false; // mem pointer gets automatically increased because of call by reference
    // increase size counter by size of current synchronisable
    currentsize+=tempsize;
  }


  //start write gamestate header
  HEADER->packetType = ENUM::Gamestate;
  HEADER->datasize = currentsize;
  HEADER->id = id;
  HEADER->diffed = false;
  HEADER->complete = true;
  HEADER->compressed = false;
  //stop write gamestate header

  COUT(5) << "G.ST.Man: Gamestate size: " << currentsize << std::endl;
  COUT(5) << "G.ST.Man: 'estimated' (and corrected) Gamestate size: " << size << std::endl;
  return true;
}

bool Gamestate::spreadData(uint8_t mode)
{
  assert(data_);
  assert(!HEADER->compressed);
  assert(!HEADER->diffed);
  uint8_t *mem=data_+sizeof(GamestateHeader);
    // get the start of the Synchronisable list
  //ObjectList<Synchronisable>::iterator it=ObjectList<Synchronisable>::begin();
  Synchronisable *s;

  // update the data of the objects we received
  while(mem < data_+sizeof(GamestateHeader)+HEADER->datasize){
    synchronisableHeader *objectheader = (synchronisableHeader*)mem;

    s = Synchronisable::getSynchronisable( objectheader->objectID );
    if(!s)
    {
      if (!Core::isMaster())
        Synchronisable::fabricate(mem, mode);
      else
        mem += objectheader->size;
//         COUT(0) << "could not fabricate synchronisable: " << objectheader->objectID << " classid: " << objectheader->classID << " creator: " << objectheader->creatorID << endl;
//       else
//         COUT(0) << "fabricated: " << objectheader->objectID << " classid: " << objectheader->classID << " creator: "  << objectheader->creatorID << endl;
    }
    else
    {
      bool b = s->updateData(mem, mode);
      assert(b);
    }
  }

   // In debug mode, check first, whether there are no duplicate objectIDs
#ifndef NDEBUG
  ObjectList<Synchronisable>::iterator it;
  for (it = ObjectList<Synchronisable>::begin(); it != ObjectList<Synchronisable>::end(); ++it) {
    if (it->getObjectID() == OBJECTID_UNKNOWN) {
      if (it->objectMode_ != 0x0) {
        COUT(0) << "Found object with OBJECTID_UNKNOWN on the client with objectMode != 0x0!" << std::endl;
        COUT(0) << "Possible reason for this error: Client created a synchronized object without the Server's approval." << std::endl;
        COUT(0) << "Objects class: " << it->getIdentifier()->getName() << std::endl;
        assert(false);
      }
    }
    else {
      ObjectList<Synchronisable>::iterator it2;
      for (it2 = ObjectList<Synchronisable>::begin(); it2 != ObjectList<Synchronisable>::end(); ++it2) {
        if (it->getObjectID() == it2->getObjectID() && *it != *it2) {
           COUT(0) << "Found duplicate objectIDs on the client!" << std::endl
                   << "Are you sure you don't create a Sychnronisable objcect with 'new' \
                       that doesn't have objectMode = 0x0?" << std::endl;
           assert(false);
        }
      }
    }
  }
#endif

  return true;
}

uint32_t Gamestate::getSize() const
{
  assert(data_);
  if(HEADER->compressed)
    return HEADER->compsize+sizeof(GamestateHeader);
  else
  {
    return HEADER->datasize+sizeof(GamestateHeader);
  }
}

bool Gamestate::operator==(packet::Gamestate gs){
  uint8_t *d1 = data_+sizeof(GamestateHeader);
  uint8_t *d2 = gs.data_+sizeof(GamestateHeader);
  assert(!isCompressed());
  assert(!gs.isCompressed());
  while(d1<data_+HEADER->datasize)
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
  uLongf buffer = (uLongf)(((HEADER->datasize + 12)*1.01)+1);
  if(buffer==0)
    return false;

  uint8_t *ndata = new uint8_t[buffer+sizeof(GamestateHeader)];
  uint8_t *dest = GAMESTATE_START(ndata);
  //unsigned char *dest = new unsigned char[buffer];
  uint8_t *source = GAMESTATE_START(data_);
  int retval;
  retval = compress( dest, &buffer, source, (uLong)(HEADER->datasize) );
  switch ( retval ) {
    case Z_OK: COUT(5) << "G.St.Man: compress: successfully compressed" << std::endl; break;
    case Z_MEM_ERROR: COUT(1) << "G.St.Man: compress: not enough memory available in gamestate.compress" << std::endl; return false;
    case Z_BUF_ERROR: COUT(2) << "G.St.Man: compress: not enough memory available in the buffer in gamestate.compress" << std::endl; return false;
    case Z_DATA_ERROR: COUT(2) << "G.St.Man: compress: data corrupted in gamestate.compress" << std::endl; return false;
  }

  //copy and modify header
  *GAMESTATE_HEADER(ndata) = *HEADER;
  //delete old data
  delete[] data_;
  //save new data
  data_ = ndata;
  HEADER->compsize = buffer;
  HEADER->compressed = true;
  assert(HEADER->compressed);
  COUT(4) << "gamestate compress datasize: " << HEADER->datasize << " compsize: " << HEADER->compsize << std::endl;
  return true;
}
bool Gamestate::decompressData()
{
  assert(HEADER);
  assert(HEADER->compressed);
  COUT(4) << "GameStateClient: uncompressing gamestate. id: " << HEADER->id << ", baseid: " << HEADER->base_id << ", datasize: " << HEADER->datasize << ", compsize: " << HEADER->compsize << std::endl;
  uint32_t datasize = HEADER->datasize;
  uint32_t compsize = HEADER->compsize;
  uint32_t bufsize;
//  assert(compsize<=datasize);
  bufsize = datasize;
  assert(bufsize!=0);
  uint8_t *ndata = new uint8_t[bufsize + sizeof(GamestateHeader)];
  uint8_t *dest = ndata + sizeof(GamestateHeader);
  uint8_t *source = data_ + sizeof(GamestateHeader);
  int retval;
  uLongf length=bufsize;
  retval = uncompress( dest, &length, source, (uLong)compsize );
  switch ( retval ) {
    case Z_OK: COUT(5) << "successfully decompressed" << std::endl; break;
    case Z_MEM_ERROR: COUT(1) << "not enough memory available" << std::endl; return false;
    case Z_BUF_ERROR: COUT(2) << "not enough memory available in the buffer" << std::endl; return false;
    case Z_DATA_ERROR: COUT(2) << "data corrupted (zlib)" << std::endl; return false;
  }

  //copy over the header
  *GAMESTATE_HEADER(ndata) = *HEADER;

  if (this->bDataENetAllocated_){
    // Memory was allocated by ENet. --> We let it be since enet_packet_destroy will
    // deallocated it anyway. So data and packet stay together.
    this->bDataENetAllocated_ = false;
  }
  else{
    // We allocated the memory in the first place (unlikely). So we destroy the old data
    // and overwrite it with the new decompressed data.
    delete[] this->data_;
  }

  //set new pointers
  data_ = ndata;
  HEADER->compressed = false;
  assert(HEADER->datasize==datasize);
  assert(HEADER->compsize==compsize);
  return true;
}

Gamestate *Gamestate::diff(Gamestate *base)
{
  assert(HEADER);
  assert(!HEADER->compressed);
  assert(!HEADER->diffed);
  //unsigned char *basep = base->getGs()/*, *gs = getGs()*/;
  uint8_t *basep = GAMESTATE_START(base->data_), *gs = GAMESTATE_START(this->data_);
  uint32_t of=0; // pointers offset
  uint32_t dest_length=0;
  dest_length=HEADER->datasize;
  if(dest_length==0)
    return NULL;
  uint8_t *ndata = new uint8_t[dest_length*sizeof(uint8_t)+sizeof(GamestateHeader)];
  uint8_t *dest = ndata + sizeof(GamestateHeader);
  while(of < GAMESTATE_HEADER(base->data_)->datasize && of < HEADER->datasize){
    *(dest+of)=*(basep+of)^*(gs+of); // do the xor
    ++of;
  }
  if(GAMESTATE_HEADER(base->data_)->datasize!=HEADER->datasize){
    uint8_t n=0;
    if(GAMESTATE_HEADER(base->data_)->datasize < HEADER->datasize){
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

Gamestate* Gamestate::doSelection(unsigned int clientID, unsigned int targetSize){
  assert(data_);
  std::list<obj>::iterator it;

  // allocate memory for new data
  uint8_t *gdata = new uint8_t[HEADER->datasize+sizeof(GamestateHeader)];
  // create a gamestate out of it
  Gamestate *gs = new Gamestate(gdata);
  uint8_t *newdata = gdata + sizeof(GamestateHeader);
  uint8_t *origdata = GAMESTATE_START(data_);

  //copy the GamestateHeader
  *(GamestateHeader*)gdata = *HEADER;

  synchronisableHeader *oldobjectheader, *newobjectheader;
  uint32_t objectOffset;
  unsigned int objectsize, destsize=0;
  // TODO: Why is this variable not used?
  //Synchronisable *object;

  //call TrafficControl
  TrafficControl::getInstance()->processObjectList( clientID, HEADER->id, &dataMap_ );

  //copy in the zeros
  for(it=dataMap_.begin(); it!=dataMap_.end();){
//    if((*it).objSize==0)
//      continue;
//    if(it->second->getSize(HEADER->id)==0) // merged from objecthierarchy2, doesn't work anymore; TODO: change this
//      continue;                            // merged from objecthierarchy2, doesn't work anymore; TODO: change this
    oldobjectheader = (synchronisableHeader*)origdata;
    newobjectheader = (synchronisableHeader*)newdata;
    if ( (*it).objSize == 0 )
    {
      ++it;
      continue;
    }
//     object = Synchronisable::getSynchronisable( (*it).objID );
//     assert(object->objectID == oldobjectheader->objectID);
    objectsize = oldobjectheader->size;
    objectOffset=sizeof(synchronisableHeader); //skip the size and the availableData variables in the objectheader
    if ( (*it).objID == oldobjectheader->objectID ){
      memcpy(newdata, origdata, objectsize);
      assert(newobjectheader->dataAvailable==true);
      ++it;
    }else{
      *newobjectheader = *oldobjectheader;
      newobjectheader->dataAvailable=false;
      memset(newdata+objectOffset, 0, objectsize-objectOffset);
    }
    newdata += objectsize;
    origdata += objectsize;
    destsize += objectsize;
  }
#ifndef NDEBUG
  uint32_t origsize = destsize;
  while ( origsize < HEADER->datasize )
  {
    oldobjectheader = (synchronisableHeader*)origdata;
    objectsize = oldobjectheader->size;
    origdata += objectsize;
    origsize += objectsize;
  }
  assert(origsize==HEADER->datasize);
  assert(destsize!=0);
#endif
  ((GamestateHeader*)gdata)->datasize = destsize;
  return gs;
}


Gamestate *Gamestate::undiff(Gamestate *base)
{
  assert(this && base);assert(HEADER);
  assert(HEADER->diffed);
  assert(!HEADER->compressed && !GAMESTATE_HEADER(base->data_)->compressed);
  //unsigned char *basep = base->getGs()/*, *gs = getGs()*/;
  uint8_t *basep = GAMESTATE_START(base->data_);
  uint8_t *gs = GAMESTATE_START(this->data_);
  uint32_t of=0; // pointers offset
  uint32_t dest_length=0;
  dest_length=HEADER->datasize;
  if(dest_length==0)
    return NULL;
  uint8_t *ndata = new uint8_t[dest_length*sizeof(uint8_t)+sizeof(GamestateHeader)];
  uint8_t *dest = ndata + sizeof(GamestateHeader);
  while(of < GAMESTATE_HEADER(base->data_)->datasize && of < HEADER->datasize){
    *(dest+of)=*(basep+of)^*(gs+of); // do the xor
    ++of;
  }
  if(GAMESTATE_HEADER(base->data_)->datasize!=HEADER->datasize){
    uint8_t n=0;
    if(GAMESTATE_HEADER(base->data_)->datasize < HEADER->datasize){
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


uint32_t Gamestate::calcGamestateSize(int32_t id, uint8_t mode)
{
  uint32_t size=0;
    // get the start of the Synchronisable list
  ObjectList<Synchronisable>::iterator it;
    // get total size of gamestate
  for(it = ObjectList<Synchronisable>::begin(); it; ++it)
    size+=it->getSize(id, mode); // size of the actual data of the synchronisable
  return size;
}

} //namespace packet

} //namespace orxonox
