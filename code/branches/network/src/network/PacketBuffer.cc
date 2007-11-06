// C++ PacketBuffer
// 
// Author Oliver Scheuss
#include <iostream>
#include "network/PacketBuffer.h"

namespace network{

PacketBuffer::PacketBuffer(){
  closed=false;
  first=NULL;
  last=NULL;
}

bool PacketBuffer::push(PacketEnvelope pck){
  boost::mutex::scoped_lock lock(networkPacketBufferMutex);
  // also works if fifo is null (queue empty)
  // just to be sure last is really the last element
  while(last!=NULL && last->next!=NULL)
    last=last->next;
  // first element?
  if(first==NULL){
    first=new QueueItem;
    last=first;
    last->next=NULL;
    // change this!!!!!!!
    last->packet = new PacketEnvelope;
    last->packet->data = pck.data;
    last->packet->length = pck.length;
  } else {
    //insert a new element at the bottom
    last->next = new QueueItem;
    last=last->next;
    // initialize last->next
    last->next=NULL;
    // save the packet to the new element
    // change this!!!!!!!!
    last->packet = new PacketEnvelope;
    last->packet->data = pck.data;
    last->packet->length = pck.length;
  }
  return true;
}

PacketEnvelope PacketBuffer::pop(){
  boost::mutex::scoped_lock lock(networkPacketBufferMutex);
  if(first!=NULL){
    QueueItem *temp = first;
    // get packet
    PacketEnvelope *p = first->packet;
    // remove first element
    first = first->next;
    delete temp;
    return *p;
  } else{
    PacketEnvelope p = {0,0};
    return p;
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
