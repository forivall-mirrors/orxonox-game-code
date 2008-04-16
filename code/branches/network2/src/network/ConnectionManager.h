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
#ifndef _ConnectionManager_H__
#define _ConnectionManager_H__

#include <string>
#include <map>
// enet library for networking support
#include <enet/enet.h>

#include "NetworkPrereqs.h"
#include "PacketBuffer.h"
#include "PacketManager.h"

namespace std
{
  bool operator<(ENetAddress a, ENetAddress b);
}

namespace network
{
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
    ConnectionManager(ClientInformation *head);
    ConnectionManager(int port, const char *address, ClientInformation *head);
    ConnectionManager(int port, std::string address, ClientInformation *head);
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

    //##### for testing purpose only #####
    ConnectionManager();
    std::map<int, int> testGetClientsShip() { 
      return clientsShip; 
    }
    void testAddClientsShipID( int clientID, int objectID ) {
      addClientsObjectID( clientID, objectID );
    }
    int testGetClientsShipID( int clientID ) { 
      return getClientsShipID( clientID ); 
    }
    int testGetObjectsClientID( int objectID ) {
       return getObjectsClientID( objectID );
    }
    void testDeleteClientsIDReg( int clientID ) { 
      deleteClientIDReg( clientID );
    }
    void testDeleteObjectIDReg( int objectID ) {
      deleteObjectIDReg( objectID );
    }
    //##### for testing purpose only #####
  private:
    bool clientDisconnect(ENetPeer *peer);
    //bool clientDisconnect(ENetPeer peer);
    bool processData(ENetEvent *event);
    bool addClient(ENetEvent *event);
    void receiverThread();
    void disconnectClients();
    int getClientID(ENetPeer peer);
    int getClientID(ENetAddress address);
    void syncClassid(int clientID);
    ENetPeer *getClientPeer(int clientID);
    PacketBuffer buffer;
    PacketGenerator packet_gen;

    ENetHost *server;
    ENetAddress bindAddress;

    bool quit; // quit-variable (communication with threads)
    ClientInformation *head_;
    
    //functions to map what object every clients uses
    std::map<int, int> clientsShip;
    void addClientsObjectID( int clientID, int objectID );
    int getClientsShipID( int clientID );
    int getObjectsClientID( int objectID );
    void deleteClientIDReg( int clientID );
    void deleteObjectIDReg( int objectID );
    int getNumberOfClients();
  };

}

#endif /* _ConnectionManager_H__ */
