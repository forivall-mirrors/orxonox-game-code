//
// Dummy server to test ConnectionManager and PacketBuffer classes
//
// Author: Oliver Scheuss


#include <iostream>
#include <enet/enet.h>
#include "ConnectionManager.h"
#include "PacketManager.h"

// workaround for usleep(int) under windows
#ifdef WIN32
#include "winbase.h"
#endif


using namespace network;

int main(){
  ConnectionManager server = ConnectionManager();
  bool quit=false;
  ENetPacket *packet;
  server.createListener();

  PacketDecoder dec;

  while(!quit){
    if(server.queueEmpty())
// under windows, use Sleep(milliseconds) instead of usleep(microseconds)
// Warning: Sleep(1) is ten times longer than usleep(100)!
#ifdef WIN32
      Sleep(1);
#else
      usleep(100);
#endif
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
