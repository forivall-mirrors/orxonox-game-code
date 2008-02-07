/*
*   ORXONOX - the hottest 3D action shooter ever to exist
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
*      Oliver Scheuss, (C) 2007
*   Co-authors:
*      ...
*
*/

// C++ PacketBuffer
// d
// Author Oliver Scheuss

#include <iostream>
#include <queue>
#include <string>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>

#include "PacketBuffer.h"

namespace network
{
  boost::mutex networkPacketBufferMutex;

  PacketBuffer::PacketBuffer() {
    closed=false;
    first=NULL;
    last=NULL;
  }
  //this is needed in order to make the packetbuffer threadsafe


  bool PacketBuffer::push(ENetEvent *ev) {
    boost::mutex::scoped_lock lock(networkPacketBufferMutex);
    //std::cout << "event size inside packetbuffer " << ev->packet->dataLength << std::endl;
    //   if(closed)
    //     return false;
    // first element?
    if(first==NULL){
      first=new QueueItem;
      last=first;
      last->next=NULL;
      // change this!!!!!!!
      last->packet = ev->packet;
      last->address = ev->peer->address;
    } else {
      //insert a new element at the bottom
      last->next = new QueueItem;
      last=last->next;
      // initialize last->next
      last->next=NULL;
      // save the packet to the new element
      last->packet = ev->packet;
      last->address = ev->peer->address;
    }
    return true;
  }

  ENetPacket *PacketBuffer::pop() {
    boost::mutex::scoped_lock lock(networkPacketBufferMutex);
    //std::cout << "packetbuffer pop" << std::endl;
    if(first!=NULL /*&& !closed*/){
      QueueItem *temp = first;
      // get packet
      ENetPacket *pck=first->packet;
      // remove first element
      first = first->next;
      delete temp;
      //std::cout << "pop size of packet " << pck->dataLength << std::endl;
      return pck;
    } else{
      //std::cout << "nothing to return" << std::endl;
      return NULL;
    }
  }

  ENetPacket *PacketBuffer::pop(ENetAddress &address) {
    boost::mutex::scoped_lock lock(networkPacketBufferMutex);
    //std::cout << "packetbuffer pop(address)" << std::endl;
    if(first!=NULL /*&& !closed*/){
      QueueItem *temp = first;
      // get packet
      ENetPacket *pck=first->packet;
      address = first->address;
      // remove first element
      first = first->next;
      delete temp;
      //std::cout << "pop(address) size of packet " << pck->dataLength << std::endl;
      return pck;
    } else{
      return NULL;
    }
  }

  bool PacketBuffer::isEmpty() {
    return (first==NULL);
  }

  void PacketBuffer::print() {
    QueueItem *temp=first;
    while(temp!=NULL){
      //    std::cout << temp->packet->data << std::endl;
      temp=temp->next;
    }

  }

  bool PacketBuffer::isClosed() {
    return closed;
  }

  void PacketBuffer::setClosed(bool value){
    closed=value;
    return;
  }

} // namespace network
