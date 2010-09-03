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

#include "util/Debug.h"
#include "core/GameMode.h"
#include "core/ObjectList.h"
#include "network/synchronisable/Synchronisable.h"
#include "network/GamestateHandler.h"

namespace orxonox {

namespace packet {

#define GAMESTATE_START(data) (data + GamestateHeader::getSize())

#define PACKET_FLAG_GAMESTATE  PacketFlag::Reliable

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
  header_(0)
{
  flags_ = flags_ | PACKET_FLAG_GAMESTATE;
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
  data_ = data;
  header_ = new GamestateHeader(data_);
}


Gamestate::Gamestate(const Gamestate& g) :
    Packet( *(Packet*)&g ), nrOfVariables_(0)
{
  flags_ = flags_ | PACKET_FLAG_GAMESTATE;
  header_ = new GamestateHeader(data_);
  sizes_ = g.sizes_;
}


Gamestate::~Gamestate()
{
  if( header_ )
    delete header_;
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
  if(!data_)
  {
    COUT(2) << "GameStateManager: could not allocate memory" << std::endl;
    return false;
  }

  // create the header object
  assert( header_ == 0 );
  header_ = new GamestateHeader(data_);

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
//     if(!it->getData(mem, id, mode))
//       return false; // mem pointer gets automatically increased because of call by reference
    // increase size counter by size of current synchronisable
    currentsize+=tempsize;
  }


  //start write gamestate header
  header_->setDataSize( currentsize );
  header_->setID( id );
  header_->setBaseID( GAMESTATEID_INITIAL );
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
  uint8_t *mem=data_+GamestateHeader::getSize();
  Synchronisable *s;

  // update the data of the objects we received
  while(mem < data_+GamestateHeader::getSize()+header_->getDataSize())
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
        mem += objectheader.getDataSize() + ( objectheader.isDiffed() ? SynchronisableHeaderLight::getSize() : SynchronisableHeader::getSize() );
      }
    }
    else
    {
      bool b = s->updateData(mem, mode);
      assert(b);
    }
  }
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
          COUT(0) << "Found object with OBJECTID_UNKNOWN on the client with objectMode != 0x0!" << std::endl;
          COUT(0) << "Possible reason for this error: Client created a synchronized object without the Server's approval." << std::endl;
          COUT(0) << "Objects class: " << it->getIdentifier()->getName() << std::endl;
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
            COUT(0) << "Found duplicate objectIDs on the client!" << std::endl
                    << "Are you sure you don't create a Sychnronisable objcect with 'new' \
                        that doesn't have objectMode = 0x0?" << std::endl;
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
  if(header_->isCompressed())
    return header_->getCompSize()+GamestateHeader::getSize();
  else
  {
    return header_->getDataSize()+GamestateHeader::getSize();
  }
}


bool Gamestate::operator==(packet::Gamestate gs)
{
  uint8_t *d1 = data_+GamestateHeader::getSize();
  uint8_t *d2 = gs.data_+GamestateHeader::getSize();
  GamestateHeader* h1 = new GamestateHeader(data_);
  GamestateHeader* h2 = new GamestateHeader(gs.data_);
  assert(h1->getDataSize() == h2->getDataSize());
  assert(!isCompressed());
  assert(!gs.isCompressed());
  return memcmp(d1, d2, h1->getDataSize())==0;
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
  uint8_t *source = data_ + GamestateHeader::getSize();
  int retval;
  retval = compress( dest, &buffer, source, (uLong)(header_->getDataSize()) );
  switch ( retval )
  {
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
  COUT(0) << "gamestate compress datasize: " << header_->getDataSize() << " compsize: " << header_->getCompSize() << std::endl;
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
    case Z_OK: COUT(5) << "successfully decompressed" << std::endl; break;
    case Z_MEM_ERROR: COUT(1) << "not enough memory available" << std::endl; return false;
    case Z_BUF_ERROR: COUT(2) << "not enough memory available in the buffer" << std::endl; return false;
    case Z_DATA_ERROR: COUT(2) << "data corrupted (zlib)" << std::endl; return false;
  }

  //copy over the header
  GamestateHeader *temp = header_;
  header_ = new GamestateHeader( data_, header_ );
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
  header_->setCompressed( false );
  assert(header_->getDataSize()==datasize);
  assert(header_->getCompSize()==compsize);
  return true;
}


