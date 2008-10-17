#include "ClientConnectionListener.h"
#include "core/CoreIncludes.h"

namespace network{

  ClientConnectionListener::ClientConnectionListener()
  { 
    RegisterRootObject(ClientConnectionListener); 
  }

  void ClientConnectionListener::getConnectedClients(){
    this->clientConnected(0); //server client id
    ClientInformation *client = ClientInformation::getBegin();
    while(client){
      this->clientConnected(client->getID());
      client=client->next();
    }
  }

}


