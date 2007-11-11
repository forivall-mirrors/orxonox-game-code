// C++ PacketBuffer
// d
// Author Oliver Scheuss

#ifndef NETWORK_PACKETBUFFER_CC
#define NETWORK_PACKETBUFFER_CC

#include <iostream>
#include "network/PacketBuffer.h"

namespace network{
  
   boost::mutex networkPacketBufferMutex;
  
PacketBuffer::PacketBuffer(){
  closed=false;
  first=NULL;
  last=NULL;
}
    //this is needed in order to make the packetbuffer threadsafe
  

bool PacketBuffer::push(ENetPacket *pck){
  boost::mutex::scoped_lock lock(networkPacketBufferMutex);
//   if(closed)
//     return false;
  // also works if fifo is null (queue empty)
  // just to be sure last is really the last element
  /*if(last!=NULL)
    while(last->next!=NULL)
      last=last->next;*/
  // first element?
  if(first==NULL){
    first=new QueueItem;
    last=first;
    last->next=NULL;
    // change this!!!!!!!
    last->packet = pck;
    } else {
    //insert a new element at the bottom
    last->next = new QueueItem;
    last=last->next;
    // initialize last->next
    last->next=NULL;
    // save the packet to the new element
    last->packet = pck;
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

bool PacketBuffer::isEmpty(){
  return (first==NULL);
}

void PacketBuffer::print(){
  QueueItem *temp=first;
  while(temp!=NULL){
    std::cout << temp->packet->data << std::endl;
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
