#ifndef _NETWORK_CLIENTCONNECTIONLISTENER__
#define _NETWORK_CLIENTCONNECTIONLISTENER__

#include "NetworkPrereqs.h"
#include "ClientInformation.h"
#include "core/OrxonoxClass.h"

namespace orxonox{

  class _NetworkExport ClientConnectionListener : virtual public OrxonoxClass
  {
    friend class Server;

  public:
    ClientConnectionListener();
    virtual ~ClientConnectionListener() {}

    void getConnectedClients();

  protected:
    virtual void clientConnected(unsigned int clientID) = 0;
    virtual void clientDisconnected(unsigned int clientID) = 0;
  };

}


#endif
