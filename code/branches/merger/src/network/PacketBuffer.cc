// C++ PacketBuffer
// d
// Author Oliver Scheuss

#ifndef NETWORK_PACKETBUFFER_CC
#define NETWORK_PACKETBUFFER_CC

#include <iostream>
#include "PacketBuffer.h"

namespace network{

   boost::mutex networkPacketBufferMutex;

PacketBuffer::PacketBuffer(){
  closed=false;
  first=NULL;
  last=NULL;
}
    //this is needed in order to make the packetbuffer threadsafe


bool PacketBuffer::push(ENetEvent *ev){
  boost::mutex::scoped_lock lock(networkPacketBufferMutex);
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

ENetPacket *PacketBuffer::pop(){
  boost::mutex::scoped_lock lock(networkPacketBufferMutex);
  if(first!=NULL /*&& !closed*/){
    QueueItem *temp = first;
    // get packet
    ENetPacket *pck=first->packet;
    // remove first element
    first = first->next;
    delete temp;
    return pck;
  } else{
    return NULL;
  }
}

ENetPacket *PacketBuffer::pop(ENetAddress &address){
  boost::mutex::scoped_lock lock(networkPacketBufferMutex);
  if(first!=NULL /*&& !closed*/){
    QueueItem *temp = first;
    // get packet
    ENetPacket *pck=first->packet;
    address = first->address;
    // remove first element
    first = first->next;
    delete temp;
    return pck;
  } else{
    return NULL;
  }
}

bool PacketBuffer::isEmpty(){
  return (first==NULL);
}

void PacketBuffer::print(){
  QueueItem *temp=first;
  while(temp!=NULL){
//    std::cout << temp->packet->data << std::endl;
    temp=temp->next;
  }

}

bool PacketBuffer::isClosed(){
  return closed;
}

void PacketBuffer::setClosed(bool value){
  closed=value;
  return;
}

}// namespace network

#endif
