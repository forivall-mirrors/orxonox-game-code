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


#include "Acknowledgement.h"
#include "network/Host.h"
#include "network/GamestateHandler.h"

namespace network {
namespace packet {

#define PACKET_FLAGS_ACK    0
#define _PACKETID           0
#define _ACKID              _PACKETID + sizeof(network::packet::ENUM::Type)
  
Acknowledgement::Acknowledgement( unsigned int id, int clientID )
 : Packet()
{
  flags_ = flags_ | PACKET_FLAGS_ACK;
  data_=new unsigned char[ getSize() ];
  *(ENUM::Type *)(data_ + _PACKETID ) = ENUM::Acknowledgement;
  *(unsigned int *)(data_ + _ACKID ) = id;
  clientID_=clientID;
}

Acknowledgement::Acknowledgement( unsigned char *data, int clientID )
  : Packet(data, clientID)
{
}

Acknowledgement::~Acknowledgement()
{
}

unsigned int Acknowledgement::getSize() const{
  return _ACKID + sizeof(unsigned int);
}

bool Acknowledgement::process(){
  bool b = GamestateHandler::ackGamestate(data_[_ACKID], clientID_);
  delete this;
  return b;
}

unsigned int Acknowledgement::getAckID(){
  return *(unsigned int *)&data_[ _ACKID ];
}

} //namespace packet
} //namespace network
