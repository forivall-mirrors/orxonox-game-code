//
// C++ Interface: PacketBuffer
//
// Description: 
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef NETWORK_PACKETBUFFER_H 
#define NETWORK_PACKETBUFFER_H

#include <queue>
#include <string>
#include <enet/enet.h>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>


namespace network{


struct PacketEnvelope{
  int length;
  int data;
};

struct QueueItem{
  ENetPacket *packet;
  ENetAddress address;
  QueueItem *next;
};

class PacketBuffer{
public:
  PacketBuffer();
  bool isEmpty();
  bool isClosed();
  void setClosed(bool value);
  void print();
  // pops a packet from the queue
  ENetPacket *pop();
  ENetPacket *pop(ENetAddress &address);
  // pushs a packet to the queue
  bool push(ENetEvent *ev);
  
private:
  QueueItem *first;
  QueueItem *last;
  bool closed;
  
};

} //namespace
#endif /* NETWORK_PACKETBUFFER_H */
