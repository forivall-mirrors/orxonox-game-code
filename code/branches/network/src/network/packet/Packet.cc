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


#include <enet/enet.h>
#include <boost/bind.hpp>

#include "Packet.h"
#include "network/ConnectionManager.h"
#include "network/ClientInformation.h"

#include "Acknowledgement.h"
#include "Chat.h"
#include "ClassID.h"
#include "Gamestate.h"
#include "Welcome.h"
#include "network/Host.h"
#include "core/CoreIncludes.h"

namespace network{

namespace packet{
  
#define PACKET_FLAG_DEFAULT ENET_PACKET_FLAG_NO_ALLOCATE
#define _PACKETID           0
  
std::map<ENetPacket *, Packet *> Packet::packetMap_;
  
Packet::Packet(){
  flags_ = PACKET_FLAG_DEFAULT;
  packetDirection_ = ENUM::Outgoing;
  clientID_=0;
  data_=0;
  enetPacket_=0;
}

Packet::Packet(unsigned char *data, int clientID){
  flags_ = PACKET_FLAG_DEFAULT;
  packetDirection_ = ENUM::Outgoing;
  clientID_=clientID;
  data_=data;
  enetPacket_=0;
}

Packet::Packet(ENetPacket *packet, ENetPeer *peer){
  packetDirection_ = ENUM::Incoming;
  enetPacket_ = packet;
  clientID_ = ClientInformation::findClient(&peer->address)->getID();
  data_ = packet->data;
  enet_packet_destroy(packet);
}

Packet::Packet(const Packet &p){
  flags_=p.flags_;
  if(p.data_){
    data_ = new unsigned char[p.getSize()];
    memcpy(data_, p.data_, p.getSize());
  }else
    data_=0;
}

Packet::~Packet(){
  if(enetPacket_)
    enet_packet_destroy(enetPacket_);
  if(data_)
    delete[] data_;
}

bool Packet::send(){
  if(packetDirection_ != ENUM::Outgoing && packetDirection_ != ENUM::Bidirectional )
    return false;
  if(!enetPacket_){
    if(!data_)
      return false;
    enetPacket_ = enet_packet_create(getData(), getSize(), getFlags());
    enetPacket_->freeCallback = &Packet::deletePacket;
    packetMap_[enetPacket_] = this;
  }
  network::Host::addPacket( enetPacket_, clientID_);
  enetPacket_ = 0; // otherwise we have a double free because enet already handles the deallocation of the packet
  return true;
}

Packet *Packet::createPacket(ENetPacket *packet, ENetPeer *peer){
  unsigned char *data = packet->data;
  unsigned int clientID = ClientInformation::findClient(&peer->address)->getID();
  Packet *p;
  switch( *(ENUM::Type *)&data[_PACKETID] )
  {
    case ENUM::Acknowledgement:
      p = new Acknowledgement( data, clientID );
      break;
    case ENUM::Chat:
      p = new Chat( data, clientID );
      break;
    case ENUM::ClassID:
      p = new ClassID( data, clientID );
      break;
    case ENUM::Gamestate:
      // TODO: remove brackets
      p = new Gamestate( data, true, clientID );
      break;
    case ENUM::Welcome:
      p = new Welcome( data, clientID );
    default:
      assert(0); //TODO: repair this
      break;
  }
  return p;
}

void Packet::deletePacket(ENetPacket *packet){
  assert(packetMap_[packet]);
  delete packetMap_[packet];
}

} // namespace packet

} // namespace network

