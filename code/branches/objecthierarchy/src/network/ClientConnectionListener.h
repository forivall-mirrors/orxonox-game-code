#ifndef _NETWORK_CLIENTCONNECTIONLISTENER__
#define _NETWORK_CLIENTCONNECTIONLISTENER__

#include "NetworkPrereqs.h"
#include "ClientInformation.h"
#include "core/OrxonoxClass.h"

namespace network{

  class _NetworkExport ClientConnectionListener : public orxonox::OrxonoxClass
  {
  public:
    ClientConnectionListener();

    void getConnectedClients();

    virtual void clientConnected(unsigned int clientID) = 0;
    virtual void clientDisconnected(unsigned int clientID) = 0;
  };

}


#endif
