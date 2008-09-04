#include "Acknowledgement.h"
#include "Packet.h"
#include "network/Host.h"
#include "network/GamestateHandler.h"

namespace network {
namespace packet {

#define PACKET_FLAGS_ACK    0
#define _PACKETID           0
#define _ACKID              _PACKETID + sizeof(network::packet::ENUM::Type)
  
Acknowledgement::Acknowledgement( unsigned int id, int clientID )
 : PacketContent()
{
  flags_ = flags_ | PACKET_FLAGS_ACK;
  data_=new unsigned char[ getSize() ];
  *(ENUM::Type *)&data_[ _PACKETID ] = ENUM::Acknowledgement;
  *(unsigned int *)&data_[ _ACKID ] = id;
  clientID_=clientID;
}

Acknowledgement::Acknowledgement( unsigned char *data, int clientID )
  : PacketContent(data, clientID)
{
  clientID_=clientID;
}

Acknowledgement::~Acknowledgement()
{
}

unsigned char *Acknowledgement::getData(){
  return data_;
}

unsigned int Acknowledgement::getSize() const{
  return _ACKID + sizeof(unsigned int);
}

bool Acknowledgement::process(){
  return GamestateHandler::ackGamestate(data_[_ACKID], clientID_);
}

unsigned int Acknowledgement::getAckID(){
  return *(unsigned int *)&data_[ _ACKID ];
}

} //namespace packet
} //namespace network
