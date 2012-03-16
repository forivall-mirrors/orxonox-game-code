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

#include "util/Output.h"
#include "network/GamestateHandler.h"
#include "network/Host.h"

namespace orxonox {
namespace packet {

#define PACKET_FLAGS_ACK    0
#define _PACKETID           0
#define _ACKID              _PACKETID + sizeof(packet::Type::Value)

Acknowledgement::Acknowledgement( unsigned int id, unsigned int peerID )
 : Packet()
{
  flags_ = flags_ | PACKET_FLAGS_ACK;
  data_=new uint8_t[ getSize() ];
  *(Type::Value *)(data_ + _PACKETID ) = Type::Acknowledgement;
  *(uint32_t *)(data_ + _ACKID ) = id;
  peerID_=peerID;
}

Acknowledgement::Acknowledgement( uint8_t *data, unsigned int peerID )
  : Packet(data, peerID)
{
}

Acknowledgement::~Acknowledgement()
{
}

unsigned int Acknowledgement::getSize() const{
  return _ACKID + sizeof(uint32_t);
}

bool Acknowledgement::process(orxonox::Host* host){
  orxout(verbose_more, context::packets) << "processing ACK with ID: " << getAckID() << endl;
  bool b = host->ackGamestate(getAckID(), peerID_);
  delete this;
  return b;
}

unsigned int Acknowledgement::getAckID(){
  return *(uint32_t *)(data_ + _ACKID);
}

} //namespace packet
} //namespace orxonox
