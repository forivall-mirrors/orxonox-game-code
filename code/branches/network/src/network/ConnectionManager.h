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
// enet library for networking support
#include <enet/enet.h>
// boost.thread library for multithreading support
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
// headerfile
#include "network/ConnectionManager.h"
#include "network/PacketBuffer.h"

namespace network{
  //
#define NETWORK_PORT 5555
#define NETWORK_MAX_CONNECTIONS 50
#define NETWORK_WAIT_TIMEOUT 5000
#define NETWORK_SEND_WAIT 5
  //just some defines to make life easier ;)
// #define ENetEvent std::ENetEvent
// #define ENetHost std::ENetHost
// #define ENetAddress std::ENetAddress
// #define ENetPeer std::ENetPeer
  
  struct ClientList{
    ENetEvent *event;
    int ID;
    ClientList *next;
  };
  
  class ConnectionManager{
    public:
    ConnectionManager();
    ConnectionManager(int port, int address);
    ENetPacket *getPacket(); // thread1
    // check wheter the packet queue is empty
    bool queueEmpty();
    // create a new listener thread
    void createListener();
    bool quitListener();
    // add a packet to queue for a specific client peer
    bool addPacket(ENetPacket *packet, ENetPeer *peer);
    // add ad packet to queue for a specific client ID
    bool addPacket(ENetPacket *packet, int ID);
    // add a packet to queue for all clients
    bool addPacketAll(ENetPacket *packet);
    // send out all queued packets
    bool sendPackets(ENetEvent *event);
    private:
    bool clientDisconnect(ENetPeer *peer);
    bool processData(ENetEvent *event);
    bool addClient(ENetEvent *event);
    // implementation of the listener
    void receiverThread(); //thread2
    //packetbuffer
    PacketBuffer buffer;
    // enet stuff
    ENetHost *server;
    ENetAddress bindAddress;
    // quit-variable (communication with threads)
    bool quit;
    // clientlist
    ClientList *client;
//     thread group
//     boost::thread_group threads;
  };
  
  
  
  
  
  
  
  
}

#endif
