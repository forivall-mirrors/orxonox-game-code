#include "ClassID.h"
#include "Packet.h"
#include "core/CoreIncludes.h"
#include <string>
#include <assert.h>

namespace network {
namespace packet {

#define PACKET_FLAGS_CLASSID  ENET_PACKET_FLAG_RELIABLE
#define _PACKETID             0
#define _CLASSID              _PACKETID + sizeof(ENUM::Type)
#define _CLASSNAMELENGTH      _CLASSID + sizeof(unsigned int)
#define _CLASSNAME            _CLASSNAMELENGTH + sizeof(classNameLength_)
  
  ClassID::ClassID( unsigned int classID, std::string className )
 : PacketContent()
{
  flags_ = flags_ | PACKET_FLAGS_CLASSID;
  classNameLength_=className.length()+1;
  assert(getSize());
  data_=new unsigned char[ getSize() ];
  if(!data_)
    return;
  *(packet::ENUM::Type *)&data_[ _PACKETID ] = packet::ENUM::ClassID;
  *(unsigned int *)&data_[ _CLASSID ] = classID;
  *(unsigned int *)&data_[ _CLASSNAMELENGTH ] = classNameLength_;
  memcpy( &data_[ _CLASSNAME ], (void *)className.c_str(), classNameLength_ );
}

ClassID::ClassID( unsigned char *data, int clientID )
  : PacketContent(data, clientID)
{
  memcpy( (void *)&classNameLength_, &data[ sizeof(ENUM::Type) + sizeof(int) ], sizeof(classNameLength_) );
}

ClassID::~ClassID()
{
}

unsigned char *ClassID::getData(){
  return data_;
}

unsigned int ClassID::getSize() const{
  return sizeof(network::packet::ENUM::Type) + sizeof(int) + classNameLength_;
}

bool ClassID::process(){
  orxonox::Identifier *id=ID( std::string((const char*)&data_[ sizeof(network::packet::ENUM::Type) ]) );
  if(id==NULL)
    return false;
  id->setNetworkID( getClassID() );
  return true;
}

unsigned int ClassID::getClassID(){
  return *(unsigned int *)&data_[ _CLASSID ];
}

// unsigned int ClassID::getClassNameLength(){
//   return *(unsigned int *)&data[ _CLASSNAMELENGTH ];
// }

} //namespace packet
}//namespace network
