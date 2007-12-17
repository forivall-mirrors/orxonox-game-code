//
// C++ Interface: ClientConnection
//
// Description:
//
//
// Author:  Oliver Scheuss, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NETWORK_CLIENTCONNECTION_H
#define NETWORK_CLIENTCONNECTION_H

#include <iostream>
#include <string>
// enet library for networking support
#include <enet/enet.h>
// boost.thread library for multithreading support
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
// headerfile
#include "ClientConnection.h"
#include "PacketBuffer.h"

namespace network{
  //
#define NETWORK_PORT 55556
#define NETWORK_CLIENT_MAX_CONNECTIONS 5
#define NETWORK_WAIT_TIMEOUT 5
#define NETWORK_SEND_WAIT 5
#define NETWORK_CLIENT_CHANNELS 2


  class ClientConnection{
    public:
    ClientConnection(int port, std::string address);
    ClientConnection(int port, const char* address);
    ENetPacket *getPacket(ENetAddress &address); // thread1
    ENetPacket *getPacket(); // thread1
    // check wheter the packet queue is empty
    bool queueEmpty();
    // create a new listener thread
    bool createConnection();
    bool closeConnection();
    // add a packet to queue for the server
    bool addPacket(ENetPacket *packet);
    // send out all queued packets
    bool sendPackets();
    // send out all queued packets and save result in event
    bool sendPackets(ENetEvent *event);
    bool waitEstablished(int milisec);
    private:
    bool processData(ENetEvent *event);
    // implementation of the listener
    void receiverThread(); //thread2
    //packetbuffer
    bool establishConnection();
    bool disconnectConnection();
    PacketBuffer buffer;
    // enet stuff
    ENetHost *client;
    ENetAddress serverAddress;
    // quit-variable (communication with threads)
    bool quit;
    bool established;
    // clientlist
    ENetPeer *server;
  };








}

#endif
