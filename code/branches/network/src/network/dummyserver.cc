//
// Dummy server to test ConnectionManager and PacketBuffer classes
//
// Author: Oliver Scheuss


#include <iostream>
#include "enet/enet.h"
#include "network/ConnectionManager.h"

using namespace network;

int main(){
  ConnectionManager server = ConnectionManager();
  bool quit=false;
  ENetPacket *packet;
  server.createListener();
  while(!quit){
    if(server.queueEmpty())
      usleep(100);
    else{
      packet=server.getPacket();
      if(packet==NULL){
        // there was some error
        //std::cout << "null pointer" << std::endl;
        quit=true;
      }
      else
        std::cout << "We received: " << packet->data << std::endl;
    }
  }
  server.quitListener();
  return 0;
}
