//
// Dummy server to test ConnectionManager and PacketBuffer classes
//
// Author: Oliver Scheuss


#include <iostream>
#include "enet/enet.h"
#include "network/ConnectionManager.h"
#include "network/PacketManager.h"

using namespace network;

int main(){
  ConnectionManager server = ConnectionManager();
  bool quit=false;
  ENetPacket *packet;
  ENetEvent event;
  server.createListener();
  
  PacketDecoder dec;
  
  while(!quit){
    if(server.queueEmpty())
      usleep(100);
    else{
      ENetAddress addr;
      packet=server.getPacket(addr);
      if(packet==NULL){
        // there was some error
        //std::cout << "null pointer" << std::endl;
        quit=true;
      }
      else{
        //std::cout << "We received: " << packet->data << std::endl;
        dec.elaborate(packet, 1);
      }
    }
  }
  server.quitListener();
  return 0;
}
