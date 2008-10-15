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
#include <boost/thread/recursive_mutex.hpp>
#include "PacketBuffer.h"

namespace boost { class thread; }

namespace network
{

    const int NETWORK_PORT = 55556;
    const int NETWORK_CLIENT_MAX_CONNECTIONS = 5;
    const int NETWORK_CLIENT_WAIT_TIME = 1;
    const int NETWORK_CLIENT_CONNECT_TIMEOUT = 3000; // miliseconds
    const int NETWORK_CLIENT_CHANNELS = 2;


  class ClientConnection{
  public:
    ClientConnection(int port, std::string address);
    ClientConnection(int port, const char* address);
    ~ClientConnection();
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
