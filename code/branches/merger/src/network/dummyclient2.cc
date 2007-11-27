//
// Dummy client to test ConnectionManager, PacketBuffer, ClientConnection and other classes
//
// Author: Oliver Scheuss

#include <iostream>
#include <string>
#include <enet/enet.h>
#include "network/PacketManager.h"
#include "network/ClientConnection.h"

using namespace network;

int main(){
  network::PacketGenerator pck;

  std::cout << "Enter address of the server xxx.xxx.xxx.xxx (enter for localhost)" << std::endl;
  std::string str;
  std::getline(std::cin, str);
  std::cout << "You entered: " << str << std::endl;
  if(str.compare("")==0)
    str="127.0.0.1";

  ClientConnection client = ClientConnection(55556, str);

  client.createConnection();
  
  if(client.waitEstablished(10000))
    std::cout << "Connection established" << std::endl;
  else
    std::cout << "Connection failed" << std::endl;
  
  ENetPacket *packet;
  ENetEvent event;
  
  
  
  for(int i=0; i<10; i++){
        // weihnachtsmann bringt packete
        // extend the packet and append the string foo to it
        // send packet to peer on channel id 0
    client.addPacket(pck.chatMessage("test"));
        // keep the timeout very small for low delay
    if(client.sendPackets(&event)){
      std::cout << "successfully sent: " << event.type << std::endl;
    }else{
      std::cout << "failed sending" << std::endl;
    }
    //usleep(100000);
  }

        // now disconnect
  if(client.closeConnection())
    std::cout << "Connection successfully closed" << std::endl;
  else
    std::cout << "Connection closing failed" << std::endl;

        // 3 seconds timeout
  return 0;
}

