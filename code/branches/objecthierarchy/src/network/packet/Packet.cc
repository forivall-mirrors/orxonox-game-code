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
#include "DeleteObjects.h"
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
  bDataENetAllocated_ = false;
}

void blub(ENetPacket *packet){
  COUT(4) << "blubb" << std::endl;
}

Packet::Packet(uint8_t *data, unsigned int clientID){
  flags_ = PACKET_FLAG_DEFAULT;
  packetDirection_ = ENUM::Incoming;
  clientID_=clientID;
  data_=data;
  enetPacket_=0;
  bDataENetAllocated_ = false;
}


Packet::Packet(const Packet &p){
  enetPacket_=p.enetPacket_;
  flags_=p.flags_;
  packetDirection_ = p.packetDirection_;
  clientID_ = p.clientID_;
  if(p.data_){
    data_ = new uint8_t[p.getSize()];
    memcpy(data_, p.data_, p.getSize());
  }else
    data_=0;
  bDataENetAllocated_ = p.bDataENetAllocated_;
}

/**
@brief
    Destroys a packet completely.
    
    That also means destroying the ENetPacket if one exists. There 
*/
Packet::~Packet(){
  // Deallocate data_ memory if necessary.
  if (this->bDataENetAllocated_){
    // In this case ENet allocated data_ and will destroy it.
  }
  else if (this->data_) {
    // This destructor was probably called as a consequence to ENet executing our callback.
    // It simply serves us to be able to deallocate the packet content (data_) ourselves since
    // we have created it in the first place.
    delete[] this->data_;
  }

  // Destroy the ENetPacket if necessary.
  // Note: For the case ENet used the callback to destroy the packet, we have already set
  // enetPacket_ to NULL to avoid destroying it again.
  if (this->enetPacket_){
    // enetPacket_->data gets destroyed too by ENet if it was allocated by it.
    enet_packet_destroy(enetPacket_);
  }
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
    // We deliver ENet the data address so that it doesn't memcpy everything again.
    // --> We have to delete data_ ourselves!
    enetPacket_ = enet_packet_create(getData(), getSize(), getFlags());
    enetPacket_->freeCallback = &Packet::deletePacket;
    // Add the packet to a global list so we can access it again once enet calls our
    // deletePacket method. We can of course only give a one argument function to the ENet C library.
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
    case ENUM::DeleteObjects:
      break;
    default:
      assert(0); //there was some error, if this is the case
      break;
  }
#endif
//  ENetPacket *temp = enetPacket_;
//  enetPacket_ = 0; // otherwise we have a double free because enet already handles the deallocation of the packet
  network::Host::addPacket( enetPacket_, clientID_);
  return true;
}

Packet *Packet::createPacket(ENetPacket *packet, ENetPeer *peer){
  uint8_t *data = packet->data;
  assert(ClientInformation::findClient(&peer->address)->getID() != (unsigned int)-2 || !Host::isServer());
  unsigned int clientID = ClientInformation::findClient(&peer->address)->getID();
  Packet *p;
  COUT(5) << "packet type: " << *(ENUM::Type *)&data[_PACKETID] << std::endl;
  switch( *(ENUM::Type *)(data + _PACKETID) )
  {
    case ENUM::Acknowledgement:
      COUT(4) << "ack" << std::endl;
      p = new Acknowledgement( data, clientID );
      break;
    case ENUM::Chat:
      COUT(4) << "chat" << std::endl;
      p = new Chat( data, clientID );
      break;
    case ENUM::ClassID:
      COUT(4) << "classid" << std::endl;
      p = new ClassID( data, clientID );
      break;
    case ENUM::Gamestate:
      COUT(4) << "gamestate" << std::endl;
      // TODO: remove brackets
      p = new Gamestate( data, clientID );
      break;
    case ENUM::Welcome:
      COUT(4) << "welcome" << std::endl;
      p = new Welcome( data, clientID );
      break;
    case ENUM::DeleteObjects:
      COUT(4) << "deleteobjects" << std::endl;
      p = new DeleteObjects( data, clientID );
      break;
    default:
      assert(0); //TODO: repair this
      break;
  }

  // Data was created by ENet
  p->bDataENetAllocated_ = true;

  return p;
}

/**
@brief
    ENet calls this method whenever it wants to destroy a packet that contains
    data we allocated ourselves.
*/
void Packet::deletePacket(ENetPacket *enetPacket){
  // Get our Packet from a gloabal map with all Packets created in the send() method of Packet.
  std::map<ENetPacket*, Packet*>::iterator it = packetMap_.find(enetPacket);
  assert(it != packetMap_.end());
  // Make sure we don't delete it again in the destructor
  it->second->enetPacket_ = 0;
  delete it->second;
  packetMap_.erase(it);
}

} // namespace packet

} // namespace network

