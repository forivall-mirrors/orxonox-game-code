#include <iostream>
#include <string>
#include <enet/enet.h>
#include <network/PacketManager.h>
#include <network/PacketTypes.h>
#include <network/Client.h>
#include "core/Tickable.h"
#include "core/BaseObject.h"
#include "Synchronisable.h"
#include "core/CoreIncludes.h"
#include "objects/SpaceShip.h"

int main() {
  std::cout << "dummyclient4" << std::endl;
  network::Client* client;
  client = network::Client::createSingleton( "127.0.0.1", 55556 );
  if(client->establishConnection())
    std::cout << "established connection" << std::endl;
  else{
    std::cout << "could not establish connection" << std::endl;
    return -1;
  }
  if(client->closeConnection())
    std::cout << "closed connection" << std::endl;
  else
    std::cout << "could not close connection" << std::endl;
}


