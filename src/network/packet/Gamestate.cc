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

#define GAMESTATE_START(data) (data + GamestateHeader::getSize())

#define PACKET_FLAG_GAMESTATE  ENET_PACKET_FLAG_RELIABLE

// Gamestate::Gamestate()
// {
//   flags_ = flags_ | PACKET_FLAG_GAMESTATE;
// }

Gamestate::Gamestate()
{
  flags_ = flags_ | PACKET_FLAG_GAMESTATE;
  header_ = 0;
}

Gamestate::Gamestate(uint8_t *data, unsigned int clientID):
    Packet(data, clientID)
{
  flags_ = flags_ | PACKET_FLAG_GAMESTATE;
  header_ = new GamestateHeader(data_);
}

Gamestate::Gamestate(uint8_t *data)
{
  flags_ = flags_ | PACKET_FLAG_GAMESTATE;
  data_=data;
  header_ = new GamestateHeader(data_);
}

Gamestate::Gamestate(const Gamestate& g) :
    Packet( *(Packet*)&g )
{
  flags_ = flags_ | PACKET_FLAG_GAMESTATE;
  header_ = new GamestateHeader(data_);
}


Gamestate::~Gamestate()
{
}

bool Gamestate::collectData(int id, uint8_t mode)
{
  assert(this->header_==0); // make sure the header didn't exist before
  uint32_t tempsize=0, currentsize=0;
  assert(data_==0);
  uint32_t size = calcGamestateSize(id, mode);

  COUT(4) << "G.ST.Man: producing gamestate with id: " << id << std::endl;
  if(size==0)
    return false;
  data_ = new uint8_t[size + GamestateHeader::getSize()];
  if(!data_){
    COUT(2) << "GameStateManager: could not allocate memory" << std::endl;
    return false;
  }
  
  // create the header object
  header_ = new GamestateHeader(data_);

  //start collect data synchronisable by synchronisable
  uint8_t *mem=data_;
  mem += GamestateHeader::getSize();
  ObjectList<Synchronisable>::iterator it;
  for(it = ObjectList<Synchronisable>::begin(); it; ++it){
    
    tempsize=it->getSize(id, mode);
#ifndef NDEBUG
    if(currentsize+tempsize > size){
      assert(0); // if we don't use multithreading this part shouldn't be neccessary
      // start allocate additional memory
      COUT(3) << "G.St.Man: need additional memory" << std::endl;
      ObjectList<Synchronisable>::iterator temp = it;
      uint32_t addsize=tempsize;
      while(++temp)
        addsize+=temp->getSize(id, mode);
      data_ = (uint8_t *)realloc(data_, GamestateHeader::getSize() + currentsize + addsize);
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
  header_->setDataSize( currentsize );
  header_->setID( id );
  header_->setDiffed( false );
  header_->setComplete( true );
  header_->setCompressed( false );
  //stop write gamestate header

  COUT(5) << "G.ST.Man: Gamestate size: " << currentsize << std::endl;
  COUT(5) << "G.ST.Man: 'estimated' (and corrected) Gamestate size: " << size << std::endl;
  return true;
}

bool Gamestate::spreadData(uint8_t mode)
{
  COUT(4) << "processing gamestate with id " << header_->getID() << endl;
  assert(data_);
  assert(!header_->isCompressed());
  assert(!header_->isDiffed());
  uint8_t *mem=data_+GamestateHeader::getSize();
    // get the start of the Synchronisable list
  //ObjectList<Synchronisable>::iterator it=ObjectList<Synchronisable>::begin();
  Synchronisable *s;

  // update the data of the objects we received
  while(mem < data_+GamestateHeader::getSize()+header_->getDataSize()){
    SynchronisableHeader objectheader(mem);

    s = Synchronisable::getSynchronisable( objectheader.getObjectID() );
    if(!s)
    {
      if (!Core::isMaster())
      {
        Synchronisable::fabricate(mem, mode);
      }
      else
      {
        mem += objectheader.getDataSize();
      }
//         COUT(0) << "could not fabricate synchronisable: " << objectheader->objectID << " classid: " << objectheader->classID << " creator: " << objectheader->creatorID << endl;
//       else
//         COUT(0) << "fabricated: " << objectheader->objectID << " classid: " << objectheader->classID << " creator: "  << objectheader->creatorID << endl;
    }
    else
    {
      bool b = s->updateData(mem, mode);
//      if(!b)
//        COUT(0) << "data could not be updated" << endl;
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
  if(header_->isCompressed())
    return header_->getCompSize()+GamestateHeader::getSize();
  else
  {
    return header_->getDataSize()+GamestateHeader::getSize();
  }
}

bool Gamestate::operator==(packet::Gamestate gs){
  uint8_t *d1 = data_+GamestateHeader::getSize();
  uint8_t *d2 = gs.data_+GamestateHeader::getSize();
  assert(!isCompressed());
  assert(!gs.isCompressed());
  while(d1<data_+header_->getDataSize())
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
  assert(data_);
  assert(!header_->isCompressed());
  uLongf buffer = (uLongf)(((header_->getDataSize() + 12)*1.01)+1);
  if(buffer==0)
    return false;

  uint8_t *ndata = new uint8_t[buffer+GamestateHeader::getSize()];
  uint8_t *dest = ndata + GamestateHeader::getSize();
  //unsigned char *dest = new unsigned char[buffer];
  uint8_t *source = data_ + GamestateHeader::getSize();
  int retval;
  retval = compress( dest, &buffer, source, (uLong)(header_->getDataSize()) );
  switch ( retval ) {
    case Z_OK: COUT(5) << "G.St.Man: compress: successfully compressed" << std::endl; break;
    case Z_MEM_ERROR: COUT(1) << "G.St.Man: compress: not enough memory available in gamestate.compress" << std::endl; return false;
    case Z_BUF_ERROR: COUT(2) << "G.St.Man: compress: not enough memory available in the buffer in gamestate.compress" << std::endl; return false;
    case Z_DATA_ERROR: COUT(2) << "G.St.Man: compress: data corrupted in gamestate.compress" << std::endl; return false;
  }

  //copy and modify header
  GamestateHeader *temp = header_;
  header_ = new GamestateHeader(ndata, temp);
  delete temp;
  //delete old data
  delete[] data_;
  //save new data
  data_ = ndata;
  header_->setCompSize( buffer );
  header_->setCompressed( true );
  COUT(5) << "gamestate compress datasize: " << header_->getDataSize() << " compsize: " << header_->getCompSize() << std::endl;
  return true;
}
bool Gamestate::decompressData()
{
  assert(data_);
  assert(header_->isCompressed());
  COUT(4) << "GameStateClient: uncompressing gamestate. id: " << header_->getID() << ", baseid: " << header_->getBaseID() << ", datasize: " << header_->getDataSize() << ", compsize: " << header_->getCompSize() << std::endl;
  uint32_t datasize = header_->getDataSize();
  uint32_t compsize = header_->getCompSize();
  uint32_t bufsize;
//  assert(compsize<=datasize);
  bufsize = datasize;
  assert(bufsize!=0);
  uint8_t *ndata = new uint8_t[bufsize + GamestateHeader::getSize()];
  uint8_t *dest = ndata + GamestateHeader::getSize();
  uint8_t *source = data_ + GamestateHeader::getSize();
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
  GamestateHeader *temp = header_;
  header_ = new GamestateHeader( data_, header_ );
  delete temp;

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
  header_->setCompressed( false );
  assert(header_->getDataSize()==datasize);
  assert(header_->getCompSize()==compsize);
  return true;
}

Gamestate *Gamestate::diff(Gamestate *base)
{
  assert(data_);
  assert(!header_->isCompressed());
  assert(!header_->isDiffed());
  GamestateHeader diffHeader(base->data_);
  //unsigned char *basep = base->getGs()/*, *gs = getGs()*/;
  uint8_t *basep = GAMESTATE_START(base->data_), *gs = GAMESTATE_START(this->data_);
  uint32_t of=0; // pointers offset
  uint32_t dest_length=0;
  dest_length=header_->getDataSize();
  if(dest_length==0)
    return NULL;
  uint8_t *ndata = new uint8_t[dest_length*sizeof(uint8_t)+GamestateHeader::getSize()];
  uint8_t *dest = ndata + GamestateHeader::getSize();
  while(of < diffHeader.getDataSize() && of < header_->getDataSize()){
    *(dest+of)=*(basep+of)^*(gs+of); // do the xor
    ++of;
  }
  if(diffHeader.getDataSize()!=header_->getDataSize()){
    uint8_t n=0;
    if(diffHeader.getDataSize() < header_->getDataSize()){
      while(of<dest_length){
        *(dest+of)=n^*(gs+of);
        of++;
      }
    }
  }

  Gamestate *g = new Gamestate(ndata, getClientID());
  *(g->header_) = *header_;
  g->header_->setDiffed( true );
  g->header_->setBaseID( base->getID() );
  g->flags_=flags_;
  g->packetDirection_ = packetDirection_;
  return g;
}

Gamestate* Gamestate::doSelection(unsigned int clientID, unsigned int targetSize){
  assert(data_);
  std::list<obj>::iterator it;

  // allocate memory for new data
  uint8_t *gdata = new uint8_t[header_->getDataSize()+GamestateHeader::getSize()];
  // create a gamestate out of it
  Gamestate *gs = new Gamestate(gdata);
  uint8_t *newdata = gdata + GamestateHeader::getSize();
  uint8_t *origdata = GAMESTATE_START(data_);

  //copy the GamestateHeader
  assert(gs->header_);
  *(gs->header_) = *header_;

  uint32_t objectOffset;
  unsigned int objectsize, destsize=0;
  // TODO: Why is this variable not used?
  //Synchronisable *object;

  //call TrafficControl
  TrafficControl::getInstance()->processObjectList( clientID, header_->getID(), &dataMap_ );

  //copy in the zeros
  for(it=dataMap_.begin(); it!=dataMap_.end();){
//    if((*it).objSize==0)
//      continue;
//    if(it->second->getSize(HEADER->id)==0) // merged from objecthierarchy2, doesn't work anymore; TODO: change this
//      continue;                            // merged from objecthierarchy2, doesn't work anymore; TODO: change this
    SynchronisableHeader oldobjectheader(origdata);
    SynchronisableHeader newobjectheader(newdata);
    if ( (*it).objSize == 0 )
    {
      ++it;
      continue;
    }
//     object = Synchronisable::getSynchronisable( (*it).objID );
//     assert(object->objectID == oldobjectheader->objectID);
    objectsize = oldobjectheader.getDataSize();
    objectOffset=SynchronisableHeader::getSize(); //skip the size and the availableData variables in the objectheader
    if ( (*it).objID == oldobjectheader.getObjectID() ){
      memcpy(newdata, origdata, objectsize);
      assert(newobjectheader.isDataAvailable()==true);
      ++it;
    }else{
      newobjectheader = oldobjectheader;
      newobjectheader.setDataAvailable(false);
      memset(newdata+objectOffset, 0, objectsize-objectOffset);
    }
    newdata += objectsize;
    origdata += objectsize;
    destsize += objectsize;
  }
#ifndef NDEBUG
  uint32_t origsize = destsize;
  while ( origsize < header_->getDataSize() )
  {
    SynchronisableHeader oldobjectheader(origdata);
    objectsize = oldobjectheader.getDataSize();
    origdata += objectsize;
    origsize += objectsize;
  }
  assert(origsize==header_->getDataSize());
  assert(destsize!=0);
#endif
  gs->header_->setDataSize( destsize );
  return gs;
}


Gamestate *Gamestate::undiff(Gamestate *base)
{
  assert(this && base);assert(data_);
  assert(header_->isDiffed());
  assert(!header_->isCompressed() && !base->header_->isCompressed());
  //unsigned char *basep = base->getGs()/*, *gs = getGs()*/;
  uint8_t *basep = GAMESTATE_START(base->data_);
  uint8_t *gs = GAMESTATE_START(this->data_);
  uint32_t of=0; // pointers offset
  uint32_t dest_length=0;
  dest_length=header_->getDataSize();
  if(dest_length==0)
    return NULL;
  uint8_t *ndata = new uint8_t[dest_length*sizeof(uint8_t)+GamestateHeader::getSize()];
  uint8_t *dest = ndata + GamestateHeader::getSize();
  while(of < base->header_->getDataSize() && of < header_->getDataSize()){
    *(dest+of)=*(basep+of)^*(gs+of); // do the xor
    ++of;
  }
  if(base->header_->getDataSize()!=header_->getDataSize()){
    uint8_t n=0;
    if(base->header_->getDataSize() < header_->getDataSize()){
      while(of < dest_length){
        *(dest+of)=n^*(gs+of);
        of++;
      }
    }
  }
  Gamestate *g = new Gamestate(ndata, getClientID());
  assert(g->header_);
  *(g->header_) = *header_;
  g->header_->setDiffed( false );
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
