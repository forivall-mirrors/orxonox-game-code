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


#include "Packet.h"

#include <cassert>
#include <enet/enet.h>
#include <boost/bind.hpp>

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

void blub(ENetPacket *packet){
  COUT(4) << "blubb" << std::endl;
}

Packet::Packet(unsigned char *data, int clientID){
  flags_ = PACKET_FLAG_DEFAULT;
  packetDirection_ = ENUM::Incoming;
  clientID_=clientID;
  data_=data;
  enetPacket_=0;
}

/*Packet::Packet(ENetPacket *packet, ENetPeer *peer){
  packetDirection_ = ENUM::Incoming;
  enetPacket_ = packet;
  clientID_ = ClientInformation::findClient(&peer->address)->getID();
  data_ = packet->data;
}*/

Packet::Packet(const Packet &p){
  enetPacket_=p.enetPacket_;
  flags_=p.flags_;
  packetDirection_ = p.packetDirection_;
  clientID_ = p.clientID_;
  if(p.data_){
    data_ = new unsigned char[p.getSize()];
    memcpy(data_, p.data_, p.getSize());
  }else
    data_=0;
}

Packet::~Packet(){
  if(enetPacket_){
    assert(enetPacket_->freeCallback==0);
    enet_packet_destroy(enetPacket_);
  }
  if(data_)
    delete[] data_;
}

bool Packet::send(){
  if(packetDirection_ != ENUM::Outgoing && packetDirection_ != ENUM::Bidirectional ){
    assert(0);
    return false;
  }
  if(!enetPacket_){
    if(!data_){
      assert(0);
      return false;
    }
    enetPacket_ = enet_packet_create(getData(), getSize(), getFlags());
    enetPacket_->freeCallback = &Packet::deletePacket;
//     enetPacket_->freeCallback = &blub;
    packetMap_[enetPacket_] = this;
  }
#ifndef NDEBUG 
  switch( *(ENUM::Type *)(data_ + _PACKETID) )
  {
    case ENUM::Acknowledgement:
    case ENUM::Chat:
    case ENUM::ClassID:
    case ENUM::Gamestate:
    case ENUM::Welcome:
      break;
    default:
      assert(0); //there was some error, if this is the case
      break;
  }
#endif
  ENetPacket *temp = enetPacket_;
  enetPacket_ = 0; // otherwise we have a double free because enet already handles the deallocation of the packet
  network::Host::addPacket( temp, clientID_);
  return true;
}

Packet *Packet::createPacket(ENetPacket *packet, ENetPeer *peer){
  unsigned char *data = packet->data;
  unsigned int clientID = ClientInformation::findClient(&peer->address)->getID();
  Packet *p;
  COUT(3) << "packet type: " << *(ENUM::Type *)&data[_PACKETID] << std::endl;
  switch( *(ENUM::Type *)(data + _PACKETID) )
  {
    case ENUM::Acknowledgement:
      COUT(3) << "ack" << std::endl;
      p = new Acknowledgement( data, clientID );
      break;
    case ENUM::Chat:
      COUT(3) << "chat" << std::endl;
      p = new Chat( data, clientID );
      break;
    case ENUM::ClassID:
      COUT(3) << "classid" << std::endl;
      p = new ClassID( data, clientID );
      break;
    case ENUM::Gamestate:
      COUT(3) << "gamestate" << std::endl;
      // TODO: remove brackets
      p = new Gamestate( data, clientID );
      break;
    case ENUM::Welcome:
      COUT(3) << "welcome" << std::endl;
      p = new Welcome( data, clientID );
      break;
    default:
      assert(0); //TODO: repair this
      break;
  }
  return p;
}

void Packet::deletePacket(ENetPacket *packet){
  assert(packetMap_[packet]);
  assert(packetMap_[packet]->enetPacket_==0);
  delete packetMap_[packet];
}

} // namespace packet

} // namespace network