inline void /*Gamestate::*/diffObject( uint8_t*& newData, uint8_t*& origData, uint8_t*& baseData, SynchronisableHeader& objectHeader, std::vector<uint32_t>::iterator& sizes )
{
  //       COUT(4) << "dodiff" << endl;
  //       if(baseOffset==0)
  //       {
  //         assert(origOffset==0);
  //       }
  uint32_t objectOffset = SynchronisableHeader::getSize(); // offset inside the object in the origData and baseData
  // Check whether the whole object stayed the same
  if( memcmp( origData+objectOffset, baseData+objectOffset, objectHeader.getDataSize()) == 0 )
  {
    //         COUT(4) << "skip object" << Synchronisable::getSynchronisable(h.getObjectID())->getIdentifier()->getName() << endl;
    origData += objectOffset + objectHeader.getDataSize(); // skip the whole object
    baseData += objectOffset + objectHeader.getDataSize();
    sizes += Synchronisable::getSynchronisable(objectHeader.getObjectID())->getNrOfVariables();
  }
  else
  {
    //         if( Synchronisable::getSynchronisable(h.getObjectID())->getIdentifier()->getName() == "Bot" )
    //           COUT(0) << "blub" << endl;
    //         COUT(4) << "object diff: " << Synchronisable::getSynchronisable(h.getObjectID())->getIdentifier()->getName() << endl;
    //         COUT(4) << "diff " << h.getObjectID() << ":";
    // Now start to diff the Object
    SynchronisableHeaderLight h2(newData);
    h2 = objectHeader; // copy over the objectheader
    VariableID variableID = 0;
    uint32_t newObjectOffset = SynchronisableHeaderLight::getSize();
    // iterate through all variables
    while( objectOffset < objectHeader.getDataSize()+SynchronisableHeader::getSize() )
    {
      // check whether variable changed and write id and copy over variable to the new stream
      // otherwise skip variable
//       assert(sizes != this->sizes_.end());
      uint32_t varSize = *sizes;
      assert( varSize == Synchronisable::getSynchronisable(objectHeader.getObjectID())->getVarSize(variableID) );
      if ( varSize != 0 )
      {
        if ( memcmp(origData+objectOffset, baseData+objectOffset, varSize) != 0 )
        {
          //               COUT(4) << "copy variable" << endl;
          *(VariableID*)(newData+newObjectOffset) = variableID; // copy over the variableID
          newObjectOffset += sizeof(VariableID);
          memcpy( newData+newObjectOffset, origData+objectOffset, varSize );
          newObjectOffset += varSize;
          objectOffset += varSize;
        }
        else
        {
          //               COUT(4) << "skip variable" << endl;
          objectOffset += varSize;
        }
      }
//           else
//             COUT(4) << "varsize 0" << endl;

      ++variableID;
      ++sizes;
    }
            
    if( Synchronisable::getSynchronisable(objectHeader.getObjectID())->getNrOfVariables() != variableID )
      sizes += Synchronisable::getSynchronisable(objectHeader.getObjectID())->getNrOfVariables() - variableID;
    //         COUT(4) << endl;
    
    h2.setDiffed(true);
    h2.setDataSize(newObjectOffset-SynchronisableHeaderLight::getSize());
    assert(objectOffset == objectHeader.getDataSize()+SynchronisableHeader::getSize());
    assert(h2.getDataSize()>0);
    origData += objectOffset;
    //         baseOffset += temp + h.getDataSize()+SynchronisableHeader::getSize() - baseData;
    //baseOffset += objectOffset;
    //         SynchronisableHeader htemp(temp);
    //         baseOffset += SynchronisableHeader::getSize() + htemp.getDataSize();
    //         {
      //           SynchronisableHeader htemp2( baseData+(temp-baseData+objectOffset) );
    //           if( baseData+(temp-baseData+objectOffset) < baseData+baseLength )
    //           {
      //             assert(htemp2.getClassID()<500);
    //             assert(htemp2.getDataSize()!=0 && htemp2.getDataSize()<1000);
    //             assert(htemp2.isDiffed()==false);
    //           }
    //         }
    baseData += objectOffset;
    newData += newObjectOffset;
  }
}

