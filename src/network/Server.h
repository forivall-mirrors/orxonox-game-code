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

#ifndef _Server_H__
#define _Server_H__

#include <string>

#include "PacketManager.h"
#include "NetworkPrereqs.h"

namespace network
{
  /**
  * This class is the root class of the network module for a server.
  * It implements all functions necessary for a Server
  */
  class _NetworkExport Server : public PacketDecoder{
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
    void processAck( ack *data, int clientID);
    ConnectionManager *connection;
    GameStateManager *gamestates;
    PacketGenerator packet_gen;

    ClientInformation *clients;
  };




}

#endif /* _Server_H__ */
