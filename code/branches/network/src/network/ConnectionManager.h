/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Oliver Scheuss, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

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

#include "NetworkPrereqs.h"

#include <string>
#include <map>
// enet library for networking support
#include <enet/enet.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include "PacketBuffer.h"
#include "packet/Packet.h"

namespace std
{
  bool operator<(ENetAddress a, ENetAddress b);
}

namespace network
{
#define NETWORK_PORT 55556
#define NETWORK_MAX_CONNECTIONS 50
#define NETWORK_WAIT_TIMEOUT 1
#define NETWORK_DEFAULT_CHANNEL 0

  struct ClientList{
    ENetEvent *event;
    int ID;
    ClientList *next;
  };

  class ConnectionManager{
    public:
    static boost::recursive_mutex enet_mutex;
    ConnectionManager();
    //ConnectionManager(ClientInformation *head);
    ConnectionManager(int port);
    ConnectionManager(int port, const char *address);
    ConnectionManager(int port, std::string address);
    ~ConnectionManager();
    //ENetPacket *getPacket(ENetAddress &address); // thread1
    //ENetPacket *getPacket(int &clientID);
    ENetEvent *getEvent();
    bool queueEmpty();
    void createListener();
    bool quitListener();
//     bool addPacket(Packet::Packet *packet);
    static bool addPacket(ENetPacket *packet, ENetPeer *peer);
    static bool addPacket(ENetPacket *packet, int ID);
    static bool addPacketAll(ENetPacket *packet);
  //  bool sendPackets(ENetEvent *event);
    bool sendPackets();
    //bool createClient(int clientID);
    void disconnectClient(ClientInformation *client);
    void syncClassid(unsigned int clientID);

  private:
//     bool clientDisconnect(ENetPeer *peer);
//     bool removeClient(int clientID);
    bool processData(ENetEvent *event);
    //bool addClient(ENetEvent *event);
    void receiverThread();
    void disconnectClients();
    int getClientID(ENetPeer peer);
    int getClientID(ENetAddress address);
    ENetPeer *getClientPeer(int clientID);
    //bool createShip(ClientInformation *client);
    bool removeShip(ClientInformation *client);
    PacketBuffer buffer;

    ENetHost *server;
    ENetAddress bindAddress;

    bool quit; // quit-variable (communication with threads)

    boost::thread *receiverThread_;
    static ConnectionManager *instance_;

  };

}

#endif /* _ConnectionManager_H__ */