inline void /*Gamestate::*/copyObject( uint8_t*& newData, uint8_t*& origData, uint8_t*& baseData, SynchronisableHeader& objectHeader, std::vector<uint32_t>::iterator& sizes )
{
  //       COUT(4) << "docopy" << endl;
  // Just copy over the whole Object
  memcpy( newData, origData, objectHeader.getDataSize()+SynchronisableHeader::getSize() );
  newData += objectHeader.getDataSize()+SynchronisableHeader::getSize();
  origData += objectHeader.getDataSize()+SynchronisableHeader::getSize();
  SynchronisableHeader baseHeader( baseData );
  baseData += baseHeader.getDataSize()+SynchronisableHeader::getSize();
  //       COUT(4) << "copy " << h.getObjectID() << endl;
  //       COUT(4) << "copy " << h.getObjectID() << ":";
  sizes += Synchronisable::getSynchronisable(objectHeader.getObjectID())->getNrOfVariables();
//   for( unsigned int i = 0; i < Synchronisable::getSynchronisable(objectHeader.getObjectID())->getNrOfVariables(); ++i )
//   {
//     //         COUT(4) << " " << *sizes;
//     ++sizes;
//   }
    //       COUT(4) << endl;
}

Gamestate* Gamestate::diffVariables(Gamestate *base)
{
  assert(this && base); assert(data_ && base->data_);
  assert(!header_->isCompressed() && !base->header_->isCompressed());
  assert(!header_->isDiffed());


  // *** first do a raw diff of the two gamestates

  uint8_t *baseData = GAMESTATE_START(base->data_);
  uint8_t *origData = GAMESTATE_START(this->data_);
  uint8_t *origDataEnd = origData + header_->getDataSize();
  uint8_t *baseDataEnd = baseData + base->header_->getDataSize();
//   uint32_t origLength = header_->getDataSize();
//   uint32_t baseLength = base->header_->getDataSize();

  assert( header_->getDataSize() && base->header_->getDataSize() );
  
  uint32_t newDataSize = header_->getDataSize() + GamestateHeader::getSize() + sizeof(uint32_t)*this->nrOfVariables_;
  uint8_t *newData = new uint8_t[newDataSize]; // this is the maximum size needed in the worst case
  uint8_t *dest = GAMESTATE_START(newData);

//   uint32_t baseOffset = 0; //offset in the diffed stream
//   uint32_t origOffset = 0; //offset in the new stream with removed 0's
  std::vector<uint32_t>::iterator sizes = this->sizes_.begin();

  while( origData < origDataEnd )
  {
    //iterate through all objects

    SynchronisableHeader h(origData);

    // Find (if possible) the current object in the datastream of the old gamestate
    // Start at the current offset position
    if(baseData >= baseDataEnd)
      baseData = GAMESTATE_START(base->data_);
//     uint8_t* temp = baseData + baseOffset;
    uint32_t objectID = h.getObjectID();
    assert(baseData < baseDataEnd);
    assert(dest < newData + newDataSize);
    assert(sizes != this->sizes_.end());
    bool diffedObject = false;
    while ( baseData < baseDataEnd )
    {
      SynchronisableHeader htemp(baseData);
      assert( htemp.getDataSize()!=0 );
      if ( htemp.getObjectID() == objectID )
      {
        assert( h.getClassID() == htemp.getClassID() );
//         goto DODIFF;
        diffObject(dest, origData, baseData, h, sizes);
        diffedObject = true;
        break;
      }
      {
        SynchronisableHeader htemp2(baseData+htemp.getDataSize()+SynchronisableHeader::getSize());
        if( baseData+htemp.getDataSize()+SynchronisableHeader::getSize() < baseDataEnd )
        {
          assert(htemp2.getClassID()<500);
          assert(htemp2.getDataSize()!=0 && htemp2.getDataSize()<1000);
          assert(htemp2.isDiffed()==false);
        }
      }
      baseData += htemp.getDataSize()+SynchronisableHeader::getSize();
//       assert( baseData+htemp.getDataSize()+SynchronisableHeader::getSize() <=baseData+baseLength );
//       temp += htemp.getDataSize()+SynchronisableHeader::getSize();
      
    }
    // If not found start looking at the beginning
    assert( diffedObject || baseData == baseDataEnd );
    baseData = GAMESTATE_START(base->data_);
    {
      SynchronisableHeader htemp2(baseData);
      if( baseData < baseDataEnd )
      {
        assert(htemp2.getClassID()<500);
        assert(htemp2.getDataSize()!=0 && htemp2.getDataSize()<1000);
        assert(htemp2.isDiffed()==false);
      }
    }
    while ( !diffedObject && baseData < baseDataEnd )
    {
      SynchronisableHeader htemp(baseData);
      if ( htemp.getObjectID() == objectID )
      {
        assert( h.getClassID() == htemp.getClassID() );
//         goto DODIFF;
        diffObject(dest, origData, baseData, h, sizes);
        break;
      }
//       {
//         SynchronisableHeader htemp2(baseData+htemp.getDataSize()+SynchronisableHeader::getSize());
//         if( baseData+htemp.getDataSize()+SynchronisableHeader::getSize() < baseDataEnd )
//         {
//           assert(htemp2.getClassID()<500);
//           assert(htemp2.getDataSize()!=0 && htemp2.getDataSize()<1000);
//           assert(htemp2.isDiffed()==false);
//         }
//       }
      baseData += htemp.getDataSize()+SynchronisableHeader::getSize();
    }
    // Object is new, thus never transmitted -> just copy over
//     goto DOCOPY;
    if ( baseData >= baseDataEnd )
    {
      assert(baseData == baseDataEnd);
      copyObject(dest, origData, baseData, h, sizes);
      assert(sizes != this->sizes_.end() || origData>=origDataEnd);
    }
  }
  assert(sizes==this->sizes_.end());


  Gamestate *g = new Gamestate(newData, getClientID());
  assert(g->header_);
  *(g->header_) = *header_;
  g->header_->setBaseID( base->getID() );
  g->header_->setDataSize(dest - newData - GamestateHeader::getSize());
  g->flags_=flags_;
  g->packetDirection_ = packetDirection_;
  assert(!g->isCompressed());
  return g;
}


