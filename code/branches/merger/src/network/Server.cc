//
// C++ Implementation: Server
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "Server.h"

namespace network{

  /**
   * Constructor for default values (bindaddress is set to ENET_HOST_ANY
   *
   */
  Server::Server(){


  }

  /**
   * Constructor
   * @param port Port to listen on
   * @param bindAddress Address to listen on
   */
  Server::Server(int port, std::string bindAddress) : connection(port, bindAddress){


  }

  /**
   * Constructor
   * @param port Port to listen on
   * @param bindAddress Address to listen on
   */
  Server::Server(int port, const char *bindAddress) : connection(port, bindAddress){




  }





}
