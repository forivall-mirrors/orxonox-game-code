//
// C++ Interface: ConnectionManager
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NETWORK_CONNECTIONMANAGER_H
#define NETWORK_CONNECTIONMANAGER_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
// enet library for networking support
#include <enet/enet.h>
// boost.thread library for multithreading support
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
// headerfiles
#include "ConnectionManager.h"
#include "PacketBuffer.h"

namespace std{
  bool operator<(ENetAddress a, ENetAddress b);
}

namespace network{
  //
#define NETWORK_PORT 55556
#define NETWORK_MAX_CONNECTIONS 50
#define NETWORK_WAIT_TIMEOUT 5000
#define NETWORK_SEND_WAIT 5

  struct ClientList{
    ENetEvent *event;
    int ID;
    ClientList *next;
  };
  
  class ConnectionManager{
    public:
    ConnectionManager();
    ConnectionManager(int port, const char *address);
    ConnectionManager(int port, std::string address);
    ENetPacket *getPacket(ENetAddress &address); // thread1
    ENetPacket *getPacket(int &clientID);
    bool queueEmpty();
    void createListener();
    bool quitListener();
    bool addPacket(ENetPacket *packet, ENetPeer *peer);
    bool addPacket(ENetPacket *packet, int ID);
    bool addPacketAll(ENetPacket *packet);
    bool sendPackets(ENetEvent *event);
    bool sendPackets();
    private:
    bool clientDisconnect(ENetPeer *peer);
    bool clientDisconnect(ENetPeer peer);
    bool processData(ENetEvent *event);
    bool addClient(ENetEvent *event);
    void receiverThread();
    void disconnectClients();
    int getClientID(ENetPeer peer);
    int getClientID(ENetAddress address);
    ENetPeer getClientPeer(int clientID);
    PacketBuffer buffer;
    
    ENetHost *server;
    ENetAddress bindAddress;
    
    bool quit; // quit-variable (communication with threads)
    std::map<ENetAddress, int> clientMap;
    std::map<ENetAddress, ENetPeer> peerMap;
    std::vector<ENetAddress> clientVector;
  };








}

#endif
