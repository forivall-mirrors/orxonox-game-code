//
// C++ Interface: ConnectionManager
//
// Description: 
//
//
// Author:  <>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NETWORK_CONNECTIONMANAGER_H
#define NETWORK_CONNECTIONMANAGER_H

#include PacketBuffer.h

namespace network{
  //
#define NETWORK_PORT 5555
#define NETWORK_MAX_CONNECTIONS 50
#define NETWORK_WAIT_TIMEOUT 5000
  //just some defines to make life easier ;)
#define ENetEvent std::ENetEvent
#define ENetHost std::ENetHost
#define ENetAddress std::ENetAddress
#define ENetPeer std::ENetPeer
  
  struct ClientList{
    ENetEvent event;
    int ID;
    ClientList *next;
  };
  
  class ConnectionManager{
    public:
    ConnectionManager();
    ~ConnectionManager();
    void loop(); // thread1
    bool createListener();
    bool quitServer();
    bool sendEventAll(/*.....*/);
    bool sendEvent(int clientID/*.....*/);
    void test();
    private:
    void peerLoop(int clientNumber); //thread2
    bool clientDisconnect(ENetPeer peer);
    bool processData(ENetEvent event);
    bool addClient(ENetEvent event);
    //packetbuffer
    PacketBuffer buffer;
    // enet stuff
    ENetHost *server;
    ENetAddress bindAddress;
    // quit-variable (communication with threads)
    bool quit;
    // thread group
    boost::thread_group threads;
    // clientlist
    ClientList *client;
  };
  
  
  
  
  
  
  
  
}

#endif