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
#include <enet/enet.h>
#include "network/Synchronisable.h"
#include "core/CoreIncludes.h"
#include <assert.h>

namespace network {
namespace packet {

#define PACKET_FLAGS_DELETE    ENET_PACKET_FLAG_RELIABLE
#define _PACKETID           0
#define _QUANTITY           _PACKETID + sizeof(ENUM::Type)
#define _OBJECTIDS          _QUANTITY + sizeof(unsigned int)
  
DeleteObjects::DeleteObjects()
 : Packet()
{
  flags_ = flags_ | PACKET_FLAGS_DELETE;
}

DeleteObjects::DeleteObjects( unsigned char *data, int clientID )
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
  unsigned int size = sizeof(ENUM::Type) + sizeof(unsigned int)*number;
  data_ = new unsigned char[size];
  *(ENUM::Type *)(data_ + _PACKETID ) = ENUM::DeleteObjects;
  *(unsigned int *)(data_ + _QUANTITY) = number;
  for(unsigned int i=0; i<number; i++){
    *(unsigned int *)(data_ + sizeof(ENUM::Type) + i*sizeof(unsigned int)) = Synchronisable::popDeletedObject();
  }
  return true;
}

unsigned int DeleteObjects::getSize() const{
  assert(data_);
  return _OBJECTIDS + sizeof(unsigned int) + *(unsigned int*)(data_+_QUANTITY);
}

bool DeleteObjects::process(){
  for(unsigned int i=0; i<*(unsigned int *)(data_+_QUANTITY); i++){
    COUT(3) << "deleting object with id: " << *(unsigned int*)(data_+_OBJECTIDS+i*sizeof(unsigned int)) << std::endl;
    Synchronisable::deleteObject( *(unsigned int*)(data_+_OBJECTIDS+i*sizeof(unsigned int)) );
  }
  return true;
}


} //namespace packet
} //namespace network
