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
#ifndef _ClientConnection_H__
#define _ClientConnection_H__

#include "NetworkPrereqs.h"

#include <string>
#include <enet/enet.h>
#include <boost/thread/thread.hpp>

#include "PacketBuffer.h"

namespace network
{

#define NETWORK_PORT 55556
#define NETWORK_CLIENT_MAX_CONNECTIONS 5
#define NETWORK_CLIENT_TIMEOUT 1000
#define NETWORK_SEND_WAIT 500
#define NETWORK_CLIENT_CHANNELS 2


  class ClientConnection{
  public:
    ClientConnection(int port, std::string address);
    ClientConnection(int port, const char* address);
    //ENetPacket *getPacket(ENetAddress &address); // thread1
    //ENetPacket *getPacket(); // thread1
    ENetEvent *getEvent();
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
    //bool sendPackets(ENetEvent *event);
    bool waitEstablished(int milisec);
    bool isConnected(){return established;}
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
    boost::thread *receiverThread_;
    
    static boost::recursive_mutex enet_mutex_;
  };








}

#endif /* _ClientConnection_H__ */
