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


#include "DeleteObjects.h"

#include <cassert>
#include "util/Debug.h"
#include "network/synchronisable/Synchronisable.h"

namespace orxonox {
namespace packet {

#define PACKET_FLAG_DELETE  PacketFlag::Reliable
#define _PACKETID           0
#define _QUANTITY           _PACKETID + sizeof(Type::Value)
#define _OBJECTIDS          _QUANTITY + sizeof(uint32_t)

DeleteObjects::DeleteObjects()
 : Packet()
{
  flags_ = flags_ | PACKET_FLAG_DELETE;
}

DeleteObjects::DeleteObjects( uint8_t *data, unsigned int clientID )
  : Packet(data, clientID)
{
}

DeleteObjects::~DeleteObjects()
{
}

bool DeleteObjects::fetchIDs(){
  unsigned int number = Synchronisable::getNumberOfDeletedObject();
  if(number==0)
    return false;
  COUT(4) << "sending DeleteObjects: ";
  unsigned int size = sizeof(Type::Value) + sizeof(uint32_t)*(number+1);
  data_ = new uint8_t[size];
  uint8_t *tdata = data_;
  *reinterpret_cast<Type::Value*>(tdata) = Type::DeleteObjects;
  tdata += sizeof(Type::Value);
  *(uint32_t *)tdata = number;
  tdata += sizeof(uint32_t);
  for(unsigned int i=0; i<number; i++){
    unsigned int temp = Synchronisable::popDeletedObject();
    *reinterpret_cast<uint32_t*>(tdata) = temp;
    COUT(4) << temp << ' ';
    tdata += sizeof(uint32_t);
  }
  COUT(4) << std::endl;
  return true;
}

unsigned int DeleteObjects::getSize() const{
  assert(data_);
  return _OBJECTIDS + *(uint32_t*)(data_+_QUANTITY)*sizeof(uint32_t);
}

bool DeleteObjects::process(){
  for(unsigned int i=0; i<*(unsigned int *)(data_+_QUANTITY); i++){
    COUT(4) << "deleting object with id: " << *(uint32_t*)(data_+_OBJECTIDS+i*sizeof(uint32_t)) << std::endl;
    Synchronisable::deleteObject( *(uint32_t*)(data_+_OBJECTIDS+i*sizeof(uint32_t)) );
  }
  delete this;
  return true;
}


} //namespace packet
} //namespace orxonox
