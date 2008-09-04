#include "Chat.h"
#include "Packet.h"

namespace network {
namespace packet {
  
#define PACKET_FLAGS_CHAT ENET_PACKET_FLAG_RELIABLE
#define _PACKETID         0
#define _MESSAGELENGTH    _PACKETID + sizeof(ENUM::Type)
#define _MESSAGE          _MESSAGELENGTH + sizeof(unsigned int)

Chat::Chat( std::string& message, int clientID )
 : PacketContent()
{
  flags_ = flags_ | PACKET_FLAGS_CHAT;
  messageLength_ = message.length()+1;
  data_=new unsigned char[ getSize() ];
  *(ENUM::Type *)&data_[ _PACKETID ] = packet::ENUM::Chat;
  *(unsigned int *)&data_[ _MESSAGELENGTH ] = messageLength_;
  memcpy( &data_[ _MESSAGE ], (void *)message.c_str(), messageLength_ );
  clientID_=clientID;
}

Chat::Chat( unsigned char *data, int clientID )
  : PacketContent(data)
{
  messageLength_ = *(unsigned int *)&data[ _MESSAGELENGTH ];
  clientID_ = clientID;
}

Chat::~Chat()
{
}

unsigned char *Chat::getData(){
  return data_;
}

unsigned int Chat::getSize() const{
  return _MESSAGE + messageLength_;
}

bool Chat::process(){
  //TODO: change this !!!
  return true;
}

unsigned char *Chat::getMessage(){
  return &data_[ _MESSAGE ];
}

} //namespace packet
} //namespace network
