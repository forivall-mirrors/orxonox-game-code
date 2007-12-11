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

#include "ConnectionManager.h"
#include "PacketManager.h"
#include "GameStateManager.h"
#include "enet/enet.h"
#include "ClientInformation.h"
//#include "NetworkFrameListener.h"


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
    void open();
    void close();
    bool sendMSG(std::string msg);
    bool sendMSG(const char *msg);
    void tick(float time);
    protected:
    void processQueue();
    void updateGamestate();
    private:
    bool sendGameState();
    ConnectionManager *connection;
    GameStateManager *gamestates;
    PacketGenerator packet_gen;
    
    ClientInformation *clients;
  };
 



}

#endif
