

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

#include "Welcome.h"

#include <cassert>
#include "util/Debug.h"
#include "network/Host.h"
#include "network/synchronisable/Synchronisable.h"

namespace orxonox {
namespace packet {

#define PACKET_FLAGS_CLASSID  PacketFlag::Reliable
#define _PACKETID             0
#define _CLIENTID             _PACKETID + sizeof(Type::Value)
#define _ENDIANTEST           _CLIENTID + sizeof(uint32_t)

  Welcome::Welcome( uint32_t clientID, uint32_t shipID )
 : Packet()
{
  flags_ = flags_ | PACKET_FLAGS_CLASSID;
  assert(getSize());
  data_=new uint8_t[ getSize() ];
  assert(data_);
  *(packet::Type::Value *)(data_ + _PACKETID ) = packet::Type::Welcome;
  *(uint32_t *)(data_ + _CLIENTID ) = static_cast<uint32_t>(clientID);
  *(uint32_t *)(data_ + _ENDIANTEST ) = 0xFEDC4321;
}

Welcome::Welcome( uint8_t* data, uint32_t clientID )
  : Packet(data, clientID)
{
}

Welcome::~Welcome()
{
}

uint8_t *Welcome::getData(){
  return data_;
}

unsigned int Welcome::getSize() const{
  return sizeof(packet::Type::Value) + 2*sizeof(uint32_t);
}

bool Welcome::process(){
  uint32_t clientID;
  clientID = *(uint32_t *)(data_ + _CLIENTID );
  assert(*(uint32_t *)(data_ + _ENDIANTEST ) == 0xFEDC4321);
  Host::setClientID(clientID);
  COUT(3) << "Welcome set clientId: " << clientID << endl;
  Synchronisable::setClient(true);
  delete this;
  return true;
}


} //namespace packet
}//namespace orxonox
