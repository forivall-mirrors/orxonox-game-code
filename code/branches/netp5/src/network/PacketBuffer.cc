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
 *      Oliver Scheuss, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

// C++ PacketBuffer
// d
// Author Oliver Scheuss

#include "PacketBuffer.h"

//#include <iostream>
#include <boost/thread/recursive_mutex.hpp>

namespace orxonox
{
  static boost::recursive_mutex packetBuffer_mutex_g;

  PacketBuffer::PacketBuffer() {
    closed=false;
    first=NULL;
    last=NULL;
  }
  //this is needed in order to make the packetbuffer threadsafe


  bool PacketBuffer::push(ENetEvent *ev) {
    boost::recursive_mutex::scoped_lock lock(packetBuffer_mutex_g);
    //std::cout << "event size inside packetbuffer " << ev->packet->dataLength << std::endl;
    //   if(closed)
    //     return false;
    // first element?
    if(first==NULL){
      first=new QueueItem;
      last=first;
      last->next=NULL;
      // change this!!!!!!!  ---- we are not doing stl so we won't change this
      last->event = ev;
      //last->address = ev->peer->address;
    } else {
      //insert a new element at the bottom
      last->next = new QueueItem;
      last=last->next;
      // initialize last->next
      last->next=NULL;
      // save the packet to the new element
      last->event = ev;
      //last->address = ev->peer->address;
    }
    lock.unlock();
    return true;
  }

  //returns the first element in the list without deleting it but
  //moving first pointer to next element
  /*ENetPacket *PacketBuffer::pop() {
    ENetAddress address;
    return pop(address);
  }*/
  
  ENetEvent *PacketBuffer::pop(){
    boost::recursive_mutex::scoped_lock lock(packetBuffer_mutex_g);
    //std::cout << "packetbuffer pop(address)" << std::endl;
    if(first!=NULL /*&& !closed*/){
      QueueItem *temp = first;
      // get packet
      ENetEvent *ev=first->event;
      //address = first->address;
      // remove first element
      first = first->next;
      delete temp;
      lock.unlock();
      //std::cout << "pop(address) size of packet " << pck->dataLength << std::endl;
      return ev;
    } else{
      lock.unlock();
      return NULL;
    }
  }

  /*ENetPacket *PacketBuffer::pop(ENetAddress &address) {
    boost::recursive_mutex::scoped_lock lock(packetBuffer_mutex_g);
    //std::cout << "packetbuffer pop(address)" << std::endl;
    if(first!=NULL ){
      QueueItem *temp = first;
      // get packet
      ENetPacket *pck=first->event->packet;
      address = first->event->peer->address;
      // remove first element
      first = first->next;
      delete temp;
      lock.unlock();
      //std::cout << "pop(address) size of packet " << pck->dataLength << std::endl;
      return pck;
    } else{
      lock.unlock();
      return NULL;
    }
  }*/

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

} // namespace orxonox
