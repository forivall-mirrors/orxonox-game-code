#include "ClientConnectionListener.h"
#include "core/CoreIncludes.h"
#include "core/Core.h"

namespace orxonox{

  ClientConnectionListener::ClientConnectionListener()
  { 
    RegisterRootObject(ClientConnectionListener); 
  }

  void ClientConnectionListener::getConnectedClients(){
    if(Core::showsGraphics())
      this->clientConnected(0); //server client id
    ClientInformation *client = ClientInformation::getBegin();
    while(client){
      this->clientConnected(client->getID());
      client=client->next();
    }
  }

}