Gamestate* Gamestate::diffData(Gamestate *base)
{
  assert(this && base); assert(data_ && base->data_);
  assert(!header_->isCompressed() && !base->header_->isCompressed());
  assert(!header_->isDiffed());

  uint8_t *basep = GAMESTATE_START(base->data_);
  uint8_t *gs = GAMESTATE_START(this->data_);
  uint32_t dest_length = header_->getDataSize();

  if(dest_length==0)
    return NULL;

  uint8_t *ndata = new uint8_t[dest_length*sizeof(uint8_t)+GamestateHeader::getSize()];
  uint8_t *dest = GAMESTATE_START(ndata);

  rawDiff( dest, gs, basep, header_->getDataSize(), base->header_->getDataSize() );
#ifndef NDEBUG
  uint8_t *dest2 = new uint8_t[dest_length];
  rawDiff( dest2, dest, basep, header_->getDataSize(), base->header_->getDataSize() );
  assert( memcmp( dest2, gs, dest_length) == 0 );
  delete dest2;
#endif

  Gamestate *g = new Gamestate(ndata, getClientID());
  assert(g->header_);
  *(g->header_) = *header_;
  g->header_->setDiffed( true );
  g->header_->setBaseID( base->getID() );
  g->flags_=flags_;
  g->packetDirection_ = packetDirection_;
  assert(g->isDiffed());
  assert(!g->isCompressed());
  return g;
}


Gamestate* Gamestate::undiff(Gamestate *base)
{
  assert(this && base); assert(data_ && base->data_);
  assert(!header_->isCompressed() && !base->header_->isCompressed());
  assert(header_->isDiffed());

  uint8_t *basep = GAMESTATE_START(base->data_);
  uint8_t *gs = GAMESTATE_START(this->data_);
  uint32_t dest_length = header_->getDataSize();

  if(dest_length==0)
    return NULL;

  uint8_t *ndata = new uint8_t[dest_length*sizeof(uint8_t)+GamestateHeader::getSize()];
  uint8_t *dest = ndata + GamestateHeader::getSize();

  rawDiff( dest, gs, basep, header_->getDataSize(), base->header_->getDataSize() );

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
  TrafficControl::getInstance()->processObjectList( clientID, header_->getID(), dataVector_ );

  //copy in the zeros
//   std::list<obj>::iterator itt;
//   COUT(0) << "myvector contains:";
//   for ( itt=dataVector_.begin() ; itt!=dataVector_.end(); itt++ )
//     COUT(0) << " " << (*itt).objID;
//   COUT(0) << endl;
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
  while ( origsize < header_->getDataSize() )
  {
    SynchronisableHeader oldobjectheader(origdata);
    objectsize = oldobjectheader.getDataSize()+SynchronisableHeader::getSize();
    origdata += objectsize;
    origsize += objectsize;
  }
  assert(origsize==header_->getDataSize());
  assert(destsize!=0);
#endif
  gs->header_->setDataSize( destsize );
  return gs;
}


uint32_t Gamestate::calcGamestateSize(int32_t id, uint8_t mode)
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
//   COUT(0) << "allocating " << nrOfVariables << " ints" << endl;
  this->sizes_.reserve(nrOfVariables);
  return size;
}


} //namespace packet
} //namespace orxonox
