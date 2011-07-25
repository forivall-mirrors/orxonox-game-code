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
 *      Oliver Scheuss <scheusso [at] ee.ethz.ch>, (C) 2008
 *   Co-authors:
 *      ...
 *
 */

#include "FunctionCalls.h"

#include <cassert>
#include <cstring>
#include "util/MultiType.h"
#include "network/NetworkFunction.h"

namespace orxonox {
namespace packet {

#define   PACKET_FLAGS_FUNCTIONCALLS PacketFlag::Reliable
#define   _PACKETID         0
const unsigned int FUNCTIONCALLS_MEM_ALLOCATION = 1000;

FunctionCalls::FunctionCalls()
 : Packet()
{
  flags_ = flags_ | PACKET_FLAGS_FUNCTIONCALLS;
  currentSize_ = 2*sizeof(uint32_t); // for packetid and nrOfCalls
  nrOfCalls_ = 0;
  currentMemBlocks_ = 1;
  data_=new uint8_t[ FUNCTIONCALLS_MEM_ALLOCATION ];
  *(Type::Value *)(data_ + _PACKETID ) = Type::FunctionCalls;
  *(uint32_t*)(data_+sizeof(uint32_t)) = 0; // set nrOfCalls to 0
}

FunctionCalls::FunctionCalls( uint8_t* data, unsigned int clientID )
  : Packet(data, clientID)
{
}

FunctionCalls::~FunctionCalls()
{
}


bool FunctionCalls::process(){
  assert(isDataENetAllocated());
  uint8_t* temp = data_+sizeof(uint32_t); //skip packetid
  this->nrOfCalls_ = *(uint32_t*)temp;
  temp += sizeof(uint32_t);
  for( unsigned int i = 0; i<this->nrOfCalls_; i++ )
  {
    uint32_t functionID = *(uint32_t*)temp;
    bool isStatic = *(uint8_t*)(temp+sizeof(uint32_t));
    if( isStatic )
    {
      MultiType mt1, mt2, mt3, mt4, mt5;
      NetworkFunctionStatic *fct = NetworkFunctionStatic::getFunction( functionID );
      uint32_t nrOfArguments = *(uint32_t*)(temp+sizeof(uint32_t)+sizeof(uint8_t));
      temp+=2*sizeof(uint32_t)+sizeof(uint8_t);
      switch(nrOfArguments)
      {
        case 0:
          fct->call();
          break;
        case 1:
          mt1.importData(temp);
          fct->call(mt1);
          break;
        case 2:
          mt1.importData(temp);
          mt2.importData(temp);
          fct->call(mt1, mt2);
          break;
        case 3:
          mt1.importData(temp);
          mt2.importData(temp);
          mt3.importData(temp);
          fct->call(mt1, mt2, mt3);
          break;
        case 4:
          mt1.importData(temp);
          mt2.importData(temp);
          mt3.importData(temp);
          mt4.importData(temp);
          fct->call(mt1, mt2, mt3, mt4);
          break;
        case 5:
          mt1.importData(temp);
          mt2.importData(temp);
          mt3.importData(temp);
          mt4.importData(temp);
          mt5.importData(temp);
          fct->call(mt1, mt2, mt3, mt4, mt5);
          break;
        default:
          assert(0);
      }
    }
    else // not a static function, so also handle the objectID
    {
      MultiType mt1, mt2, mt3, mt4, mt5;
      NetworkMemberFunctionBase *fct = NetworkMemberFunctionBase::getFunction( functionID );
      uint32_t nrOfArguments = *(uint32_t*)(temp+sizeof(uint32_t)+sizeof(uint8_t));
      uint32_t objectID = *(uint32_t*)(temp+2*sizeof(uint32_t)+sizeof(uint8_t));
      temp+=3*sizeof(uint32_t)+sizeof(uint8_t);
      switch(nrOfArguments)
      {
        case 0:
          fct->call(objectID);
          break;
        case 1:
          mt1.importData(temp);
          fct->call(objectID, mt1);
          break;
        case 2:
          mt1.importData(temp);
          mt2.importData(temp);
          fct->call(objectID, mt1, mt2);
          break;
        case 3:
          mt1.importData(temp);
          mt2.importData(temp);
          mt3.importData(temp);
          fct->call(objectID, mt1, mt2, mt3);
          break;
        case 4:
          mt1.importData(temp);
          mt2.importData(temp);
          mt3.importData(temp);
          mt4.importData(temp);
          fct->call(objectID, mt1, mt2, mt3, mt4);
          break;
        case 5:
          mt1.importData(temp);
          mt2.importData(temp);
          mt3.importData(temp);
          mt4.importData(temp);
          mt5.importData(temp);
          fct->call(objectID, mt1, mt2, mt3, mt4, mt5);
          break;
        default:
          assert(0);
          break;
      }
    }
  }
  delete this;
  return true;
}

void FunctionCalls::addCallStatic( uint32_t networkID, const MultiType* mt1, const MultiType* mt2, const MultiType* mt3, const MultiType* mt4, const MultiType* mt5){
  assert(!isDataENetAllocated());

  // first determine the size that has to be reserved for this call
  uint32_t callsize = 2*sizeof(uint32_t)+sizeof(uint8_t); //size for network-function-id and nrOfArguments and for bool isStatic
  uint32_t nrOfArguments = 0;
  if(mt1)
  {
    nrOfArguments++;
    callsize += mt1->getNetworkSize();
    if(mt2)
    {
      nrOfArguments++;
      callsize += mt2->getNetworkSize();
      if(mt3)
      {
        nrOfArguments++;
        callsize += mt3->getNetworkSize();
        if(mt4)
        {
          nrOfArguments++;
          callsize += mt4->getNetworkSize();
          if(mt5)
          {
            nrOfArguments++;
            callsize += mt5->getNetworkSize();
          }
        }
      }
    }
  }

  // now allocated mem if neccessary
  if( currentSize_ + callsize > currentMemBlocks_*FUNCTIONCALLS_MEM_ALLOCATION )
  {
    currentMemBlocks_ = (currentSize_ + callsize)%FUNCTIONCALLS_MEM_ALLOCATION+1;
    uint8_t *temp = new uint8_t[currentMemBlocks_*FUNCTIONCALLS_MEM_ALLOCATION];
    memcpy( temp, data_, currentSize_ );
    delete[] data_;
    data_ = temp;
  }

  // now serialise the mt values and copy the function id and isStatic
  uint8_t* temp = data_+currentSize_;
  *(uint32_t*)(data_+sizeof(uint32_t)) = *(uint32_t*)(data_+sizeof(uint32_t))+1; // increase number of calls
  *(uint32_t*)temp = networkID;
  *(uint8_t*)(temp+sizeof(uint32_t)) = true;
  *(uint32_t*)(temp+sizeof(uint32_t)+sizeof(uint8_t)) = nrOfArguments;
  temp += 2*sizeof(uint32_t)+sizeof(uint8_t);
  if(mt1)
  {
    mt1->exportData( temp ); //temp gets automatically increased
    if(mt2)
    {
      mt2->exportData( temp ); //temp gets automatically increased
      if(mt3)
      {
        mt3->exportData( temp ); //temp gets automatically increased
        if(mt4)
        {
          mt4->exportData( temp ); //temp gets automatically increased
          if(mt5)
          {
            mt5->exportData( temp ); //temp gets automatically increased
          }
        }
      }
    }
  }
  //currentSize_ += callsize;
  currentSize_ = temp-data_;

}

void FunctionCalls::addCallMember( uint32_t networkID, uint32_t objectID, const MultiType* mt1, const MultiType* mt2, const MultiType* mt3, const MultiType* mt4, const MultiType* mt5){
  assert(!isDataENetAllocated());

  // first determine the size that has to be reserved for this call
  uint32_t callsize = 3*sizeof(uint32_t)+sizeof(uint8_t); //size for network-function-id and nrOfArguments and the objectID
  uint32_t nrOfArguments = 0;
  if(mt1)
  {
    nrOfArguments++;
    callsize += mt1->getNetworkSize();
    if(mt2)
    {
      nrOfArguments++;
      callsize += mt2->getNetworkSize();
      if(mt3)
      {
        nrOfArguments++;
        callsize += mt3->getNetworkSize();
        if(mt4)
        {
          nrOfArguments++;
          callsize += mt4->getNetworkSize();
          if(mt5)
          {
            nrOfArguments++;
            callsize += mt5->getNetworkSize();
          }
        }
      }
    }
  }

  // now allocated mem if neccessary
  if( currentSize_ + callsize > currentMemBlocks_*FUNCTIONCALLS_MEM_ALLOCATION )
  {
    currentMemBlocks_ = (currentSize_ + callsize)%FUNCTIONCALLS_MEM_ALLOCATION+1;
    uint8_t *temp = new uint8_t[currentMemBlocks_*FUNCTIONCALLS_MEM_ALLOCATION];
    memcpy( temp, data_, currentSize_ );
    delete[] data_;
    data_ = temp;
  }

  // now serialise the mt values and copy the function id
  uint8_t* temp = data_+currentSize_;
  *(uint32_t*)(data_+sizeof(uint32_t)) = *(uint32_t*)(data_+sizeof(uint32_t))+1; // increase number of calls
  *(uint32_t*)temp = networkID;
  *(uint8_t*)(temp+sizeof(uint32_t)) = false;
  *(uint32_t*)(temp+sizeof(uint32_t)+sizeof(uint8_t)) = nrOfArguments;
  *(uint32_t*)(temp+2*sizeof(uint32_t)+sizeof(uint8_t)) = objectID;
  temp += 3*sizeof(uint32_t)+sizeof(uint8_t);
  if(mt1)
  {
    mt1->exportData( temp ); //temp gets automatically increased
    if(mt2)
    {
      mt2->exportData( temp ); //temp gets automatically increased
      if(mt3)
      {
        mt3->exportData( temp ); //temp gets automatically increased
        if(mt4)
        {
          mt4->exportData( temp ); //temp gets automatically increased
          if(mt5)
          {
            mt5->exportData( temp ); //temp gets automatically increased
          }
        }
      }
    }
  }
  currentSize_ += callsize;

}


} //namespace packet
} //namespace orxonox
