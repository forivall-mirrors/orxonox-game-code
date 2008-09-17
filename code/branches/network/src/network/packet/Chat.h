
#ifndef NETWORKCHAT_H
#define NETWORKCHAT_H

#include <string>

#include "Packet.h"

namespace network {
namespace packet {
/**
	@author 
*/
class Chat : public Packet
{
public:
  Chat( std::string message, unsigned int playerID );
  Chat( unsigned char* data, int clientID );
  ~Chat();
  
  inline unsigned int getSize() const;
  bool process();
  
  unsigned int getMessageLength(){ return messageLength_; };
  unsigned char *getMessage();
private:
  unsigned int messageLength_;
  int clientID_;
};

} //namespace packet
} //namespace network

#endif
