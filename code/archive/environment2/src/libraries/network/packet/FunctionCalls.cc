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
#include "network/FunctionCall.h"
#include "network/FunctionCallManager.h"
#include "network/GamestateHandler.h"
#include "network/Host.h"

namespace orxonox {
namespace packet {

#define   PACKET_FLAGS_FUNCTIONCALLS PacketFlag::Reliable
#define   _PACKETID         0
const unsigned int FUNCTIONCALLS_MEM_ALLOCATION = 1000;

FunctionCalls::FunctionCalls():
  Packet(), minGamestateID_(GAMESTATEID_INITIAL)
{
  flags_ = flags_ | PACKET_FLAGS_FUNCTIONCALLS;
  currentSize_ = 3*sizeof(uint32_t); // for packetid, nrOfCalls and minGamestateID_
}

FunctionCalls::FunctionCalls( uint8_t* data, unsigned int clientID ): 
  Packet(data, clientID), minGamestateID_(GAMESTATEID_INITIAL)
{
}

FunctionCalls::~FunctionCalls()
{
}


bool FunctionCalls::process(orxonox::Host* host)
{
  assert(isDataENetAllocated());
  
  uint8_t* temp = data_+sizeof(uint32_t); //skip packetid
  uint32_t nrOfCalls = *(uint32_t*)temp;
  temp += sizeof(uint32_t);
  this->minGamestateID_ = *(uint32_t*)temp;
  temp += sizeof(uint32_t);
  for( unsigned int i = 0; i<nrOfCalls; i++ )
  {
    FunctionCall fctCall;
    fctCall.loadData(temp);
    if( !fctCall.execute() )
    {
      FunctionCallManager::bufferIncomingFunctionCall( fctCall, minGamestateID_, this->getPeerID() );
    }
  }
  
  delete this;
  return true;
}

void FunctionCalls::addCallStatic( uint32_t networkID, const MultiType* mt1, const MultiType* mt2, const MultiType* mt3, const MultiType* mt4, const MultiType* mt5)
{
  assert(!isDataENetAllocated());
  
  this->functionCalls_.push(orxonox::FunctionCall());
  this->functionCalls_.back().setCallStatic( networkID, mt1, mt2, mt3, mt4, mt5 );
  this->currentSize_ += this->functionCalls_.back().getSize();
}

void FunctionCalls::addCallMember( uint32_t networkID, uint32_t objectID, const MultiType* mt1, const MultiType* mt2, const MultiType* mt3, const MultiType* mt4, const MultiType* mt5)
{
  assert(!isDataENetAllocated());
  
  this->functionCalls_.push(orxonox::FunctionCall());
  this->functionCalls_.back().setCallMember( networkID, objectID, mt1, mt2, mt3, mt4, mt5 );
  this->currentSize_ += this->functionCalls_.back().getSize();
}

bool FunctionCalls::send(orxonox::Host* host)
{
  this->minGamestateID_ = host->getCurrentGamestateID();
  assert(this->functionCalls_.size());
  data_=new uint8_t[ currentSize_ ];
  *(Type::Value *)(data_ + _PACKETID ) = Type::FunctionCalls; // Set the Packet ID
  *(uint32_t*)(data_+sizeof(uint32_t)) = this->functionCalls_.size(); // set nrOfCalls
  *(uint32_t*)(data_+2*sizeof(uint32_t)) = this->minGamestateID_; // set minGamestateID_
  uint8_t* temp = data_+3*sizeof(uint32_t);
  
  while( this->functionCalls_.size() )
  {
    this->functionCalls_.front().saveData( temp );
    this->functionCalls_.pop();
  }
  
  assert( temp==data_+currentSize_ );
  
  Packet::send(host);
  return true;
}



} //namespace packet
} //namespace orxonox
