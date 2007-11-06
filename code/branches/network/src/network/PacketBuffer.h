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

namespace network{

struct PacketEnvelope{
  int length;
  int data;
};

struct QueueItem{
  PacketEnvelope *packet;
  QueueItem *next;
};

class PacketBuffer{
public:
  PacketBuffer();
  bool isLocked();
  bool isEmpty();
  // pops a packet from the queue
  PacketEnvelope *pop();
  // pushs a packet to the queue
  bool push(PacketEnvelope pck);
  void print();
private:
  bool locked;
  QueueItem *first;
  QueueItem *last;
};

} //namespace
#endif /* NETWORK_PACKETBUFFER_H */
