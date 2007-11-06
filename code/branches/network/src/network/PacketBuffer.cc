// C++ PacketBuffer
// 
// Author Oliver Scheuss
#include <iostream>
#include "network/PacketBuffer.h"

namespace network{

PacketBuffer::PacketBuffer(){
  locked=false;
  first=NULL;
  last=NULL;
}

bool PacketBuffer::push(PacketEnvelope pck){
  if(!locked){
    locked=true;
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
    locked=false;
    return true;
  }
  else
    return false;
}

PacketEnvelope *PacketBuffer::pop(){
  if(!locked && first!=NULL){
    locked = true;
    QueueItem *temp = first;
    // get packet
    PacketEnvelope *p = first->packet;
    // remove first element
    first = first->next;
    delete temp;
    locked=false;
    return p;
  } else
    return NULL;
}

bool PacketBuffer::isEmpty(){
  return (first==NULL);
}

bool PacketBuffer::isLocked(){
  return locked;
}

void PacketBuffer::print(){
  QueueItem *temp=first;
  while(temp!=NULL){
    std::cout << temp->packet->data << std::endl;
    temp=temp->next;
  }
  
}

}// namespace network
