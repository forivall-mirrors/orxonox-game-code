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
 *      Oliver Scheuss <scheusso [at] orxonox.net>, (C) 2010
 *   Co-authors:
 *      ...
 *
 */

#include "FunctionCall.h"

#include <cassert>
#include "util/MultiType.h"
#include "NetworkFunction.h"
#include "NetworkFunctionManager.h"

namespace orxonox {

FunctionCall::FunctionCall()
 : nrOfArguments_(-1), objectID_(OBJECTID_UNKNOWN), size_(0)
{
}

FunctionCall::~FunctionCall()
{
}


bool FunctionCall::execute(){
  if( this->bIsStatic_ )
  {
    NetworkFunctionStatic *fct = static_cast<NetworkFunctionStatic*>(NetworkFunctionManager::getFunction( this->functionID_ ));
    assert( this->nrOfArguments_==this->arguments_.size() );
    switch(this->nrOfArguments_)
    {
      case 0:
        fct->call();
        break;
      case 1:
        fct->call(this->arguments_[0]);
        break;
      case 2:
        fct->call(this->arguments_[0], this->arguments_[1]);
        break;
      case 3:
        fct->call(this->arguments_[0], this->arguments_[1], this->arguments_[2]);
        break;
      case 4:
        fct->call(this->arguments_[0], this->arguments_[1], this->arguments_[2], this->arguments_[3]);
        break;
      case 5:
        fct->call(this->arguments_[0], this->arguments_[1], this->arguments_[2], this->arguments_[3], this->arguments_[4]);
        break;
      default:
        assert(0);
    }
  }
  else // not a static function, so also handle with the objectID
  {
    NetworkMemberFunctionBase *fct = static_cast<NetworkMemberFunctionBase*>(NetworkFunctionManager::getFunction( this->functionID_ ));
    switch(this->nrOfArguments_)
    {
      case 0:
        if( !fct->call(this->objectID_) )
          return false;
        break;
      case 1:
        if( !fct->call(this->objectID_, this->arguments_[0]) )
          return false;
        break;
      case 2:
        if( !fct->call(this->objectID_, this->arguments_[0], this->arguments_[1]) )
          return false;
        break;
      case 3:
        if( !fct->call(this->objectID_, this->arguments_[0], this->arguments_[1], this->arguments_[2]) )
          return false;
        break;
      case 4:
        if( !fct->call(this->objectID_, this->arguments_[0], this->arguments_[1], this->arguments_[2], this->arguments_[3]) )
          return false;
        break;
      case 5:
        if( !fct->call(this->objectID_, this->arguments_[0], this->arguments_[1], this->arguments_[2], this->arguments_[3], this->arguments_[4]) )
          return false;
        break;
      default:
        assert(0);
    }
  }
  return true;
}

void FunctionCall::setCallStatic( uint32_t networkID, const MultiType* mt1, const MultiType* mt2, const MultiType* mt3, const MultiType* mt4, const MultiType* mt5){

  // first determine the size that has to be reserved for this call
  uint32_t callsize = 2*sizeof(uint32_t)+sizeof(uint8_t); //size for network-function-id and nrOfArguments and for bool isStatic
  uint32_t nrOfArguments = 0;
  if(mt1)
  {
    nrOfArguments++;
    callsize += mt1->getNetworkSize();
    this->arguments_.push_back(*mt1);
    if(mt2)
    {
      nrOfArguments++;
      callsize += mt2->getNetworkSize();
      this->arguments_.push_back(*mt2);
      if(mt3)
      {
        nrOfArguments++;
        callsize += mt3->getNetworkSize();
        this->arguments_.push_back(*mt3);
        if(mt4)
        {
          nrOfArguments++;
          callsize += mt4->getNetworkSize();
          this->arguments_.push_back(*mt4);
          if(mt5)
          {
            nrOfArguments++;
            callsize += mt5->getNetworkSize();
            this->arguments_.push_back(*mt5);
          }
        }
      }
    }
  }
  this->nrOfArguments_ = nrOfArguments;
  this->size_ = callsize;
  this->bIsStatic_ = true;
  this->functionID_ = networkID;
}

void FunctionCall::setCallMember( uint32_t networkID, uint32_t objectID, const MultiType* mt1, const MultiType* mt2, const MultiType* mt3, const MultiType* mt4, const MultiType* mt5){

  // first determine the size that has to be reserved for this call
  uint32_t callsize = 3*sizeof(uint32_t)+sizeof(uint8_t); //size for network-function-id and nrOfArguments and the objectID and bIsStatic
  uint32_t nrOfArguments = 0;
  if(mt1)
  {
    nrOfArguments++;
    callsize += mt1->getNetworkSize();
    this->arguments_.push_back(*mt1);
    if(mt2)
    {
      nrOfArguments++;
      callsize += mt2->getNetworkSize();
      this->arguments_.push_back(*mt2);
      if(mt3)
      {
        nrOfArguments++;
        callsize += mt3->getNetworkSize();
        this->arguments_.push_back(*mt3);
        if(mt4)
        {
          nrOfArguments++;
          callsize += mt4->getNetworkSize();
          this->arguments_.push_back(*mt4);
          if(mt5)
          {
            nrOfArguments++;
            callsize += mt5->getNetworkSize();
            this->arguments_.push_back(*mt5);
          }
        }
      }
    }
  }
  this->nrOfArguments_ = nrOfArguments;
  this->bIsStatic_ = false;
  this->functionID_ = networkID;
  this->size_ = callsize;
  this->objectID_ = objectID;
}

void FunctionCall::loadData(uint8_t*& mem)
{
  this->functionID_ = *(uint32_t*)mem;
  this->bIsStatic_ = *(uint8_t*)(mem+sizeof(uint32_t));
  this->nrOfArguments_ = *(uint32_t*)(mem+sizeof(uint32_t)+sizeof(uint8_t));
  if( this->bIsStatic_ )
  {
    mem += 2*sizeof(uint32_t)+sizeof(uint8_t);
  }
  else
  {
    this->objectID_ = *(uint32_t*)(mem+2*sizeof(uint32_t)+sizeof(uint8_t));
    mem += 3*sizeof(uint32_t)+sizeof(uint8_t);
  }
  for( unsigned int i=0; i<this->nrOfArguments_; ++i )
  {
    this->arguments_.push_back(MultiType());
    this->arguments_.back().importData(mem);
  }
}

void FunctionCall::saveData(uint8_t*& mem)
{
  // now serialise the mt values and copy the function id and isStatic
  *(uint32_t*)mem = this->functionID_;
  *(uint8_t*)(mem+sizeof(uint32_t)) = this->bIsStatic_;
  *(uint32_t*)(mem+sizeof(uint32_t)+sizeof(uint8_t)) = this->nrOfArguments_;
  if( this->bIsStatic_ )
  {
    mem += 2*sizeof(uint32_t)+sizeof(uint8_t);
  }
  else
  {
    *(uint32_t*)(mem+2*sizeof(uint32_t)+sizeof(uint8_t)) = this->objectID_;
    mem += 3*sizeof(uint32_t)+sizeof(uint8_t);
  }
  for( std::vector<MultiType>::iterator it = this->arguments_.begin(); it!=this->arguments_.end(); ++it )
  {
    it->exportData( mem );
  }
}



} //namespace orxonox
