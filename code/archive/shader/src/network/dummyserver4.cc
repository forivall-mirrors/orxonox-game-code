#include <iostream>
#include <string>
#include <enet/enet.h>
#include <network/PacketManager.h>
#include <network/PacketTypes.h>
#include <network/Server.h>
#include "core/Tickable.h"
#include "core/BaseObject.h"
#include "Synchronisable.h"
#include "core/CoreIncludes.h"
#include "objects/SpaceShip.h"
#include "util/Sleep.h"

int main() {
  network::Server *server = new network::Server();
  server->open();
  while(true){
    usleep( 1000 );
  }
  server->close();
  return 0;
}
