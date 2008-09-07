#include <assert.h>

#include "Host.h"
#include "packet/Packet.h"

namespace network {

Host *Host::instance_=0;
  
Host::Host()
{
  assert(instance_==0);
  instance_=this;
}


Host::~Host()
{
  instance_=0;
}

bool Host::addPacket(ENetPacket *packet, int clientID){
  if(instance_)
    return instance_->queuePacket(packet, clientID);
  else
    return false;
}


// bool Host::chat(std::string& message){
//   if(!instance_)
//     return false;
//   packet::Chat *c = new packet::Chat(message, getPlayerID());
//   return instance_->sendChat(c);
// }

// bool Host::receiveChat(network::packet::Chat *message, unsigned int clientID){
//   if(instance_)
//     return instance_->processChat(message, clientID);
//   else
//     return false;
// }

int Host::getPlayerID(){ 
  if(!instance_)
    return 0;
  return instance_->playerID();
}

// unsigned int Host::getShipID(){ 
//   if(!instance_)
//     assert(0);
//   return instance_->shipID();
// }

}//namespace network
