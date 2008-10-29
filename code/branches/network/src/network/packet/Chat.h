
#ifndef NETWORKCHAT_H
#define NETWORKCHAT_H

#include "../NetworkPrereqs.h"

#include <string>
#include <cstring>

#include "Packet.h"

namespace network {
namespace packet {
/**
	@author
*/
class _NetworkExport Chat : public Packet
{
public:
  Chat( std::string message, unsigned int playerID );
  Chat( uint8_t* data, unsigned int clientID );
  ~Chat();

  inline unsigned int getSize() const;
  bool process();

  unsigned int getMessageLength(){ return messageLength_; };
  unsigned char *getMessage();
private:
  uint32_t messageLength_;
  unsigned int clientID_;
};

} //namespace packet
} //namespace network

#endif
