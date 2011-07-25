

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
#include "network/Host.h"
#include "network/Synchronisable.h"
#include "core/CoreIncludes.h"
#include <assert.h>

namespace network {
namespace packet {

#define PACKET_FLAGS_CLASSID  ENET_PACKET_FLAG_RELIABLE
#define _PACKETID             0
#define _CLIENTID             _PACKETID + sizeof(ENUM::Type)
#define _SHIPID               _CLIENTID + sizeof(unsigned int)
  
  Welcome::Welcome( unsigned int clientID, unsigned int shipID )
 : Packet()
{
  flags_ = flags_ | PACKET_FLAGS_CLASSID;
  assert(getSize());
  data_=new unsigned char[ getSize() ];
  assert(data_);
  *(packet::ENUM::Type *)(data_ + _PACKETID ) = packet::ENUM::Welcome;
  *(unsigned int *)&data_[ _CLIENTID ] = clientID;
  *(unsigned int *)&data_[ _SHIPID ] = shipID;
}

Welcome::Welcome( unsigned char *data, int clientID )
  : Packet(data, clientID)
{
}

Welcome::~Welcome()
{
}

unsigned char *Welcome::getData(){
  return data_;
}

unsigned int Welcome::getSize() const{
  return sizeof(network::packet::ENUM::Type) + 2*sizeof(unsigned int);
}

bool Welcome::process(){
  unsigned int shipID, clientID;
  clientID = *(unsigned int *)&data_[ _CLIENTID ];
  shipID = *(unsigned int *)&data_[ _SHIPID ];
  Host::setClientID(clientID);
  Host::setShipID(shipID);
  COUT(3) << "Welcome set clientId: " << clientID << " shipID: " << shipID << std::endl;
  Synchronisable::setClient(true);
  delete this;
  return true;
}


} //namespace packet
}//namespace network
