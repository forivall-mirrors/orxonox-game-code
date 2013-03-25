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

#include "Gamestate.h"

#include <zlib.h>

#include "util/Output.h"
#include "util/OrxAssert.h"
#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/object/ObjectList.h"
#include "network/synchronisable/Synchronisable.h"
#include "network/GamestateHandler.h"
#include "network/Host.h"

namespace orxonox {

namespace packet {

#define GAMESTATE_START(data) (data + GamestateHeader::getSize())

// #define PACKET_FLAG_GAMESTATE  PacketFlag::Reliable
#define PACKET_FLAG_GAMESTATE  0

inline bool memzero( uint8_t* data, uint32_t datalength)
{
  uint64_t* d = (uint64_t*)data;

  for( unsigned int i=0; i<datalength/8; i++ )
  {
    if( *(d+i) != 0 )
      return false;
  }
  // now process the rest (when datalength isn't a multiple of 4)
  for( unsigned int j = 8*(datalength/8); j<datalength; j++ )
  {
    if( *(data+j) != 0 )
      return false;
  }
  return true;
}


Gamestate::Gamestate():
  header_()
{
  flags_ = flags_ | PACKET_FLAG_GAMESTATE;
}


Gamestate::Gamestate(uint8_t *data, unsigned int clientID):
  Packet(data, clientID), header_(data)
{
  flags_ = flags_ | PACKET_FLAG_GAMESTATE;
}


Gamestate::Gamestate(uint8_t *data):
  header_(data)
{
  flags_ = flags_ | PACKET_FLAG_GAMESTATE;
  data_ = data;
}


Gamestate::Gamestate(const Gamestate& g) :
  Packet( *(Packet*)&g ), header_(this->data_), nrOfVariables_(0)
{
  flags_ = flags_ | PACKET_FLAG_GAMESTATE;
  sizes_ = g.sizes_;
}


Gamestate::~Gamestate()
{
}


bool Gamestate::collectData(int id, uint8_t mode)
{
  uint32_t tempsize=0, currentsize=0;
  assert(data_==0);
  uint32_t size = calcGamestateSize(id, mode);

  orxout(verbose_more, context::packets) << "G.ST.Man: producing gamestate with id: " << id << endl;
  if(size==0)
    return false;
  data_ = new uint8_t[size + GamestateHeader::getSize()];
  if(!data_)
  {
    orxout(internal_warning, context::packets) << "GameStateManager: could not allocate memory" << endl;
    return false;
  }

  // tell the gamestate header where to store the data
  header_.setData(this->data_);

  //start collect data synchronisable by synchronisable
  uint8_t *mem = data_; // in this stream store all data of the variables and the headers of the synchronisable
  mem += GamestateHeader::getSize();
  ObjectList<Synchronisable>::iterator it;
  for(it = ObjectList<Synchronisable>::begin(); it; ++it)
  {

//     tempsize=it->getSize(id, mode);

    tempsize = it->getData(mem, this->sizes_, id, mode);
    if ( tempsize != 0 )
      dataVector_.push_back( obj(it->getObjectID(), it->getCreatorID(), tempsize, mem-data_) );

#ifndef NDEBUG
    if(currentsize+tempsize > size)
    {
      assert(0); // if we don't use multithreading this part shouldn't be neccessary
      // start allocate additional memory
      orxout(internal_info, context::packets) << "Gamestate: need additional memory" << endl;
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
//     if(!it->getData(mem, id, mode))
//       return false; // mem pointer gets automatically increased because of call by reference
    // increase size counter by size of current synchronisable
    currentsize+=tempsize;
  }


  //start write gamestate header
  header_.setDataSize( currentsize );
  header_.setCompSize( 0 );
  header_.setID( id );
  header_.setBaseID( GAMESTATEID_INITIAL );
  header_.setDiffed( false );
  header_.setComplete( true );
  header_.setCompressed( false );
  //stop write gamestate header

  orxout(verbose_more, context::packets) << "Gamestate: Gamestate size: " << currentsize << endl;
  orxout(verbose_more, context::packets) << "Gamestate: 'estimated' (and corrected) Gamestate size: " << size << endl;
  return true;
}


bool Gamestate::spreadData(uint8_t mode)
{
  orxout(verbose_more, context::packets) << "processing gamestate with id " << header_.getID() << endl;
  assert(data_);
  assert(!header_.isCompressed());
  uint8_t *mem=data_+GamestateHeader::getSize();
  Synchronisable *s;
  
  // update the data of the objects we received
  while(mem < data_+GamestateHeader::getSize()+header_.getDataSize())
  {
    SynchronisableHeader objectheader(mem);

    s = Synchronisable::getSynchronisable( objectheader.getObjectID() );
    if(!s)
    {
      if (!GameMode::isMaster())
      {
        Synchronisable::fabricate(mem, mode);
      }
      else
      {
//         orxout(verbose, context::packets) << "not creating object of classid " << objectheader.getClassID() << endl;
        mem += objectheader.getDataSize() + ( objectheader.isDiffed() ? SynchronisableHeaderLight::getSize() : SynchronisableHeader::getSize() );
      }
    }
    else
    {
//       orxout(verbose, context::packets) << "updating object of classid " << objectheader.getClassID() << endl;
      OrxVerify(s->updateData(mem, mode), "ERROR: could not update Synchronisable with Gamestate data");
    }
  }
  assert((uintptr_t)(mem-data_) == GamestateHeader::getSize()+header_.getDataSize());
  
   // In debug mode, check first, whether there are no duplicate objectIDs
#ifndef NDEBUG
  if(this->getID()%1000==1)
  {
    std::list<uint32_t> v1;
    ObjectList<Synchronisable>::iterator it;
    for (it = ObjectList<Synchronisable>::begin(); it != ObjectList<Synchronisable>::end(); ++it)
    {
      if (it->getObjectID() == OBJECTID_UNKNOWN)
      {
        if (it->objectMode_ != 0x0)
        {
          orxout(user_error, context::packets) << "Found object with OBJECTID_UNKNOWN on the client with objectMode != 0x0!" << endl;
          orxout(user_error, context::packets) << "Possible reason for this error: Client created a synchronized object without the Server's approval." << endl;
          orxout(user_error, context::packets) << "Objects class: " << it->getIdentifier()->getName() << endl;
          assert(false);
        }
      }
      else
      {
        std::list<uint32_t>::iterator it2;
        for (it2 = v1.begin(); it2 != v1.end(); ++it2)
        {
          if (it->getObjectID() == *it2)
          {
            orxout(user_error, context::packets) << "Found duplicate objectIDs on the client!" << endl
                                                 << "Are you sure you don't create a Sychnronisable objcect with 'new' \
                                                     that doesn't have objectMode = 0x0?" << endl;
            assert(false);
          }
        }
        v1.push_back(it->getObjectID());
      }
    }
  }
#endif
  return true;
}


uint32_t Gamestate::getSize() const
{
  assert(data_);
  if(header_.isCompressed())
    return header_.getCompSize()+GamestateHeader::getSize();
  else
  {
    return header_.getDataSize()+GamestateHeader::getSize();
  }
}


bool Gamestate::operator==(packet::Gamestate gs)
{
  uint8_t *d1 = data_+GamestateHeader::getSize();
  uint8_t *d2 = gs.data_+GamestateHeader::getSize();
  GamestateHeader h1(data_);
  GamestateHeader h2(gs.data_);
  assert(h1.getDataSize() == h2.getDataSize());
  assert(!isCompressed());
  assert(!gs.isCompressed());
  return memcmp(d1, d2, h1.getDataSize())==0;
}


bool Gamestate::process(orxonox::Host* host)
{
  return host->addGamestate(this, getPeerID());
}


bool Gamestate::compressData()
{
  assert(data_);
  assert(!header_.isCompressed());
  uLongf buffer = (uLongf)(((header_.getDataSize() + 12)*1.01)+1);
  if(buffer==0)
    return false;

  uint8_t *ndata = new uint8_t[buffer+GamestateHeader::getSize()];
  uint8_t *dest = ndata + GamestateHeader::getSize();
  uint8_t *source = data_ + GamestateHeader::getSize();
  int retval;
  retval = compress( dest, &buffer, source, (uLong)(header_.getDataSize()) );
  switch ( retval )
  {
    case Z_OK: orxout(verbose_more, context::packets) << "G.St.Man: compress: successfully compressed" << endl; break;
    case Z_MEM_ERROR: orxout(internal_error, context::packets) << "G.St.Man: compress: not enough memory available in gamestate.compress" << endl; return false;
    case Z_BUF_ERROR: orxout(internal_warning, context::packets) << "G.St.Man: compress: not enough memory available in the buffer in gamestate.compress" << endl; return false;
    case Z_DATA_ERROR: orxout(internal_warning, context::packets) << "G.St.Man: compress: data corrupted in gamestate.compress" << endl; return false;
  }

  //copy and modify header
  GamestateHeader *temp = new GamestateHeader(data_);
  header_.setData(ndata);
  header_ = *temp;
  delete temp;
  //delete old data
  delete[] data_;
  //save new data
  data_ = ndata;
  header_.setCompSize( buffer );
  header_.setCompressed( true );
  orxout(verbose, context::packets) << "gamestate compress datasize: " << header_.getDataSize() << " compsize: " << header_.getCompSize() << endl;
  return true;
}


bool Gamestate::decompressData()
{
  assert(data_);
  assert(header_.isCompressed());
  orxout(verbose, context::packets) << "GameStateClient: uncompressing gamestate. id: " << header_.getID() << ", baseid: " << header_.getBaseID() << ", datasize: " << header_.getDataSize() << ", compsize: " << header_.getCompSize() << endl;
  uint32_t datasize = header_.getDataSize();
  uint32_t compsize = header_.getCompSize();
  uint32_t bufsize;
  bufsize = datasize;
  assert(bufsize!=0);
  uint8_t *ndata = new uint8_t[bufsize + GamestateHeader::getSize()];
  uint8_t *dest = ndata + GamestateHeader::getSize();
  uint8_t *source = data_ + GamestateHeader::getSize();
  int retval;
  uLongf length=bufsize;
  retval = uncompress( dest, &length, source, (uLong)compsize );
  switch ( retval )
  {
    case Z_OK: orxout(verbose_more, context::packets) << "successfully decompressed" << endl; break;
    case Z_MEM_ERROR: orxout(internal_error, context::packets) << "not enough memory available" << endl; return false;
    case Z_BUF_ERROR: orxout(internal_warning, context::packets) << "not enough memory available in the buffer" << endl; return false;
    case Z_DATA_ERROR: orxout(internal_warning, context::packets) << "data corrupted (zlib)" << endl; return false;
  }

  //copy over the header
  GamestateHeader* temp = new GamestateHeader( data_ );
  header_.setData(ndata);
  header_ = *temp;
  delete temp;

  if (this->bDataENetAllocated_)
  {
    // Memory was allocated by ENet. --> We let it be since enet_packet_destroy will
    // deallocated it anyway. So data and packet stay together.
    this->bDataENetAllocated_ = false;
  }
  else
  {
    // We allocated the memory in the first place (unlikely). So we destroy the old data
    // and overwrite it with the new decompressed data.
    delete[] this->data_;
  }

  //set new pointers
  data_ = ndata;
  header_.setCompressed( false );
  assert(header_.getDataSize()==datasize);
  assert(header_.getCompSize()==compsize);
  return true;
}


inline void /*Gamestate::*/diffObject( uint8_t*& newDataPtr, uint8_t*& origDataPtr, uint8_t*& baseDataPtr, SynchronisableHeader& objectHeader, std::vector<uint32_t>::iterator& sizes )
{
  assert( objectHeader.getDataSize() == SynchronisableHeader(baseDataPtr).getDataSize() );
  
  uint32_t objectOffset = SynchronisableHeader::getSize(); // offset inside the object in the origData and baseData
  // Check whether the whole object stayed the same
  if( memcmp( origDataPtr+objectOffset, baseDataPtr+objectOffset, objectHeader.getDataSize()) == 0 )
  {
//     orxout(verbose, context::packets) << "skip object " << Synchronisable::getSynchronisable(objectHeader.getObjectID())->getIdentifier()->getName() << endl;
    origDataPtr += objectOffset + objectHeader.getDataSize(); // skip the whole object
    baseDataPtr += objectOffset + objectHeader.getDataSize();
    sizes += Synchronisable::getSynchronisable(objectHeader.getObjectID())->getNrOfVariables();
  }
  else
  {
    // Now start to diff the Object
    SynchronisableHeaderLight newObjectHeader(newDataPtr);
    newObjectHeader = objectHeader; // copy over the objectheader
    VariableID variableID = 0;
    uint32_t diffedObjectOffset = SynchronisableHeaderLight::getSize();
    // iterate through all variables
    while( objectOffset < objectHeader.getDataSize()+SynchronisableHeader::getSize() )
    {
      // check whether variable changed and write id and copy over variable to the new stream
      // otherwise skip variable
      uint32_t varSize = *sizes;
      assert( varSize == Synchronisable::getSynchronisable(objectHeader.getObjectID())->getVarSize(variableID) );
      if ( varSize != 0 )
      {
        if ( memcmp(origDataPtr+objectOffset, baseDataPtr+objectOffset, varSize) != 0 )
        {
          *(VariableID*)(newDataPtr+diffedObjectOffset) = variableID; // copy over the variableID
          diffedObjectOffset += sizeof(VariableID);
          memcpy( newDataPtr+diffedObjectOffset, origDataPtr+objectOffset, varSize );
          diffedObjectOffset += varSize;
          objectOffset += varSize;
        }
        else
        {
          objectOffset += varSize;
        }
      }

      ++variableID;
      ++sizes;
    }
    
    // if there are variables from this object with 0 size left in sizes
    if( Synchronisable::getSynchronisable(objectHeader.getObjectID())->getNrOfVariables() != variableID )
      sizes += Synchronisable::getSynchronisable(objectHeader.getObjectID())->getNrOfVariables() - variableID;
    
    newObjectHeader.setDiffed(true);
    newObjectHeader.setDataSize(diffedObjectOffset-SynchronisableHeaderLight::getSize());
    assert(objectOffset == objectHeader.getDataSize()+SynchronisableHeader::getSize());
    assert(newObjectHeader.getDataSize()>0);
    
    origDataPtr += objectOffset;
    baseDataPtr += objectOffset;
    newDataPtr += diffedObjectOffset;
  }
}

inline void /*Gamestate::*/copyObject( uint8_t*& newData, uint8_t*& origData, uint8_t*& baseData, SynchronisableHeader& objectHeader, std::vector<uint32_t>::iterator& sizes )
{
  //       orxout(verbose, context::packets) << "docopy" << endl;
  // Just copy over the whole Object
  memcpy( newData, origData, objectHeader.getDataSize()+SynchronisableHeader::getSize() );
  SynchronisableHeader(newData).setDiffed(false);
  
  newData += objectHeader.getDataSize()+SynchronisableHeader::getSize();
  origData += objectHeader.getDataSize()+SynchronisableHeader::getSize();
//   SynchronisableHeader baseHeader( baseData );
//   baseData += baseHeader.getDataSize()+SynchronisableHeader::getSize();
  //       orxout(verbose, context::packets) << "copy " << h.getObjectID() << endl;
  //       orxout(verbose, context::packets) << "copy " << h.getObjectID() << ":";
  sizes += Synchronisable::getSynchronisable(objectHeader.getObjectID())->getNrOfVariables();
//   for( unsigned int i = 0; i < Synchronisable::getSynchronisable(objectHeader.getObjectID())->getNrOfVariables(); ++i )
//   {
//     //         orxout(verbose, context::packets) << " " << *sizes;
//     ++sizes;
//   }
    //       orxout(verbose, context::packets) << endl;
}

inline bool findObject(uint8_t*& dataPtr, uint8_t* endPtr, SynchronisableHeader& objectHeader)
{
  // Some assertions to make sure the dataPtr is valid (pointing to a SynchronisableHeader)
  {
    SynchronisableHeader htemp2(dataPtr);
    assert(htemp2.getClassID()<500);
    assert(htemp2.getDataSize()!=0 && htemp2.getDataSize()<1000);
    assert(htemp2.isDiffed()==false);
  }
  uint32_t objectID = objectHeader.getObjectID();
  while ( dataPtr < endPtr )
  {
    SynchronisableHeader htemp(dataPtr);
    assert( htemp.getDataSize()!=0 );
    if ( htemp.getObjectID() == objectID )
    {
      assert( objectHeader.getClassID() == htemp.getClassID() );
      assert( objectHeader.getCreatorID() == htemp.getCreatorID() );
      return true;
    }
    {
      if( dataPtr+htemp.getDataSize()+SynchronisableHeader::getSize() < endPtr )
      {
        SynchronisableHeader htemp2(dataPtr+htemp.getDataSize()+SynchronisableHeader::getSize());
        assert(htemp2.getClassID()<500);
        assert(htemp2.getDataSize()!=0 && htemp2.getDataSize()<1000);
        assert(htemp2.isDiffed()==false);
      }
    }
    dataPtr += htemp.getDataSize()+SynchronisableHeader::getSize();
    
  }
  assert(dataPtr == endPtr);
  
  return false;
}

Gamestate* Gamestate::diffVariables(Gamestate *base)
{
  assert(this && base); assert(data_ && base->data_);
  assert(!header_.isCompressed() && !base->header_.isCompressed());
  assert(!header_.isDiffed());
  assert( header_.getDataSize() && base->header_.getDataSize() );


  // *** first do a raw diff of the two gamestates

  uint8_t *baseDataPtr = GAMESTATE_START(base->data_);
  uint8_t *origDataPtr = GAMESTATE_START(this->data_);
  uint8_t *origDataEnd = origDataPtr + header_.getDataSize();
  uint8_t *baseDataEnd = baseDataPtr + base->header_.getDataSize();
//   uint32_t origLength = header_.getDataSize();
//   uint32_t baseLength = base->header_.getDataSize();

  // Allocate new space for diffed gamestate
  uint32_t newDataSize = header_.getDataSize() + GamestateHeader::getSize() + sizeof(uint32_t)*this->nrOfVariables_;
  uint8_t *newData = new uint8_t[newDataSize]; // this is the maximum size needed in the worst case
  uint8_t *destDataPtr = GAMESTATE_START(newData);

  std::vector<uint32_t>::iterator sizesIt = this->sizes_.begin();

  while( origDataPtr < origDataEnd )
  {
    //iterate through all objects

    SynchronisableHeader origHeader(origDataPtr);

    // Find (if possible) the current object in the datastream of the old gamestate
    // Start at the current offset position
    if(baseDataPtr == baseDataEnd)
      baseDataPtr = GAMESTATE_START(base->data_);
    uint8_t* oldBaseDataPtr = baseDataPtr;
    
    assert(baseDataPtr < baseDataEnd);
    assert(destDataPtr < newData + newDataSize);
    assert(sizesIt != this->sizes_.end());
    
    assert(Synchronisable::getSynchronisable(origHeader.getObjectID()));
    assert(ClassByID(origHeader.getClassID()));
    assert(origHeader.getDataSize() < 500);
    
    if( findObject(baseDataPtr, baseDataEnd, origHeader) )
    {
      SynchronisableHeader baseHeader(baseDataPtr);
      assert(Synchronisable::getSynchronisable(baseHeader.getObjectID()));
      assert(ClassByID(baseHeader.getClassID()));
      assert(baseHeader.getDataSize() < 500);
      if( SynchronisableHeader(baseDataPtr).getDataSize()==origHeader.getDataSize() )
      {
//         orxout(verbose, context::packets) << "diffing object in order: " << Synchronisable::getSynchronisable(origHeader.getObjectID())->getIdentifier()->getName() << endl;
        diffObject(destDataPtr, origDataPtr, baseDataPtr, origHeader, sizesIt);
      }
      else
      {
//         orxout(verbose, context::packets) << "copy object because of different data sizes (1): " << Synchronisable::getSynchronisable(origHeader.getObjectID())->getIdentifier()->getName() << endl;
        copyObject(destDataPtr, origDataPtr, baseDataPtr, origHeader, sizesIt);
        assert(sizesIt != this->sizes_.end() || origDataPtr==origDataEnd);
      }
        
    }
    else
    {
      assert( baseDataPtr == baseDataEnd );
      baseDataPtr = GAMESTATE_START(base->data_);
      if( findObject(baseDataPtr, oldBaseDataPtr, origHeader) )
      {
        SynchronisableHeader baseHeader(baseDataPtr);
        assert(Synchronisable::getSynchronisable(baseHeader.getObjectID()));
        assert(ClassByID(baseHeader.getClassID()));
        assert(baseHeader.getDataSize() < 500);
        if( SynchronisableHeader(baseDataPtr).getDataSize()==origHeader.getDataSize() )
        {
//           orxout(verbose, context::packets) << "diffing object out of order: " << Synchronisable::getSynchronisable(origHeader.getObjectID())->getIdentifier()->getName() << endl;
          diffObject(destDataPtr, origDataPtr, baseDataPtr, origHeader, sizesIt);
        }
        else
        {
//           orxout(verbose, context::packets) << "copy object because of different data sizes (2): " << Synchronisable::getSynchronisable(origHeader.getObjectID())->getIdentifier()->getName() << endl;
          copyObject(destDataPtr, origDataPtr, baseDataPtr, origHeader, sizesIt);
          assert(sizesIt != this->sizes_.end() || origDataPtr==origDataEnd);
        }
      }
      else
      {
//         orxout(verbose, context::packets) << "copy object: " << Synchronisable::getSynchronisable(origHeader.getObjectID())->getIdentifier()->getName() << endl;
        assert(baseDataPtr == oldBaseDataPtr);
        copyObject(destDataPtr, origDataPtr, baseDataPtr, origHeader, sizesIt);
        assert(sizesIt != this->sizes_.end() || origDataPtr==origDataEnd);
      }
    }
  }
  assert(sizesIt==this->sizes_.end());


  Gamestate *g = new Gamestate(newData, getPeerID());
  (g->header_) = header_;
  g->header_.setBaseID( base->getID() );
  g->header_.setDataSize(destDataPtr - newData - GamestateHeader::getSize());
  g->flags_=flags_;
  g->packetDirection_ = packetDirection_;
  assert(!g->isCompressed());
  return g;
}


/*Gamestate* Gamestate::diffData(Gamestate *base)
{
  assert(this && base); assert(data_ && base->data_);
  assert(!header_.isCompressed() && !base->header_.isCompressed());
  assert(!header_.isDiffed());

  uint8_t *basep = GAMESTATE_START(base->data_);
  uint8_t *gs = GAMESTATE_START(this->data_);
  uint32_t dest_length = header_.getDataSize();

  if(dest_length==0)
    return NULL;

  uint8_t *ndata = new uint8_t[dest_length*sizeof(uint8_t)+GamestateHeader::getSize()];
  uint8_t *dest = GAMESTATE_START(ndata);

  rawDiff( dest, gs, basep, header_.getDataSize(), base->header_.getDataSize() );
#ifndef NDEBUG
  uint8_t *dest2 = new uint8_t[dest_length];
  rawDiff( dest2, dest, basep, header_.getDataSize(), base->header_.getDataSize() );
  assert( memcmp( dest2, gs, dest_length) == 0 );
  delete dest2;
#endif

  Gamestate *g = new Gamestate(ndata, getClientID());
  assert(g->header_);
  *(g->header_) = *header_;
  g->header_.setDiffed( true );
  g->header_.setBaseID( base->getID() );
  g->flags_=flags_;
  g->packetDirection_ = packetDirection_;
  assert(g->isDiffed());
  assert(!g->isCompressed());
  return g;
}


Gamestate* Gamestate::undiff(Gamestate *base)
{
  assert(this && base); assert(data_ && base->data_);
  assert(!header_.isCompressed() && !base->header_.isCompressed());
  assert(header_.isDiffed());

  uint8_t *basep = GAMESTATE_START(base->data_);
  uint8_t *gs = GAMESTATE_START(this->data_);
  uint32_t dest_length = header_.getDataSize();

  if(dest_length==0)
    return NULL;

  uint8_t *ndata = new uint8_t[dest_length*sizeof(uint8_t)+GamestateHeader::getSize()];
  uint8_t *dest = ndata + GamestateHeader::getSize();

  rawDiff( dest, gs, basep, header_.getDataSize(), base->header_.getDataSize() );

  Gamestate *g = new Gamestate(ndata, getClientID());
  assert(g->header_);
  *(g->header_) = *header_;
  g->header_.setDiffed( false );
  g->flags_=flags_;
  g->packetDirection_ = packetDirection_;
  assert(!g->isDiffed());
  assert(!g->isCompressed());
  return g;
}


void Gamestate::rawDiff( uint8_t* newdata, uint8_t* data, uint8_t* basedata, uint32_t datalength, uint32_t baselength)
{
  uint64_t* gd = (uint64_t*)data;
  uint64_t* bd = (uint64_t*)basedata;
  uint64_t* nd = (uint64_t*)newdata;

  unsigned int i;
  for( i=0; i<datalength/8; i++ )
  {
    if( i<baselength/8 )
      *(nd+i) = *(gd+i) ^ *(bd+i);  // xor the data
    else
      *(nd+i) = *(gd+i); // just copy over the data
  }
  unsigned int j;
  // now process the rest (when datalength isn't a multiple of 4)
  for( j = 8*(datalength/8); j<datalength; j++ )
  {
    if( j<baselength )
      *(newdata+j) = *(data+j) ^ *(basedata+j); // xor
    else
      *(newdata+j) = *(data+j); // just copy
  }
  assert(j==datalength);
}*/


/*Gamestate* Gamestate::doSelection(unsigned int clientID, unsigned int targetSize){
  assert(data_);
  std::list<obj>::iterator it;

  // allocate memory for new data
  uint8_t *gdata = new uint8_t[header_.getDataSize()+GamestateHeader::getSize()];
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
  TrafficControl::getInstance()->processObjectList( clientID, header_.getID(), dataVector_ );

  //copy in the zeros
//   std::list<obj>::iterator itt;
//   orxout() << "myvector contains:";
//   for ( itt=dataVector_.begin() ; itt!=dataVector_.end(); itt++ )
//     orxout() << " " << (*itt).objID;
//   orxout() << endl;
  for(it=dataVector_.begin(); it!=dataVector_.end();){
    SynchronisableHeader oldobjectheader(origdata);
    SynchronisableHeader newobjectheader(newdata);
    if ( (*it).objSize == 0 )
    {
      ++it;
      continue;
    }
    objectsize = oldobjectheader.getDataSize()+SynchronisableHeader::getSize();
    objectOffset=SynchronisableHeader::getSize(); //skip the size and the availableData variables in the objectheader
    if ( (*it).objID == oldobjectheader.getObjectID() ){
      memcpy(newdata, origdata, objectsize);
      ++it;
    }else{
      newobjectheader = oldobjectheader;
      memset(newdata+objectOffset, 0, objectsize-objectOffset);
    }
    newdata += objectsize;
    origdata += objectsize;
    destsize += objectsize;
  }
#ifndef NDEBUG
  uint32_t origsize = destsize;
  while ( origsize < header_.getDataSize() )
  {
    SynchronisableHeader oldobjectheader(origdata);
    objectsize = oldobjectheader.getDataSize()+SynchronisableHeader::getSize();
    origdata += objectsize;
    origsize += objectsize;
  }
  assert(origsize==header_.getDataSize());
  assert(destsize!=0);
#endif
  gs->header_.setDataSize( destsize );
  return gs;
}*/


uint32_t Gamestate::calcGamestateSize(uint32_t id, uint8_t mode)
{
  uint32_t size = 0;
  uint32_t nrOfVariables = 0;
    // get the start of the Synchronisable list
  ObjectList<Synchronisable>::iterator it;
    // get total size of gamestate
  for(it = ObjectList<Synchronisable>::begin(); it; ++it){
    size+=it->getSize(id, mode); // size of the actual data of the synchronisable
    nrOfVariables += it->getNrOfVariables();
  }
//   orxout() << "allocating " << nrOfVariables << " ints" << endl;
  this->sizes_.reserve(nrOfVariables);
  return size;
}


} //namespace packet
} //namespace orxonox
