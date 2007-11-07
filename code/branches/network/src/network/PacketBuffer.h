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
#include <boost/thread/mutex.hpp>

//this is needed in order to make the packetbuffer threadsafe
boost::mutex networkPacketBufferMutex;

namespace network{

struct PacketEnvelope{
  int length;
  int data;
};

struct QueueItem{
  ENetPacket *packet;
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
  // pushs a packet to the queue
  bool push(ENetPacket *pck);
private:
  QueueItem *first;
  QueueItem *last;
  bool closed;
  
  //make it threadsafe
//   boost::mutex mutex;
};

} //namespace
#endif /* NETWORK_PACKETBUFFER_H */
