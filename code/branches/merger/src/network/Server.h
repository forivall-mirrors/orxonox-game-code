//
// C++ Interface: Server
//
// Description: 
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include <string>

#include "network/ConnectionManager.h"
#include "network/PacketManager.h"


namespace network{
  
  /**
   * This class is the root class of the network module for a server.
   * It implements all functions necessary for a Server
   */
  class Server : PacketDecoder{
    public:
    Server();
    Server(int port, std::string bindAddress);
    Server(int port, const char *bindAddress);
    
    private:
    ConnectionManager connection;
    
    
    
    
    
    
    
  };
  
  
  
  
  
}

#endif
