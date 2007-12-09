//
// Dummy server to test ConnectionManager and PacketBuffer classes
//
// Author: Oliver Scheuss


#include <iostream>
#include <enet/enet.h>
#include "ConnectionManager.h"
#include "PacketManager.h"
#include "ClientInformation.h"

#ifdef WIN32
#include <windows.h>
#define usleep(x) Sleep((x)/1000)
#else
#include <unistd.h>
#endif


using namespace network;

int main(){
  bool quit=false;
  ENetPacket *packet;
  ClientInformation clients;
  ConnectionManager server = ConnectionManager(&clients);
  server.createListener();

  PacketDecoder dec;

  while(!quit){
    if(server.queueEmpty())
// Warning: usleep(100) is Sleep(100/1000) = Sleep(0), which is nothing!
      usleep(10000000);
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
