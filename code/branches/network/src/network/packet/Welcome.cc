#include "Welcome.h"
#include "Packet.h"
#include <assert.h>

namespace network {
namespace packet {

#define PACKET_FLAGS_CLASSID  ENET_PACKET_FLAG_RELIABLE
#define _PACKETID             0
#define _CLIENTID             _PACKETID + sizeof(ENUM::Type)
#define _SHIPID               _CLIENTID + sizeof(unsigned int)
  
  Welcome::Welcome( unsigned int clientID, unsigned int shipID )
 : PacketContent()
{
  flags_ = flags_ | PACKET_FLAGS_CLASSID;
  assert(getSize());
  data_=new unsigned char[ getSize() ];
  assert(data_);
  *(packet::ENUM::Type *)&data_[ _PACKETID ] = packet::ENUM::Welcome;
  *(unsigned int *)&data_[ _CLIENTID ] = clientID;
  *(unsigned int *)&data_[ _SHIPID ] = shipID;
}

Welcome::Welcome( unsigned char *data, int clientID )
  : PacketContent(data, clientID)
{
}

Welcome::~Welcome()
{
}

unsigned char *Welcome::getData(){
  return data_;
}

unsigned int Welcome::getSize() const{
  return sizeof(network::packet::ENUM::Type) + 2*sizeof(unsigned int);
}

bool Welcome::process(){
  unsigned int shipID, clientID;
  clientID = *(unsigned int *)&data_[ _CLIENTID ];
  shipID = *(unsigned int *)&data_[ _SHIPID ];
  return true;
}


} //namespace packet
}//namespace network
